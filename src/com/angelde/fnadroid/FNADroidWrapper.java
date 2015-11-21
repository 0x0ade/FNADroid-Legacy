package com.angelde.fnadroid;

import android.app.ActivityManager;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Environment;
import android.os.Handler;
import android.os.Looper;
import android.os.Vibrator;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import java.io.Closeable;
import java.io.File;
import java.io.IOException;
import java.util.Enumeration;
import java.util.zip.ZipEntry;
import java.util.zip.ZipException;
import java.util.zip.ZipFile;

public class FNADroidWrapper {

    //modify these for your obb versions
    public final static int OBB_VERSION_MAIN = 1;
    public final static int OBB_VERSION_PATCH = 1;

    private FNADroidWrapper() {
    }

    static {
        //basic dependencies: FNA
        System.loadLibrary("openal");
        System.loadLibrary("SDL2");
        System.loadLibrary("mojoshader");
        System.loadLibrary("vorbis");
        //basic dependencies: Mono and the wrapper
        System.loadLibrary("monosgen-2.0");
        System.loadLibrary("fnadroid-wrapper");
        //add your custom dependencies here; obligatory FEZ example
        System.loadLibrary("ogg");
        //System.loadLibrary("vorbisfile"); //enc and file already included in precompiled vorbis
    }

    public static WrapperActivity context;

    //Java-side helpers
    public static void boot() {
        File dir = context.getExternalFilesDir(null);
        if (dir.getName().equals("files")) {
            dir = dir.getParentFile();
        }
        File monodir = new File(dir, "mono");
        File gamedir = new File(dir, "game");
        File homedir = new File(dir, "home");

        monodir.mkdirs();
        gamedir.mkdirs();
        homedir.mkdirs();

        //TODO put FNADroid-CS.dll to gamedir, mono to monodir

        Log.i("FNADroid", "Setting Mono dir to " + monodir.getAbsolutePath());
        setMonoDirs(fixPath(new File(monodir, "lib").getAbsolutePath()), fixPath(new File(monodir, "etc").getAbsolutePath()));
        Log.i("FNADroid", "Setting game dir to " + gamedir.getAbsolutePath());
        setGameDir(fixPath(gamedir.getAbsolutePath()));
        Log.i("FNADroid", "Setting home dir to " + homedir.getAbsolutePath());
        setHomeDir(fixPath(homedir.getAbsolutePath()));
    }

    public static String fixPath(String path) {
        return path.replace("/storage/emulated/0", "/sdcard");
    }

    public static String getObbPath(String obb, int version) {
        File file = new File(Environment.getExternalStorageDirectory() + "/Android/obb/" + context.getPackageName());
        file = new File(file, obb + "." + version + "." + context.getPackageName() + ".obb");
        //TODO check if we've downloaded the file / the app comes from the play store, otherwise return appropriately
        return fixPath(file.getAbsolutePath());
    }

    public static void closeSilently(Closeable closeable) {
        if (closeable == null) {
            return;
        }
        try {
            closeable.close();
        } catch (Exception e) {
        }
    }

    //j to cpp
    public native static void onCreate();
    public native static void onStart();
    public native static void onPause();
    public native static void onResume();
    public native static void onStop();
    private native static void setMonoDirs(String lib, String etc);
    private native static void setGameDir(String to);
    private native static void setHomeDir(String to);

    //cpp to j
    public static void showDebug(final String msg) {
        showMsg("FNADroid DEBUG", msg);
    }
    public static void showError(final String msg) {
        showMsg("FNADroid FATAL ERROR", msg);
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

    public static String getPackageName() {
        return context.getPackageName();
    }
    public static String getDataPath() {
        return context.getApplicationInfo().dataDir;
    }
    public static String getMainObbPath() {
        return getObbPath("main", OBB_VERSION_MAIN);
    }
    public static String getPatchObbPath() {
        return getObbPath("patch", OBB_VERSION_PATCH);
    }
    public static String getInstallerPackageName() {
        String s = context.getPackageManager().getInstallerPackageName(context.getPackageName());
        return s == null ? "unknown" : s;
    }
    public static boolean canGLES3() {
        return ((ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE)).getDeviceConfigurationInfo().reqGlEsVersion >= 0x30000;
    }

    public static void vibrationCancel() {
        ((Vibrator) context.getSystemService(Context.VIBRATOR_SERVICE)).cancel();
    }
    public static boolean vibrationAvailable() {
        return ((Vibrator) context.getSystemService(Context.VIBRATOR_SERVICE)).hasVibrator();
    }
    public static void vibrate(long milliseconds) {
        ((Vibrator) context.getSystemService(Context.VIBRATOR_SERVICE)).vibrate(milliseconds);
    }

    public static void extractObb(String path) {
        File dir = context.getExternalFilesDir(null);
        if (dir.getName().equals("files")) {
            dir = dir.getParentFile();
        }
        File gamedir = new File(dir, "game");
        File obb = new File(path);

        String[] split = obb.getName().split("\\.");
        String type = split[0];
        int version = Integer.parseInt(split[1]);

        final LinearLayout[] layout = {null};
        final TextView[] name = {null};
        final AlertDialog[] alert = {null};
        final ProgressBar[] progress = {null};
        context.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                layout[0] = new LinearLayout(context);
                layout[0].setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT));
                layout[0].setOrientation(LinearLayout.VERTICAL);
                layout[0].setPadding(32, 16, 32, 16);
                layout[0].addView(name[0] = new TextView(context));
                name[0].setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT));
                layout[0].addView(progress[0] = new ProgressBar(context, null, android.R.attr.progressBarStyleHorizontal));
                progress[0].setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT));
                progress[0].setIndeterminate(false);

                final AlertDialog alert_ = new AlertDialog.Builder(context).create();
                alert_.setView(layout[0]);
                alert_.setCancelable(false);
                alert_.setCanceledOnTouchOutside(false);
                alert_.show();

                alert[0] = alert_;
            }
        });

        //this is called from the native thread; keep it hanging
        while (alert[0] == null) {
            try {
                Thread.sleep(100L);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        ZipFile zip = null;
        try {
            zip = new ZipFile(obb);
            final int count = zip.size();
            int index = 0;
            for (Enumeration<? extends ZipEntry> entries = zip.entries(); entries.hasMoreElements();) {
                final ZipEntry entry = entries.nextElement();
                final int findex = index;

                Log.i("FNADroid", "Extracting " + index + " / " + count + ": " + entry.getName());

                long size = entry.getSize();
                int progressScale = 1;
                while (size > Integer.MAX_VALUE) {
                    progressScale *= 10;
                    size = entry.getSize() / progressScale;
                }
                final long fsize = size;

                context.runOnUiThread(new Runnable() {
                    public void run() {
                        name[0].setText(findex + " / " + count + ": " + entry.getName());
                        progress[0].setMax((int) fsize);
                    }
                });

                try {
                    Thread.sleep(1000l);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

                index++;
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            closeSilently(zip);
        }

    }

}
