using System;
using System.Runtime.InteropServices;
using Microsoft.Xna.Framework;

namespace FNADroid {
	
	public static class Gyroscope {
		
		[DllImport(FNADroidLib.nativeLibName, EntryPoint = "GyroscopeAvailable")]
		private static extern bool INTERNAL_IsAvailable();
		public static bool IsAvailable {
			get {
				return INTERNAL_IsAvailable();
			}
		}
		
		[Obsolete("Use RotationRate for Gyroscope instead. (This just exists as nagging warning.)")]
		public static Vector3 Data {
			get {
				return RotationRate;
			}
		}
		
		public static Vector3 RotationRate {
			get {
				return new Vector3(
					GetRotationRateAxis(0),
					GetRotationRateAxis(1),
					GetRotationRateAxis(2)
				);
			}
		}
		
		// called from Android
		public static void OnRotationRateChanged() {
			if (RotationRateChanged != null) {
				RotationRateChanged(RotationRate);
			}
		}
		public static event Action<Vector3> RotationRateChanged;
		
		[DllImport(FNADroidLib.nativeLibName, EntryPoint = "GetGyroscopeRotationRateAxis")]
		public static extern float GetRotationRateAxis(int axis);
		
	}
	
}