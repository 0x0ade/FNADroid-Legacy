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

void showMsg(const char* title, const char* msg) {
#ifndef FNADROID_DESKTOP
    jstring jsTitle = jnienv->NewStringUTF(title);
    jstring jsMsg = jnienv->NewStringUTF(msg);
    jclass clazz = jnienv->FindClass("com/angelde/fnadroid/FNADroidWrapper");
    jmethodID showMsgID = jnienv->GetStaticMethodID(clazz, "showMsg", "(Ljava/lang/String;Ljava/lang/String;)V");
    jnienv->CallStaticVoidMethod(clazz, showMsgID, jsTitle, jsMsg);
    jnienv->DeleteLocalRef(jsTitle);
    jnienv->DeleteLocalRef(jsMsg);
#endif
}
void showDebug(const char* msg) {
#ifndef FNADROID_DESKTOP
    jstring jsMsg = jnienv->NewStringUTF(msg);
    jclass clazz = jnienv->FindClass("com/angelde/fnadroid/FNADroidWrapper");
    jmethodID showMsgID = jnienv->GetStaticMethodID(clazz, "showDebug", "(Ljava/lang/String;)V");
    jnienv->CallStaticVoidMethod(clazz, showMsgID, jsMsg);
    jnienv->DeleteLocalRef(jsMsg);
#endif
}
void showError(const char* msg) {
#ifndef FNADROID_DESKTOP
    jstring jsMsg = jnienv->NewStringUTF(msg);
    jclass clazz = jnienv->FindClass("com/angelde/fnadroid/FNADroidWrapper");
    jmethodID showMsgID = jnienv->GetStaticMethodID(clazz, "showError", "(Ljava/lang/String;)V");
    jnienv->CallStaticVoidMethod(clazz, showMsgID, jsMsg);
    jnienv->DeleteLocalRef(jsMsg);
#endif
}

char* getGamePath() {
    return fnadir;
}
char* getHomePath() {
    return homedir;
}
char* getPackageName() {
#ifndef FNADROID_DESKTOP
    jclass clazz = jnienv->FindClass("com/angelde/fnadroid/FNADroidWrapper");
    jmethodID mID = jnienv->GetStaticMethodID(clazz, "getPackageName", "()Ljava/lang/String;");
    jstring jsVal = (jstring) jnienv->CallStaticObjectMethod(clazz, mID);
    const char* val = jnienv->GetStringUTFChars(jsVal, 0);
    char* valdup = strdup(val); //TODO is that duplication required?!
    jnienv->ReleaseStringUTFChars(jsVal, val);
    return valdup;
#else
    return strdup("com.angelde.fnadroid.desktop");
#endif
}
char* getDataPath() {
#ifndef FNADROID_DESKTOP
    jclass clazz = jnienv->FindClass("com/angelde/fnadroid/FNADroidWrapper");
    jmethodID mID = jnienv->GetStaticMethodID(clazz, "getDataPath", "()Ljava/lang/String;");
    jstring jsVal = (jstring) jnienv->CallStaticObjectMethod(clazz, mID);
    const char* val = jnienv->GetStringUTFChars(jsVal, 0);
    char* valdup = strdup(val); //TODO is that duplication required?!
    jnienv->ReleaseStringUTFChars(jsVal, val);
    return valdup;
#else
    return strdup(".");
#endif
}
char* getMainObbPath() {
#ifndef FNADROID_DESKTOP
    jclass clazz = jnienv->FindClass("com/angelde/fnadroid/FNADroidWrapper");
    jmethodID mID = jnienv->GetStaticMethodID(clazz, "getMainObbPath", "()Ljava/lang/String;");
    jstring jsVal = (jstring) jnienv->CallStaticObjectMethod(clazz, mID);
    const char* val = jnienv->GetStringUTFChars(jsVal, 0);
    char* valdup = strdup(val); //TODO is that duplication required?!
    jnienv->ReleaseStringUTFChars(jsVal, val);
    return valdup;
#else
    return strdup("main.obb");
#endif
}
char* getPatchObbPath() {
#ifndef FNADROID_DESKTOP
    jclass clazz = jnienv->FindClass("com/angelde/fnadroid/FNADroidWrapper");
    jmethodID mID = jnienv->GetStaticMethodID(clazz, "getPatchObbPath", "()Ljava/lang/String;");
    jstring jsVal = (jstring) jnienv->CallStaticObjectMethod(clazz, mID);
    const char* val = jnienv->GetStringUTFChars(jsVal, 0);
    char* valdup = strdup(val); //TODO is that duplication required?!
    jnienv->ReleaseStringUTFChars(jsVal, val);
    return valdup;
#else
    return strdup("patch.obb");
#endif
}
char* getInstallerPackageName() {
#ifndef FNADROID_DESKTOP
    jclass clazz = jnienv->FindClass("com/angelde/fnadroid/FNADroidWrapper");
    jmethodID mID = jnienv->GetStaticMethodID(clazz, "getInstallerPackageName", "()Ljava/lang/String;");
    jstring jsVal = (jstring) jnienv->CallStaticObjectMethod(clazz, mID);
    const char* val = jnienv->GetStringUTFChars(jsVal, 0);
    char* valdup = strdup(val); //TODO is that duplication required?!
    jnienv->ReleaseStringUTFChars(jsVal, val);
    return valdup;
#else
    return strdup("unknown");
#endif
}
int getMaximumGLES() {
#ifndef FNADROID_DESKTOP
    jclass clazz = jnienv->FindClass("com/angelde/fnadroid/FNADroidWrapper");
    jmethodID mID = jnienv->GetStaticMethodID(clazz, "getMaximumGLES", "()I");
    return (int) jnienv->CallStaticIntMethod(clazz, mID);
#else
    return 0x00020000;
#endif
}

void vibrationCancel() {
#ifndef FNADROID_DESKTOP
    jclass clazz = jnienv->FindClass("com/angelde/fnadroid/FNADroidWrapper");
    jmethodID mID = jnienv->GetStaticMethodID(clazz, "vibrationCancel", "()V");
    jnienv->CallStaticVoidMethod(clazz, mID);
#endif
}
bool vibrationAvailable() {
#ifndef FNADROID_DESKTOP
    jclass clazz = jnienv->FindClass("com/angelde/fnadroid/FNADroidWrapper");
    jmethodID mID = jnienv->GetStaticMethodID(clazz, "vibrationAvailable", "()Z");
    return (bool) jnienv->CallStaticBooleanMethod(clazz, mID);
#else
    return false;
#endif
}
void vibrate(long long milliseconds) {
#ifndef FNADROID_DESKTOP
    jclass clazz = jnienv->FindClass("com/angelde/fnadroid/FNADroidWrapper");
    jmethodID mID = jnienv->GetStaticMethodID(clazz, "vibrate", "(J)V");
    jnienv->CallStaticVoidMethod(clazz, mID, (jlong) milliseconds);
#endif
}

bool accelerometerAvailable() {
#ifndef FNADROID_DESKTOP
    jclass clazz = jnienv->FindClass("com/angelde/fnadroid/FNADroidWrapper");
    jmethodID mID = jnienv->GetStaticMethodID(clazz, "accelerometerAvailable", "()Z");
    return (bool) jnienv->CallStaticBooleanMethod(clazz, mID);
#else
    return false;
#endif
}
float getAccelerometerAxis(int axis) {
#ifndef FNADROID_DESKTOP
    jclass clazz = jnienv->FindClass("com/angelde/fnadroid/FNADroidWrapper");
    jmethodID mID = jnienv->GetStaticMethodID(clazz, "getAccelerometerAxis", "(I)F");
    return (float) jnienv->CallStaticFloatMethod(clazz, mID, (jint) axis);
#else
    return 0;
#endif
}

bool gyroscopeAvailable() {
#ifndef FNADROID_DESKTOP
    jclass clazz = jnienv->FindClass("com/angelde/fnadroid/FNADroidWrapper");
    jmethodID mID = jnienv->GetStaticMethodID(clazz, "gyroscopeAvailable", "()Z");
    return (bool) jnienv->CallStaticBooleanMethod(clazz, mID);
#else
    return false;
#endif
}
float getGyroscopeRotationRateAxis(int axis) {
#ifndef FNADROID_DESKTOP
    jclass clazz = jnienv->FindClass("com/angelde/fnadroid/FNADroidWrapper");
    jmethodID mID = jnienv->GetStaticMethodID(clazz, "getGyroscopeRotationRateAxis", "(I)F");
    return (float) jnienv->CallStaticFloatMethod(clazz, mID, (jint) axis);
#else
    return 0;
#endif
}

int getMaximumTouchCount() {
#ifndef FNADROID_DESKTOP
    jclass clazz = jnienv->FindClass("com/angelde/fnadroid/FNADroidWrapper");
    jmethodID mID = jnienv->GetStaticMethodID(clazz, "getMaximumTouchCount", "()I");
    return (int) jnienv->CallStaticIntMethod(clazz, mID);
#else
    return 0;
#endif
}

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
int j_GetMaximumGLES() {
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
