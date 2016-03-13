using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Reflection;

namespace FNADroid {
	
	public static class FNADroidBoot {
		
		public static void Main(string[] args) {
			Console.SetOut(new LogcatWriter() {
				Tag = "FNADroid-Wrapper",
				Level = Log.Level.INFO
			});
			Console.SetError(new LogcatWriter() {
				Tag = "FNADroid-Wrapper",
				Level = Log.Level.ERROR
			});
			Console.WriteLine("Hello from FNADroid-Boot!");
			
			Environment.SetEnvironmentVariable("HOME", FNADroidLib.GetHomePath());
			
			//Allow games / FNA(?) to check for this environment variable.
			Environment.SetEnvironmentVariable("FNADROID_ENABLED", "1");
			
			int es = FNADroidLib.GetMaximumGLES();
			int esMaj = (int) Math.Floor(es / 10D);
			int esMin = es - esMaj * 10;
			string esString = esMaj + "." + esMin;
			
			FNADroidLib.PopupDebug("Package: " + FNADroidLib.GetPackageName() + "\nInstaller: " + FNADroidLib.GetInstallerPackageName() + "\nGLES: " + esString);
			
			//Set GLES version
			//Backwards-compatibility with "original" (unmodified) FNA
			Environment.SetEnvironmentVariable("FNA_OPENGL_FORCE_ES2", "1");
			//The FNADroid FNA clone supports setting the version.
			Environment.SetEnvironmentVariable("FNA_OPENGL_FORCE_ES", esString);
			
			//Breaks accuracy, but Android supports getting (estimating) the maximum.
			//Automatic maximum currently enabled for testing / demonstration purposes.
			Environment.SetEnvironmentVariable("FNA_TOUCH_FORCE_MAXIMUM", "0");
			
			FNADroidPlatform.Boot();
			
			Assembly.LoadFrom(Path.Combine(FNADroidLib.GetGamePath(), "game.exe")).EntryPoint.Invoke(null, new object[] { new string[] { /*args*/ } });
		}
		
	}
	
}

