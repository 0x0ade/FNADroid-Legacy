// A random JNI tool (tm) generated that check. We don't need it.
//#ifndef _Included_com_angelde_fnadroid_FNADroidWrapper
//#define _Included_com_angelde_fnadroid_FNADroidWrapper

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

//j to c
#ifndef FNADROID_DESKTOP

JNIEXPORT void JNICALL Java_com_angelde_fnadroid_FNADroidWrapper_onCreate(JNIEnv* env, jclass cls) {
}

JNIEXPORT void JNICALL Java_com_angelde_fnadroid_FNADroidWrapper_onStart(JNIEnv* env, jclass cls) {
}

JNIEXPORT void JNICALL Java_com_angelde_fnadroid_FNADroidWrapper_onPause(JNIEnv* env, jclass cls) {
}

JNIEXPORT void JNICALL Java_com_angelde_fnadroid_FNADroidWrapper_onResume(JNIEnv* env, jclass cls) {
}

JNIEXPORT void JNICALL Java_com_angelde_fnadroid_FNADroidWrapper_onStop(JNIEnv* env, jclass cls) {
	mono_jit_cleanup(domain);
}

JNIEXPORT void JNICALL Java_com_angelde_fnadroid_FNADroidWrapper_setMonoDirs(JNIEnv* env, jclass cls, jstring jsLib, jstring jsEtc) {
    const char* lib = (*env)->GetStringUTFChars(env, jsLib, 0);
    const char* etc = (*env)->GetStringUTFChars(env, jsEtc, 0);

    mono_set_dirs(lib, etc);

    (*env)->ReleaseStringUTFChars(env, jsLib, lib);
    (*env)->ReleaseStringUTFChars(env, jsEtc, etc);
}

JNIEXPORT void JNICALL Java_com_angelde_fnadroid_FNADroidWrapper_setGameDir(JNIEnv* env, jclass cls, jstring jsTo) {
    const char* to = (*env)->GetStringUTFChars(env, jsTo, 0);

    fnadir = strdup(to);

    (*env)->ReleaseStringUTFChars(env, jsTo, to);
}

JNIEXPORT void JNICALL Java_com_angelde_fnadroid_FNADroidWrapper_setHomeDir(JNIEnv* env, jclass cls, jstring jsTo) {
    const char* to = (*env)->GetStringUTFChars(env, jsTo, 0);

    homedir = strdup(to);

    (*env)->ReleaseStringUTFChars(env, jsTo, to);
}

MonoMethod* oadc;
JNIEXPORT void JNICALL Java_com_angelde_fnadroid_FNADroidWrapper_onAccelerometerDataChanged(JNIEnv* env, jclass cls) {
    if (fnadroid && !oadc) {
        MonoMethodDesc* oadcDesc = mono_method_desc_new("FNADroid.Accelerometer:OnDataChanged()", true);
        oadc = mono_method_desc_search_in_image(oadcDesc, fnadroidi);
    }
    if (oadc) {
        mono_runtime_invoke(oadc, NULL, NULL, NULL);
    }
}

MonoMethod* orrc;
JNIEXPORT void JNICALL Java_com_angelde_fnadroid_FNADroidWrapper_onGyroscopeRotationRateChanged(JNIEnv* env, jclass cls) {
    if (fnadroid && !orrc) {
        MonoMethodDesc* orrcDesc = mono_method_desc_new("FNADroid.Gyroscope:OnRotationRateChanged()", true);
        orrc = mono_method_desc_search_in_image(orrcDesc, fnadroidi);
    }
    if (orrc) {
        mono_runtime_invoke(orrc, NULL, NULL, NULL);
    }
}

JNIEXPORT void JNICALL Java_com_angelde_fnadroid_FNADroidWrapper_attachThread(JNIEnv* env, jclass cls) {
	mono_thread_attach(domain);
}

#endif // FNADROID_DESKTOP
//#endif // _Included_com_angelde_fnadroid_FNADroidWrapper
