#include "fnadroid-wrapper.h"

#include <jni.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <SDL_main.h>
#include <SDL.h>

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