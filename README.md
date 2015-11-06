# FNADroid
# Currently a stub, still polishing FEZDroid.
## Unofficial "FNA for Android" example wrapper.

Ever wanted to publish an Android version of your FNA game? Never wanted to use MonoGame or pay weird fees just to get it running? Then FNADroid is.. wait, this is not for anyone quite yet.

* Uses a custom compiled Mono (".NET for Linux") runtime, staying up-to-date and performant.
* Supports only 99% of Android devices, because, really, who uses ARMv5 nowdays?
* Out-of-the-box only supports only [67.8% (Android 4.3+)](https://developer.android.com/about/dashboards/index.html?utm_source=suzunone) devices due to GLESv3 code & device performance
* Tries to work without many changes, out of the box.
* Only compiles on Linux.
* Uses GLESv2, killing all kinds of buffers in your game.
* Tries to use GLESv3, but only newer devices support it.
