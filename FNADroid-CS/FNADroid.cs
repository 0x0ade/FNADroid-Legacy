using System;
using System.IO;
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
    public static extern string GetHomePath();
    [DllImport(nativeLibName)]
    public static extern string GetMainObbPath();
    [DllImport(nativeLibName)]
    public static extern string GetPatchObbPath();
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
        
        //Check for GLES3 and use a GLES3 context instead if possile (custom built / new enough).
        if (CanGLES3()) {
            PopupDebug("This device supports GLES3+. Asking FNA to use it...");
            Environment.SetEnvironmentVariable("FNA_OPENGL_FORCE_ES3", "1");
        } else {
            PopupDebug("This device supports GLES2. FNA already uses this on Android.");
        }
        
        //Force FNA to read the content from the ZIP (main OBB) if possible (custom built / new enough).
        string obbMainPath = GetMainObbPath();
        if (File.Exists(obbMainPath)) {
            Environment.SetEnvironmentVariable("FNA_CONTENT_FORCE_ZIP", obbMainPath);
            
            string obbPatchPath = GetPatchObbPath();
            //Unzip patch OBB if available and version differs
            //TODO
        }
    }
    
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
