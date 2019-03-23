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

/**********************************************************************/
/*****                 Optimized triangle rendering               *****/
/**********************************************************************/

/* Smooth-shaded, z-less triangle, RGBA color. */
void natDSmoothRGBAZTriangle(GLcontext * ctx, GLuint v0, GLuint v1,
				       GLuint v2, GLuint pv)
{
  amigaMesaContext amesa;
  struct RastPort *rp;

  amesa = ((amigaMesaContext) ctx->DriverCtx);
  rp = amesa->rp;

  (void)pv;
#define INTERP_Z	1
#define INTERP_RGB	1
#undef	INTERP_ALPHA	/* alpha-channel is useless for amiga (currently) */
#define INNER_LOOP(LEFT, RIGHT, Y) {									\
    GLshort i, len = RIGHT - LEFT;									\
    for (i = 0; i < len; i++, zRow++) {									\
      GLdepth z = FixedToDepth(ffz);									\
      if (z < *zRow) {											\
        SetAPen(rp, PLD_RGBA(amesa, FixedToInt(ffr), FixedToInt(ffg), FixedToInt(ffb), i + LEFT, Y));	\
        WritePixel(rp, i + LEFT, Y);									\
        *zRow = z;											\
      }													\
      ffr += fdrdx;  ffg += fdgdx;  ffb += fdbdx; /* ffa += fdadx; */					\
      ffz += fdzdx;											\
    }													\
  }

#include "../../tritemp.h"
}

/* Flat-shaded, z-less triangle, RGBA color. */
void natDFlatRGBAZTriangle(GLcontext * ctx, GLuint v0, GLuint v1,
				     GLuint v2, GLuint pv)
{
  struct RastPort *rp;

  rp = ((amigaMesaContext) ctx->DriverCtx)->rp;

#define INTERP_Z	1
#define INNER_LOOP(LEFT, RIGHT, Y) {					\
    GLshort i, len = RIGHT - LEFT;					\
    for (i = 0; i < len; i++, zRow++) {					\
      GLdepth z = FixedToDepth(ffz);					\
      if (z < *zRow) {							\
        SetAPen(rp, PLD_RGBA(((amigaMesaContext) ctx->DriverCtx),	\
		ctx->VB->ColorPtr->data[pvert][0],				\
		ctx->VB->ColorPtr->data[pvert][1],				\
		ctx->VB->ColorPtr->data[pvert][2], i + LEFT, Y));		\
	WritePixel(rp, i + LEFT, Y);					\
        *zRow = z;							\
      }									\
      ffz += fdzdx;							\
    }									\
  }

#include "../../tritemp.h"
}

/* Smooth-shaded, z-less triangle, RGBA color. */
void natDSmoothRGBATriangle(GLcontext * ctx, GLuint v0, GLuint v1,
				       GLuint v2, GLuint pv)
{
  amigaMesaContext amesa;
  struct RastPort *rp;

  amesa = ((amigaMesaContext) ctx->DriverCtx);
  rp = amesa->rp;

  (void)pv;
#define INTERP_RGB	1
#undef	INTERP_ALPHA	/* alpha-channel is useless for amiga (currently) */
#define INNER_LOOP(LEFT, RIGHT, Y) {									\
    GLshort i, len = RIGHT - LEFT;									\
    for (i = 0; i < len; i++) {										\
      SetAPen(rp, PLD_RGBA(amesa, FixedToInt(ffr), FixedToInt(ffg), FixedToInt(ffb), i + LEFT, Y));	\
      WritePixel(rp, i + LEFT, Y);									\
      ffr += fdrdx;  ffg += fdgdx;  ffb += fdbdx; /* ffa += fdadx; */					\
    }													\
  }

#include "../../tritemp.h"
}

/* Flat-shaded, z-less triangle, RGBA color. */
void natDFlatRGBATriangle(GLcontext * ctx, GLuint v0, GLuint v1,
				     GLuint v2, GLuint pv)
{
  struct RastPort *rp;

  rp = ((amigaMesaContext) ctx->DriverCtx)->rp;

#define INNER_LOOP(LEFT, RIGHT, Y) {				\
    GLshort len = RIGHT - LEFT;					\
    while (--len >= 0) {					\
      SetAPen(rp, PLD_RGBA(((amigaMesaContext) ctx->DriverCtx),	\
		ctx->VB->ColorPtr->data[pvert][0],			\
		ctx->VB->ColorPtr->data[pvert][1],			\
		ctx->VB->ColorPtr->data[pvert][2], len + LEFT, Y));	\
      WritePixel(rp, len + LEFT, Y);				\
    }
  }

#include "../../tritemp.h"
}
