/* As running "mono --aot" on the device is quite impractical,
 * FNADroid instead uses mono_compile_assembly.
 */

#include "fnadroid-aotgen.h"
#include "fnadroid-wrapper.h"

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#ifdef FNADROID_DESKTOP
#include <string.h>
#endif

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/threads.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/utils/mono-logger.h>

#define FUNC_BASE_STR_2(A) #A
#define FUNC_BASE_STR_1(A) FUNC_BASE_STR_2(A)
#define FUNC_BASE_str FUNC_BASE_STR_1(FUNC_BASE)

bool _fd_ag_init = false;
void fd_ag_init() {
    if (_fd_ag_init) {
        return;
    }
    _fd_ag_init = true;
    LOGI("Initializing FNADroid-AOTGEN");
    
    //Black magic...
    LOGI("Applying black magic");
    LOGI("Base func (dumped): " FUNC_BASE_str);
    LOGI("Base addr (dumped): 0x%x", ADDR_BASE);
    LOGI("Base func addr (runtime): %p", (void*) &FUNC_BASE);
    
    mono_compile_assembly = (f_mono_compile_assembly*) (ADDR_mono_compile_assembly - ADDR_BASE + ((uintptr_t) &FUNC_BASE));
    LOGI("mono_compile_assembly addr (dumped): 0x%x", ADDR_mono_compile_assembly);
    LOGI("mono_compile_assembly addr (runtime): %p", (void*) mono_compile_assembly);
    
    parse_optimizations = (f_parse_optimizations*) (ADDR_parse_optimizations - ADDR_BASE + ((uintptr_t) &FUNC_BASE));
    LOGI("parse_optimizations addr (dumped): 0x%x", ADDR_parse_optimizations);
    LOGI("parse_optimizations addr (runtime): %p", (void*) parse_optimizations);
    
    //If you want to use LLVM: Recompile Mono with --llvm to support it.
    //mono_use_llvm = true;
    
    LOGI("Parsing optimizations");
    //TODO find out why calling only one of those works
    //fd_ag_opt = mono_parse_default_optimizations("all"); //-O=all, which actually excludes some optimizations
    fd_ag_opt = parse_optimizations(fd_ag_opt, "all", true);
    
    fd_ag_aotopt = ""; //TODO
    
    //fd_ag_domain = NULL; //TODO default domain?
    
    LOGI("Done initing!");
}

int fd_ag_domain_gen(MonoDomain* domain, const char* path) {
    LOGI("Poking Mono's AOT compiler for %s", path);
    fd_ag_init();
    
    MonoImageOpenStatus* assStatus = NULL;
    MonoAssembly* ass;
    if (!domain) {
        ass = mono_assembly_open_full(path, assStatus, false);
    } else {
        ass = mono_domain_assembly_open(domain, path);
    }
    
    int res = mono_compile_assembly(ass, fd_ag_opt, fd_ag_aotopt);
    
    if (!domain) {
        LOGI("Closing domainless assembly %s", path);
        //TODO debug crash
        //mono_assembly_close(ass);
    }
    
    return res;
}

int fd_ag_gen(const char* path) {
    fd_ag_init();
    return fd_ag_domain_gen(fd_ag_domain, path);
}
