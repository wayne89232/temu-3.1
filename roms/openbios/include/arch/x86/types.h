/* tag: data types for forth engine
 *
 * This file is autogenerated by types.sh. Do not edit!
 *
 * Copyright (C) 2003-2005 Stefan Reinauer, Patrick Mauritz
 *
 * See the file "COPYING" for further information about
 * the copyright and warranty status of this work.
 */

#ifndef __TYPES_H
#define __TYPES_H

#include <inttypes.h>

/* endianess */

#include "autoconf.h"

/* physical address: XXX theoretically 36 bits for PAE */

typedef uint32_t phys_addr_t;

#define FMT_plx "%08" PRIx32

/* cell based types */

typedef int32_t		cell;
typedef uint32_t	ucell;
typedef int64_t		dcell;
typedef uint64_t	ducell;

#define FMT_cell    "%" PRId32
#define FMT_ucell   "%" PRIu32
#define FMT_ucellx  "%08" PRIx32
#define FMT_ucellX  "%08" PRIX32

typedef int32_t         prom_arg_t;
typedef uint32_t        prom_uarg_t;

#define PRIdPROMARG     PRId32
#define PRIuPROMARG     PRIu32
#define PRIxPROMARG     PRIx32
#define FMT_prom_arg    "%" PRIdPROMARG
#define FMT_prom_uarg   "%" PRIuPROMARG
#define FMT_prom_uargx  "%08" PRIxPROMARG

#define FMT_elf     "%#x"

#define bitspercell	(sizeof(cell)<<3)
#define bitsperdcell	(sizeof(dcell)<<3)

#define BITS		32

#define PAGE_SHIFT	12

/* size named types */

typedef unsigned char   u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned long long u64;

typedef char  		s8;
typedef short		s16;
typedef int		s32;
typedef long long	s64;

#endif
