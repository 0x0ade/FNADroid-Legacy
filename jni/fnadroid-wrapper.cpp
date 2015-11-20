#include "fnadroid-wrapper.h"

#include <jni.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <android/sensor.h>
#include <android/log.h>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/threads.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/utils/mono-logger.h>

#define SDL_MAIN_HANDLED
#include <SDL_main.h>
#include <SDL.h>

//main embedded mono code

int SDL_main(int argc, char* argv[]) {
    chdir(fnadir);

    mono_config_parse(NULL);
    domain = mono_jit_init_version("fnadroid-domain", "v4.0.30319");

    mono_thread_attach(domain);

    mscorlib = mono_get_corlib();
    if (!mscorlib) {
        LOGE("mscorlib could not be loaded!");
        return -1;
    }

    fnadroid = mono_domain_assembly_open(domain, "FNADroid-CS.dll");
    if (fnadroid) {
        //FNADroid-CS.dll is a helper assembly containing helper code.
        //Games and FNADroid itself should not rely on it, but it's helpful in some cases.
        //For example, early versions of the mono binaries delivered with FNADroid fail to
        //pipe STDOUT (Console.Out) to logcat.
        fnadroidi = mono_assembly_get_image(fnadroid);
        MonoMethodDesc* runDesc = mono_method_desc_new("FNADroid:Boot()", true);
        MonoMethod* run = mono_method_desc_search_in_image(runDesc, fnadroidi);
        if (run) {
            mono_runtime_invoke(run, NULL, NULL, NULL);
        } else {
            LOGI("FNADroid-CS.dll found, but not Boot()");
        }
    }

    assembly = mono_domain_assembly_open(domain, "game.exe");
    if (!assembly) {
        LOGE("Assembly could not be loaded!");
        return -1;
    }

    char *argv_[2];
    argv_[0] = strdup("--android");
    argv_[1] = NULL;
    mono_jit_exec(domain, assembly, 1, argv_);
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

//j to c

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

#ifdef __cplusplus
}
#endif
#endif

//FIXME update to ndk 10e+
#include "fnadroid-glue.cpp"
