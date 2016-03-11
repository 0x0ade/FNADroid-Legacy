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

#define SDL_MAIN_HANDLED
#include <SDL_main.h>
#include <SDL.h>
#else
#include <string.h>
#endif

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/threads.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/utils/mono-logger.h>

//main embedded mono code

#ifndef FNADROID_DESKTOP
int SDL_main(int argc, char* argv[]) {
    chdir(fnadir);
#else
int fnadroid_boot() {
#endif
    mono_config_parse(NULL);
    domain = mono_jit_init("fnadroid-domain");

    mono_thread_attach(domain);

    mscorlib = mono_get_corlib();
    if (!mscorlib) {
        LOGE("mscorlib could not be loaded!");
        return -1;
    }

    fnadroid = mono_domain_assembly_open(domain, "FNADroid-Lib.dll");
    if (fnadroid) {
        //FNADroid-Lib.dll is a helper assembly containing helper code.
        //We need a reference to the assembly and the image to invoke methods in it from Java.
        fnadroidi = mono_assembly_get_image(fnadroid);
    } else {
        LOGI("FNADroid-Lib.dll not found");
    }

    assembly = mono_domain_assembly_open(domain, "FNADroid-Boot.exe");
    if (!assembly) {
        LOGE("Assembly could not be loaded!");
        return -1;
    }

    char *argv_[2];
    argv_[0] = strdup("--android");
    argv_[1] = NULL;
    LOGI("Executing managed code...");
    mono_jit_exec(domain, assembly, 1, argv_);
    LOGI("... aand we're back!");
    return 0;
}

//java activity and helper methods

void onCreate() {
}

void onStart() {
}

void onPause() {
}

void onResume() {
}

void onStop() {
    mono_jit_cleanup(domain);
}

#ifndef _Included_com_angelde_fnadroid_FNADroidWrapper
#define _Included_com_angelde_fnadroid_FNADroidWrapper
#ifdef __cplusplus
extern "C" {
#endif

//j to c
#ifndef FNADROID_DESKTOP

JNIEXPORT void JNICALL Java_com_angelde_fnadroid_FNADroidWrapper_onCreate(JNIEnv* env, jclass cls) {
	onCreate();
}

JNIEXPORT void JNICALL Java_com_angelde_fnadroid_FNADroidWrapper_onStart(JNIEnv* env, jclass cls) {
	onStart();
}

JNIEXPORT void JNICALL Java_com_angelde_fnadroid_FNADroidWrapper_onPause(JNIEnv* env, jclass cls) {
	onPause();
}

JNIEXPORT void JNICALL Java_com_angelde_fnadroid_FNADroidWrapper_onResume(JNIEnv* env, jclass cls) {
	onResume();
}

JNIEXPORT void JNICALL Java_com_angelde_fnadroid_FNADroidWrapper_onStop(JNIEnv* env, jclass cls) {
	onStop();
}

JNIEXPORT void JNICALL Java_com_angelde_fnadroid_FNADroidWrapper_setMonoDirs(JNIEnv* env, jclass cls, jstring jsLib, jstring jsEtc) {
    const char* lib = env->GetStringUTFChars(jsLib, 0);
    const char* etc = env->GetStringUTFChars(jsEtc, 0);

    mono_set_dirs(lib, etc);

    env->ReleaseStringUTFChars(jsLib, lib);
    env->ReleaseStringUTFChars(jsEtc, etc);
}

JNIEXPORT void JNICALL Java_com_angelde_fnadroid_FNADroidWrapper_setGameDir(JNIEnv* env, jclass cls, jstring jsTo) {
    const char* to = env->GetStringUTFChars(jsTo, 0);

    fnadir = strdup(to);

    env->ReleaseStringUTFChars(jsTo, to);
}

JNIEXPORT void JNICALL Java_com_angelde_fnadroid_FNADroidWrapper_setHomeDir(JNIEnv* env, jclass cls, jstring jsTo) {
    const char* to = env->GetStringUTFChars(jsTo, 0);

    homedir = strdup(to);

    env->ReleaseStringUTFChars(jsTo, to);
}

MonoMethod* oadc;
JNIEXPORT void JNICALL Java_com_angelde_fnadroid_FNADroidWrapper_onAccelerometerDataChanged(JNIEnv* env, jclass cls) {
    if (fnadroid && !oadc) {
        MonoMethodDesc* oadcDesc = mono_method_desc_new("FNADroid:OnAccelerometerDataChanged()", true);
        oadc = mono_method_desc_search_in_image(oadcDesc, fnadroidi);
    }
    if (oadc) {
        mono_runtime_invoke(oadc, NULL, NULL, NULL);
    }
}

#endif //ifndef FNADROID_DESKTOP

#ifdef __cplusplus
}
#endif
#endif

//FIXME update to ndk 10e+
#include "fnadroid-glue.cpp"
