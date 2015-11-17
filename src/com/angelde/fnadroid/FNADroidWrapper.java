package com.angelde.fnadroid;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.widget.Toast;

import java.io.File;

public class FNADroidWrapper {

    private FNADroidWrapper() {
    }

    static {
        //basic dependencies: FNA
        System.loadLibrary("openal");
        System.loadLibrary("SDL2");
        System.loadLibrary("mojoshader");
        //basic dependencies: Mono and the wrapper
        System.loadLibrary("monosgen-2.0");
        System.loadLibrary("fnadroid-wrapper");
        //add your custom dependencies here; obligatory FEZ example
        System.loadLibrary("ogg");
        System.loadLibrary("vorbis");
        //System.loadLibrary("vorbisfile"); //enc and file already included in precompiled vorbis
    }

    public static WrapperActivity context;

    public native static void onCreate();
    public native static void onStart();
    public native static void onPause();
    public native static void onResume();
    public native static void onStop();
    public native static void setMonoDirs(String lib, String etc);
    public native static void setGameDir(String to);

    public static void showError(final String msg) {
        showMsg("FNADroid FATAL ERROR", msg);
    }

    public static void showDebug(final String msg) {
        showMsg("FNADroid DEBUG", msg);
    }

    public static void showMsg(final String title, final String msg) {
        final AlertDialog[] alert = {null};
        context.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                final AlertDialog alert_ = new AlertDialog.Builder(context).create();
                alert_.setTitle(title);
                alert_.setMessage(msg);
                alert_.setButton(AlertDialog.BUTTON_NEUTRAL, "OK", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        alert_.dismiss();
                    }
                });
                alert_.show();
                alert[0] = alert_;
            }
        });
        //showMsg is called from the native thread; keep it hanging
        while (alert[0] == null || alert[0].isShowing()) {
            try {
                Thread.sleep(100L);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public static void setMonoDirsAuto() {
        File dir = context.getExternalFilesDir(null);
        if (dir == null) {
            return;
        }
        if (dir.getName().equals("files")) {
            dir = dir.getParentFile();
        }
        File monodir = new File(dir, "mono");
        monodir.mkdirs();
        File gamedir = new File(dir, "game");
        gamedir.mkdirs();
        new File(dir, "home").mkdirs();

        Log.i("FNADroid", "Setting Mono dir to " + monodir.getAbsolutePath());
        setMonoDirs(fixPath(new File(monodir, "lib").getAbsolutePath()), fixPath(new File(monodir, "etc").getAbsolutePath()));
        Log.i("FNADroid", "Setting game dir to " + gamedir.getAbsolutePath());
        setGameDir(fixPath(gamedir.getAbsolutePath()));
        //TODO also set the other dirs ("fake" home)
    }

    public static String fixPath(String path) {
        return path.replace("/storage/emulated/0", "/sdcard");
    }

}
