/* $NetBSD: elf_machdep.h,v 1.10 2012/02/05 17:40:08 reinoud Exp $ */

/*
 * Automatically generated by genheaders.sh on Sun Feb  5 18:39:34 CET 2012
 * Do not modify directly!
 */
#ifndef _USERMODE_ELF_MACHDEP_H
#define _USERMODE_ELF_MACHDEP_H

#if defined(__i386__)
#include "../../i386/include/elf_machdep.h"
#elif defined(__x86_64__)
#include "../../amd64/include/elf_machdep.h"
#else
#error port me
#endif

#endif
