/*
 * $Id$
 *
 * This file belongs to FreeMiNT.  It's not in the original MiNT 1.12
 * distribution.  See the file Changes.MH for a detailed log of changes.
 *
 *
 * Author: Thomas Binder <gryf@hrzpub.tu-darmstadt.de>
 * Started: 1998-01-29
 *
 * please send suggestions, patches or bug reports to me or
 * the MiNT mailing list
 *
 *
 * Zweck:
 * Enthaelt die Tabellen zur Wandlung von Unicode in den Atari-ST-Zeichensatz,
 * und Atari-ST-Zeichensatz nach Unicode.
 * Entstanden mit Hilfe des Tools tcs.
 *
 * History:
 *
 * 1998-01-29
 * (gryf) Erstellung
 *
 * 1998-05-06
 * (fn) some changes for use with MiNT
 *
 * 1998-07-01
 * (gryf) Atari-ST-Zeichensatz -> Unicode Tabelle
 *
 * 1998-07-16
 * (rainer) some name corrections
 *
 *
 */

# include "mint/stat.h"		/* struct xattr */

# include "libkern/libkern.h"	/* strcpy(), strcat() */

# include "console.h"		/* c_conws() */
# include "info.h"		/* messages */
# include "init.h"		/* *sysdir */
# include "k_fds.h"		/* FP_ALLOC() */
# include "kmemory.h"		/* kmalloc(), kfree() */
# include "proc.h"		/* rootproc */
# include "unicode.h"

static uchar
cp00[256] =
{
  /* U+0000 - U+0007 */
     0,  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0008 - U+000F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0010 - U+0017 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0018 - U+001F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0020 - U+0027 */
  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
  /* U+0028 - U+002F */
  0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
  /* U+0030 - U+0037 */
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
  /* U+0038 - U+003F */
  0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
  /* U+0040 - U+0047 */
  0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
  /* U+0048 - U+004F */
  0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
  /* U+0050 - U+0057 */
  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
  /* U+0058 - U+005F */
  0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
  /* U+0060 - U+0067 */
  0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
  /* U+0068 - U+006F */
  0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
  /* U+0070 - U+0077 */
  0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
  /* U+0078 - U+007F */
  0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e,  '?',
  /* U+0080 - U+0087 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0088 - U+008F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0090 - U+0097 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0098 - U+009F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+00A0 - U+00A7 */
   '?', 0xad, 0x9b, 0x9c,  '?', 0x9d,  '?', 0xdd,
  /* U+00A8 - U+00AF */
  0xb9, 0xbd, 0xa6, 0xae, 0xaa,  '?', 0xbe, 0xff,
  /* U+00B0 - U+00B7 */
  0xf8, 0xf1, 0xfd, 0xfe, 0xba, 0xe6, 0xbc, 0xfa,
  /* U+00B8 - U+00BF */
   '?',  '?', 0xa7, 0xaf, 0xac, 0xab,  '?', 0xa8,
  /* U+00C0 - U+00C7 */
  0xb6,  '?',  '?', 0xb7, 0x8e, 0x8f, 0x92, 0x80,
  /* U+00C8 - U+00CF */
   '?', 0x90,  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+00D0 - U+00D7 */
   '?', 0xa5,  '?',  '?',  '?', 0xb8, 0x99,  '?',
  /* U+00D8 - U+00DF */
  0xb2,  '?',  '?',  '?', 0x9a,  '?',  '?', 0x9e,
  /* U+00E0 - U+00E7 */
  0x85, 0xa0, 0x83, 0xb0, 0x84, 0x86, 0x91, 0x87,
  /* U+00E8 - U+00EF */
  0x8a, 0x82, 0x88, 0x89, 0x8d, 0xa1, 0x8c, 0x8b,
  /* U+00F0 - U+00F7 */
   '?', 0xa4, 0x95, 0xa2, 0x93, 0xb1, 0x94, 0xf6,
  /* U+00F8 - U+00FF */
  0xb3, 0x97, 0xa3, 0x96, 0x81,  '?',  '?', 0x98
};

static uchar
cp01[256] =
{
  /* U+0100 - U+0107 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0108 - U+010F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0110 - U+0117 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0118 - U+011F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0120 - U+0127 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0128 - U+012F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0130 - U+0137 */
   '?',  '?', 0xc1, 0xc0,  '?',  '?',  '?',  '?',
  /* U+0138 - U+013F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0140 - U+0147 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0148 - U+014F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0150 - U+0157 */
   '?',  '?', 0xb5, 0xb4,  '?',  '?',  '?',  '?',
  /* U+0158 - U+015F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0160 - U+0167 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0168 - U+016F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0170 - U+0177 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0178 - U+017F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0180 - U+0187 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0188 - U+018F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0190 - U+0197 */
   '?',  '?', 0x9f,  '?',  '?',  '?',  '?',  '?',
  /* U+0198 - U+019F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+01A0 - U+01A7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+01A8 - U+01AF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+01B0 - U+01B7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+01B8 - U+01BF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+01C0 - U+01C7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+01C8 - U+01CF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+01D0 - U+01D7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+01D8 - U+01DF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+01E0 - U+01E7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+01E8 - U+01EF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+01F0 - U+01F7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+01F8 - U+01FF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?'
};

static uchar
cp03[256] =
{
  /* U+0300 - U+0307 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0308 - U+030F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0310 - U+0317 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0318 - U+031F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0320 - U+0327 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0328 - U+032F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0330 - U+0337 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0338 - U+033F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0340 - U+0347 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0348 - U+034F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0350 - U+0357 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0358 - U+035F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0360 - U+0367 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0368 - U+036F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0370 - U+0377 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0378 - U+037F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0380 - U+0387 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0388 - U+038F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0390 - U+0397 */
   '?',  '?',  '?', 0xe2,  '?',  '?',  '?',  '?',
  /* U+0398 - U+039F */
  0xe9,  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+03A0 - U+03A7 */
   '?',  '?',  '?', 0xe4,  '?',  '?', 0xe8,  '?',
  /* U+03A8 - U+03AF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+03B0 - U+03B7 */
   '?', 0xe0, 0xe1,  '?', 0xeb,  '?',  '?',  '?',
  /* U+03B8 - U+03BF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+03C0 - U+03C7 */
  0xe3,  '?',  '?', 0xe5, 0xe7,  '?', 0xed,  '?',
  /* U+03C8 - U+03CF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+03D0 - U+03D7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+03D8 - U+03DF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+03E0 - U+03E7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+03E8 - U+03EF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+03F0 - U+03F7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+03F8 - U+03FF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?'
};

static uchar
cp05[256] =
{
  /* U+0500 - U+0507 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0508 - U+050F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0510 - U+0517 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0518 - U+051F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0520 - U+0527 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0528 - U+052F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0530 - U+0537 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0538 - U+053F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0540 - U+0547 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0548 - U+054F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0550 - U+0557 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0558 - U+055F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0560 - U+0567 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0568 - U+056F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0570 - U+0577 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0578 - U+057F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0580 - U+0587 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0588 - U+058F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0590 - U+0597 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+0598 - U+059F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+05A0 - U+05A7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+05A8 - U+05AF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+05B0 - U+05B7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+05B8 - U+05BF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+05C0 - U+05C7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+05C8 - U+05CF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+05D0 - U+05D7 */
  0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9,
  /* U+05D8 - U+05DF */
  0xca, 0xcb, 0xd9, 0xcc, 0xcd, 0xda, 0xce, 0xd8,
  /* U+05E0 - U+05E7 */
  0xcf, 0xd0, 0xd1, 0xdb, 0xd2, 0xdc, 0xd3, 0xd4,
  /* U+05E8 - U+05EF */
  0xd5, 0xd6, 0xd7,  '?',  '?',  '?',  '?',  '?',
  /* U+05F0 - U+05F7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+05F8 - U+05FF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?'
};

static uchar
cp20[256] =
{
  /* U+2000 - U+2007 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2008 - U+200F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2010 - U+2017 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2018 - U+201F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2020 - U+2027 */
  0xbb,  '?', 0xf9,  '?',  '?',  '?',  '?',  '?',
  /* U+2028 - U+202F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2030 - U+2037 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2038 - U+203F */
  0xde,  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2040 - U+2047 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2048 - U+204F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2050 - U+2057 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2058 - U+205F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2060 - U+2067 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2068 - U+206F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2070 - U+2077 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2078 - U+207F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?', 0xfc,
  /* U+2080 - U+2087 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2088 - U+208F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2090 - U+2097 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2098 - U+209F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+20A0 - U+20A7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+20A8 - U+20AF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+20B0 - U+20B7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+20B8 - U+20BF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+20C0 - U+20C7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+20C8 - U+20CF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+20D0 - U+20D7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+20D8 - U+20DF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+20E0 - U+20E7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+20E8 - U+20EF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+20F0 - U+20F7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+20F8 - U+20FF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?'
};

static uchar
cp21[256] =
{
  /* U+2100 - U+2107 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2108 - U+210F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2110 - U+2117 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2118 - U+211F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2120 - U+2127 */
   '?',  '?', 0xbf,  '?',  '?',  '?', 0xea,  '?',
  /* U+2128 - U+212F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2130 - U+2137 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2138 - U+213F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2140 - U+2147 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2148 - U+214F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2150 - U+2157 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2158 - U+215F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2160 - U+2167 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2168 - U+216F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2170 - U+2177 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2178 - U+217F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2180 - U+2187 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2188 - U+218F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2190 - U+2197 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2198 - U+219F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+21A0 - U+21A7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+21A8 - U+21AF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+21B0 - U+21B7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+21B8 - U+21BF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+21C0 - U+21C7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+21C8 - U+21CF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+21D0 - U+21D7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+21D8 - U+21DF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+21E0 - U+21E7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+21E8 - U+21EF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+21F0 - U+21F7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+21F8 - U+21FF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?'
};

static uchar
cp22[256] =
{
  /* U+2200 - U+2207 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2208 - U+220F */
  0xee,  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2210 - U+2217 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2218 - U+221F */
   '?',  '?', 0xfb,  '?',  '?',  '?', 0xdf,  '?',
  /* U+2220 - U+2227 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2228 - U+222F */
   '?', 0xef,  '?',  '?',  '?',  '?', 0xec,  '?',
  /* U+2230 - U+2237 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2238 - U+223F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2240 - U+2247 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2248 - U+224F */
  0xf7,  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2250 - U+2257 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2258 - U+225F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2260 - U+2267 */
   '?', 0xf0,  '?',  '?', 0xf3, 0xf2,  '?',  '?',
  /* U+2268 - U+226F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2270 - U+2277 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2278 - U+227F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2280 - U+2287 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2288 - U+228F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2290 - U+2297 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2298 - U+229F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+22A0 - U+22A7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+22A8 - U+22AF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+22B0 - U+22B7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+22B8 - U+22BF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+22C0 - U+22C7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+22C8 - U+22CF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+22D0 - U+22D7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+22D8 - U+22DF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+22E0 - U+22E7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+22E8 - U+22EF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+22F0 - U+22F7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+22F8 - U+22FF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?'
};

static uchar
cp23[256] =
{
  /* U+2300 - U+2307 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2308 - U+230F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2310 - U+2317 */
  0xa9,  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2318 - U+231F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2320 - U+2327 */
  0xf4, 0xf5,  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2328 - U+232F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2330 - U+2337 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2338 - U+233F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2340 - U+2347 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2348 - U+234F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2350 - U+2357 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2358 - U+235F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2360 - U+2367 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2368 - U+236F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2370 - U+2377 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2378 - U+237F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2380 - U+2387 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2388 - U+238F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2390 - U+2397 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+2398 - U+239F */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+23A0 - U+23A7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+23A8 - U+23AF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+23B0 - U+23B7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+23B8 - U+23BF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+23C0 - U+23C7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+23C8 - U+23CF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+23D0 - U+23D7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+23D8 - U+23DF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+23E0 - U+23E7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+23E8 - U+23EF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+23F0 - U+23F7 */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?',
  /* U+23F8 - U+23FF */
   '?',  '?',  '?',  '?',  '?',  '?',  '?',  '?'
};

uchar *
t_uni2atari[256] =
{
  /* U+0000 - U+07FF */
  cp00, cp01, NULL, cp03, NULL, cp05, NULL, NULL,
  /* U+0800 - U+0FFF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+1000 - U+17FF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+1800 - U+1FFF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+2000 - U+27FF */
  cp20, cp21, cp22, cp23, NULL, NULL, NULL, NULL,
  /* U+2800 - U+2FFF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+3000 - U+37FF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+3800 - U+3FFF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+4000 - U+47FF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+4800 - U+4FFF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+5000 - U+57FF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+5800 - U+5FFF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+6000 - U+67FF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+6800 - U+6FFF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+7000 - U+77FF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+7800 - U+7FFF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+8000 - U+87FF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+8800 - U+8FFF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+9000 - U+97FF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+9800 - U+9FFF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+A000 - U+A7FF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+A800 - U+AFFF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+B000 - U+B7FF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+B800 - U+BFFF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+C000 - U+C7FF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+C800 - U+CFFF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+D000 - U+D7FF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+D800 - U+DFFF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+E000 - U+E7FF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+E800 - U+EFFF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+F000 - U+F7FF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* U+F800 - U+FFFF */
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

uchar
t_atari2uni[256] =
{
	/* 0x80 - 0x83 */
	0x00, 0xc7, 0x00, 0xfc, 0x00, 0xe9, 0x00, 0xe2,
	/* 0x84 - 0x87 */
	0x00, 0xe4, 0x00, 0xe0, 0x00, 0xe5, 0x00, 0xe7,
	/* 0x88 - 0x8b */
	0x00, 0xea, 0x00, 0xeb, 0x00, 0xe8, 0x00, 0xef,
	/* 0x8c - 0x8f */
	0x00, 0xee, 0x00, 0xec, 0x00, 0xc4, 0x00, 0xc5,
	/* 0x90 - 0x93 */
	0x00, 0xc9, 0x00, 0xe6, 0x00, 0xc6, 0x00, 0xf4,
	/* 0x94 - 0x97 */
	0x00, 0xf6, 0x00, 0xf2, 0x00, 0xfb, 0x00, 0xf9,
	/* 0x98 - 0x9b */
	0x00, 0xff, 0x00, 0xd6, 0x00, 0xdc, 0x00, 0xa2,
	/* 0x9c - 0x9f */
	0x00, 0xa3, 0x00, 0xa5, 0x00, 0xdf, 0x01, 0x92,
	/* 0xa0 - 0xa3 */
	0x00, 0xe1, 0x00, 0xed, 0x00, 0xf3, 0x00, 0xfa,
	/* 0xa4 - 0xa7 */
	0x00, 0xf1, 0x00, 0xd1, 0x00, 0xaa, 0x00, 0xba,
	/* 0xa8 - 0xab */
	0x00, 0xbf, 0x23, 0x10, 0x00, 0xac, 0x00, 0xbd,
	/* 0xac - 0xaf */
	0x00, 0xbc, 0x00, 0xa1, 0x00, 0xab, 0x00, 0xbb,
	/* 0xb0 - 0xb3 */
	0x00, 0xe3, 0x00, 0xf5, 0x00, 0xd8, 0x00, 0xf8,
	/* 0xb4 - 0xb7 */
	0x01, 0x53, 0x01, 0x52, 0x00, 0xc0, 0x00, 0xc3,
	/* 0xb8 - 0xbb */
	0x00, 0xd5, 0x00, 0xa8, 0x00, 0xb4, 0x20, 0x20,
	/* 0xbc - 0xbf */
	0x00, 0xb6, 0x00, 0xa9, 0x00, 0xae, 0x21, 0x22,
	/* 0xc0 - 0xc3 */
	0x01, 0x33, 0x01, 0x32, 0x05, 0xd0, 0x05, 0xd1,
	/* 0xc4 - 0xc7 */
	0x05, 0xd2, 0x05, 0xd3, 0x05, 0xd4, 0x05, 0xd5,
	/* 0xc8 - 0xcb */
	0x05, 0xd6, 0x05, 0xd7, 0x05, 0xd8, 0x05, 0xd9,
	/* 0xcc - 0xcf */
	0x05, 0xdb, 0x05, 0xdc, 0x05, 0xde, 0x05, 0xe0,
	/* 0xd0 - 0xd3 */
	0x05, 0xe1, 0x05, 0xe2, 0x05, 0xe4, 0x05, 0xe6,
	/* 0xd4 - 0xd7 */
	0x05, 0xe7, 0x05, 0xe8, 0x05, 0xe9, 0x05, 0xea,
	/* 0xd8 - 0xdb */
	0x05, 0xdf, 0x05, 0xda, 0x05, 0xdd, 0x05, 0xe3,
	/* 0xdc - 0xdf */
	0x05, 0xe5, 0x00, 0xa7, 0x20, 0x38, 0x22, 0x1e,
	/* 0xe0 - 0xe3 */
	0x03, 0xb1, 0x03, 0xb2, 0x03, 0x93, 0x03, 0xc0,
	/* 0xe4 - 0xe7 */
	0x03, 0xa3, 0x03, 0xc3, 0x00, 0xb5, 0x03, 0xc4,
	/* 0xe8 - 0xeb */
	0x03, 0xa6, 0x03, 0xb8, 0x21, 0x26, 0x03, 0xb4,
	/* 0xec - 0xef */
	0x22, 0x2e, 0x03, 0xc6, 0x22, 0x08, 0x22, 0x0f,
	/* 0xf0 - 0xf3 */
	0x22, 0x61, 0x00, 0xb1, 0x22, 0x65, 0x22, 0x64,
	/* 0xf4 - 0xf7 */
	0x23, 0x20, 0x23, 0x21, 0x00, 0xf7, 0x22, 0x48,
	/* 0xf8 - 0xfb */
	0x00, 0xb0, 0x20, 0x22, 0x00, 0xb7, 0x22, 0x1a,
	/* 0xfc - 0xff */
	0x20, 0x7f, 0x00, 0xb2, 0x00, 0xb3, 0x00, 0xaf
};

# ifdef SOFT_UNITABLE

# define CP_SIZE 256

static long
load_unicode_table(FILEPTR *fp, char *name, long len)
{
	uchar *buf, *s;
	char msg[64];

	if (len % 3) return 0;		/* file length other than n*3 */

	buf = (uchar *)kmalloc(len);
	if (!buf) return 0;		/* Out of memory */

	if ((*fp->dev->read)(fp, buf, len) == len)
	{
		ushort a;

		/* *s points to entries loaded from the disk.
		 * s[0] is the elder byte of the unicode entry,
		 *	i.e. the codepage number.
		 * s[1] is the younger byte, i.e. the codepage offset
		 * s[2] is the ASCII code for the character to put into
		 */

		s = buf;
		for (a = 0; a < (len/3); a++)
		{
			uchar *codepage = t_uni2atari[s[0]];
			ushort offset;

			if (codepage == NULL)
			{
				ushort b;

				codepage = (uchar *)kmalloc(CP_SIZE);

				if (!codepage)
				{
					kfree(buf);
					return 0;	/* Out of memory */
				}

				t_uni2atari[s[0]] = codepage;

				for (b = 0; b < CP_SIZE; b++)
					codepage[b] = '?';
			}
			offset = s[2];
			codepage[s[1]] = offset;
			offset <<= 1;
			t_atari2uni[offset] = s[0];
			t_atari2uni[offset+1] = s[1];

			s += 3;		/* moving to the next entry */
		}
	}
	kfree(buf);

	ksprintf(msg, sizeof(msg), MSG_unitable_loaded, name);
	c_conws(msg);

	return 1;
}

void
init_unicode(void)
{
	FILEPTR *fp;
	XATTR xa;
	long ret;
	char name[32];

	ret = FP_ALLOC(rootproc, &fp);
	if (ret) return;

	strcpy(name, sysdir);
	strcat(name, "unicode.tbl");

	if(!do_open(&fp, name, O_RDONLY, 0, &xa))
	{
		ret = load_unicode_table(fp, name, xa.size);
		do_close(rootproc, fp);
	}
	else
	{
		fp->links = 0;		/* suppress complaints */
		FP_FREE(fp);
	}
}

# endif

/* EOF */
