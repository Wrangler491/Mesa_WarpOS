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

#include "natFastTrianglesDBn8.E"
#include "natFastTrianglesDBnD.E"
#include "natFastTrianglesDBnG.E"

/**********************************************************************/
/*****                 Optimized triangle rendering               *****/
/**********************************************************************/

/* Smooth-shaded, z-less triangle, RGBA color. */
void natSmoothCI32ZTriangle(GLcontext * ctx, GLuint v0, GLuint v1,
				       GLuint v2, GLuint pv)
{
  struct RastPort *rp;
  GLuint *ItoP;

  rp = ((amigaMesaContext) ctx->DriverCtx)->rp;
  ItoP = ((amigaMesaContext) ctx->DriverCtx)->ItoP;

  (void)pv;
#define INTERP_Z	1
#define INTERP_INDEX	1
#define INNER_LOOP(LEFT, RIGHT, Y) {			\
    GLshort i, len = RIGHT - LEFT;			\
    for (i = 0; i < len; i++, zRow++) {			\
      GLdepth z = FixedToDepth(ffz);			\
      if (z < *zRow) {					\
        SetAPen(rp, GetRGBPLocal(FixedToInt(ffi)));	\
        WritePixel(rp, i + LEFT, Y);			\
        *zRow = z;					\
      }							\
      ffi += fdidx;					\
      ffz += fdzdx;					\
    }							\
  }

#include "../../tritemp.h"
}

/* Flat-shaded, z-less triangle, RGBA color. */
void natFlatCI32ZTriangle(GLcontext * ctx, GLuint v0, GLuint v1,
				     GLuint v2, GLuint pv)
{
  struct RastPort *rp = ((amigaMesaContext) ctx->DriverCtx)->rp;

#define INTERP_Z	1
#define SETUP_CODE	SetAPen(rp, GetRGBP(((amigaMesaContext) ctx->DriverCtx), ctx->VB->IndexPtr->data[pvert]));

#define INNER_LOOP(LEFT, RIGHT, Y) {	\
    GLshort i, len = RIGHT - LEFT;	\
    for (i = 0; i < len; i++, zRow++) {	\
      GLdepth z = FixedToDepth(ffz);	\
      if (z < *zRow) {			\
	WritePixel(rp, i + LEFT, Y);	\
        *zRow = z;			\
      }					\
      ffz += fdzdx;			\
    }					\
  }

#include "../../tritemp.h"
}

/* Smooth-shaded, z-less triangle, RGBA color. */
void natSmoothCI32Triangle(GLcontext * ctx, GLuint v0, GLuint v1,
				       GLuint v2, GLuint pv)
{
  struct RastPort *rp;
  GLuint *ItoP;

  rp = ((amigaMesaContext) ctx->DriverCtx)->rp;
  ItoP = ((amigaMesaContext) ctx->DriverCtx)->ItoP;

  (void)pv;
#define INTERP_INDEX	1
#define INNER_LOOP(LEFT, RIGHT, Y) {			\
    GLshort i, len = RIGHT - LEFT;			\
    for (i = 0; i < len; i++) {				\
      SetAPen(rp, GetRGBPLocal(FixedToInt(ffi)));	\
      WritePixel(rp, i + LEFT, Y);			\
      ffi += fdidx;					\
    }							\
  }

#include "../../tritemp.h"
}

/* Flat-shaded, z-less triangle, RGBA color. */
void natFlatCI32Triangle(GLcontext * ctx, GLuint v0, GLuint v1,
				     GLuint v2, GLuint pv)
{
  struct RastPort *rp;

  rp = ((amigaMesaContext) ctx->DriverCtx)->rp;

#define SETUP_CODE	SetAPen(rp, GetRGBP(((amigaMesaContext) ctx->DriverCtx), ctx->VB->IndexPtr->data[pvert]));

#define INNER_LOOP(LEFT, RIGHT, Y) {	\
    GLshort len = RIGHT - LEFT;		\
    while (--len >= 0)			\
      WritePixel(rp, len + LEFT, Y);	\
  }

#include "../../tritemp.h"
}

/* Return pointer to an accelerated triangle function if possible. */
triangle_func test_natChooseTriangleFunction(GLcontext * ctx)
{
  GLboolean RGBMode = ctx->Visual->RGBAflag;
  palMode trueColor = ((amigaMesaContext)ctx->DriverCtx)->trueColor;

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
      if (RGBMode)
	return IS_SHIFT(trueColor) ? nat8SmoothRGBAZTriangle :
	       IS_MATCH(trueColor) ? natGSmoothRGBAZTriangle :
				     natDSmoothRGBAZTriangle;
      else
	return natSmoothCI32ZTriangle;
    }
    else {
      if (RGBMode)
	return IS_SHIFT(trueColor) ? nat8FlatRGBAZTriangle :
	       IS_MATCH(trueColor) ? natGFlatRGBAZTriangle :
				     natDFlatRGBAZTriangle;
      else
	return natFlatCI32ZTriangle;
    }
  }
  else if (ctx->RasterMask == 0) {
    if (ctx->Light.ShadeModel == GL_SMOOTH) {
      if (RGBMode)
	return IS_SHIFT(trueColor) ? nat8SmoothRGBATriangle :
	       IS_MATCH(trueColor) ? natGSmoothRGBATriangle :
				     natDSmoothRGBATriangle;
      else
	return natSmoothCI32Triangle;
    }
    else {
      if (RGBMode)
	return IS_SHIFT(trueColor) ? nat8FlatRGBATriangle :
	       IS_MATCH(trueColor) ? natGFlatRGBATriangle :
				     natDFlatRGBATriangle;
      else
	return natFlatCI32Triangle;
    }
  }

  return NULL;
}
