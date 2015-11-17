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
    
    public static void Boot() {
        Console.WriteLine("Hello, World.. wherever this goes.");
        
        Console.SetOut(new LogcatWriter() {
            OnWrite = PrintInfo
        });
        
        Console.SetError(new LogcatWriter() {
            OnWrite = PrintError
        });
        
        Console.WriteLine("Hello, Logcat!");
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
