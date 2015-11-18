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
        
        PopupDebug("Vibration available: " + VibrationAvailable());
        
        Vibrate(1000L);
        PopupDebug("Vibrating for 1s");
        
        Vibrate(30000L);
        PopupDebug("Vibration for 30s or cancel");
        VibrationCancel();
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
