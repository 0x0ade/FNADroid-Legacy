package org.libsdl.app;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import android.app.*;
import android.content.*;
import android.view.*;
import android.view.inputmethod.BaseInputConnection;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputConnection;
import android.view.inputmethod.InputMethodManager;
import android.widget.AbsoluteLayout;
import android.os.*;
import android.util.Log;
import android.graphics.*;
import android.media.*;
import android.hardware.*;

/**
 SDLSurface. This is what we draw on, so we need to know when it's created
 in order to do anything useful.

 Because of this, that's where we set up the SDL thread
 */
public class SDLSurface extends SurfaceView implements SurfaceHolder.Callback,
        View.OnKeyListener, View.OnTouchListener, SensorEventListener {

    // Sensors
    protected static SensorManager mSensorManager;
    public static SensorManager getSensorManager() {
        return mSensorManager;
    }
    protected static Display mDisplay;

    // Keep track of the surface size to normalize touch events
    protected static float mWidth, mHeight;

    // Startup
    public SDLSurface(Context context) {
        super(context);
        getHolder().addCallback(this);

        setFocusable(true);
        setFocusableInTouchMode(true);
        requestFocus();
        setOnKeyListener(this);
        setOnTouchListener(this);

        mDisplay = ((WindowManager)context.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
        mSensorManager = (SensorManager)context.getSystemService(Context.SENSOR_SERVICE);

        if(Build.VERSION.SDK_INT >= 12) {
            setOnGenericMotionListener(new SDLGenericMotionListener_API12());
        }

        // Some arbitrary defaults to avoid a potential division by zero
        mWidth = 1.0f;
        mHeight = 1.0f;
    }

    public Surface getNativeSurface() {
        return getHolder().getSurface();
    }

    // Called when we have a valid drawing surface
    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Log.v("SDL", "surfaceCreated()");
        holder.setType(SurfaceHolder.SURFACE_TYPE_GPU);
    }

    // Called when we lose the surface
    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.v("SDL", "surfaceDestroyed()");
        // Call this *before* setting mIsSurfaceReady to 'false'
        SDLActivity.handlePause();
        SDLActivity.mIsSurfaceReady = false;
        SDLActivity.onNativeSurfaceDestroyed();
    }

    // Called when the surface is resized
    @Override
    public void surfaceChanged(SurfaceHolder holder,
                               int format, int width, int height) {
        Log.v("SDL", "surfaceChanged()");

        int sdlFormat = 0x15151002; // SDL_PIXELFORMAT_RGB565 by default
        switch (format) {
            case PixelFormat.A_8:
                Log.v("SDL", "pixel format A_8");
                break;
            case PixelFormat.LA_88:
                Log.v("SDL", "pixel format LA_88");
                break;
            case PixelFormat.L_8:
                Log.v("SDL", "pixel format L_8");
                break;
            case PixelFormat.RGBA_4444:
                Log.v("SDL", "pixel format RGBA_4444");
                sdlFormat = 0x15421002; // SDL_PIXELFORMAT_RGBA4444
                break;
            case PixelFormat.RGBA_5551:
                Log.v("SDL", "pixel format RGBA_5551");
                sdlFormat = 0x15441002; // SDL_PIXELFORMAT_RGBA5551
                break;
            case PixelFormat.RGBA_8888:
                Log.v("SDL", "pixel format RGBA_8888");
                sdlFormat = 0x16462004; // SDL_PIXELFORMAT_RGBA8888
                break;
            case PixelFormat.RGBX_8888:
                Log.v("SDL", "pixel format RGBX_8888");
                sdlFormat = 0x16261804; // SDL_PIXELFORMAT_RGBX8888
                break;
            case PixelFormat.RGB_332:
                Log.v("SDL", "pixel format RGB_332");
                sdlFormat = 0x14110801; // SDL_PIXELFORMAT_RGB332
                break;
            case PixelFormat.RGB_565:
                Log.v("SDL", "pixel format RGB_565");
                sdlFormat = 0x15151002; // SDL_PIXELFORMAT_RGB565
                break;
            case PixelFormat.RGB_888:
                Log.v("SDL", "pixel format RGB_888");
                // Not sure this is right, maybe SDL_PIXELFORMAT_RGB24 instead?
                sdlFormat = 0x16161804; // SDL_PIXELFORMAT_RGB888
                break;
            default:
                Log.v("SDL", "pixel format unknown " + format);
                break;
        }

        mWidth = width;
        mHeight = height;
        SDLActivity.onNativeResize(width, height, sdlFormat);
        Log.v("SDL", "Window size:" + width + "x"+height);

        // Set mIsSurfaceReady to 'true' *before* making a call to handleResume
        SDLActivity.mIsSurfaceReady = true;
        SDLActivity.onNativeSurfaceChanged();


        if (SDLActivity.mSDLThread == null) {
            // This is the entry point to the C app.
            // Start up the C app thread and enable sensor input for the first time

            SDLActivity.mSDLThread = new Thread(new SDLMain(), "SDLThread");
            enableSensor(Sensor.TYPE_ACCELEROMETER, true);
            com.angelde.fnadroid.FNADroidWrapper.hookedEnableAdditionalSensors(this, true);
            SDLActivity.mSDLThread.start();

            // Set up a listener thread to catch when the native thread ends
            new Thread(new Runnable(){
                @Override
                public void run(){
                    try {
                        SDLActivity.mSDLThread.join();
                    }
                    catch(Exception e){}
                    finally{
                        // Native thread has finished
                        if (! SDLActivity.mExitCalledFromJava) {
                            SDLActivity.handleNativeExit();
                        }
                    }
                }
            }).start();
        }
    }

    // unused
    @Override
    public void onDraw(Canvas canvas) {}


    // Key events
    @Override
    public boolean onKey(View  v, int keyCode, KeyEvent event) {
        // Dispatch the different events depending on where they come from
        // Some SOURCE_DPAD or SOURCE_GAMEPAD are also SOURCE_KEYBOARD
        // So, we try to process them as DPAD or GAMEPAD events first, if that fails we try them as KEYBOARD

        if ( (event.getSource() & 0x00000401) != 0 || /* API 12: SOURCE_GAMEPAD */
                (event.getSource() & InputDevice.SOURCE_DPAD) != 0 ) {
            if (event.getAction() == KeyEvent.ACTION_DOWN) {
                if (SDLActivity.onNativePadDown(event.getDeviceId(), keyCode) == 0) {
                    return true;
                }
            } else if (event.getAction() == KeyEvent.ACTION_UP) {
                if (SDLActivity.onNativePadUp(event.getDeviceId(), keyCode) == 0) {
                    return true;
                }
            }
        }

        if( (event.getSource() & InputDevice.SOURCE_KEYBOARD) != 0) {
            if (event.getAction() == KeyEvent.ACTION_DOWN) {
                //Log.v("SDL", "key down: " + keyCode);
                SDLActivity.onNativeKeyDown(keyCode);
                return true;
            }
            else if (event.getAction() == KeyEvent.ACTION_UP) {
                //Log.v("SDL", "key up: " + keyCode);
                SDLActivity.onNativeKeyUp(keyCode);
                return true;
            }
        }

        return false;
    }

    // Touch events
    @Override
    public boolean onTouch(View v, MotionEvent event) {
        /* Ref: http://developer.android.com/training/gestures/multi.html */
        final int touchDevId = event.getDeviceId();
        final int pointerCount = event.getPointerCount();
        int action = event.getActionMasked();
        int pointerFingerId;
        int i = -1;
        float x,y,p;

        switch(action) {
            case MotionEvent.ACTION_MOVE:
                for (i = 0; i < pointerCount; i++) {
                    pointerFingerId = event.getPointerId(i);
                    x = event.getX(i) / mWidth;
                    y = event.getY(i) / mHeight;
                    p = event.getPressure(i);
                    SDLActivity.onNativeTouch(touchDevId, pointerFingerId, action, x, y, p);
                }
                break;

            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_DOWN:
                // Primary pointer up/down, the index is always zero
                i = 0;
            case MotionEvent.ACTION_POINTER_UP:
            case MotionEvent.ACTION_POINTER_DOWN:
                // Non primary pointer up/down
                if (i == -1) {
                    i = event.getActionIndex();
                }

                pointerFingerId = event.getPointerId(i);
                x = event.getX(i) / mWidth;
                y = event.getY(i) / mHeight;
                p = event.getPressure(i);
                SDLActivity.onNativeTouch(touchDevId, pointerFingerId, action, x, y, p);
                break;

            default:
                break;
        }

        return true;
    }

    // Sensor events
    public void enableSensor(int sensortype, boolean enabled) {
        // TODO: This uses getDefaultSensor - what if we have >1 accels?
        Sensor sensor = mSensorManager.getDefaultSensor(sensortype);
        if (sensor == null) {
            // Nothing to do here - this device doesn't support the sensor.
            return;
        }
        if (enabled) {
            mSensorManager.registerListener(this,
                    sensor,
                    SensorManager.SENSOR_DELAY_GAME, null);
        } else {
            mSensorManager.unregisterListener(this,
                    sensor);
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
        // TODO
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
            float x, y;
            switch (mDisplay.getRotation()) {
                case Surface.ROTATION_90:
                    x = -event.values[1];
                    y = event.values[0];
                    break;
                case Surface.ROTATION_270:
                    x = event.values[1];
                    y = -event.values[0];
                    break;
                case Surface.ROTATION_180:
                    x = -event.values[1];
                    y = -event.values[0];
                    break;
                default:
                    x = event.values[0];
                    y = event.values[1];
                    break;
            }
            SDLActivity.onNativeAccel(-x / SensorManager.GRAVITY_EARTH,
                    y / SensorManager.GRAVITY_EARTH,
                    event.values[2] / SensorManager.GRAVITY_EARTH - 1);
        }
        com.angelde.fnadroid.FNADroidWrapper.hookedOnSensorChanged(event);
    }
}