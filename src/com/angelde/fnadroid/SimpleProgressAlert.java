package com.angelde.fnadroid;

import android.app.Activity;
import android.app.AlertDialog;
import android.os.Looper;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.TextView;

public class SimpleProgressAlert {
    
    public AlertDialog alert;
    
    public TextView text;
    public ProgressBar progress;

    public SimpleProgressAlert(final Activity context) {
        boolean mainThread = Thread.currentThread() == Looper.getMainLooper().getThread();

        Runnable run = new Runnable() {
            @Override
            public void run() {
                LinearLayout layout = new LinearLayout(context);
                layout.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT));
                layout.setOrientation(LinearLayout.VERTICAL);
                layout.setPadding(32, 16, 32, 16);
                layout.addView(text = new TextView(context));
                text.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT));
                layout.addView(progress = new ProgressBar(context, null, android.R.attr.progressBarStyleHorizontal));
                progress.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT));
                progress.setIndeterminate(false);

                final AlertDialog alert_ = new AlertDialog.Builder(context).create();
                alert_.setView(layout);
                alert_.setCancelable(false);
                alert_.setCanceledOnTouchOutside(false);
                alert_.show();

                alert = alert_;
            }
        };

        if (mainThread) {
            run.run();
        } else {
            context.runOnUiThread(run);

            //this is called from the native thread; keep it hanging
            while (alert == null) {
                try {
                    Thread.sleep(100L);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

}
