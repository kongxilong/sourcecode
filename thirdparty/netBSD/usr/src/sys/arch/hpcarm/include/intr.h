/* 	$NetBSD: intr.h,v 1.15 2010/11/13 14:07:07 uebayasi Exp $	*/

/*
 * Copyright (c) 1997 Mark Brinicombe.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by Mark Brinicombe
 *	for the NetBSD Project.
 * 4. The name of the company nor the name of the author may be used to
 *    endorse or promote products derived from this software without specific
 *    prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _HPCARM_INTR_H_
#define _HPCARM_INTR_H_

#ifdef _KERNEL

#define IPL_NONE	0
#define IPL_SOFTCLOCK	1
#define IPL_SOFTBIO	2
#define IPL_SOFTNET	3
#define IPL_SOFTSERIAL	4
#define IPL_VM		5
#define IPL_SCHED	6
#define IPL_HIGH	7

#define NIPL		8

#define	IST_UNUSABLE	-1	/* interrupt cannot be used */
#define	IST_NONE	0	/* none (dummy) */
#define	IST_PULSE	1	/* pulsed */
#define	IST_EDGE	2	/* edge-triggered */
#define	IST_LEVEL	3	/* level-triggered */

#define	IST_LEVEL_LOW	IST_LEVEL
#define	IST_LEVEL_HIGH	4
#define	IST_EDGE_FALLING IST_EDGE
#define	IST_EDGE_RISING	5
#define	IST_EDGE_BOTH	6

#ifdef __OLD_INTERRUPT_CODE	/* XXX XXX XXX */

#include <machine/irqhandler.h>
#include <arm/arm32/psl.h>

#else	/* !__OLD_INTERRUPT_CODE */

#define	__NEWINTR	/* enables new hooks in cpu_fork()/cpu_switch() */

#ifndef _LOCORE

#include <sys/queue.h>

#if defined(_LKM)

int	_splraise(int);
int	_spllower(int);
void	splx(int);
void	_setsoftintr(int);

#else	/* _LKM */

#include "opt_arm_intr_impl.h"

#if defined(ARM_INTR_IMPL)

/*
 * Each board needs to define the following functions:
 *
 * int	_splraise(int);
 * int	_spllower(int);
 * void	splx(int);
 * void	_setsoftintr(int);
 *
 * These may be defined as functions, static inline functions, or macros,
 * but there must be a _spllower() and splx() defined as functions callable
 * from assembly language (for cpu_switch()).  However, since it's quite
 * useful to be able to inline splx(), you could do something like the
 * following:
 *
 * in <boardtype>_intr.h:
 * 	static inline int
 *	boardtype_splx(int spl)
 *	{...}
 *
 *	#define splx(nspl)	boardtype_splx(nspl)
 *	...
 * and in boardtype's machdep code:
 *
 *	...
 *	#undef splx
 *	int
 *	splx(int spl)
 *	{
 *		return boardtype_splx(spl);
 *	}
 */

#include ARM_INTR_IMPL

#else	/* ARM_INTR_IMPL */

#error ARM_INTR_IMPL not defined.

#endif	/* ARM_INTR_IMPL */

#endif	/* _LKM */

#define splsoft()	_splraise(IPL_SOFT)

typedef uint8_t ipl_t;
typedef struct {
	ipl_t _ipl;
} ipl_cookie_t;

static inline ipl_cookie_t
makeiplcookie(ipl_t ipl)
{

	return (ipl_cookie_t){._ipl = ipl};
}

static inline int
splraiseipl(ipl_cookie_t icookie)
{

	return _splraise(icookie._ipl);
}

#define	spl0()		_spllower(IPL_NONE)

#include <sys/spl.h>

#endif	/* ! _LOCORE */

#endif	/* __OLD_INTERRUPT_CODE */

#endif	/* _KERNEL */

#endif	/* _HPCARM_INTR_H */
