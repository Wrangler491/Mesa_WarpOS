//#ifndef	AMIGAMESApal_H
//#define	AMIGAMESApal_H

/*
 * $Id: $
 */

/*
 * Mesa 3-D graphics library
 * Version:  3.1
 * Copyright (C) 1995  Brian Paul  (brianp@ssec.wisc.edu)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifdef	PALETTE_DEFS
/* first part defines amigaMesa-specifig tag
 * second part defines the choosen palette (aka modelling of available colors)
 * third part defines methog to fetch data (aka encoding of given colors
 *
 * at the moment dithering is only available in RGBMode, that means if we are
 * in index-mode the index-pixels are directly (without dither) mapped to
 * pen-pixels, it could be an improvement to offer dithering even for index-modes
 */
typedef enum PaletteModeID {
  /* dithering */
  AMESA_HPCR_DITHER = -8,					/* hp-color-recovery */
  AMESA_GREY_DITHER = -4,
  AMESA_WEIGHTED_DITHER = -2,					/* default standard 0 */
  AMESA_TRUECOLOR_DITHER = -1,					/* matches below 0 */
  /* matching */
  AMESA_HPCR_MATCH = 8,						/* hp-color-recovery */
  AMESA_GREY_MATCH = 4,
  AMESA_WEIGHTED_MATCH = 2,					/* default standard 0 */
  AMESA_TRUECOLOR_MATCH = 1,					/* matches below 0 */
  /* shifting */
  AMESA_TRUECOLOR_SHIFT = 0,					/* shifts above 0 */
  /* invalid */
  AMESA_DEADFOOD = 0xDEAD					/* dead man */
} palMode;

#define	MATCHtoDITHER(bool)		((bool) = -(bool))	/* if in shift-mode dither isn't available */
#define	DITHERtoMATCH(bool)		((bool) = -(bool))	/* if in shift-mode dither isn't available */

#define	IS_HPCR(bool)			(((bool) ==  8) || ((bool) == -8))
#define	IS_GREY(bool)			(((bool) ==  4) || ((bool) == -4))
#define	IS_WEIGHTED(bool)		(((bool) ==  2) || ((bool) == -2))
#define	IS_TRUECOLOR(bool)		(((bool) >= -1) && ((bool) <=  1))

#define	IS_DITHER(bool)			((bool) <  0)
#define	IS_MATCH(bool)			((bool) >  0)
#define	IS_SHIFT(bool)			((bool) == 0)

/*****************************************************************************
 *
 * fetch-macros
 */

/* compositions */
#define c8toc32(x)			(((((((x) << 8) | (x)) << 8) | (x)) << 8) | (x))	/* byte-pen to pattern-pen */
#define	c24tocg(r,g,b)			(((r << 5) + (g << 6) + (g << 5) + (b << 3) + b) >> 9)	/* rgb-color to 64-grey (quick approximation) */
#define	c24toc8(r,g,b)			((((r) & 0xE0) >> 0) |	\
					 (((g) & 0xE0) >> 3) |	\
					 (((b) & 0xE0) >> 6))					/* rgb-color to 332-color */

/* TrueColor-RGB */
#define TC_RGBA(r,g,b)			(((((((r) << 8) | (g)) << 8) | (b)) << 8) | (0))	/* byte-rgb to long-rgba */
#define TC_ARGB(r,g,b)			(((((((0) << 8) | (r)) << 8) | (g)) << 8) | (b))	/* byte-rgb to long-argb */
#define	TC_Color(col,rr,gg,bb,aa)	{col.r = (W3D_Float)rr;	\
					 col.g = (W3D_Float)gg;	\
					 col.b = (W3D_Float)bb;	\
					 col.a = (W3D_Float)aa;}				/* rgba to float-rgba */

/* Grey-RGB */
#define	GR_RGBA(am, r, g, b)		(am->Palette[c24tocg(r, g, b)].PenNo)			/* rgb-color to grey-pen */

/* Palette-Index */
#define PLD_RGBA(am, r, g, b, x, y)	(GetPenDithered(r, g, b, am, x, y))			/* rgb-color to dithered-pen */
#define PLG_RGBA(am, r, g, b)		(GetPen(r, g, b, am))					/* rgb-color to pen */
#define PL8_RGBA(am, r, g, b)		(am->Palette[c24toc8(r, g, b)].PenNo)			/* rgb-color to 332-pen */

/* the format of index is RGBI, red|green|blue|index */
#define MakeRGBP(am, r, g, b, x, y)	(TC_RGBA(r, g, b) | PL_RGBA(am, r, g, b, x, y))		/* rgb to long-rgbp */
#define MakeRGBA(am, r, g, b, a)	(TC_RGBA(r, g, b) | a)					/* rgb to long-rgba */
/* get the red/green/blue/pen from a given index */
#define	GetRGBP(am, idx)		(am->ItoP[(GLubyte)(idx)])				/* index to pen */
#define	GetRGBPLocal(idx)		(ItoP[(GLubyte)(idx)])					/* index to pen */
/* get the red/green/blue/index from a given pen */
#define	GetIndex(am, idx)		(am->PtoI[(GLubyte)(idx)])				/* pen to index */
#define	GetIndexLocal(idx)		(PtoI[(GLubyte)(idx)])					/* pen to index */

/*****************************************************************************/

struct ColorEntry {
  struct {
    unsigned char r, g, b, a;
  } orgEntry;
  struct {
    unsigned char r, g, b, a;
  } mapEntry;
/*ULONG PenNo; */
#define	PenNo	mapEntry.a
};

#define	CACHED	0xFF
#define	INVALID	0x00
struct ColorCache {
  unsigned char PenCch, R, B, valid;
};

#define palNumCols332	256
#define palNumCols	189
#define palNumColsGrey	64
#define palNumColsHPCR	256

#define	palNumColsMax	256

#define	ERROR_RANGE	512	/* -255 = 0 - 255, 255 = 255 - 0: -255 -> 255 */
#endif

/*****************************************************************************/

#ifdef	PALETTE_PROTOS
#if 0
ULONG GetPen(register unsigned char R __asm__("d0"),
	     register unsigned char G __asm__("d1"),
	     register unsigned char B __asm__("d2"),
	     register amigaMesaContext amesa __asm__("a0"));
ULONG GetPenDithered(register unsigned char R __asm__("d0"),
		     register unsigned char G __asm__("d1"),
		     register unsigned char B __asm__("d2"),
		     register amigaMesaContext amesa __asm__("a0"),
		     register unsigned char X __asm__("d3"),	/* as the ditherkernel doesn't exceed 2^8	*/
		     register unsigned char Y __asm__("d4"));	/* we can safe shrink the ccordinates to it	*/
#endif

ULONG GetPen(unsigned char R, unsigned char G, unsigned char B, amigaMesaContext amesa);
ULONG GetPenDithered( unsigned char R, unsigned char G, unsigned char B, 
				amigaMesaContext amesa, unsigned char X, unsigned char Y);

void AllocCMap(amigaMesaContext, struct Screen *);
void FreeCMap(amigaMesaContext, struct Screen *);
void RethinkCMap(amigaMesaContext);
void ConvertCMap(amigaMesaContext, palMode, struct Screen *Scr);

static inline GLuint PL_RGBA(amigaMesaContext am, GLubyte r, GLubyte g, GLubyte b, GLushort x, GLushort y) {
  return IS_SHIFT(am->trueColor) ? PL8_RGBA(am, r, g, b) :
	 IS_MATCH(am->trueColor) ? PLG_RGBA(am, r, g, b) :
				   PLD_RGBA(am, r, g, b, x, y);
};
#endif

//#endif
