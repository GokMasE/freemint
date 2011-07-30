/*
 * $Id$
 *
 * XaAES - XaAES Ain't the AES (c) 1992 - 1998 C.Graham
 *                                 1999 - 2003 H.Robbers
 *                                 2004 - 2006 F.Naumann & O.Skancke
 *                                 2009 - 2010 P.Wratt
 *
 * A multitasking AES replacement for FreeMiNT
 *
 * This file is part of XaAES.
 *
 * XaAES is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * XaAES is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XaAES; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*// CYAN - cyan*/
#include "../gradients.h"
short start = START;

/* ---------------------------------------------------------------------------------- */
/* -----------  Standard client window colour gradients --------------------------------- */
/* ---------------------------------------------------------------------------------- */
struct xa_gradient otop_vslide_gradient =
{
	(unsigned long)OTOP_VSLIDE_GRADIENT,
	-1, 0,
	 0, 16,

	1, 0, {0},
	{{400,500,500},{600,700,700}},
};
struct xa_gradient otop_hslide_gradient =
{
	(unsigned long)OTOP_HSLIDE_GRADIENT,
	 0, -1,
	16,  0,

	0, 0, {0},
	{{400,500,500},{600,700,700}},
};
struct xa_gradient otop_vslider_gradient =
{
	(unsigned long)OTOP_VSLIDER_GRADIENT,
	-1, 0,
	 0, 16,

	1, 0, {0},
	{{900,1000,1000},{600,700,700}},
};
struct xa_gradient otop_hslider_gradient =
{
	(unsigned long)OTOP_HSLIDER_GRADIENT,
	 0, -1,
	16,  0,

	0, 0, {0},
	{{900,1000,1000},{600,700,700}},
};

struct xa_gradient utop_vslide_gradient =
{
	(unsigned long)UTOP_VSLIDE_GRADIENT,
	-1, 0,
	 0, 16,

	1, 0, {0},
	{{300,350,350},{500,550,550}},
};
struct xa_gradient utop_hslide_gradient =
{
	(unsigned long)UTOP_HSLIDE_GRADIENT,
	 0, -1,
	16,  0,

	0, 0, {0},
	{{300,350,350},{500,550,550}},
};
struct xa_gradient utop_vslider_gradient =
{
	(unsigned long)UTOP_VSLIDER_GRADIENT,
	-1, 0,
	 0, 16,

	1, 0, {0},
	{{800,850,850},{500,550,550}},
};
struct xa_gradient utop_hslider_gradient =
{
	(unsigned long)UTOP_HSLIDER_GRADIENT,
	 0, -1,
	16,  0,

	0, 0, {0},
	{{800,850,850},{500,550,550}},
};

struct xa_gradient otop_title_gradient =
{
	(unsigned long)OTOP_TITLE_GRADIENT,
	 0, -1,
	16,  0,

	0, 0, {0},
	{{700,900,900},{500,700,700}},
};
struct xa_gradient utop_title_gradient =
{
	(unsigned long)UTOP_TITLE_GRADIENT,
	0, -1,
	16, 0,

	0,0,{0},
	{{600,650,650},{800,850,850}},
};

struct xa_gradient otop_info_gradient =
{
	(unsigned long)OTOP_INFO_GRADIENT,
	0, -1,
	16, 0,

	0,0,{0},
	{{800,850,850},{950,1000,1000}},
/*// 	3, 1, {3, 0},*/
/*// 	{{200,200,200},{600,600,600},{900,900,900}},*/
};
struct xa_gradient utop_info_gradient =
{
	(unsigned long)UTOP_INFO_GRADIENT,
	0, -1,
	16, 0,

	0, 0, {0},
	{{600,650,650},{800,850,850}},
};

struct xa_gradient otop_grey_gradient =
{
	(unsigned long)OTOP_GREY_GRADIENT,
	-1, -1,
	0, 0,

	2, 0, {0},
	{{600,700,700},{900,1000,1000}},
};

struct xa_gradient utop_grey_gradient =
{
	(unsigned long)UTOP_GREY_GRADIENT,
	-1, -1,
	0, 0,

	2, 0, {0},
	{{500,550,550},{800,850,850}},
/*	{{500,500,500},{900,900,900}}, */
};

/* ---------------------------------------------------------------------------------- */
/* ---------------- Alert window colour gradients --------------------------------------- */
/* ---------------------------------------------------------------------------------- */

struct xa_gradient alert_otop_title_gradient =
{
	(unsigned long)ALERT_OTOP_TITLE_GRADIENT,
	-1, -1,
	0, 0,

	2, 0,{0},
	{{400,250,250},{900,850,850}},
};

struct xa_gradient alert_utop_title_gradient =
{
	(unsigned long)ALERT_UTOP_TITLE_GRADIENT,
	-1, -1,
	0, 0,

	2, 0, {0},
	{{550,530,530},{750,730,730}},
/*	{{500,500,500},{900,900,900}}, */
};

struct xa_gradient alert_utop_grey_gradient =
{
	(unsigned long)ALERT_UTOP_GREY_GRADIENT,
	-1, -1,
	0, 0,

	2, 0, {0},
	{{350,330,330},{750,730,730}},
/*	{{500,500,500},{900,900,900}}, */
};

/* ---------------------------------------------------------------------------------- */
/* ---------------- Scroll List window colour gradients --------------------------------- */
/* ---------------------------------------------------------------------------------- */
struct xa_gradient slist_otop_vslide_gradient =
{
	(unsigned long)SLIST_OTOP_VSLIDE_GRADIENT,
	-1, 0,
	0, 16,

	1, 0, {0},
	{{300,350,350},{500,550,550}},
};
struct xa_gradient slist_otop_hslide_gradient =
{
	(unsigned long)SLIST_OTOP_HSLIDE_GRADIENT,
	0, -1,
	16, 0,

	0, 0, {0},
	{{300,350,350},{500,550,550}},
};
struct xa_gradient slist_otop_vslider_gradient =
{
	(unsigned long)SLIST_OTOP_VSLIDER_GRADIENT,
	-1, 0,
	0, 16,

	1, 0, {0},
	{{800,850,850},{500,550,550}},
};
struct xa_gradient slist_otop_hslider_gradient =
{
	(unsigned long)SLIST_OTOP_HSLIDER_GRADIENT,
	0, -1,
	16, 0,

	0, 0, {0},
	{{800,850,850},{500,550,550}},
};
struct xa_gradient slist_utop_vslide_gradient =
{
	(unsigned long)SLIST_UTOP_VSLIDE_GRADIENT,
	-1, 0,
	0, 16,

	1, 0, {0},
	{{300,350,350},{500,550,550}},
};
struct xa_gradient slist_utop_hslide_gradient =
{
	(unsigned long)SLIST_UTOP_HSLIDE_GRADIENT,
	0, -1,
	16, 0,

	0, 0, {0},
	{{300,350,350},{500,550,550}},
};
struct xa_gradient slist_utop_vslider_gradient =
{
	(unsigned long)SLIST_UTOP_VSLIDER_GRADIENT,
	-1, 0,
	0, 16,

	1, 0, {0},
	{{800,850,850},{500,550,550}},
};
struct xa_gradient slist_utop_hslider_gradient =
{
	(unsigned long)SLIST_UTOP_HSLIDER_GRADIENT,
	0, -1,
	16, 0,

	0, 0, {0},
	{{800,850,850},{500,550,550}},
};

struct xa_gradient slist_otop_title_gradient =
{
	(unsigned long)SLIST_OTOP_TITLE_GRADIENT,
	-1, -1,
	0, 0,

	2, 0,{0},
	{{00,400,400},{600,1000,1000}},
};
struct xa_gradient slist_utop_title_gradient =
{
	(unsigned long)SLIST_UTOP_TITLE_GRADIENT,
	-1, -1,
	0, 0,

	2, 0, {0},
	{{500,550,550},{700,750,750}},
/*	{{500,500,500},{900,900,900}}, */
};

struct xa_gradient slist_otop_info_gradient =
{
	(unsigned long)SLIST_OTOP_INFO_GRADIENT,
	0, -1,
	16, 0,

	3, 1, {3, 0},
	{{200,250,200},{600,650,650},{900,950,950}},
};
struct xa_gradient slist_utop_info_gradient =
{
	(unsigned long)SLIST_UTOP_INFO_GRADIENT,
	0, -1,
	16, 0,

	0, 0, {0},
	{{400,450,450},{700,750,750}},
};
struct xa_gradient slist_otop_grey_gradient =
{
	(unsigned long)SLIST_OTOP_GREY_GRADIENT,
	-1, -1,
	0, 0,

	2, 0, {0},
	{{500,550,550},{900,950,950}},
};
struct xa_gradient slist_utop_grey_gradient =
{
	(unsigned long)SLIST_UTOP_GREY_GRADIENT,
	-1, -1,
	0, 0,

	2, 0, {0},
	{{300,350,350},{700,750,750}},
/*	{{500,500,500},{900,900,900}}, */
};


struct xa_gradient menu_gradient =
{
	(unsigned long)MENU_GRADIENT,
	 0, -1,
	16,  0,

	0, 0, {0},
/*// 	{{750,750,750},{900,900,900}},*/
	{{900,950,950},{600,650,650}},
};
struct xa_gradient sel_title_gradient =
{
	(unsigned long)SEL_TITLE_GRADIENT,
	0, -1,
	16, 0,
	0,0,{0},
	{{600,650,650},{950,1000,1000}},
};
struct xa_gradient sel_popent_gradient =
{
	(unsigned long)SEL_POPENT_GRADIENT,
	0, -1,
	16, 0,
	0,0,{0},
	{{950,1000,1000},{800,1000,1000}},
};

struct xa_gradient indbutt_gradient =
{
	(unsigned long)INDBUTT_GRADIENT,
	-1,   0,
	 0,  16,

	4, 1, { -35, 0, },
	{{700,750,750},{900,950,950},{700,750,750}},
};
struct xa_gradient sel_indbutt_gradient =
{
	(unsigned long)SEL_INDBUTT_GRADIENT,
	-1,   0,
	 0,  16,

	4, 1, { -35, 0, },
	{{700,750,750},{500,550,550},{700,750,750}},
};
struct xa_gradient actbutt_gradient =
{
	(unsigned long)ACTBUTT_GRADIENT,
	0, -1,
	16, 0,

	0, 0, { 0 },
	{{900,950,950},{700,750,750}},
};

struct xa_gradient popbkg_gradient =
{
	(unsigned long)POPBKG_GRADIENT,
	0, -1,
	16, 0,

	3, 1, {-40, 0},
	{{800,850,850}, {900,950,950}, {700,750,750}},
};

short stop = STOP;
