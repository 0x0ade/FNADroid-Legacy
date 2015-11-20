#ifndef FNADROID_WRAPPER_H
#define FNADROID_WRAPPER_H

#include <jni.h>

#include <android/log.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>

#define  LOG_TAG    "fnadroid-wrapper"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

//mono side
MonoDomain* domain;

MonoAssembly* assembly;
MonoImage* mscorlib;

MonoAssembly* fnadroid;
MonoImage* fnadroidi;

//java side
JNIEnv* jnienv;

//bridge / wrapper config
char* fnadir;
char* homedir;

#endif
