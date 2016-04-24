#include <fnadroid-wrapper.h>

//Hidden call test
#ifdef TEST_CALL_HIDDEN
typedef int f_add(int a, int b);
f_add* add;
//Change these when testing on your own - get them via objdump -t
#define ADDR_h_add 0x0000000000000fa0
#define ADDR_BASE 0x0000000000000fb4
#define FUNC_BASE fnadroid_boot
#endif

int main(int argc, char* argv[]) {
    #ifdef TEST_CALL_HIDDEN
    add = (f_add*) (ADDR_h_add - ADDR_BASE + ((uintprt_t) &FUNC_BASE));
    printf("add addr: %p\n", (void*) add);
    printf("add: %i\n", add(1, 2));
    return;
    #endif
    
    fnadir = ".";
    homedir = ".";
    LOGI("fnadroid_boot();");
    int val = fnadroid_boot();
    LOGI("mono_jit_cleanup(domain);");
    mono_jit_cleanup(domain);
    LOGI("exit");
    return val;
}
