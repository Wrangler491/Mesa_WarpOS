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

#include <math.h>
#include <stddef.h>

#define	NO_CONTEXT_AVAILABLE
#include <AOS/amigamesa.h>
#include <graphics/view.h>

#include "palettes/palettes.c"							/* pre-defined palettes */
#include "palettes/error.c"							/* match-error tables */
#include "palettes/remap.c"							/* remap-tables */
#include "palettes/clamps.c"							/* clamping-tables */

#define	MATCH_ERROR
#define	REAL_HPCR
//#undef	ASM_CODE
#undef	MATCH_6BITS
#undef	REVISIT_PALETTE
#undef	REVISIT_YCrCb

/*****************************************************************************/
/*                               modelling				     */
/*****************************************************************************/

/* allocate all pens for a given max-number of colors into a given list */
static BOOL AllocColors(struct ColorEntry *Cols, GLshort Number, struct Screen *Screen)
{
  register LONG pen;
  register BOOL fail = GL_TRUE;
  register struct ColorMap *ColorMap = Screen->ViewPort.ColorMap;
  ULONG ColTab[3];
  GLshort fill = Number;
#ifdef	REVISIT_PALETTE
  ULONG ColorTable[256][3];
  GLshort j = Number, k = 256;
#endif

  do {
    GLshort newNumber = remap[Number];

    if ((pen = ObtainBestPen(ColorMap, c8toc32(Cols[newNumber].orgEntry.r),
				       c8toc32(Cols[newNumber].orgEntry.g),
				       c8toc32(Cols[newNumber].orgEntry.b),
				       OBP_Precision, PRECISION_EXACT, TAG_DONE)) == -1)
      fail = GL_FALSE;
    else {
      Cols[newNumber].PenNo = pen;
      GetRGB32(ColorMap, pen, 1, ColTab);
      Cols[newNumber].mapEntry.r = ColTab[0] >> 24;
      Cols[newNumber].mapEntry.g = ColTab[1] >> 24;
      Cols[newNumber].mapEntry.b = ColTab[2] >> 24;
    }
  } while (--Number >= 0);							/* Number is the max-value, not including! */

#ifdef	REVISIT_PALETTE
  GetRGB32(ColorMap, 0, 256, ColorTable[0]);

  while (--k >= 0) {
    GLshort newNumber = remap[k];
    register GLshort i = j;

#ifndef	REVISIT_YCrCb
    register GLshort match = 0x7FFF, max = 0;

    for (pen = 0; i >= 0; i--) {
      register GLshort thismatch, thismax;

#ifdef	MATCH_ERROR
      register GLshort sum;

      if ((thismatch = (GLshort)((ColorTable[i][0] >> 24)) - (GLshort)Cols[newNumber].orgEntry.r) < 0)
	thismatch = -thismatch;
      thismax = thismatch;
      if ((sum = (GLshort)((ColorTable[i][1] >> 24)) - (GLshort)Cols[newNumber].orgEntry.g) < 0)
	sum = -sum;
      if (sum > thismax)
        thismax = sum;
      thismatch += sum;
      if ((sum = (GLshort)((ColorTable[i][2] >> 24)) - (GLshort)Cols[newNumber].orgEntry.b) < 0)
	sum = -sum;
      if (!(thismatch += sum)) {
	pen = i;
	break;
      }
      if (sum > thismax)
        thismax = sum;
#else
      register GLshort sum;

      sum        = error111[(GLshort)((ColorTable[i][0] >> 24)) - (((GLshort)Cols[newNumber].orgEntry.r) * 4) + 0];
      thismatch  = sum;
      thismax    = sum;
      sum        = error111[(GLshort)((ColorTable[i][1] >> 24)) - (((GLshort)Cols[newNumber].orgEntry.g) * 4) + 1];
      thismatch += sum;
      if (sum > thismax)
        thismax = sum;
      sum        = error111[(GLshort)((ColorTable[i][2] >> 24)) - (((GLshort)Cols[newNumber].orgEntry.b) * 4) + 2];
      if (!(thismatch += sum)) {
	pen = i;
	break;
      }
      if (sum > thismax)
        thismax = sum;
#endif
#else
    /* maybe the best match is to match in YCrCb- not in RGB-space */
    register GLint match = 1000000000, max = 0;
    double Y, Cr, Cb;

#define	toY(R, G, B)	( 0.29900 * R + 0.58700 * G + 0.11400 * B)
#define	toCr(R, G, B)	(-0.16874 * R - 0.33126 * G + 0.50000 * B)
#define	toCb(R, G, B)	( 0.50000 * R - 0.41869 * G - 0.08131 * B)

    Y = toY(Cols[newNumber].orgEntry.r, Cols[newNumber].orgEntry.g, Cols[newNumber].orgEntry.b);
    Cr = toCr(Cols[newNumber].orgEntry.r, Cols[newNumber].orgEntry.g, Cols[newNumber].orgEntry.b);
    Cb = toCb(Cols[newNumber].orgEntry.r, Cols[newNumber].orgEntry.g, Cols[newNumber].orgEntry.b);

    for (pen = 0; i >= 0; i--) {
      register double sum;
      register double thismatch, thismax;

      if ((thismatch = toY((ColorTable[i][0] >> 24), (ColorTable[i][1] >> 24), (ColorTable[i][2] >> 24)) - Y) < 0)
	thismatch = -thismatch;
      thismax = thismatch;

      if ((sum = toCr((ColorTable[i][0] >> 24), (ColorTable[i][1] >> 24), (ColorTable[i][2] >> 24)) - Cr) < 0)
	sum = -sum;
      if (sum > thismax)
        thismax = sum;
      thismatch += sum;

      if ((sum = toCb((ColorTable[i][0] >> 24), (ColorTable[i][1] >> 24), (ColorTable[i][2] >> 24)) - Cb) < 0)
	sum = -sum;
      if (!(thismatch += sum)) {
	pen = i;
	break;
      }
      if (sum > thismax)
        thismax = sum;
#endif

      /* this is the key for good pen-matching! :^) */
      if (match < (GLint)thismatch)
        continue;
      if (match == (GLint)thismatch) {
        if (max < (GLint)thismax)
          continue;
        /* what to do now? match is equal, max is equal */
        if (max == (GLint)thismax) {
        }
      }

      match = (GLint)thismatch;
      max = (GLint)thismax;
      pen = i;
    }
    
    if (pen != Cols[newNumber].PenNo) {
      DEBUGOUT(0, " original: 0x%06x -> old: 0x%06x (%4d/%4d/%4d), new: 0x%06x (%4d/%4d/%4d) -> old: 0x%x, new: 0x%x\n",
	       *((ULONG *)&Cols[newNumber].orgEntry) >> 8,
	       *((ULONG *)&Cols[newNumber].mapEntry) >> 8,
	       Cols[newNumber].mapEntry.r - Cols[newNumber].orgEntry.r,
	       Cols[newNumber].mapEntry.g - Cols[newNumber].orgEntry.g,
	       Cols[newNumber].mapEntry.b - Cols[newNumber].orgEntry.b,
	       ((ColorTable[pen][0] >>  8) & 0x00FF0000) |
	       ((ColorTable[pen][1] >> 16) & 0x0000FF00) |
	       ((ColorTable[pen][2] >> 24) & 0x000000FF),
	       (ColorTable[pen][0] >> 24) - Cols[newNumber].orgEntry.r,
	       (ColorTable[pen][1] >> 24) - Cols[newNumber].orgEntry.g,
	       (ColorTable[pen][2] >> 24) - Cols[newNumber].orgEntry.b,
	       Cols[newNumber].PenNo, pen);
#if 0
      /* lock new key without changing it's value and release old pen */
      ObtainPen(Screen->ViewPort.ColorMap, pen, 0, 0, 0, PENF_NO_SETCOLOR);
      ReleasePen(Screen->ViewPort.ColorMap, Cols[newNumber].PenNo);
#endif
      Cols[newNumber].PenNo = pen;
    }
  }
#endif

  /* this is for PenNo-clamping of grey-levels beyond 64 */
  if (fill < 255) {
    do {
      Cols[++fill].PenNo = Cols[fill].PenNo;
    } while (fill <= 255);
  }

  return (BOOL) fail;
};

/* free all pens for all PaletteModes */
static void FreeColors(struct ColorEntry *Cols, GLshort Number, struct Screen *Screen)
{
  while (--Number >= 0)
    ReleasePen(Screen->ViewPort.ColorMap, Cols[Number].PenNo);
};

/*****************************************************************************/
/*                               encoding				     */
/*****************************************************************************/

/* match a pen to a RGB */
/*static inline ULONG Match(register unsigned char R __asm__("d0"),
			  register unsigned char G __asm__("d1"),
			  register unsigned char B __asm__("d2"),
			  register amigaMesaContext amesa __asm__("a0")) {	*/

static inline ULONG Match(unsigned char R,
			  unsigned char G, unsigned char B, amigaMesaContext amesa) {

  //register ULONG pen = 0;
	ULONG pen = 0;

  /* pen-caching doesn't make sense for dithers, as the
   * random character of the dither-matrice are allmost
   * never the same for neighbourhood-pixels (spans/arrays)
   * only for random jumping pixels (random + random = linear)
   * but that's highly unlikly
   * you can take a look at the cache-performance and verify
   * this statement
   */
  if (TstF(amesa->flags, PALETTE_CACHE)) {
#define	cachedPen	pen
    /* use G as LUT 'cause it has the middle energy */
    if ((cachedPen = *((ULONG *) & amesa->penCache[G]))) {
      cachedPen >>= 8;
      if ((unsigned char)cachedPen == (unsigned char)B) {
	cachedPen >>= 8;
	if ((unsigned char)cachedPen == (unsigned char)R) {
	  cachedPen >>= 8;
	  return cachedPen;
	}
      }
    }
    cachedPen = ((((R << 8) | B) << 8) | CACHED);
    *((ULONG *) & amesa->penCache[G]) = cachedPen;
  }

{
  register GLshort i = amesa->PaletteCols;
  register struct ColorEntry *entries = amesa->Palette + i;
#ifdef	MATCH_ERROR
  register GLshort *errors = (GLshort *)amesa->MatchErrors;
#endif

  /* infinite bad start-match */
  register GLshort match = 0x7FFF;

  /* pal332-mask:
   * 
   *   0x00 = 0b000 00000
   *   0x20 = 0b001 00000
   *   0x40 = 0b010 00000
   *   0x80 = 0b100 00000
   *   0xA0 = 0b101 00000
   *   0xC0 = 0b110 00000
   *   0xE0 = 0b111 00000
   *  -------------------
   *   0xE0 = 0b111 00000
   * 
   *  if (!(R & (~0xE0)))
   *    entries -= 256 - (((R >> 5) + 1) * 32);
   *    entries += (R >> 5) * 32;
   *  if (!(G & (~0xE0)))
   *    entries -=  32 - (((G >> 5) + 1) *  4);
   *    entries += (G >> 5) *  4;
   *  if (!(B & (~0xC0)))
   *    entries -=   4 - (((B >> 6) + 1) *  1);
   *    entries += (B >> 5) *  1;
   * 
   * pal189-mask:
   *  
   *   0x00 = 0b0000 0000
   *   0x1F = 0b0001 1111
   *   0x3F = 0b0011 1111
   *   0x5F = 0b0101 1111
   *   0x7F = 0b1000 1111
   *   0x9F = 0b1001 1111
   *   0xBF = 0b1011 1111
   *   0xDF = 0b1101 1111
   *   0xFF = 0b1111 1111
   *  -------------------
   *   0xF0 = 0b1111 ????
   * 
   *  if (!(R & (~0xFF)))
   *    entries += ?;
   *
   * palgrey-mask:
   *  
   *   0x00 = 0b000000 00
   *   0x04 = 0b000001 00
   *   0x08 = 0b000010 00
   *   0x0C = 0b000011 00
   *   .... = ........ 00
   *   0xFC = 0b111111 00
   *  -------------------
   *   0xFC = 0b111111 00
   * 
   *  if (!(R & (~0xFC)))
   *    entries -= 64 - (((R >> 2) + 1) *  1);
   *  if (!(G & (~0xFC)))
   *    entries -= 16 - (((G >> 2) + 1) *  1);
   *  if (!(B & (~0xFC)))
   *    entries -=  4 - (((B >> 2) + 1) *  1);
   */

  /* find match */
  for (; i >= 0; i--, entries--) {
    register GLshort thismatch;

#ifdef	MATCH_ERROR
    if (!(thismatch = errors[(((GLshort)entries->mapEntry.r - (GLshort)R) * 4) + 0] +
		      errors[(((GLshort)entries->mapEntry.g - (GLshort)G) * 4) + 1] +
		      errors[(((GLshort)entries->mapEntry.b - (GLshort)B) * 4) + 2])) {
      pen = i;
      break;
    }
#else
    register GLshort sum;

    if ((thismatch = (GLshort)(entries->mapEntry.r) - (GLshort)R) < 0)
      thismatch = -thismatch;
    if ((sum = (GLshort)(entries->mapEntry.g) - (GLshort)G) < 0)
      sum = -sum;
    thismatch += sum;
    if ((sum = (GLshort)(entries->mapEntry.b) - (GLshort)B) < 0)
      sum = -sum;
    if (!(thismatch += sum)) {
      pen = i;
      break;
    }
#endif

    if (match > thismatch) {
      match = thismatch;
      pen = i;
    }
  }
}

  pen = (ULONG) (amesa->Palette[pen].PenNo);
  if (amesa->flags & PALETTE_CACHE)
    amesa->penCache[G].PenCch = (unsigned char)(pen);

  return pen;
};

/*****************************************************************************/

/*ULONG GetPen(register unsigned char R __asm__("d0"),
	     register unsigned char G __asm__("d1"),
	     register unsigned char B __asm__("d2"),
	     register amigaMesaContext amesa __asm__("a0")) { */

ULONG GetPen(unsigned char R, unsigned char G, unsigned char B, amigaMesaContext amesa) { 
  /* quick original HPCR-match, looks very dark
   * is not so good without dithering, in the
   * XMesa they talk about a special colormap
   * but I havn't found it
   */
#ifdef	REAL_HPCR
  if (IS_HPCR(amesa->trueColor)) {
#ifndef ASM_CODE
    return amesa->Palette[c24toc8(((GLshort *)amesa->MatchErrors)[(R * 4) + 0],
				  ((GLshort *)amesa->MatchErrors)[(G * 4) + 1],
				  ((GLshort *)amesa->MatchErrors)[(B * 4) + 2])].PenNo;
#else
    __asm__("
	.equ	erroffs,%5
	.equ	paloffs,%4
	.equ	amesa,%3
	.equ	err,a1
	.equ	mask,d3
	.equ	r,%0
	.equ	g,%1
	.equ	b,%2

	move%.l	amesa@(erroffs),err		|  amesa->MatchErrors
	moveq	#0xFFFFFFE0,mask
	move%.w	err@(0,r:w:8),r			|  amesa->MatchErrors[R][0]
	and%.w	mask,r				|  amesa->MatchErrors[R][0] & 0xE0
	move%.w	err@(2,g:w:8),g			|  amesa->MatchErrors[G][1]
	and%.w	mask,g				|  amesa->MatchErrors[G][1] & 0xE0
	move%.w	err@(4,b:w:8),b			|  amesa->MatchErrors[B][2]
	and%.w	mask,b				|  amesa->MatchErrors[B][2] & 0xE0
	lsr%.w	#3,g				| (amesa->MatchErrors[G][1] & 0xE0) >> 3
	lsr%.w	#6,b				| (amesa->MatchErrors[B][2] & 0xE0) >> 6
	or%.w	g,r
	or%.w	b,r
	move%.b	amesa@(paloffs+9,r:w:8),r"
	: "=d" (R)
	: "d" (R)
	, "d" (G)
	, "d" (B)
	, "a" (amesa)
	, "i" (117)	/* offsetof(amesa, Palette) doesn't work here ATTENTION if it changes */
	, "i" (2158)	/* offsetof(amesa, MatchErrors) doesn't work here ATTENTION if it changes */
	: "d3", "a1", "cc");
    return R;
#endif
  }
#endif

  /* every grey-value we can compute does really exists
   * in the grey-map so the following transformation is
   * allways the same result as if we try to match
   * (the match was done in AllocCMap)
   * take a look at "palettes/quickgrey.c" why I choose
   * this approximated (and on the first view) totally
   * wrong function
   * the function does produce levels beyond 64 (69,...)
   * but for grey we must clamp negative values so we can
   * put the value for palette[64].PenNo into the higher
   * (unused) PenNos
   */
  if (IS_GREY(amesa->trueColor)) {
#ifndef ASM_CODE
    return amesa->Palette[((R << 5) + (G << 6) + (G << 5) + (G << 1) + (B << 3) + B) >> 9].PenNo;	/* aprox. to shifts */
#else
    __asm__("
	.equ	paloffs,%4
	.equ	amesa,%3
	.equ	r,%0
	.equ	g,%1
	.equ	b,%2

	lsl%.w	#5,r		|                                                  (r << 5)
	add%.w	b,r		|                                              b + (r << 5)
	lsl%.w	#3,b		|                                   (b << 3)
	add%.w	b,r		|                                   (b << 3) + b + (r << 5)
	add%.w	g,g		|                        (g << 1)
	add%.w	g,r		|                        (g << 1) + (b << 3) + b + (r << 5)
	lsl%.w	#4,g		|             (g << 5)
	add%.w	g,r		|             (g << 5) + (g << 1) + (b << 3) + b + (r << 5)
	add%.w	g,g		|  (g << 6)
	add%.w	g,r		|  (g << 6) + (g << 5) + (g << 1) + (b << 3) + b + (r << 5)
	lsr%.w	#8,r		| ((g << 6) + (g << 5) + (g << 1) + (b << 3) + b + (r << 5)) >> 9
	lsr%.w	#1,r
	move%.b	amesa@(paloffs+9,r:w:8),r"
	: "=d" (R)
	: "d" (R)
	, "d" (G)
	, "d" (B)
	, "a" (amesa)
	, "i" (117)	/* offsetof(amesa, Palette) doesn't work here ATTENTION if it changes */
	: "cc");
    return R;
#endif
  }

  /* on most graphics-cards there are only 6bits per palette-entry
   * so the error produced by this shouldn't be too high but make the
   * matching faster (hopefull, not tested yet)
   */
#ifdef	MATCH_6BITS
  R &= 0xFC;
  G &= 0xFC;
  B &= 0xFC;
#endif

  return Match(R, G, B, amesa);
};

/*****************************************************************************/

/* red, green and blue have different modification-values
 * max red is -15/16
 * max green is -15/16
 * max blue is -30/32
 * max alpha is 0 )alpga exists only for alignment reasons)
 *
 * generation:
 *  r = ?
 *  g = -r + 1;
 *  b = 
 */
static const GLshort DMatrix[16][2][4] = {
  { { 16,-11,  6,  0}, {-15, 12, -4,  0} },
  { { -4, 15,-18,  0}, {  5,-14, 20,  0} },
  { {  1, -7, 26,  0}, {  0,  8,-24,  0} },
  { {-11,  3, 14,  0}, { 12, -2, 16,  0} },
  { { 14, -8,  2,  0}, {-13,  9,  0,  0} },
  { { -6, 14,-22,  0}, {  7,-13, 24,  0} },
  { {  3, -4, 30,  0}, { -2,  5,-28,  0} },
  { { -9,  2,-10,  0}, { 10, -1, 12,  0} },
  { { 15,-10,  8,  0}, {-14, 11, -6,  0} },
  { { -5, 16,-16,  0}, {  6,-15, 18,  0} },
  { {  2, -6, 28,  0}, { -1,  7,-26,  0} },
  { {-10,  4,-12,  0}, { 11, -3, 14,  0} },
  { { 13, -9,  4,  0}, {-12, 10, -2,  0} },
  { { -7, 13,-20,  0}, {  8,-12, 22,  0} },
  { {  4, -5, 32,  0}, { -3,  6,-30,  0} },
  { { -8,  1,  8,  0}, {  9,  0, 10,  0} },
};

#define	DITHER_APPROX(r, g, b, x, y) {			\
  x &= 0xF;						\
  y &= 0x1;						\
							\
  r = clampFF[(GLshort)r + DMatrix[x][y][0] + 32];	\
  g = clampFF[(GLshort)g + DMatrix[x][y][1] + 32];	\
  b = clampFF[(GLshort)b + DMatrix[x][y][2] + 32];	\
}

// ULONG GetPenDithered(register unsigned char R __asm__("d0"),
//		     register unsigned char G __asm__("d1"),
//		     register unsigned char B __asm__("d2"),
//		     register amigaMesaContext amesa __asm__("a0"),
//		     register unsigned char X __asm__("d3"),	/* as the ditherkernel doesn't exceed 2^8	*/
//		     register unsigned char Y __asm__("d4")) {	/* we can safe shrink the ccordinates to uc	*/

ULONG GetPenDithered( unsigned char R, unsigned char G, unsigned char B, 
				amigaMesaContext amesa, unsigned char X,	/* as the ditherkernel doesn't exceed 2^8	*/
				unsigned char Y) {	/* we can safe shrink the ccordinates to uc	*/

  /* quick original HPCR-match, looks very dark
   * but is fast and looks considerable better
   * than shifting or matching, in the
   * XMesa they talk about a special colormap
   * but I havn't found it
   */
#ifdef	REAL_HPCR
  if (IS_HPCR(amesa->trueColor)) {
#ifndef ASM_CODE
    X &= 0xF;
    Y &= 0x1;
    return amesa->Palette[c24toc8(((GLshort *)amesa->MatchErrors)[(R * 4) + 0] + DMatrix[X][Y][0],
				  ((GLshort *)amesa->MatchErrors)[(G * 4) + 1] + DMatrix[X][Y][1],
				  ((GLshort *)amesa->MatchErrors)[(B * 4) + 2] + DMatrix[X][Y][2])].PenNo;
#else
    __asm__("
	.equ	y,%8
	.equ	x,%7
	.equ	mat,%6
	.equ	erroffs,%5
	.equ	paloffs,%4
	.equ	amesa,%3
	.equ	err,a1
	.equ	mask,d3
	.equ	r,%0
	.equ	g,%1
	.equ	b,%2

	and%.w	#0x000F,x			| X & 0xF
	and%.w	#0x0001,y			| Y & 0x1
	add%.w	x,x
	or%.w	y,x
	move%.l	amesa@(erroffs),err		|  amesa->MatchErrors
	moveq	#0xFFFFFFE0,mask
	move%.w	err@(0,r:w:8),r			|  amesa->MatchErrors[R][0]
	add%.w	mat@(0,x:w:8),r			| DMatrix[X][Y][0]
	and%.w	mask,r				|  amesa->MatchErrors[R][0] & 0xE0
	move%.w	err@(2,g:w:8),g			|  amesa->MatchErrors[G][1]
	add%.w	mat@(1,x:w:8),g			| DMatrix[X][Y][1]
	and%.w	mask,g				|  amesa->MatchErrors[G][1] & 0xE0
	move%.w	err@(4,b:w:8),b			|  amesa->MatchErrors[B][2]
	add%.w	mat@(2,x:w:8),b			| DMatrix[X][Y][2]
	and%.w	mask,b				|  amesa->MatchErrors[B][2] & 0xE0
	lsr%.w	#3,g				| (amesa->MatchErrors[G][1] & 0xE0) >> 3
	lsr%.w	#6,b				| (amesa->MatchErrors[B][2] & 0xE0)>> 6
	or%.w	g,r
	or%.w	b,r
	move%.b	amesa@(paloffs+9,r:w:8),r"
	: "=d" (R)
	: "d" (R)
	, "d" (G)
	, "d" (B)
	, "a" (amesa)
	, "i" (117)	/* offsetof(amesa, Palette) doesn't work here ATTENTION if it changes */
	, "i" (2158)	/* offsetof(amesa, MatchErrors) doesn't work here ATTENTION if it changes */
	, "a" (DMatrix)
	, "d" (X)
	, "d" (Y)
	: "a1", "cc");
    return R;
#endif
  }
#endif

  /* apply the HPCR-dither-kernel as if it is a
   * ordered dither-matrix, gives good results
   */
  DITHER_APPROX(R, G, B, X, Y);

  /* every grey-value we can compute does really exists
   * in the grey-map so the following transformation is
   * allways the same result as if we try to match
   * (the match was done in AllocCMap)
   * take a look at "palettes/quickgrey.c" why I choose
   * this approximated (and on the first view) totally
   * wrong function
   * the function does produce levels beyond 64 (69,...)
   * but for grey we must clamp negative values so we can
   * put the value for palette[64].PenNo into the higher
   * (unused) PenNos
   */
  if (IS_GREY(amesa->trueColor)) {
#ifndef ASM_CODE
    return amesa->Palette[((R << 5) + (G << 6) + (G << 5) + (G << 1) + (B << 3) + B) >> 9].PenNo;	/* aprox. to shifts */
#else
    __asm__("
	.equ	paloffs,%4
	.equ	amesa,%3
	.equ	pal,%3
	.equ	r,%0
	.equ	g,%1
	.equ	b,%2

	lsl%.w	#5,r		|                                                  (r << 5)
	add%.w	b,r		|                                              b + (r << 5)
	lsl%.w	#3,b		|                                   (b << 3)
	add%.w	b,r		|                                   (b << 3) + b + (r << 5)
	add%.w	g,g		|                        (g << 1)
	add%.w	g,r		|                        (g << 1) + (b << 3) + b + (r << 5)
	lsl%.w	#4,g		|             (g << 5)
	add%.w	g,r		|             (g << 5) + (g << 1) + (b << 3) + b + (r << 5)
	add%.w	g,g		|  (g << 6)
	add%.w	g,r		|  (g << 6) + (g << 5) + (g << 1) + (b << 3) + b + (r << 5)
	lsr%.w	#8,r		| ((g << 6) + (g << 5) + (g << 1) + (b << 3) + b + (r << 5)) >> 9
	lsr%.w	#1,r
	move%.b	amesa@(paloffs+9,r:w:8),r"
	: "=d" (R)
	: "d" (R)
	, "d" (G)
	, "d" (B)
	, "a" (amesa)
	, "i" (117)	/* offsetof(amesa, Palette) doesn't work here ATTENTION if it changes */
	: "cc");
    return R;
#endif
  }

  /* on most graphics-cards there are only 6bits per palette-entry
   * so the error produced by this shouldn't be too high but make the
   * matching faster (hopefull, not tested yet)
   */
#ifdef	MATCH_6BITS
  R &= 0xFC;
  G &= 0xFC;
  B &= 0xFC;
#endif

  return Match(R, G, B, amesa);
};

/*****************************************************************************/
/*                               preparing				     */
/*****************************************************************************/

/* calls AllocColors depend of the PaletteMode */
void AllocCMap(amigaMesaContext amesa, struct Screen *Scr)
{
  DEBUGOUT(1, "AllocCMap(0x%08x, 0x%08x)\n", amesa, Scr);

  /* clear pen-cache */
  if (TstF(amesa->flags, PALETTE_CACHE))
    bzero(amesa->penCache, sizeof(amesa->penCache));

  /* clear and refill palette */
  if (IS_WEIGHTED(amesa->trueColor)) {
    amesa->PaletteCols = palNumCols - 1;
    amesa->MatchErrors = &error111[ERROR_RANGE / 2][0];

    CopyMem(Palette, amesa->Palette, palNumCols * sizeof(struct ColorEntry));
  }
  else if (IS_GREY(amesa->trueColor)) {
    amesa->PaletteCols = palNumColsGrey - 1;
    amesa->MatchErrors = &errorRGB[ERROR_RANGE / 2][0];

    CopyMem(PaletteGrey, amesa->Palette, palNumColsGrey * sizeof(struct ColorEntry));
  }
  else if (IS_HPCR(amesa->trueColor)) {
    amesa->PaletteCols = palNumColsHPCR - 1;
#ifdef	REAL_HPCR
    amesa->MatchErrors = &errorHPCR[ERROR_RANGE / 2][0];
#else
    amesa->MatchErrors = &error111[ERROR_RANGE / 2][0];
#endif

#ifdef	REAL_HPCR
    CopyMem(Palette332, amesa->Palette, palNumCols * sizeof(struct ColorEntry));
#else
    CopyMem(PaletteHPCR, amesa->Palette, palNumCols * sizeof(struct ColorEntry));
#endif
  }
  else {
    amesa->PaletteCols = palNumCols332 - 1;
    amesa->MatchErrors = &error565[ERROR_RANGE / 2][0];

    CopyMem(Palette332, amesa->Palette, palNumCols332 * sizeof(struct ColorEntry));
  }

  AllocColors(amesa->Palette, amesa->PaletteCols, Scr);
#ifdef	AOS_WARP3D
  if (amesa->TDdriver) {
    W3D_Color *TDpalette = &amesa->TDdriver->Palette;
    struct ColorEntry *palette = amesa->Palette;
    short int i = 256;
    
    while (--i >= 0) {
      TDpalette->r = (W3D_Float)palette->orgEntry.r;
      TDpalette->g = (W3D_Float)palette->orgEntry.g;
      TDpalette->b = (W3D_Float)palette->orgEntry.b;
      TDpalette->a = (W3D_Float)                0.0;
      TDpalette++;
        palette++;
    }
  }
#endif
};

/* calls FreeColors depend of the PaletteMode */
void FreeCMap(amigaMesaContext amesa, struct Screen *Scr)
{
  DEBUGOUT(1, "FreeCMap(0x%08x, 0x%08x)\n", amesa, Scr);

  FreeColors(amesa->Palette, amesa->PaletteCols + 1, Scr);
};

void RethinkCMap(amigaMesaContext amesa)
{
  GLshort index = 256;
  GLuint *ItoP, *PtoI;

  DEBUGOUT(1, "RethinkCMap(0x%08x)\n", amesa);

  ItoP = amesa->ItoP;
  PtoI = amesa->PtoI;

  while (--index >= 0) {
    GLubyte r, g, b, pen;

    r = (GetRGBPLocal(index) >> 24) & 0xFF;
    g = (GetRGBPLocal(index) >> 16) & 0xFF;
    b = (GetRGBPLocal(index) >>  8) & 0xFF;
    pen = PLG_RGBA(amesa, r, g, b);						/* no dither (index-mode), no shift (gives better quality) */

    /* the format of index is RGBP, red|green|blue|pen */
    GetRGBPLocal(index) = (GetRGBPLocal(index) & 0xFFFFFF00) | pen;
    /* the index recovers from index = PtoI[pen] */
    GetIndexLocal(pen) = (GetIndexLocal(pen) & 0xFFFFFF00) | index;
  }
}

/* converts one PaletteMode to another on the run */
void ConvertCMap(amigaMesaContext amesa, palMode newColor, struct Screen *Scr)
{
  DEBUGOUT(1, "ConvertCMap(0x%08x, 0x%x, 0x%08x)\n", amesa, newColor, Scr);

  if (amesa->trueColor != newColor) {
    amesa->trueColor = newColor;

    if (amesa->depth <= 8) {
      FreeCMap(amesa, Scr);
      AllocCMap(amesa, Scr);
      RethinkCMap(amesa);
    }
  }
};
