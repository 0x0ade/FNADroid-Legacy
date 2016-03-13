using System;
using System.Runtime.InteropServices;
using Microsoft.Xna.Framework;

namespace FNADroid {
	
	public static class Vibration {
		
		[DllImport(FNADroidLib.nativeLibName, EntryPoint = "VibrationAvailable")]
		private static extern bool INTERNAL_IsAvailable();
		public static bool IsAvailable {
			get {
				return INTERNAL_IsAvailable();
			}
		}
		
		[DllImport(FNADroidLib.nativeLibName, EntryPoint = "Vibrate")]
		public static extern void Vibrate(long milliseconds);
		
		[DllImport(FNADroidLib.nativeLibName, EntryPoint = "VibrationCancel")]
		public static extern void Cancel();
		
	}
	
}