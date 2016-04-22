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
    //mono_jit_set_aot_only(true); //TODO generate binaries on first run
    //mono_trace_set_level_string("debug"); //TODO check if we had some crashes in a given timespan
    domain = mono_jit_init_version("fnadroid-domain", "v4.0.30319");

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

#ifndef FNADROID_DESKTOP
    LOGI("Attaching Android UI thread to Mono...");
    jclass clazz = (*jnienv)->FindClass(jnienv, "com/angelde/fnadroid/FNADroidWrapper");
    jmethodID attachUIThreadID = (*jnienv)->GetStaticMethodID(jnienv, clazz, "attachUIThread", "()V");
    (*jnienv)->CallStaticVoidMethod(jnienv, clazz, attachUIThreadID);
    LOGI("Android UI thread should soon be attached to Mono.");
#endif

    LOGI("Executing managed code...");
    char *argv_[2];
    argv_[0] = strdup("FNADroid-Boot.exe");
    argv_[1] = NULL;
    int val = mono_jit_exec(domain, assembly, 1, argv_);
    LOGI("... and we're back!");
    return val;
}
