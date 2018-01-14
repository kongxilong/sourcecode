/* $NetBSD: cpu_ucode.c,v 1.1 2012/01/13 16:05:15 cegger Exp $ */
/*
 * Copyright (c) 2012 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Christoph Egger.
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
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: cpu_ucode.c,v 1.1 2012/01/13 16:05:15 cegger Exp $");

#include "opt_cpu_ucode.h"

#include <sys/param.h>
#include <sys/cpuio.h>
#include <sys/cpu.h>

#include <dev/firmload.h>

#include <machine/cpuvar.h>
#include <machine/cputypes.h>

#include <x86/cpu_ucode.h>

static struct cpu_ucode_softc ucode_softc;

int
cpu_ucode_get_version(void *data)
{
	struct cpu_ucode *ucode = data;

	switch (cpu_vendor) {
	case CPUVENDOR_AMD:
		return cpu_ucode_amd_get_version(ucode);
	default:
		ucode->version = (uint64_t)-1;
		return EOPNOTSUPP;
	}

	return 0;
}

int
cpu_ucode_md_open(firmware_handle_t *fwh, const char *fwname)
{
	switch (cpu_vendor) {
	case CPUVENDOR_AMD:
		return cpu_ucode_amd_firmware_open(fwh, fwname);
	case CPUVENDOR_INTEL:
		return EOPNOTSUPP; /* not yet supported */
	default:
		return EOPNOTSUPP;
	}
}

int
cpu_ucode_apply(void *data)
{
	struct cpu_ucode *ucode = data;
	struct cpu_ucode_softc *sc = &ucode_softc;
	int error;

	error = cpu_ucode_load(sc, ucode->fwname);
	if (error)
		return error;

	switch (cpu_vendor) {
	case CPUVENDOR_AMD:
		error = cpu_ucode_amd_apply(sc);
		break;
	default:
		return EOPNOTSUPP;
	}

	if (sc->sc_blob != NULL)
		firmware_free(sc->sc_blob, 0);
	sc->sc_blob = NULL;
	sc->sc_blobsize = 0;
	return error;
}
