#ifndef FNADROID_AOTGEN_H
#define FNADROID_AOTGEN_H

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>

typedef unsigned int guint32;
typedef bool gboolean;
//alt.: Use eglib (also used by Mono)
//#include <glib.h>

#ifndef ADDR_BASE
#warning "You must define ADDR_BASE to use any other hidden symbol in FNADroid-AOTGEN! (Use objdump -t)"
#define ADDR_BASE 0x00000000
#endif
#ifndef FUNC_BASE
#warning "You must define FUNC_BASE to use any other hidden symbol in FNADroid-AOTGEN! (Use objdump -t)"
#define FUNC_BASE mono_jit_init
#endif

//Non-public-API Mono functions

//From mono/mini/aot-compiler.h
//int mono_compile_assembly(MonoAssembly* ass, guint32 opts, const char* aot_options); //hidden / local
typedef int f_mono_compile_assembly(MonoAssembly* ass, guint32 opts, const char* aot_options);
f_mono_compile_assembly* mono_compile_assembly;
#ifndef ADDR_mono_compile_assembly
#warning "You must define ADDR_mono_compile_assembly to use mono_compile_assembly! (Use objdump -t)"
#define ADDR_mono_compile_assembly 0x00000000
#endif

//From mono/mini/driver.c
int mono_parse_default_optimizations(const char* p); //global

//Useless, unless you want to add optimizations. Use mono_parse_default_optimizations instead.
//guint32 parse_optimizations(guint32 opt, const char* p, gboolean cpu_opts); //static / local
typedef guint32 f_parse_optimizations(guint32 opt, const char* p, gboolean cpu_opts);
f_parse_optimizations* parse_optimizations;
#ifndef ADDR_parse_optimizations
#warning "You must define ADDR_parse_optimizations to use parse_optimizations! (Use objdump -t)"
#define ADDR_parse_optimizations 0x00000000
#endif

//From mono/mini/mini.h
gboolean mono_use_llvm; //global


//fd: fnadroid; ag: aotgen

guint32 fd_ag_opt; //The optimizations that will be applied on fd_ag_gen. Default: -O=all
char* fd_ag_aotopt; //The AOT options. Default: TODO
MonoDomain* fd_ag_domain; //The Mono domain to use when compiling. Default: None

void fd_ag_init(); //Call this before your own setup as it sets up the default values.

int fd_ag_domain_gen(MonoDomain* domain, const char* path);
int fd_ag_gen(const char* path);

#endif
