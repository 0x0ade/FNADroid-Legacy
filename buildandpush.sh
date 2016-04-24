#!/bin/bash
# Remove the connect line if you debug with an USB cable
xbuild FNADroid-CS/FNADroid-CS.sln &&
ndk-build -C jni && \
ant debug && \
adb connect 192.168.2.204 && \
adb wait-for-device && \
adb install -r bin/fezdroid-debug.apk && \
adb logcat -c && \
adb shell 'am start -S com.angelde.fnadroid/.WrapperActivity' && \
adb logcat *:d | grep -i -e fnadroid -e mono -e DEBUG -e SDLThread
