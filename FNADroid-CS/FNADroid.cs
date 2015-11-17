using System;
using System.Runtime.InteropServices;

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
        PrintInfo("Hello Info!");
        PrintWarn("Hello Warn!");
        PrintError("Hello Error!");
        Popup("Hello Popup Title!", "Hello Popup Message!");
        PopupDebug("Hello Debug!");
        PopupError("Hello Error Again!");
    }
    
}
