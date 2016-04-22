#ifndef FNADROID_WRAPPER_H
#define FNADROID_WRAPPER_H

#ifndef FNADROID_DESKTOP
#include <jni.h>
#include <android/log.h>

#define  LOG_TAG    "fnadroid-wrapper"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#else
#define  LOGI(...)  printf("[I][FNADroid-Desktop] %s\n", __VA_ARGS__)
#define  LOGW(...)  printf("[W][FNADroid-Desktop] %s\n", __VA_ARGS__)
#define  LOGE(...)  printf("[E][FNADroid-Desktop] %s\n", __VA_ARGS__)
#endif

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>

//mono side
MonoDomain* domain;

MonoAssembly* assembly;
MonoImage* mscorlib;

MonoAssembly* fnadroid;
MonoImage* fnadroidi;

#ifndef FNADROID_DESKTOP
//java side
JNIEnv* jnienv;
#endif

//bridge / wrapper config
char* fnadir;
char* homedir;

#ifdef FNADROID_DESKTOP
//boot method for DesktopTest
int fnadroid_boot();
#endif

#endif
