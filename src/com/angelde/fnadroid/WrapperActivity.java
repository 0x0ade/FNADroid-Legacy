package com.angelde.fnadroid;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorManager;
import android.os.Bundle;
import org.libsdl.app.SDLActivity;

public class WrapperActivity extends SDLActivity {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        FNADroidWrapper.context = this;
        FNADroidWrapper.boot();

        super.onCreate(savedInstanceState);

        FNADroidWrapper.onCreate();
    }

    @Override
    protected void onStart() {
        super.onStart();

        FNADroidWrapper.onStart();
    }

    @Override
    protected void onPause() {
        FNADroidWrapper.onPause();

        super.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();

        FNADroidWrapper.onResume();
    }

    @Override
    protected void onStop() {
        FNADroidWrapper.onStop();

        super.onStop();
    }

}