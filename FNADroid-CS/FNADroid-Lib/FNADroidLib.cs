using System;
using System.Runtime.InteropServices;

namespace FNADroid {
	
	public static class FNADroidLib {
		
		//Fun fact: The mono documentation (MonoDoc?) promotes the use of __Internal.
		//__Internal causes a DllNotFoundException..?!
		public const string nativeLibName = "fnadroid-wrapper";
		
		[DllImport(nativeLibName)]
		public static extern void Popup(string title, string msg);
		[DllImport(nativeLibName)]
		public static extern void PopupDebug(string msg);
		[DllImport(nativeLibName)]
		public static extern void PopupError(string msg);
		
		[DllImport(nativeLibName)]
		public static extern string GetGamePath();
		[DllImport(nativeLibName)]
		public static extern string GetHomePath();
		[DllImport(nativeLibName)]
		public static extern string GetPackageName();
		[DllImport(nativeLibName)]
		public static extern string GetDataPath();
		[DllImport(nativeLibName)]
		public static extern string GetMainObbPath();
		[DllImport(nativeLibName)]
		public static extern string GetPatchObbPath();
		[DllImport(nativeLibName)]
		public static extern string GetInstallerPackageName();
		[DllImport(nativeLibName, EntryPoint = "GetMaximumGLES")]
		private static extern int j_GetMaximumGLES();
		public static int GetMaximumGLES() {
			int esAndroid = j_GetMaximumGLES();
			//Android returns the value in bitshifted hex.
			//The upper order 16 bits represent the major version and
			//the lower order 16 bits the minor version.
			int esMaj = esAndroid >> 16;
			int esMin = esAndroid & 0x0000FFFF;
			
			return esMaj * 10 + esMin;
		}
		
		[DllImport(nativeLibName)]
		public static extern int GetMaximumTouchCount();
		
	}
	
}
