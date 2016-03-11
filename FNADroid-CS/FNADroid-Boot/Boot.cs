using System;
using System.IO;
using System.Runtime.InteropServices;

namespace FNADroid {

        public static class Boot {

		private const string nativeLibName = "fnadroid-wrapper";
                [DllImport(nativeLibName)]
                private static extern void PrintInfo(string msg);
                [DllImport(nativeLibName)]
                private static extern void PrintWarn(string msg);
                [DllImport(nativeLibName)]
                private static extern void PrintError(string msg);


                public static void Main(string[] args) {
			Console.WriteLine("Hello STDOUT pre-Logcat!");
			Console.SetOut(new LogcatWriter() {
                                OnWrite = PrintInfo
                        });
                        Console.SetError(new LogcatWriter() {
                                OnWrite = PrintError
                        });
			Console.WriteLine("Hello from FNADroid-Boot!");

			//We need to actually initialize the library somewhere...
			Lib.Boot();

			//TODO launch game.exe or read config and launch it
		}

        }

}

