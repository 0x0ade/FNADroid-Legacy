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

import java.io.*;
import java.net.URL;
import java.net.URLConnection;
import java.nio.channels.Channels;
import java.nio.channels.FileChannel;
import java.nio.channels.ReadableByteChannel;
import java.util.Enumeration;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

public class FNADroidWrapper {
    private FNADroidWrapper() {}

    //variables the FNADroid user / publisher should replace
    public final static String OBB_MAIN_TITLE = "Mono";
    public final static int OBB_MAIN_VERSION = 1;
    public final static String OBB_MAIN_URL = "https://www.dropbox.com/s/7xnlpcyr2gbiyhp/mono-bin.zip?raw=1";
    public final static int OBB_PATCH_VERSION = 1;
    public final static String OBB_PATCH_TITLE = "Escape From Minimalism";
    public final static String OBB_PATCH_URL = "https://www.dropbox.com/s/4fx97zk56xz5wrp/EscapeFromMinimalism.zip?raw=1";

    //main wrapper

    private static byte[] ioBuffer = new byte[2048];
    public static boolean obbUseFallbackPath = false;

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

        Log.i("FNADroid", "Setting Mono dir to " + monodir.getAbsolutePath());
        setMonoDirs(fixPath(new File(monodir, "lib").getAbsolutePath()), fixPath(new File(monodir, "etc").getAbsolutePath()));
        Log.i("FNADroid", "Setting game dir to " + gamedir.getAbsolutePath());
        setGameDir(fixPath(gamedir.getAbsolutePath()));
        Log.i("FNADroid", "Setting home dir to " + homedir.getAbsolutePath());
        setHomeDir(fixPath(homedir.getAbsolutePath()));

        File out = new File(gamedir, "FNADroid-CS.dll");

        if (out.exists()) {
            return;
        }

        InputStream is = null;
        FileOutputStream fos = null;
        try {
            is = context.getAssets().open("FNADroid-CS.dll");
            fos = new FileOutputStream(out);

            int read;
            while ((read = is.read(ioBuffer)) != -1) {
                fos.write(ioBuffer, 0, read);
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            closeSilently(is);
            closeSilently(fos);
        }
    }

    /**
     * Called from SDLMain thread.
     */
    public static void hookedBoot() {
        extractObb(getMainObbPath());
        extractObb(getPatchObbPath());
    }

    public static String fixPath(String path) {
        return path.replace("/storage/emulated/0", "/sdcard");
    }

    public static String getObbPath(String obb, int version) {
        File file = new File(Environment.getExternalStorageDirectory() + "/Android/obb/" + context.getPackageName());
        if (!file.exists() && !obbUseFallbackPath) {
            Log.i("FNADroid", "Creating folder " + file.getAbsolutePath());
            obbUseFallbackPath = !file.mkdirs();
            if (obbUseFallbackPath) {
                Log.i("FNADroid", "Failed - using fallback path...");
            }
        }

        if (obbUseFallbackPath) {
            file = context.getExternalFilesDir(null);
            if (file.getName().equals("files")) {
                file = file.getParentFile();
            }
            file = new File(file, "obb-fallback");
            if (!file.exists()) {
                file.mkdirs();
            }
        }

        file = new File(file, obb + "." + version + "." + context.getPackageName() + ".obb");

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
        return getObbPath("main", OBB_MAIN_VERSION);
    }
    public static String getPatchObbPath() {
        return getObbPath("patch", OBB_PATCH_VERSION);
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

    public static void downloadObb(String path, final String type) {
        String url_ = "";
        String title_ = "";
        if (type.equals("main")) {
            url_ = OBB_MAIN_URL;
            title_ = OBB_MAIN_TITLE;
        } else if (type.equals("patch")) {
            url_ = OBB_PATCH_URL;
            title_ = OBB_PATCH_TITLE;
        }

        if (url_.isEmpty()) {
            Log.w("FNADroid", type + " obb URL is empty!");
            return;
        }

        final String title = title_;

        //For those who really need it: Feel free to implement loading OBBs from Play Store here.
        //Actually, that would be in FNADroid, but loading from a non-Play-Store path universally
        //offers one huge profit: Non-Play-Store-Stores (Humble, Amazon) will work, too!

        File obb = new File(path);

        Log.i("FNADroid", type + " download destination: " + obb.getAbsolutePath());

        final SimpleProgressAlert alert = new SimpleProgressAlert(context);
        context.runOnUiThread(new Runnable() {
            public void run() {
                alert.text.setText("Downloading " + title);
                alert.progress.setProgress(0);
            }
        });

        URL url;
        URLConnection connection;
        ReadableByteChannel rbc = null;
        FileOutputStream fos = null;
        FileChannel foc = null;
        try {
            Log.i("FNADroid", "Downloading to " + path);
            url = new URL(url_);
            connection = url.openConnection();
            rbc = Channels.newChannel(connection.getInputStream());
            fos = new FileOutputStream(obb);
            foc = fos.getChannel();

            long expected = connection.getContentLength();
            Log.i("FNADroid", "expected size: " + expected);
            long size = expected;
            int progressScale = 1;
            while (size > Integer.MAX_VALUE) {
                progressScale *= 10;
                size = expected / progressScale;
            }
            final long fsize = size;
            final int fprogressScale = progressScale;

            context.runOnUiThread(new Runnable() {
                public void run() {
                    alert.progress.setMax((int) fsize);
                }
            });

            final long[] received = {0, 0};
            while (received[0] < expected) {
                received[0] += foc.transferFrom(rbc, received[0], 2048);
                if (received[1] == 0) {
                    received[1] = 1;
                    context.runOnUiThread(new Runnable() {
                        public void run() {
                            alert.progress.setProgress((int) (received[0] / fprogressScale));
                            received[1] = 0;
                        }
                    });
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            closeSilently(rbc);
            closeSilently(foc);
            closeSilently(fos);
        }

        alert.alert.cancel();
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

        for (int i = 0; i < version; i++) {
            new File(getObbPath(type, i)).delete();
        }

        if (!obb.exists()) {
            downloadObb(path, type);
        }
        if (!obb.exists()) {
            return;
        }

        final SimpleProgressAlert alert = new SimpleProgressAlert(context);
        final boolean[] alertLock = {false};

        ZipFile zip = null;
        try {
            zip = new ZipFile(obb);
            final int count = zip.size();
            int index = 0;
            for (Enumeration<? extends ZipEntry> entries = zip.entries(); entries.hasMoreElements();) {
                final ZipEntry entry = entries.nextElement();
                final int findex = index;

                Log.i("FNADroid", "Extracting " + index + " / " + count + ": " + entry.getName());

                long esize = entry.getSize();
                long size = esize;
                int progressScale = 1;
                while (size > Integer.MAX_VALUE) {
                    progressScale *= 10;
                    size = esize / progressScale;
                }
                final long fsize = size;
                final int fprogressScale = progressScale;

                if (!alertLock[0]) {
                    alertLock[0] = true;
                    context.runOnUiThread(new Runnable() {
                        public void run() {
                            alert.text.setText(findex + " / " + count + ": " + entry.getName());
                            alert.progress.setProgress(0);
                            alert.progress.setMax((int) fsize);
                            alertLock[0] = false;
                        }
                    });
                }

                File out;
                String entryName = entry.getName();
                if (entryName.startsWith("mono/")) {
                    out = new File(dir, entryName);
                } else {
                    out = new File(gamedir, entryName);
                }

                if (entry.isDirectory()) {
                    out.mkdirs();
                    continue;
                }

                if (out.exists()) {
                    long existingSize;
                    RandomAccessFile raf = null;
                    try {
                        raf = new RandomAccessFile(out, "r");
                        existingSize = raf.length();
                    } catch (IOException e_) {
                        throw e_;
                    } finally {
                        closeSilently(raf);
                    }
                    raf.close();

                    if (existingSize == esize) {
                        continue;
                    }
                    out.delete();
                }
                out.createNewFile();

                InputStream is = null;
                FileOutputStream fos = null;

                try {
                    is = zip.getInputStream(entry);
                    fos = new FileOutputStream(out);

                    final long[] readCompletely = {0, 0};
                    int read;
                    while ((read = is.read(ioBuffer)) != -1) {
                        fos.write(ioBuffer, 0, read);
                        readCompletely[0] += read;
                        if (readCompletely[1] == 0) {
                            readCompletely[1] = 1;
                            context.runOnUiThread(new Runnable() {
                                public void run() {
                                    alert.progress.setProgress((int) (readCompletely[0] / fprogressScale));
                                    readCompletely[1] = 0;
                                }
                            });
                        }
                    }
                } catch (IOException e_) {
                    throw e_;
                } finally {
                    closeSilently(is);
                    closeSilently(fos);
                }


                index++;
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            closeSilently(zip);
        }

        alert.alert.cancel();
    }

}
