using System;
using Microsoft.Xna.Framework;
using System.Reflection;
using System.Runtime.InteropServices;
using Microsoft.Xna.Framework.Input;

namespace FNADroid {
	
	public static class FNADroidPlatform {
		
		private static int _GamePad_GAMEPAD_COUNT;
		
		public static GamePadCapabilities PhoneGamePadCapabilities;
		
		public static bool PhoneAsGamePad = false;
		
		public static void Log(string value) {
			FNADroid.Log.d("FNA", value);
		}
		
		public static MulticastDelegate fna_GetMaximumTouchCount;
		public static int GetMaximumTouchCount() {
			string maxEnv = Environment.GetEnvironmentVariable(
					"FNA_TOUCH_FORCE_MAXIMUM"
			);
			int max;
			if (int.TryParse(maxEnv, out max) && max == 0) {
				return FNADroidLib.GetMaximumTouchCount();
			}
			return (int) fna_GetMaximumTouchCount.DynamicInvoke();
		}
		
		public static MulticastDelegate fna_SetGamePadVibration;
		public static bool SetGamePadVibration(int index, float leftMotor, float rightMotor) {
			if ((Environment.GetEnvironmentVariable(
				"FNA_GAMEPAD_VIBRATION_PHONE"
			) == "1") || PhoneAsGamePad && index == 0) {
				long time = (long) (325f * ((leftMotor + rightMotor) / 2f));
				if (time < 100) {
					Vibration.Cancel();
				} else {
					Vibration.Vibrate(time);
				}
				return true;
			}
			return (bool) fna_SetGamePadVibration.DynamicInvoke(PhoneAsGamePad ? index - 1 : index, leftMotor, rightMotor);
		}
		
		public static Func<GamePadCapabilities> GetPhoneGamePadCapabilitiesHook; //for cheap on-screen gamepads in games that need them
		public static MulticastDelegate fna_GetGamePadCapabilities;
		public static GamePadCapabilities GetGamePadCapabilities(int index) {
			//Mute gamepad 0, move everything; Completely replace the GamePadCapabilities
			if (index == 0 && PhoneAsGamePad) {
				if (GetPhoneGamePadStateHook != null) {
					return GetPhoneGamePadCapabilitiesHook();
				}
				
				return PhoneGamePadCapabilities;
			}
			
			return (GamePadCapabilities) fna_GetGamePadCapabilities.DynamicInvoke(PhoneAsGamePad ? index - 1 : index);
		}
		
		public static Func<GamePadState> GetPhoneGamePadStateHook; //for cheap on-screen gamepads in games that need them
		public static MulticastDelegate fna_GetGamePadState;
		public static GamePadState GetGamePadState(int index, GamePadDeadZone deadZoneMode) {
			bool backPressed = false;
			if (index == 0) {
				int numkeys;
				IntPtr pKeys = SDL2.SDL.SDL_GetKeyboardState(out numkeys);
				backPressed = 1 == Marshal.ReadByte(pKeys + (int) SDL2.SDL.SDL_Scancode.SDL_SCANCODE_AC_BACK);
				//TODO "It will be valid for the whole lifetime of the application and should not be freed by the caller." Uhh...
			}
			
			//Mute gamepad 0, move everything; Completely replace the GamePadState
			if (index == 0 && PhoneAsGamePad) {
				if (GetPhoneGamePadStateHook != null) {
					return GetPhoneGamePadStateHook();
				}
				
				Vector3 accelerometerData = Accelerometer.Data;
				
				Vector2 leftStick = new Vector2(accelerometerData.X, accelerometerData.Y);
				Vector2 rightStick = Vector2.Zero;
				
				float leftTrigger = 0f;
				float rightTrigger = 0f;
				
				ButtonState dpadUp = ButtonState.Released;
				ButtonState dpadDown = ButtonState.Released;
				ButtonState dpadLeft = ButtonState.Released;
				ButtonState dpadRight = ButtonState.Released;
				
				Buttons buttons = (Buttons) 0;
				
				if (backPressed) {
					buttons |= Buttons.Back;
				}
				
				GamePadState newState = new GamePadState(
					new GamePadThumbSticks(leftStick, rightStick),
					new GamePadTriggers(leftTrigger, rightTrigger),
					new GamePadButtons(buttons),
					new GamePadDPad(dpadUp, dpadDown, dpadLeft, dpadRight)
				);
				//newState.IsConnected = state.IsConnected; //true by default
				//newState.PacketNumber = state.PacketNumber; //uhh...
				return newState;
			}
			
			GamePadState state = (GamePadState) fna_GetGamePadState.DynamicInvoke(PhoneAsGamePad ? index - 1 : index, deadZoneMode);
			
			//Just "enhance" the first controller
			if (index == 0) {
				GamePadButtons buttons = state.Buttons;
				Buttons tmpButtons = (Buttons) 0;
				
				if (buttons.A == ButtonState.Pressed) {
					tmpButtons |= Buttons.A;
				}
				if (buttons.B == ButtonState.Pressed) {
					tmpButtons |= Buttons.B;
				}
				if (buttons.X == ButtonState.Pressed) {
					tmpButtons |= Buttons.X;
				}
				if (buttons.Y == ButtonState.Pressed) {
					tmpButtons |= Buttons.Y;
				}
				if (buttons.Back == ButtonState.Pressed) {
					tmpButtons |= Buttons.Back;
				}
				if (buttons.BigButton == ButtonState.Pressed) {
					tmpButtons |= Buttons.BigButton;
				}
				if (buttons.Start == ButtonState.Pressed) {
					tmpButtons |= Buttons.Start;
				}
				if (buttons.LeftStick == ButtonState.Pressed) {
					tmpButtons |= Buttons.LeftStick;
				}
				if (buttons.RightStick == ButtonState.Pressed) {
					tmpButtons |= Buttons.RightStick;
				}
				if (buttons.LeftShoulder == ButtonState.Pressed) {
					tmpButtons |= Buttons.LeftShoulder;
				}
				if (buttons.RightShoulder == ButtonState.Pressed) {
					tmpButtons |= Buttons.RightShoulder;
				}
				
				if (backPressed) {
					tmpButtons |= Buttons.Back;
				}
				
				GamePadState newState = new GamePadState(
					state.ThumbSticks,
					state.Triggers,
					new GamePadButtons(tmpButtons),
					state.DPad
				);
				//newState.IsConnected = state.IsConnected; //true by default
				//newState.PacketNumber = state.PacketNumber; //uhh...
				return newState;
			}
			
			return state;
		}
		
		public static void Boot() {
			//SDL2 doesn't support changing this after SDL_SYS_JoystickInit has been called.
			//We're handling the accelerometer controller on our own.
			SDL2.SDL.SDL_SetHint("SDL_HINT_ACCELEROMETER_AS_JOYSTICK", "0");
			
			//The FNAPlatform hooking reflection part.. oh, so shiny!
			Assembly asm = Assembly.GetAssembly(typeof(Game));
			Type t;
			FieldInfo f;
			
			
			t = asm.GetType("Microsoft.Xna.Framework.Input.GamePad");
			
			f = t.GetField("GAMEPAD_COUNT", BindingFlags.Static | BindingFlags.NonPublic);
			_GamePad_GAMEPAD_COUNT = (int) f.GetValue(null);
			
			
			PhoneGamePadCapabilities = new GamePadCapabilities();
			t = PhoneGamePadCapabilities.GetType();
			
			t.GetProperty("IsConnected", BindingFlags.Instance | BindingFlags.Public)
				.GetSetMethod(true).Invoke(PhoneGamePadCapabilities, new object[] {
					true
			});
			t.GetProperty("HasBackButton", BindingFlags.Instance | BindingFlags.Public)
				.GetSetMethod(true).Invoke(PhoneGamePadCapabilities, new object[] {
					true
			});
			t.GetProperty("HasLeftXThumbStick", BindingFlags.Instance | BindingFlags.Public)
				.GetSetMethod(true).Invoke(PhoneGamePadCapabilities, new object[] {
					true
			});
			t.GetProperty("HasLeftYThumbStick", BindingFlags.Instance | BindingFlags.Public)
				.GetSetMethod(true).Invoke(PhoneGamePadCapabilities, new object[] {
					true
			});
			t.GetProperty("HasLeftVibrationMotor", BindingFlags.Instance | BindingFlags.Public)
				.GetSetMethod(true).Invoke(PhoneGamePadCapabilities, new object[] {
					true
			});
			t.GetProperty("HasRightVibrationMotor", BindingFlags.Instance | BindingFlags.Public)
				.GetSetMethod(true).Invoke(PhoneGamePadCapabilities, new object[] {
					true
			});
			t.GetProperty("GamePadType", BindingFlags.Instance | BindingFlags.Public)
				.GetSetMethod(true).Invoke(PhoneGamePadCapabilities, new object[] {
					GamePadType.Unknown
			});
			
			t = asm.GetType("Microsoft.Xna.Framework.FNAPlatform");
			
			f = t.GetField("Log");
			f.SetValue(null, (Action<string>) Log);
			
			Hook("GetMaximumTouchCount");
			Hook("SetGamePadVibration");
			Hook("GetGamePadCapabilities");
			Hook("GetGamePadState");
		}
		
		public static void Hook(string name) {
			Type tt = typeof(FNADroidPlatform);
			FieldInfo tf = tt.GetField("fna_" + name);
			
			Assembly asm = Assembly.GetAssembly(typeof(Game));
			Type t = asm.GetType("Microsoft.Xna.Framework.FNAPlatform");
			FieldInfo f = t.GetField(name);
			
			tf.SetValue(null, f.GetValue(null));
			f.SetValue(null, Delegate.CreateDelegate(asm.GetType("Microsoft.Xna.Framework.FNAPlatform+" + name + "Func"), tt.GetMethod(name)));
		}
		
	}
	
}