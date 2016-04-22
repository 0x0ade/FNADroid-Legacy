#include <fnadroid-wrapper.h>

int main(int argc, char* argv[]) {
    fnadir = ".";
    homedir = ".";
    LOGI("fnadroid_boot();");
    int val = fnadroid_boot();
    LOGI("mono_jit_cleanup(domain);");
    mono_jit_cleanup(domain);
    LOGI("exit");
    return val;
}
