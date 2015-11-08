#include <jni.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <android/sensor.h>
#include <android/log.h>

//#include <glib/glib.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/threads.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/utils/mono-logger.h>

#define SDL_MAIN_HANDLED
#include <SDL_main.h>
#include <SDL.h>

#include <AL/al.h>
#include <AL/alc.h>

#define  LOG_TAG    "fnadroid-wrapper"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

//mono side
MonoDomain* domain;
MonoAssembly* assembly;
MonoImage* mscorlib;

//java side
JNIEnv* jnienv;

//bridge / wrapper config
char* fnadir = "/sdcard/Android/data/com.angelde.fnadroid/files/game";

void showError(char* msg) {
    jstring jsMsg = jnienv->NewStringUTF(msg);
    jclass clazz = jnienv->FindClass("com/angelde/fnadroid/FNADroidWrapper");
    jmethodID showMsgID = jnienv->GetStaticMethodID(clazz, "showError", "(Ljava/lang/String;)V");
    jnienv->CallStaticVoidMethod(clazz, showMsgID, jsMsg);
    jnienv->DeleteLocalRef(jsMsg);
}

void showDebug(char* msg) {
    jstring jsMsg = jnienv->NewStringUTF(msg);
    jclass clazz = jnienv->FindClass("com/angelde/fnadroid/FNADroidWrapper");
    jmethodID showMsgID = jnienv->GetStaticMethodID(clazz, "showDebug", "(Ljava/lang/String;)V");
    jnienv->CallStaticVoidMethod(clazz, showMsgID, jsMsg);
    jnienv->DeleteLocalRef(jsMsg);
}

int SDL_main(int argc, char* argv[]) {
    chdir(fnadir);

    mono_config_parse(NULL);
    mono_trace_set_level_string("debug");
    domain = mono_jit_init_version("fnadroid-domain", "v4.0.30319");

    mono_thread_attach(domain);

    mscorlib = mono_get_corlib();
    if (!mscorlib) {
        LOGE("mscorlib could not be loaded!");
        char buf[2048];
        strcpy(buf, "mscorlib could not be loaded!\nGame path: ");
        strcat(buf, fnadir);
        strcat(buf, "\nCurrent directory: ");
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        strcat(buf, cwd);
        showError(buf);
        return -1;
    }

    assembly = mono_domain_assembly_open(domain, "game.exe");
    if (!assembly) {
        LOGE("Assembly could not be loaded!");
        char buf[2048];
        strcpy(buf, "Assembly could not be loaded!\nGame path: ");
        strcat(buf, fnadir);
        strcat(buf, "\nCurrent directory: ");
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        strcat(buf, cwd);
        showError(buf);
        return -1;
    }

    int argc_ = 1;
    char* argv_[] = {"--android"};
    mono_jit_exec(domain, assembly, argc_, argv_);
}

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

#ifdef __cplusplus
}
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif
//BEGIN SDL_android_main.c
/*
    SDL_android_main.c, placed in the public domain by Sam Lantinga  3/13/14
*/
//#include "../src/SDL_internal.h"

#ifdef __ANDROID__

/* Include the SDL main definition header */
//#include "SDL_main.h"

/*******************************************************************************
                 Functions called by JNI
*******************************************************************************/
//#include <jni.h>

/* Called before SDL_main() to initialize JNI bindings in SDL library */
extern void SDL_Android_Init(JNIEnv* env, jclass cls);

/* Start up the SDL app */
void Java_org_libsdl_app_SDLActivity_nativeInit(JNIEnv* env, jclass cls, jobject obj)
{
    //CUSTOM START
    //Make the environment public
    jnienv = env;
    //CUSTOM END
    /* This interface could expand with ABI negotiation, calbacks, etc. */
    SDL_Android_Init(env, cls);

    SDL_SetMainReady();

    /* Run the application code! */
    int status;
    char *argv[2];
    argv[0] = SDL_strdup("SDL_app");
    argv[1] = NULL;
    status = SDL_main(1, argv);

    /* Do not issue an exit or the whole application will terminate instead of just the SDL thread */
    /* exit(status); */
}

#endif /* __ANDROID__ */

/* vi: set ts=4 sw=4 expandtab: */
//END SDL_android_main.c
#ifdef __cplusplus
}
#endif
