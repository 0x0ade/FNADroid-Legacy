using System;
using System.IO;
using System.IO.Compression;
using System.Runtime.InteropServices;
using System.Text;

public static class FNADroid {
    
    //Fun fact: The mono documentation (MonoDoc?) promotes the use of __Internal.
    //__Internal causes a DllNotFoundException..?!
    private const string nativeLibName = "fnadroid-wrapper";
    
    [DllImport(nativeLibName)]
    public static extern void PrintInfo(string msg);
    
    [DllImport(nativeLibName)]
    public static extern void PrintWarn(string msg);
    
    [DllImport(nativeLibName)]
    public static extern void PrintError(string msg);
    
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
    [DllImport(nativeLibName)]
    private static extern int j_GetMaxGLES();
    public static int GetMaxGLES() {
        int esAndroid = j_GetMaxGLES();
        //Android returns the value in bitshifted hex.
        //The upper order 16 bits represent the major version and
        //the lower order 16 bits the minor version.
        int esMaj = esAndroid >> 16;
        int esMin = esAndroid & 0x0000FFFF;
        
        return esMaj * 10 + esMin;
    }
    
    [DllImport(nativeLibName)]
    public static extern void VibrationCancel();
    [DllImport(nativeLibName)]
    public static extern bool VibrationAvailable();
    [DllImport(nativeLibName)]
    public static extern void Vibrate(long milliseconds);
    
    public static void Boot() {
        Console.WriteLine("Hello, World.. wherever this goes.");
            
        Console.SetOut(new LogcatWriter() {
            OnWrite = PrintInfo
        });
        
        Console.SetError(new LogcatWriter() {
            OnWrite = PrintError
        });
        
        Console.WriteLine("Hello, Logcat!");
        
        Environment.SetEnvironmentVariable("HOME", GetHomePath());
        
        //Allow games / FNA(?) to check for this environment variable.
        Environment.SetEnvironmentVariable("FNADROID_ENABLED", "1");
        
        int es = GetMaxGLES();
        int esMaj = (int) Math.Floor(es / 10D);
        int esMin = es - esMaj * 10;
        string esString = esMaj + "." + esMin;
        
        PopupDebug("Package: " + GetPackageName() + "\nInstaller: " + GetInstallerPackageName() + "\nGLES: " + esString);
        
        //Set GLES version
        //Backwards-compatibility with "original" (unmodified) FNA
        Environment.SetEnvironmentVariable("FNA_OPENGL_FORCE_ES2", "1");
        //The FNADroid FNA clone supports setting the version.
        Environment.SetEnvironmentVariable("FNA_OPENGL_FORCE_ES", esString);
    }
    
    [DllImport(nativeLibName)]
    internal static extern void j_ExtractObb(string path);
    public static void ExtractObb(string path) {
        //Allow the game to extract custom obbs if required.
        //For example, deliver an "installer"
        //start installer and let installer
        //load custom data from obb.
        
        if (ExtractObbHook != null) {
            ExtractObbHook(path);
            return;
        }
        
        j_ExtractObb(path);
    }
    
    public static Action<string> ExtractObbHook;
    
}

public class LogcatWriter : TextWriter {
    public override Encoding Encoding {
        get {
            return Encoding.UTF8;
        }
    }
    
    public Action<string> OnWrite;

    public override void Write(string value) {
        if (OnWrite != null) {
            OnWrite(value);
        }
        base.Write(value);
    }

}
