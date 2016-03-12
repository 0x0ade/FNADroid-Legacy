using System;
using System.Runtime.InteropServices;
using Microsoft.Xna.Framework;

namespace FNADroid {
	
	public static class Log {
		
		public enum Level : int {
			VERBOSE = 2,
			DEBUG = 3,
			INFO = 4,
			WARN = 5,
			ERROR = 6,
			ASSERT = 7,
		}
		
		[DllImport(FNADroidLib.nativeLibName, EntryPoint="Log")]
		public static extern int Line(Level priority, string tag, string msg);
		
		public static int v(string tag, string msg) { return Line(Level.VERBOSE, tag, msg); }
		public static int d(string tag, string msg) { return Line(Level.DEBUG, tag, msg); }
		public static int i(string tag, string msg) { return Line(Level.INFO, tag, msg); }
		public static int w(string tag, string msg) { return Line(Level.WARN, tag, msg); }
		public static int e(string tag, string msg) { return Line(Level.ERROR, tag, msg); }
		public static int wtf(string tag, string msg) { return Line(Level.ASSERT, tag, msg); }
		
	}
	
}