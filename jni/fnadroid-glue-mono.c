#include "fnadroid-wrapper.h"

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef FNADROID_DESKTOP
#include <jni.h>
#include <android/sensor.h>
#include <android/log.h>
#else
#include <string.h>
#include <stdbool.h>
#endif

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/threads.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/utils/mono-logger.h>

//m to c, c to j

#ifndef FNADROID_DESKTOP
jclass com_angelde_fnadroid_FNADroidWrapper;
jclass getFNADroidClass() {
    if (com_angelde_fnadroid_FNADroidWrapper == 0) {
        com_angelde_fnadroid_FNADroidWrapper = (*jnienv)->FindClass(jnienv, "com/angelde/fnadroid/FNADroidWrapper");
    }
    return com_angelde_fnadroid_FNADroidWrapper;
}

#define DEFMID(N) jmethodID N ## ID;
#define GETMID(N,T) if (! N ## ID) N ## ID = (*jnienv)->GetStaticMethodID(jnienv, clazz, #N , T ); jmethodID mID = N ## ID ;

#else
#define DEFMID(N)
#define GETMID(N,T)
#endif

DEFMID(showMsg)
void Popup(const char* title, const char* msg) {
#ifndef FNADROID_DESKTOP
    jstring jsTitle = (*jnienv)->NewStringUTF(jnienv, title);
    jstring jsMsg = (*jnienv)->NewStringUTF(jnienv, msg);
    jclass clazz = getFNADroidClass();
    GETMID(showMsg, "(Ljava/lang/String;Ljava/lang/String;)V")
    (*jnienv)->CallStaticVoidMethod(jnienv, clazz, mID, jsTitle, jsMsg);
    (*jnienv)->DeleteLocalRef(jnienv, jsTitle);
    (*jnienv)->DeleteLocalRef(jnienv, jsMsg);
#endif
}
DEFMID(showDebug)
void PopupDebug(const char* msg) {
#ifndef FNADROID_DESKTOP
    jstring jsMsg = (*jnienv)->NewStringUTF(jnienv, msg);
    jclass clazz = getFNADroidClass();
    GETMID(showDebug, "(Ljava/lang/String;)V")
    (*jnienv)->CallStaticVoidMethod(jnienv, clazz, mID, jsMsg);
    (*jnienv)->DeleteLocalRef(jnienv, jsMsg);
#endif
}
DEFMID(showError)
void PopupError(const char* msg) {
#ifndef FNADROID_DESKTOP
    jstring jsMsg = (*jnienv)->NewStringUTF(jnienv, msg);
    jclass clazz = getFNADroidClass();
    GETMID(showError, "(Ljava/lang/String;)V")
    (*jnienv)->CallStaticVoidMethod(jnienv, clazz, mID, jsMsg);
    (*jnienv)->DeleteLocalRef(jnienv, jsMsg);
#endif
}

char* GetGamePath() {
    return strdup(fnadir);
}
char* GetHomePath() {
    return strdup(homedir);
}
DEFMID(getPackageName)
char* GetPackageName() {
#ifndef FNADROID_DESKTOP
    jclass clazz = getFNADroidClass();
    GETMID(getPackageName, "()Ljava/lang/String;")
    jstring jsVal = (jstring) (*jnienv)->CallStaticObjectMethod(jnienv, clazz, mID);
    const char* val = (*jnienv)->GetStringUTFChars(jnienv, jsVal, 0);
    char* valdup = strdup(val); //TODO is that duplication required?!
    (*jnienv)->ReleaseStringUTFChars(jnienv, jsVal, val);
    (*jnienv)->DeleteLocalRef(jnienv, jsVal);
    return valdup;
#else
    return strdup("com.angelde.fnadroid.desktop");
#endif
}
DEFMID(getDataPath)
char* GetDataPath() {
#ifndef FNADROID_DESKTOP
    jclass clazz = getFNADroidClass();
    GETMID(getDataPath, "()Ljava/lang/String;")
    jstring jsVal = (jstring) (*jnienv)->CallStaticObjectMethod(jnienv, clazz, mID);
    const char* val = (*jnienv)->GetStringUTFChars(jnienv, jsVal, 0);
    char* valdup = strdup(val); //TODO is that duplication required?!
    (*jnienv)->ReleaseStringUTFChars(jnienv, jsVal, val);
    (*jnienv)->DeleteLocalRef(jnienv, jsVal);
    return valdup;
#else
    return strdup(".");
#endif
}
DEFMID(getMainObbPath)
char* GetMainObbPath() {
#ifndef FNADROID_DESKTOP
    jclass clazz = getFNADroidClass();
    GETMID(getMainObbPath, "()Ljava/lang/String;");
    jstring jsVal = (jstring) (*jnienv)->CallStaticObjectMethod(jnienv, clazz, mID);
    const char* val = (*jnienv)->GetStringUTFChars(jnienv, jsVal, 0);
    char* valdup = strdup(val); //TODO is that duplication required?!
    (*jnienv)->ReleaseStringUTFChars(jnienv, jsVal, val);
    (*jnienv)->DeleteLocalRef(jnienv, jsVal);
    return valdup;
#else
    return strdup("main.obb");
#endif
}
DEFMID(getPatchObbPath)
char* GetPatchObbPath() {
#ifndef FNADROID_DESKTOP
    jclass clazz = getFNADroidClass();
    GETMID(getPatchObbPath, "()Ljava/lang/String;")
    jstring jsVal = (jstring) (*jnienv)->CallStaticObjectMethod(jnienv, clazz, mID);
    const char* val = (*jnienv)->GetStringUTFChars(jnienv, jsVal, 0);
    char* valdup = strdup(val); //TODO is that duplication required?!
    (*jnienv)->ReleaseStringUTFChars(jnienv, jsVal, val);
    (*jnienv)->DeleteLocalRef(jnienv, jsVal);
    return valdup;
#else
    return strdup("patch.obb");
#endif
}
DEFMID(getInstallerPackageName)
char* GetInstallerPackageName() {
#ifndef FNADROID_DESKTOP
    jclass clazz = getFNADroidClass();
    GETMID(getInstallerPackageName, "()Ljava/lang/String;")
    jstring jsVal = (jstring) (*jnienv)->CallStaticObjectMethod(jnienv, clazz, mID);
    const char* val = (*jnienv)->GetStringUTFChars(jnienv, jsVal, 0);
    char* valdup = strdup(val); //TODO is that duplication required?!
    (*jnienv)->ReleaseStringUTFChars(jnienv, jsVal, val);
    (*jnienv)->DeleteLocalRef(jnienv, jsVal);
    return valdup;
#else
    return strdup("unknown");
#endif
}
DEFMID(getMaximumGLES)
int GetMaximumGLES() {
#ifndef FNADROID_DESKTOP
    jclass clazz = getFNADroidClass();
    GETMID(getMaximumGLES, "()I")
    return (int) (*jnienv)->CallStaticIntMethod(jnienv, clazz, mID);
#else
    return 0x00020000;
#endif
}

DEFMID(vibrationCancel)
void VibrationCancel() {
#ifndef FNADROID_DESKTOP
    jclass clazz = getFNADroidClass();
    GETMID(vibrationCancel, "()V");
    (*jnienv)->CallStaticVoidMethod(jnienv, clazz, mID);
#endif
}
DEFMID(vibrationAvailable)
bool VibrationAvailable() {
#ifndef FNADROID_DESKTOP
    jclass clazz = getFNADroidClass();
    GETMID(vibrationAvailable, "()Z")
    return (bool) (*jnienv)->CallStaticBooleanMethod(jnienv, clazz, mID);
#else
    return false;
#endif
}
DEFMID(vibrate)
void Vibrate(long long milliseconds) {
#ifndef FNADROID_DESKTOP
    jclass clazz = getFNADroidClass();
    GETMID(vibrate, "(J)V")
    (*jnienv)->CallStaticVoidMethod(jnienv, clazz, mID, (jlong) milliseconds);
#endif
}

DEFMID(accelerometerAvailable)
bool AccelerometerAvailable() {
#ifndef FNADROID_DESKTOP
    jclass clazz = getFNADroidClass();
    GETMID(accelerometerAvailable, "()Z")
    return (bool) (*jnienv)->CallStaticBooleanMethod(jnienv, clazz, mID);
#else
    return false;
#endif
}
DEFMID(getAccelerometerAxis)
float GetAccelerometerAxis(int axis) {
#ifndef FNADROID_DESKTOP
    jclass clazz = getFNADroidClass();
    GETMID(getAccelerometerAxis, "(I)F")
    return (float) (*jnienv)->CallStaticFloatMethod(jnienv, clazz, mID, (jint) axis);
#else
    return 0;
#endif
}

DEFMID(gyroscopeAvailable)
bool GyroscopeAvailable() {
#ifndef FNADROID_DESKTOP
    jclass clazz = getFNADroidClass();
    GETMID(gyroscopeAvailable, "()Z")
    return (bool) (*jnienv)->CallStaticBooleanMethod(jnienv, clazz, mID);
#else
    return false;
#endif
}
DEFMID(getGyroscopeRotationRateAxis)
float GetGyroscopeRotationRateAxis(int axis) {
#ifndef FNADROID_DESKTOP
    jclass clazz = getFNADroidClass();
    GETMID(getGyroscopeRotationRateAxis, "(I)F")
    return (float) (*jnienv)->CallStaticFloatMethod(jnienv, clazz, mID, (jint) axis);
#else
    return 0;
#endif
}

DEFMID(getMaximumTouchCount)
int GetMaximumTouchCount() {
#ifndef FNADROID_DESKTOP
    jclass clazz = getFNADroidClass();
    GETMID(getMaximumTouchCount, "()I")
    return (int) (*jnienv)->CallStaticIntMethod(jnienv, clazz, mID);
#else
    return 0;
#endif
}

#ifdef DEFMID
#undef DEFMID
#endif
#ifdef GETMID
#undef GETMID
#endif

void Log(const int level, const char* tag, const char* msg) {
#ifndef FNADROID_DESKTOP
    __android_log_print(level, tag, "%s", msg);
#else
    printf("[%i][%s] %s\n", level, tag, msg);
#endif
}
