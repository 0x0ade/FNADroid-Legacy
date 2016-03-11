This directory contains a testing implementation of the FNADroid wrapper and glue.

Feel free to use it to debug FNADroid and FNADroid games.

Currently only Linux is supported. The output is libfnadroid-wrapper.so, which functions as a stub implementation of the Android-Mono glue, and fnadroid-runner, which functions as the actual wrapper (confusing, huh?). Call it without args or run FNADroid-Boot.exe via mono to test FNADroid in various situations.
