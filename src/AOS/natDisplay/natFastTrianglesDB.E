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

#include "natFastTrianglesDB8.E"
#include "natFastTrianglesDBD.E"
#include "natFastTrianglesDBG.E"

/**********************************************************************/
/*****                 Optimized triangle rendering               *****/
/**********************************************************************/

/* Smooth-shaded, z-less triangle, RGBA color. */
void natSmoothCI32ZTriangleDB(GLcontext * ctx, GLuint v0, GLuint v1,
				       GLuint v2, GLuint pv)
{
  GLubyte *db;
  GLuint *ItoP;

  db = dbPenGet(((amigaMesaContext) ctx->DriverCtx));
  ItoP = ((amigaMesaContext) ctx->DriverCtx)->ItoP;

  (void)pv;
#define INTERP_Z	1
#define INTERP_INDEX	1
#define INNER_LOOP(LEFT, RIGHT, Y) {		\
    GLshort i, len = RIGHT - LEFT;		\
    GLubyte *img = dbPen(db, LEFT, Y);   	\
    for (i = 0; i < len; i++, img++, zRow++) {	\
      GLdepth z = FixedToDepth(ffz);		\
      if (z < *zRow) {				\
        *img = GetRGBPLocal(FixedToInt(ffi));	\
        *zRow = z;				\
      }						\
      ffi += fdidx;				\
      ffz += fdzdx;				\
    }						\
  }

#include "../../tritemp.h"
}

/* Flat-shaded, z-less triangle, RGBA color. */
void natFlatCI32ZTriangleDB(GLcontext * ctx, GLuint v0, GLuint v1,
				     GLuint v2, GLuint pv)
{
  GLubyte *db;
  GLuint *ItoP;

  db = dbPenGet(((amigaMesaContext) ctx->DriverCtx));
  ItoP = ((amigaMesaContext) ctx->DriverCtx)->ItoP;

#define INTERP_Z	1
#define SETUP_CODE	GLuint pixel = GetRGBPLocal(VB->IndexPtr->data[pv][0]);

#define INNER_LOOP(LEFT, RIGHT, Y) {		\
    GLshort i, len = RIGHT - LEFT;		\
    GLubyte *img = dbPen(db, LEFT, Y);   	\
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
void natSmoothCI32TriangleDB(GLcontext * ctx, GLuint v0, GLuint v1,
				       GLuint v2, GLuint pv)
{
  GLubyte *db;
  GLuint *ItoP;

  db = dbPenGet(((amigaMesaContext) ctx->DriverCtx));
  ItoP = ((amigaMesaContext) ctx->DriverCtx)->ItoP;

  (void)pv;
#define INTERP_INDEX	1
#define INNER_LOOP(LEFT, RIGHT, Y) {		\
    GLshort len = RIGHT - LEFT;			\
    GLubyte *img = dbPen(db, LEFT, Y);   	\
    while (--len >= 0)				\
      *img++ = GetRGBPLocal(FixedToInt(ffi));	\
      ffi += fdidx;				\
    }						\
  }

#include "../../tritemp.h"
}

/* Flat-shaded, z-less triangle, RGBA color. */
void natFlatCI32TriangleDB(GLcontext * ctx, GLuint v0, GLuint v1,
				     GLuint v2, GLuint pv)
{
  GLubyte *db;
  GLuint *ItoP;

  db = dbPenGet(((amigaMesaContext) ctx->DriverCtx));
  ItoP = ((amigaMesaContext) ctx->DriverCtx)->ItoP;

#define SETUP_CODE	GLuint pixel = GetRGBPLocal(VB->IndexPtr->data[pv][0]);

#define INNER_LOOP(LEFT, RIGHT, Y) {	\
    GLshort len = RIGHT - LEFT;		\
    GLubyte *img = dbPen(db, LEFT, Y);  \
    while (--len >= 0)			\
      *img++ = pixel;			\
  }

#include "../../tritemp.h"
}

/* Return pointer to an accelerated triangle function if possible. */
triangle_func test_natChooseTriangleFunctionDB(GLcontext * ctx)
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
	return IS_SHIFT(trueColor) ? nat8SmoothRGBAZTriangleDB :
	       IS_MATCH(trueColor) ? natGSmoothRGBAZTriangleDB :
				     natDSmoothRGBAZTriangleDB;
      else
	return natSmoothCI32ZTriangleDB;
    }
    else {
      if (RGBMode)
	return IS_SHIFT(trueColor) ? nat8FlatRGBAZTriangleDB :
	       IS_MATCH(trueColor) ? natGFlatRGBAZTriangleDB :
				     natDFlatRGBAZTriangleDB;
      else
	return natFlatCI32ZTriangleDB;
    }
  }
  else if (ctx->RasterMask == 0) {
    if (ctx->Light.ShadeModel == GL_SMOOTH) {
      if (RGBMode)
	return IS_SHIFT(trueColor) ? nat8SmoothRGBATriangleDB :
	       IS_MATCH(trueColor) ? natGSmoothRGBATriangleDB :
				     natDSmoothRGBATriangleDB;
      else
	return natSmoothCI32TriangleDB;
    }
    else {
      if (RGBMode)
	return IS_SHIFT(trueColor) ? nat8FlatRGBATriangleDB :
	       IS_MATCH(trueColor) ? natGFlatRGBATriangleDB :
				     natDFlatRGBATriangleDB;
      else
	return natFlatCI32TriangleDB;
    }
  }

  return NULL;
}
