LOCAL_PATH := $(call my-dir)

#Import libraries from libs-games folder
#ADD YOUR LIBRARIES HERE.. or not, if you're lucky enough and mono dyn. loads them
include $(CLEAR_VARS)
LOCAL_MODULE := ogg
LOCAL_SRC_FILES := libs-game/libogg.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := vorbis
LOCAL_SRC_FILES := libs-game/libvorbis.so
include $(PREBUILT_SHARED_LIBRARY)

#Compile MojoShader
include $(CLEAR_VARS)
TARGET_PLATFORM := 18
LOCAL_MODULE    := mojoshader
LOCAL_SRC_FILES := mojoshader/mojoshader.c mojoshader/mojoshader_common.c mojoshader/mojoshader_effects.c mojoshader/mojoshader_opengl.c
LOCAL_LDLIBS    := -landroid -lEGL -lGLESv2 -lm -ldl
LOCAL_CFLAGS += -Ijni/mojoshader/GL \
    -DMOJOSHADER_NO_VERSION_INCLUDE \
    -DMOJOSHADER_EFFECT_SUPPORT \
    -DMOJOSHADER_FLIP_RENDERTARGET \
    -DMOJOSHADER_DEPTH_CLIPPING \
    -DMOJOSHADER_XNA4_VERTEX_TEXTURES \
    -DSUPPORT_PROFILE_D3D=0 \
    -DSUPPORT_PROFILE_BYTECODE=0 \
    -DSUPPORT_PROFILE_ARB1=0 \
    -DSUPPORT_PROFILE_ARB1_NV=0
include $(BUILD_SHARED_LIBRARY)

#Import libraries from libs folder
include $(CLEAR_VARS)
LOCAL_MODULE := monosgen-2.0
LOCAL_SRC_FILES := libs/libmonosgen-2.0.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := SDL2
LOCAL_SRC_FILES := libs/libSDL2.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := openal
LOCAL_SRC_FILES := libs/libopenal.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := openal_static
LOCAL_SRC_FILES := libs/libopenal.a
include $(PREBUILT_STATIC_LIBRARY)

#Compile fnadroid-wrapper
include $(CLEAR_VARS)
TARGET_PLATFORM := 18
LOCAL_MODULE    := fnadroid-wrapper
LOCAL_SRC_FILES := fnadroid-wrapper.cpp
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2 -Wl,-rpath,/sdcard/Android/data/com.angelde.fnadroid/mono/lib -Wl,--export-dynamic -lm -ldl
LOCAL_STATIC_LIBRARIES := android_native_app_glue openal_static
LOCAL_SHARED_LIBRARIES := SDL2 monosgen-2.0
LOCAL_CFLAGS += -I/sdcard/Android/data/com.angelde.fnadroid/mono/include/mono-2.0 -I../sdl/include -Ijni/include -D_REENTRANT
include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
