/*	$NetBSD: rump_syscalls_compat.h,v 1.7 2011/01/17 18:24:17 pooka Exp $	*/

/*-
 * Copyright (c) 2010, 2011 Antti Kantee.  All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _RUMP_RUMP_SYSCALLS_COMPAT_H_
#define _RUMP_RUMP_SYSCALLS_COMPAT_H_

#ifndef _KERNEL
#ifdef __NetBSD__
#include <sys/param.h>

/* time_t change */
#if !__NetBSD_Prereq__(5,99,7)
#define RUMP_SYS_RENAME_STAT rump___sysimpl_stat30
#define RUMP_SYS_RENAME_FSTAT rump___sysimpl_fstat30
#define RUMP_SYS_RENAME_LSTAT rump___sysimpl_lstat30

#define RUMP_SYS_RENAME_POLLTS rump___sysimpl_pollts
#define RUMP_SYS_RENAME_SELECT rump___sysimpl_select
#define RUMP_SYS_RENAME_PSELECT rump___sysimpl_pselect
#define RUMP_SYS_RENAME_KEVENT rump___sysimpl_kevent

#define RUMP_SYS_RENAME_UTIMES rump___sysimpl_utimes
#define RUMP_SYS_RENAME_FUTIMES rump___sysimpl_futimes
#define RUMP_SYS_RENAME_LUTIMES rump___sysimpl_lutimes

#define RUMP_SYS_RENAME_MKNOD rump___sysimpl_mknod
#define RUMP_SYS_RENAME_FHSTAT rump___sysimpl_fhstat40
#endif /* __NetBSD_Prereq(5,99,7) */

#endif /* __NetBSD__ */
#endif /* _KERNEL */

#endif /* _RUMP_RUMP_SYSCALLS_COMPAT_H_ */
