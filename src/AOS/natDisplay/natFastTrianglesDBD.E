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
void natDSmoothRGBAZTriangleDB(GLcontext * ctx, GLuint v0, GLuint v1,
				       GLuint v2, GLuint pv)
{
  amigamesaContext amesa;
  GLubyte *db;

  amesa = ((amigaMesaContext) ctx->DriverCtx);
  db = dbPenGet(amesa);

  (void)pv;
#define INTERP_Z	1
#define INTERP_RGB	1
#undef	INTERP_ALPHA	/* alpha-channel is useless for amiga (currently) */
#define INNER_LOOP(LEFT, RIGHT, Y) {								\
    GLshort i, len = RIGHT - LEFT;								\
    GLubyte *img = dbPen(db, LEFT, Y);   							\
    for (i = 0; i < len; i++, img++, zRow++) {							\
      GLdepth z = FixedToDepth(ffz);								\
      if (z < *zRow) {										\
        *img = PLD_RGBA(amesa, FixedToInt(ffr), FixedToInt(ffg), FixedToInt(ffb), i + LEFT, Y);	\
        *zRow = z;										\
      }												\
      ffr += fdrdx;  ffg += fdgdx;  ffb += fdbdx; /* ffa += fdadx; */				\
      ffz += fdzdx;										\
    }												\
  }

#include "../../tritemp.h"
}

/* Flat-shaded, z-less triangle, RGBA color. */
void natDFlatRGBAZTriangleDB(GLcontext * ctx, GLuint v0, GLuint v1,
				     GLuint v2, GLuint pv)
{
  GLubyte *db;

  db = dbPenGet(((amigaMesaContext) ctx->DriverCtx));

#define INTERP_Z	1
#define INNER_LOOP(LEFT, RIGHT, Y) {						\
    GLshort i, len = RIGHT - LEFT;						\
    GLubyte *img = dbPen(db, LEFT, Y);   					\
    for (i = 0; i < len; i++, img++, zRow++) {					\
      GLdepth z = FixedToDepth(ffz);						\
      if (z < *zRow) {								\
        *img = PLD_RGBA(((amigaMesaContext) ctx->DriverCtx),			\
					    VB->ColorPtr->data[pv][0],			\
					    VB->ColorPtr->data[pv][1],			\
					    VB->ColorPtr->data[pv][2], i + LEFT, Y);	\
        *zRow = z;								\
      }										\
      ffz += fdzdx;								\
    }										\
  }

#include "../../tritemp.h"
}

/* Smooth-shaded, z-less triangle, RGBA color. */
void natDSmoothRGBATriangleDB(GLcontext * ctx, GLuint v0, GLuint v1,
				       GLuint v2, GLuint pv)
{
  amigaMesaContext amesa;
  GLubyte *db;

  amesa = ((amigaMesaContext) ctx->DriverCtx);
  db = dbPenGet(amesa);

  (void)pv;
#define INTERP_RGB	1
#undef	INTERP_ALPHA	/* alpha-channel is useless for amiga (currently) */
#define INNER_LOOP(LEFT, RIGHT, Y) {								\
    GLshort len = RIGHT - LEFT, i = 0;								\
    GLubyte *img = dbPen(db, LEFT, Y);   							\
    while (--len >= 0) {									\
      *img++ = PLD_RGBA(amesa, FixedToInt(ffr), FixedToInt(ffg), FixedToInt(ffb), i + LEFT, Y);	\
      ffr += fdrdx;  ffg += fdgdx;  ffb += fdbdx; /* ffa += fdadx; */ i++;			\
    }												\
  }

#include "../../tritemp.h"
}

/* Flat-shaded, z-less triangle, RGBA color. */
void natDFlatRGBATriangleDB(GLcontext * ctx, GLuint v0, GLuint v1,
				     GLuint v2, GLuint pv)
{
  GLubyte *db;

  db = dbPenGet(((amigaMesaContext) ctx->DriverCtx));

#define INNER_LOOP(LEFT, RIGHT, Y) {						\
    GLshort len = RIGHT - LEFT;							\
    GLubyte *img = dbPen(db, RIGHT, Y);  					\
    while (--len >= 0) {							\
      *--img = PLD_RGBA(((amigaMesaContext) ctx->DriverCtx),			\
					    VB->ColorPtr->data[pv][0],			\
					    VB->ColorPtr->data[pv][1],			\
					    VB->ColorPtr->data[pv][2], len + LEFT, Y);	\
      i++;									\
    }										\
  }

#include "../../tritemp.h"
}
