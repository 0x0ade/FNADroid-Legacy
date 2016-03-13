//FIXME update to ndk 10e+
/*
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
#endif

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/threads.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/utils/mono-logger.h>
*/

//cpp to j

#ifndef FNADROID_DESKTOP
jclass com_angelde_fnadroid_FNADroidWrapper;
jclass getFNADroidClass() {
    if (com_angelde_fnadroid_FNADroidWrapper == NULL) {
        com_angelde_fnadroid_FNADroidWrapper = jnienv->FindClass("com/angelde/fnadroid/FNADroidWrapper");
    }
    return com_angelde_fnadroid_FNADroidWrapper;
}

#define DEFMID(N) jmethodID N ## ID;
#define GETMID(N,T) if (! N ## ID) N ## ID = jnienv->GetStaticMethodID(clazz, #N , T ); jmethodID mID = N ## ID ;

#else
#define DEFMID(N)
#define GETMID(N,T)
#endif

DEFMID(showMsg)
void showMsg(const char* title, const char* msg) {
#ifndef FNADROID_DESKTOP
    jstring jsTitle = jnienv->NewStringUTF(title);
    jstring jsMsg = jnienv->NewStringUTF(msg);
    jclass clazz = getFNADroidClass();
    GETMID(showMsg, "(Ljava/lang/String;Ljava/lang/String;)V")
    jnienv->CallStaticVoidMethod(clazz, mID, jsTitle, jsMsg);
    jnienv->DeleteLocalRef(jsTitle);
    jnienv->DeleteLocalRef(jsMsg);
#endif
}
DEFMID(showDebug)
void showDebug(const char* msg) {
#ifndef FNADROID_DESKTOP
    jstring jsMsg = jnienv->NewStringUTF(msg);
    jclass clazz = getFNADroidClass();
    GETMID(showDebug, "(Ljava/lang/String;)V")
    jnienv->CallStaticVoidMethod(clazz, mID, jsMsg);
    jnienv->DeleteLocalRef(jsMsg);
#endif
}
DEFMID(showError)
void showError(const char* msg) {
#ifndef FNADROID_DESKTOP
    jstring jsMsg = jnienv->NewStringUTF(msg);
    jclass clazz = getFNADroidClass();
    GETMID(showError, "(Ljava/lang/String;)V")
    jnienv->CallStaticVoidMethod(clazz, mID, jsMsg);
    jnienv->DeleteLocalRef(jsMsg);
#endif
}

char* getGamePath() {
    return strdup(fnadir);
}
char* getHomePath() {
    return strdup(homedir);
}
DEFMID(getPackageName)
char* getPackageName() {
#ifndef FNADROID_DESKTOP
    jclass clazz = getFNADroidClass();
    GETMID(getPackageName, "()Ljava/lang/String;")
    jstring jsVal = (jstring) jnienv->CallStaticObjectMethod(clazz, mID);
    const char* val = jnienv->GetStringUTFChars(jsVal, 0);
    char* valdup = strdup(val); //TODO is that duplication required?!
    jnienv->ReleaseStringUTFChars(jsVal, val);
    jnienv->DeleteLocalRef(jsVal);
    return valdup;
#else
    return strdup("com.angelde.fnadroid.desktop");
#endif
}
DEFMID(getDataPath)
char* getDataPath() {
#ifndef FNADROID_DESKTOP
    jclass clazz = getFNADroidClass();
    GETMID(getDataPath, "()Ljava/lang/String;")
    jstring jsVal = (jstring) jnienv->CallStaticObjectMethod(clazz, mID);
    const char* val = jnienv->GetStringUTFChars(jsVal, 0);
    char* valdup = strdup(val); //TODO is that duplication required?!
    jnienv->ReleaseStringUTFChars(jsVal, val);
    jnienv->DeleteLocalRef(jsVal);
    return valdup;
#else
    return strdup(".");
#endif
}
DEFMID(getMainObbPath)
char* getMainObbPath() {
#ifndef FNADROID_DESKTOP
    jclass clazz = getFNADroidClass();
    GETMID(getMainObbPath, "()Ljava/lang/String;");
    jstring jsVal = (jstring) jnienv->CallStaticObjectMethod(clazz, mID);
    const char* val = jnienv->GetStringUTFChars(jsVal, 0);
    char* valdup = strdup(val); //TODO is that duplication required?!
    jnienv->ReleaseStringUTFChars(jsVal, val);
    jnienv->DeleteLocalRef(jsVal);
    return valdup;
#else
    return strdup("main.obb");
#endif
}
DEFMID(getPatchObbPath)
char* getPatchObbPath() {
#ifndef FNADROID_DESKTOP
    jclass clazz = getFNADroidClass();
    GETMID(getPatchObbPath, "()Ljava/lang/String;")
    jstring jsVal = (jstring) jnienv->CallStaticObjectMethod(clazz, mID);
    const char* val = jnienv->GetStringUTFChars(jsVal, 0);
    char* valdup = strdup(val); //TODO is that duplication required?!
    jnienv->ReleaseStringUTFChars(jsVal, val);
    jnienv->DeleteLocalRef(jsVal);
    return valdup;
#else
    return strdup("patch.obb");
#endif
}
DEFMID(getInstallerPackageName)
char* getInstallerPackageName() {
#ifndef FNADROID_DESKTOP
    jclass clazz = getFNADroidClass();
    GETMID(getInstallerPackageName, "()Ljava/lang/String;")
    jstring jsVal = (jstring) jnienv->CallStaticObjectMethod(clazz, mID);
    const char* val = jnienv->GetStringUTFChars(jsVal, 0);
    char* valdup = strdup(val); //TODO is that duplication required?!
    jnienv->ReleaseStringUTFChars(jsVal, val);
    jnienv->DeleteLocalRef(jsVal);
    return valdup;
#else
    return strdup("unknown");
#endif
}
DEFMID(getMaximumGLES)
int getMaximumGLES() {
#ifndef FNADROID_DESKTOP
    jclass clazz = getFNADroidClass();
    GETMID(getMaximumGLES, "()I")
    return (int) jnienv->CallStaticIntMethod(clazz, mID);
#else
    return 0x00020000;
#endif
}

DEFMID(vibrationCancel)
void vibrationCancel() {
#ifndef FNADROID_DESKTOP
    jclass clazz = getFNADroidClass();
    GETMID(vibrationCancel, "()V");
    jnienv->CallStaticVoidMethod(clazz, mID);
#endif
}
DEFMID(vibrationAvailable)
bool vibrationAvailable() {
#ifndef FNADROID_DESKTOP
    jclass clazz = getFNADroidClass();
    GETMID(vibrationAvailable, "()Z")
    return (bool) jnienv->CallStaticBooleanMethod(clazz, mID);
#else
    return false;
#endif
}
DEFMID(vibrate)
void vibrate(long long milliseconds) {
#ifndef FNADROID_DESKTOP
    jclass clazz = getFNADroidClass();
    GETMID(vibrate, "(J)V")
    jnienv->CallStaticVoidMethod(clazz, mID, (jlong) milliseconds);
#endif
}

DEFMID(accelerometerAvailable)
bool accelerometerAvailable() {
#ifndef FNADROID_DESKTOP
    jclass clazz = getFNADroidClass();
    GETMID(accelerometerAvailable, "()Z")
    return (bool) jnienv->CallStaticBooleanMethod(clazz, mID);
#else
    return false;
#endif
}
DEFMID(getAccelerometerAxis)
float getAccelerometerAxis(int axis) {
#ifndef FNADROID_DESKTOP
    jclass clazz = getFNADroidClass();
    GETMID(getAccelerometerAxis, "(I)F")
    return (float) jnienv->CallStaticFloatMethod(clazz, mID, (jint) axis);
#else
    return 0;
#endif
}

DEFMID(gyroscopeAvailable)
bool gyroscopeAvailable() {
#ifndef FNADROID_DESKTOP
    jclass clazz = getFNADroidClass();
    GETMID(gyroscopeAvailable, "()Z")
    return (bool) jnienv->CallStaticBooleanMethod(clazz, mID);
#else
    return false;
#endif
}
DEFMID(getGyroscopeRotationRateAxis)
float getGyroscopeRotationRateAxis(int axis) {
#ifndef FNADROID_DESKTOP
    jclass clazz = getFNADroidClass();
    GETMID(getGyroscopeRotationRateAxis, "(I)F")
    return (float) jnienv->CallStaticFloatMethod(clazz, mID, (jint) axis);
#else
    return 0;
#endif
}

DEFMID(getMaximumTouchCount)
int getMaximumTouchCount() {
#ifndef FNADROID_DESKTOP
    jclass clazz = getFNADroidClass();
    GETMID(getMaximumTouchCount, "()I")
    return (int) jnienv->CallStaticIntMethod(clazz, mID);
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

//m to c, c to cpp
#ifdef __cplusplus
extern "C" {
#endif

void Log(const int level, const char* tag, const char* msg) {
#ifndef FNADROID_DESKTOP
    __android_log_print(level, tag, "%s", msg);
#else
    printf("[%i][%s] %s\n", level, tag, msg);
#endif
}

void Popup(const char* title, const char* msg) {
    showMsg(title, msg);
}
void PopupDebug(const char* msg) {
    showDebug(msg);
}
void PopupError(const char* msg) {
    showError(msg);
}

char* GetGamePath() {
    return getGamePath();
}
char* GetHomePath() {
    return getHomePath();
}
char* GetPackageName() {
    return getPackageName();
}
char* GetDataPath() {
    return getDataPath();
}
char* GetMainObbPath() {
    return getMainObbPath();
}
char* GetPatchObbPath() {
    return getPatchObbPath();
}
char* GetInstallerPackageName() {
    return getInstallerPackageName();
}
int GetMaximumGLES() {
    return getMaximumGLES();
}

void VibrationCancel() {
    vibrationCancel();
}
bool VibrationAvailable() {
    return vibrationAvailable();
}
void Vibrate(long long milliseconds) {
    vibrate(milliseconds);
}

bool AccelerometerAvailable() {
    return accelerometerAvailable();
}
float GetAccelerometerAxis(int axis) {
    return getAccelerometerAxis(axis);
}

bool GyroscopeAvailable() {
    return gyroscopeAvailable();
}
float GetGyroscopeRotationRateAxis(int axis) {
    return getGyroscopeRotationRateAxis(axis);
}

int GetMaximumTouchCount() {
    return getMaximumTouchCount();
}

#ifdef __cplusplus
}
#endif
