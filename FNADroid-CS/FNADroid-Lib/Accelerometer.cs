using System;
using System.Runtime.InteropServices;
using Microsoft.Xna.Framework;

namespace FNADroid {
	
	public static class Accelerometer {
		
		[DllImport(FNADroidLib.nativeLibName, EntryPoint = "AccelerometerAvailable")]
		private static extern bool INTERNAL_IsAvailable();
		public static bool IsAvailable {
			get {
				return INTERNAL_IsAvailable();
			}
		}
		
		public static Vector3 Data {
			get {
				return new Vector3(
					GetAxis(0),
					GetAxis(1),
					GetAxis(2)
				);
			}
		}
		
		// called from Android
		public static void OnDataChanged() {
			if (DataChanged != null) {
				DataChanged(Data);
			}
		}
		public static event Action<Vector3> DataChanged;
		
		[DllImport(FNADroidLib.nativeLibName, EntryPoint = "GetAccelerometerAxis")]
		public static extern float GetAxis(int axis);
		
	}
	
}