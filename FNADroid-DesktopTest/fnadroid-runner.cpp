#include <fnadroid-wrapper.h>

int main(int argc, char* argv[]) {
    LOGI("fnadroid_boot();");
    int val = fnadroid_boot();
    LOGI("mono_jit_cleanup(domain);\n");
    mono_jit_cleanup(domain);
    LOGI("exit\n");
    return val;
}
