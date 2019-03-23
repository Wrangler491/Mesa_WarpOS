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
void cybSmoothRGBAZTriangleDB(GLcontext * ctx, GLuint v0, GLuint v1,
				     GLuint v2, GLuint pv)
{
  GLuint *db;

  db = dbRGBAGet(((amigaMesaContext) ctx->DriverCtx));

  (void)pv;
#define INTERP_Z	1
#define INTERP_RGB	1
#undef	INTERP_ALPHA	/* alpha-channel is useless for amiga (currently) */
#define INNER_LOOP(LEFT, RIGHT, Y) {						\
    GLshort i, len = RIGHT - LEFT;						\
    GLuint *img = dbRGBA(db, LEFT, Y);   					\
    for (i = 0; i < len; i++, img++, zRow++) {					\
      GLdepth z = FixedToDepth(ffz);						\
      if (z < *zRow) {								\
        *img = TC_RGBA(FixedToInt(ffr), FixedToInt(ffg), FixedToInt(ffb));	\
        *zRow = z;								\
      }										\
      ffr += fdrdx;  ffg += fdgdx;  ffb += fdbdx; /* ffa += fdadx; */		\
      ffz += fdzdx;								\
    }										\
  }

#include "../../tritemp.h"
}

/* Smooth-shaded, z-less triangle, RGBA color. */
void cybSmoothCI32ZTriangleDB(GLcontext * ctx, GLuint v0, GLuint v1,
				     GLuint v2, GLuint pv)
{
  GLuint *db;
  GLuint *ItoP;

  db = dbRGBAGet(((amigaMesaContext) ctx->DriverCtx));
  ItoP = ((amigaMesaContext) ctx->DriverCtx)->ItoP;

  (void)pv;
#define INTERP_Z	1
#define INTERP_INDEX	1
#define INNER_LOOP(LEFT, RIGHT, Y) {						\
    GLshort i, len = RIGHT - LEFT;						\
    GLuint *img = dbRGBA(db, LEFT, Y);   					\
    for (i = 0; i < len; i++, img++, zRow++) {					\
      GLdepth z = FixedToDepth(ffz);						\
      if (z < *zRow) {								\
        *img = GetRGBPLocal(FixedToInt(ffi));					\
        *zRow = z;								\
      }										\
      ffi += fdidx;								\
      ffz += fdzdx;								\
    }										\
  }

#include "../../tritemp.h"
}

/* Flat-shaded, z-less triangle, RGBA color. */
void cybFlatRGBAZTriangleDB(GLcontext * ctx, GLuint v0, GLuint v1,
				   GLuint v2, GLuint pv)
{
  GLuint *db;

  db = dbRGBAGet(((amigaMesaContext) ctx->DriverCtx));

#define INTERP_Z	1
#define SETUP_CODE	GLuint pixel = TC_RGBA(VB->ColorPtr->data[pv][0], VB->ColorPtr->data[pv][1], VB->ColorPtr->data[pv][2]);

#define INNER_LOOP(LEFT, RIGHT, Y) {		\
    GLshort i, len = RIGHT - LEFT;		\
    GLuint *img = dbRGBA(db, LEFT, Y);   	\
    for (i = 0; i < len; i++, img++, zRow++) {	\
      GLdepth z = FixedToDepth(ffz);		\
      if (z < *zRow) {				\
        *img = pixel;				\
        *zRow = z;				\
      }						\
      ffz += fdzdx;				\
    }						\
  }

#include "../../tritemp.h"
}

/* Flat-shaded, z-less triangle, RGBA color. */
void cybFlatCI32ZTriangleDB(GLcontext * ctx, GLuint v0, GLuint v1,
				   GLuint v2, GLuint pv)
{
  GLuint *db;
  GLuint *ItoP;

  db = dbRGBAGet(((amigaMesaContext) ctx->DriverCtx));
  ItoP = ((amigaMesaContext) ctx->DriverCtx)->ItoP;

#define INTERP_Z	1
#define SETUP_CODE	GLuint pixel = GetRGBPLocal(VB->IndexPtr->data[pv][0]);

#define INNER_LOOP(LEFT, RIGHT, Y) {		\
    GLshort i, len = RIGHT - LEFT;		\
    GLuint *img = dbRGBA(db, LEFT, Y);   	\
    for (i = 0; i < len; i++, img++, zRow++) {	\
      GLdepth z = FixedToDepth(ffz);		\
      if (z < *zRow) {				\
        *img = pixel;				\
        *zRow = z;				\
      }						\
      ffz += fdzdx;				\
    }						\
  }

#include "../../tritemp.h"
}

/* Smooth-shaded, z-less triangle, RGBA color. */
void cybSmoothRGBATriangleDB(GLcontext * ctx, GLuint v0, GLuint v1,
				     GLuint v2, GLuint pv)
{
  GLuint *db;

  db = dbRGBAGet(((amigaMesaContext) ctx->DriverCtx));

  (void)pv;
#define INTERP_RGB	1
#undef	INTERP_ALPHA	/* alpha-channel is useless for amiga (currently) */
#define INNER_LOOP(LEFT, RIGHT, Y) {						\
    GLuint *img = dbRGBA(db, LEFT, Y);   					\
    GLshort len = RIGHT - LEFT;							\
    while (--len >= 0)								\
      *img++ = TC_RGBA(FixedToInt(ffr), FixedToInt(ffg), FixedToInt(ffb));	\
      ffr += fdrdx;  ffg += fdgdx;  ffb += fdbdx; /* ffa += fdadx; */		\
    }										\
  }

#include "../../tritemp.h"
}

/* Smooth-shaded, z-less triangle, RGBA color. */
void cybSmoothCI32TriangleDB(GLcontext * ctx, GLuint v0, GLuint v1,
				     GLuint v2, GLuint pv)
{
  GLuint *db;
  GLuint *ItoP;

  db = dbRGBAGet(((amigaMesaContext) ctx->DriverCtx));
  ItoP = ((amigaMesaContext) ctx->DriverCtx)->ItoP;

  (void)pv;
#define INTERP_INDEX	1
#define INNER_LOOP(LEFT, RIGHT, Y) {		\
    GLuint *img = dbRGBA(db, LEFT, Y);   	\
    GLshort len = RIGHT - LEFT;			\
    while (--len >= 0) {			\
      *img++ = GetRGBPLocal(FixedToInt(ffi));	\
      ffi += fdidx;				\
    }						\
  }

#include "../../tritemp.h"
}

/* Flat-shaded, z-less triangle, RGBA color. */
void cybFlatRGBATriangleDB(GLcontext * ctx, GLuint v0, GLuint v1,
				   GLuint v2, GLuint pv)
{
  GLuint *db;

  db = dbRGBAGet(((amigaMesaContext) ctx->DriverCtx));

#define SETUP_CODE	GLuint pixel = TC_RGBA(VB->ColorPtr->data[pv][0], VB->ColorPtr->data[pv][1], VB->ColorPtr->data[pv][2]);

#define INNER_LOOP(LEFT, RIGHT, Y) {	\
    GLuint *img = dbRGBA(db, LEFT, Y);  \
    GLshort len = RIGHT - LEFT;		\
    while (--len >= 0)			\
      *img++ = pixel;			\
  }

#include "../../tritemp.h"
}

/* Flat-shaded, z-less triangle, RGBA color. */
void cybFlatCI32TriangleDB(GLcontext * ctx, GLuint v0, GLuint v1,
				   GLuint v2, GLuint pv)
{
  GLuint *db;
  GLuint *ItoP;

  db = dbRGBAGet(((amigaMesaContext) ctx->DriverCtx));
  ItoP = ((amigaMesaContext) ctx->DriverCtx)->ItoP;

#define SETUP_CODE	GLuint pixel = GetRGBPLocal(VB->IndexPtr->data[pv][0]);

#define INNER_LOOP(LEFT, RIGHT, Y) {	\
    GLuint *img = dbRGBA(db, LEFT, Y);  \
    GLshort len = RIGHT - LEFT;		\
    while (--len >= 0)			\
      *img++ = pixel;			\
  }

#include "../../tritemp.h"
}

/* Return pointer to an accelerated triangle function if possible. */
triangle_func test_cybChooseTriangleFunctionDB(GLcontext * ctx)
{
  GLboolean rgbmode = ctx->Visual->RGBAflag;

  if (ctx->Polygon.SmoothFlag)
    return NULL;
  if (ctx->Polygon.StippleFlag)
    return NULL;
  if (ctx->Texture.Enabled)
    return NULL;

  if ((ctx->RasterMask == DEPTH_BIT) &&
      (ctx->Depth.Func == GL_LESS) &&
      (ctx->Depth.Mask)) {
    if (ctx->Light.ShadeModel == GL_SMOOTH) {
      if (rgbmode)
	return cybSmoothRGBAZTriangle;
      else
	return cybSmoothCI32ZTriangle;
    }
    else {
      if (rgbmode)
	return cybFlatRGBAZTriangle;
      else
	return cybFlatCI32ZTriangle;
    }
  }
  else if (ctx->RasterMask == 0) {
    if (ctx->Light.ShadeModel == GL_SMOOTH) {
      if (rgbmode)
	return cybSmoothRGBATriangle;
      else
	return cybSmoothCI32Triangle;
    }
    else {
      if (rgbmode)
	return cybFlatRGBATriangle;
      else
	return cybFlatCI32Triangle;
    }
  }

  return NULL;
}
