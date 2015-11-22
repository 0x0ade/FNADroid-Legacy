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
    public static extern bool CanGLES3();
    
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
        
        PopupDebug("Package: " + GetPackageName() + "\nInstaller: " + GetInstallerPackageName() + "\nGLESv" + (CanGLES3() ? "3+" : "2"));
        
        //Check for GLES3 and use a GLES3 context instead if possile (custom built / new enough).
        if (CanGLES3()) {
            Environment.SetEnvironmentVariable("FNA_OPENGL_FORCE_ES3", "1");
        } else {
            PopupDebug("This device supports GLES2. FNA already uses this on Android.");
        }
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
