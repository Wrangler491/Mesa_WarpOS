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
void cybSmoothRGBAZTriangle(GLcontext * ctx, GLuint v0, GLuint v1,
				     GLuint v2, GLuint pv)
{
  struct RastPort *rp;

  rp = ((amigaMesaContext) ctx->DriverCtx)->rp;

  (void)pv;
#define INTERP_Z	1
#define INTERP_RGB	1
#undef	INTERP_ALPHA	/* alpha-channel is useless for amiga (currently) */
#define INNER_LOOP(LEFT, RIGHT, Y) {									\
    GLshort i, len = RIGHT - LEFT;									\
    for (i = 0; i < len; i++, zRow++) {									\
      GLdepth z = FixedToDepth(ffz);									\
      if (z < *zRow) {										\
	WriteRGBPixel(rp, i + LEFT, Y, TC_RGBA(FixedToInt(ffr), FixedToInt(ffg), FixedToInt(ffb)) >> 8);\
        *zRow = z;											\
      }													\
      ffr += fdrdx;  ffg += fdgdx;  ffb += fdbdx; /* ffa += fdadx; */					\
      ffz += fdzdx;											\
    }													\
  }

#include "../../tritemp.h"
}

/* Smooth-shaded, z-less triangle, RGBA color. */
void cybSmoothCI32ZTriangle(GLcontext * ctx, GLuint v0, GLuint v1,
				     GLuint v2, GLuint pv)
{
  struct RastPort *rp;
  GLuint *ItoP;

  rp = ((amigaMesaContext) ctx->DriverCtx)->rp;
  ItoP = ((amigaMesaContext) ctx->DriverCtx)->ItoP;

  (void)pv;
#define INTERP_Z	1
#define INTERP_INDEX	1
#define INNER_LOOP(LEFT, RIGHT, Y) {						\
    GLshort i, len = RIGHT - LEFT;						\
    for (i = 0; i < len; i++, zRow++) {						\
      GLdepth z = FixedToDepth(ffz);						\
      if (z < *zRow) {								\
	WriteRGBPixel(rp, i + LEFT, Y, GetRGBPLocal(FixedToInt(ffi)) >> 8);	\
        *zRow = z;								\
      }										\
      ffi += fdidx;								\
      ffz += fdzdx;								\
    }										\
  }

#include "../../tritemp.h"
}

/* Flat-shaded, z-less triangle, RGBA color. */
void cybFlatRGBAZTriangle(GLcontext * ctx, GLuint v0, GLuint v1,
				   GLuint v2, GLuint pv)
{
  struct RastPort *rp;

  rp = ((amigaMesaContext) ctx->DriverCtx)->rp;

#define INTERP_Z	1
#define SETUP_CODE	GLuint pixel = TC_RGBA(VB->ColorPtr->data[pv][0], VB->ColorPtr->data[pv][1], VB->ColorPtr->data[pv][2]) >> 8;

#define INNER_LOOP(LEFT, RIGHT, Y) {		\
    GLshort i, len = RIGHT - LEFT;		\
    for (i = 0; i < len; i++, zRow++) {		\
      GLdepth z = FixedToDepth(ffz);		\
      if (z < *zRow) {				\
	WriteRGBPixel(rp, i + LEFT, Y, pixel);	\
        *zRow = z;				\
      }						\
      ffz += fdzdx;				\
    }						\
  }

#include "../../tritemp.h"
}

/* Flat-shaded, z-less triangle, RGBA color. */
void cybFlatCI32ZTriangle(GLcontext * ctx, GLuint v0, GLuint v1,
				   GLuint v2, GLuint pv)
{
  struct RastPort *rp;
  GLuint *ItoP;

  rp = ((amigaMesaContext) ctx->DriverCtx)->rp;
  ItoP = ((amigaMesaContext) ctx->DriverCtx)->ItoP;

#define INTERP_Z	1
#define SETUP_CODE	GLuint pixel = GetRGBPLocal(VB->IndexPtr->data[pv][0]) >> 8;

#define INNER_LOOP(LEFT, RIGHT, Y) {		\
    GLshort i, len = RIGHT - LEFT;		\
    for (i = 0; i < len; i++, zRow++) {		\
      GLdepth z = FixedToDepth(ffz);		\
      if (z < *zRow) {				\
	WriteRGBPixel(rp, i + LEFT, Y, pixel);	\
        *zRow = z;				\
      }						\
      ffz += fdzdx;				\
    }						\
  }

#include "../../tritemp.h"
}

/* Smooth-shaded, z-less triangle, RGBA color. */
void cybSmoothRGBATriangle(GLcontext * ctx, GLuint v0, GLuint v1,
				     GLuint v2, GLuint pv)
{
  struct RastPort *rp;

  rp = ((amigaMesaContext) ctx->DriverCtx)->rp;

  (void)pv;
#define INTERP_RGB	1
#undef	INTERP_ALPHA	/* alpha-channel is useless for amiga (currently) */
#define INNER_LOOP(LEFT, RIGHT, Y) {									\
    GLshort i, len = RIGHT - LEFT;									\
    for (i = 0; i < len; i++) {										\
      WriteRGBPixel(rp, i + LEFT, Y, TC_RGBA(FixedToInt(ffr), FixedToInt(ffg), FixedToInt(ffb)) >> 8);	\
      ffr += fdrdx;  ffg += fdgdx;  ffb += fdbdx; /* ffa += fdadx; */					\
    }													\
  }

#include "../../tritemp.h"
}

/* Smooth-shaded, z-less triangle, RGBA color. */
void cybSmoothCI32Triangle(GLcontext * ctx, GLuint v0, GLuint v1,
				     GLuint v2, GLuint pv)
{
  struct RastPort *rp;
  GLuint *ItoP;

  rp = ((amigaMesaContext) ctx->DriverCtx)->rp;
  ItoP = ((amigaMesaContext) ctx->DriverCtx)->ItoP;

  (void)pv;
#define INTERP_INDEX	1
#define INNER_LOOP(LEFT, RIGHT, Y) {						\
    GLshort i, len = RIGHT - LEFT;						\
    for (i = 0; i < len; i++) {							\
      WriteRGBPixel(rp, i + LEFT, Y, GetRGBPLocal(FixedToInt(ffi)) >> 8);	\
      ffi += fdidx;								\
    }										\
  }

#include "../../tritemp.h"
}

/* Flat-shaded, z-less triangle, RGBA color. */
void cybFlatRGBATriangle(GLcontext * ctx, GLuint v0, GLuint v1,
				   GLuint v2, GLuint pv)
{
  struct RastPort *rp;

  rp = ((amigaMesaContext) ctx->DriverCtx)->rp;

#define SETUP_CODE	GLuint pixel = TC_RGBA(VB->ColorPtr->data[pv][0], VB->ColorPtr->data[pv][1], VB->ColorPtr->data[pv][2]) >> 8;

#define INNER_LOOP(LEFT, RIGHT, Y) {		\
    GLshort len = RIGHT - LEFT;			\
    while (--len >= 0)				\
      WriteRGBPixel(rp, len + LEFT, Y, pixel);	\
  }

#include "../../tritemp.h"
}

/* Flat-shaded, z-less triangle, RGBA color. */
void cybFlatCI32Triangle(GLcontext * ctx, GLuint v0, GLuint v1,
				   GLuint v2, GLuint pv)
{
  struct RastPort *rp;
  GLuint *ItoP;

  rp = ((amigaMesaContext) ctx->DriverCtx)->rp;
  ItoP = ((amigaMesaContext) ctx->DriverCtx)->ItoP;

#define SETUP_CODE	GLuint pixel = GetRGBPLocal(VB->IndexPtr->data[pv][0]) >> 8;

#define INNER_LOOP(LEFT, RIGHT, Y) {		\
    GLshort len = RIGHT - LEFT;			\
    while (--len >= 0)				\
      WriteRGBPixel(rp, len + LEFT, Y, pixel);	\
  }

#include "../../tritemp.h"
}

/* Return pointer to an accelerated triangle function if possible. */
triangle_func test_cybChooseTriangleFunction(GLcontext * ctx)
{
  GLboolean rgbmode = ctx->Visual->RGBAflag;

  if (ctx->Polygon.SmoothFlag)
    return NULL;
  if (ctx->Polygon.StippleFlag)
    return NULL;
  if (ctx->Texture.Enabled)
    return NULL;

  if (ctx->RasterMask == 0) {
    if (ctx->Light.ShadeModel == GL_SMOOTH)
      return rgbmode ? cybSmoothRGBATriangle : cybSmoothCI32Triangle;
    else
      return rgbmode ? cybFlatRGBATriangle : cybFlatCI32Triangle;
  }
  else if ((ctx->RasterMask == DEPTH_BIT) &&
	   (ctx->Depth.Func == GL_LESS) &&
	   (ctx->Depth.Mask)) {
    if (ctx->Light.ShadeModel == GL_SMOOTH)
      return rgbmode ? cybSmoothRGBAZTriangle : cybSmoothCI32ZTriangle;
    else
      return rgbmode ? cybFlatRGBAZTriangle : cybFlatCI32ZTriangle;
  }

  return NULL;
}
