# FNADroid
## Unofficial "FNA for Android" wrapper.

Ever wanted to publish an Android version of your FNA game? Never wanted to use MonoGame or pay weird fees just to get it running? Then FNADroid is.. well, still in its early phase, but maybe suited for you!

* Uses a custom compiled Mono (".NET for Linux") runtime, staying up-to-date and performant.
* Supports only 99% of Android devices; who uses ARMv5 nowdays?
* Out-of-the-box only supports only [67.8% (Android 4.3+)](https://developer.android.com/about/dashboards/index.html?utm_source=suzunone) devices due to possible future GLESv3 code & device performance (it's possible that the minimum API will be lowered in the future).
* Tries to work with mainline FNA.
* Only compiles on Linux.
* Uses GLESv2, possibly killing your game.
