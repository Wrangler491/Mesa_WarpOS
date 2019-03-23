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
/*****                   Optimized line rendering                 *****/
/**********************************************************************/

/* Draw a flat-shaded, RGB line into an osmesa buffer. */
void cybFlatRGBALineDB(GLcontext * ctx, GLuint vert0, GLuint vert1, GLuint pvert)
{
  GLuint *db;
  GLuint pixel;

  db = dbRGBAGet(((amigaMesaContext) ctx->DriverCtx));
  pixel = TC_RGBA(ctx->VB->ColorPtr->data[pvert][0], ctx->VB->ColorPtr->data[pvert][1], ctx->VB->ColorPtr->data[pvert][2]);

#define INTERP_XY	1
#define CLIP_HACK	1
#define PLOT(X,Y)	*dbRGBA(db, X, Y) = pixel

#include "../../linetemp.h"
}

/* Draw a flat-shaded, RGB line into an osmesa buffer. */
void cybFlatCI32LineDB(GLcontext * ctx, GLuint vert0, GLuint vert1, GLuint pvert)
{
  GLuint *db;
  GLuint pixel;

  db = dbRGBAGet(((amigaMesaContext) ctx->DriverCtx));
  pixel = GetRGBP(((amigaMesaContext) ctx->DriverCtx), ctx->VB->IndexPtr->data[pvert])

#define INTERP_XY	1
#define CLIP_HACK	1
#define PLOT(X,Y)	*dbRGBA(db, X, Y) = pixel

#include "../../linetemp.h"
}

/* Draw a flat-shaded, Z-less, RGB line into an osmesa buffer. */
void cybFlatRGBAZLineDB(GLcontext * ctx, GLuint vert0, GLuint vert1, GLuint pvert)
{
  GLuint *db;
  GLuint pixel;

  db = dbRGBAGet(((amigaMesaContext) ctx->DriverCtx));
  pixel = TC_RGBA(ctx->VB->ColorPtr->data[pvert][0], ctx->VB->ColorPtr->data[pvert][1], ctx->VB->ColorPtr->data[pvert][2]);

#define INTERP_XY	1
#define INTERP_Z	1
#define CLIP_HACK	1
#define PLOT(X,Y)		\
  if (Z < *zPtr) {		\
    *dbRGBA(db, X, Y) = pixel;	\
    *zPtr = Z;			\
  }

#include "../../linetemp.h"
}

/* Draw a flat-shaded, Z-less, RGB line into an osmesa buffer. */
void cybFlatCI32ZLineDB(GLcontext * ctx, GLuint vert0, GLuint vert1, GLuint pvert)
{
  GLuint *db;
  GLuint pixel;

  db = dbRGBAGet(((amigaMesaContext) ctx->DriverCtx));
  pixel = GetRGBP(((amigaMesaContext) ctx->DriverCtx), ctx->VB->IndexPtr->data[pvert]);

#define INTERP_XY	1
#define INTERP_Z	1
#define CLIP_HACK	1
#define PLOT(X,Y)		\
  if (Z < *zPtr) {		\
    *dbRGBA(db, X, Y) = pixel;	\
    *zPtr = Z;			\
  }

#include "../../linetemp.h"
}

/* Draw a flat-shaded, alpha-blended, RGB line into an osmesa buffer. */
void cybFlatBlendRGBALineDB(GLcontext * ctx, GLuint vert0, GLuint vert1, GLuint pvert)
{
  GLuint *db;
  struct vertex_buffer *VB;
  GLint avalue, msavalue, rvalue, gvalue, bvalue;

  db = dbRGBAGet(((amigaMesaContext) ctx->DriverCtx));
  VB = ctx->VB;
  avalue = VB->ColorPtr->data[pvert][3];
  msavalue = 255 - avalue;
  rvalue = VB->ColorPtr->data[pvert][0] * avalue;
  gvalue = VB->ColorPtr->data[pvert][1] * avalue;
  bvalue = VB->ColorPtr->data[pvert][2] * avalue;

#define INTERP_XY	1
#define CLIP_HACK	1
#define PLOT(X,Y) {								\
    GLuint *ptr4 = dbRGBA(db, X, Y);						\
    *ptr4 = TC_RGBA((((((*ptr4) >> 24) & 0xff) * msavalue + rvalue) >> 8),	\
		    (((((*ptr4) >> 16) & 0xff) * msavalue + gvalue) >> 8),	\
		    (((((*ptr4) >>  8) & 0xff) * msavalue + bvalue) >> 8));	\
  }

#include "../../linetemp.h"
}

/* Draw a flat-shaded, Z-less, alpha-blended, RGB line into an osmesa buffer. */
void cybFlatBlendRGBAZLineDB(GLcontext * ctx, GLuint vert0, GLuint vert1, GLuint pvert)
{
  GLuint *db;
  struct vertex_buffer *VB;
  GLint avalue, msavalue, rvalue, gvalue, bvalue;

  db = dbRGBAGet(((amigaMesaContext) ctx->DriverCtx));
  VB = ctx->VB;
  avalue = VB->ColorPtr->data[pvert][3];
  msavalue = 255 - avalue;
  rvalue = VB->ColorPtr->data[pvert][0] * avalue;
  gvalue = VB->ColorPtr->data[pvert][1] * avalue;
  bvalue = VB->ColorPtr->data[pvert][2] * avalue;

#define INTERP_XY	1
#define INTERP_Z	1
#define CLIP_HACK	1
#define PLOT(X,Y)								\
  if (Z < *zPtr) {								\
    GLuint *ptr4 = dbRGBA(db, X, Y); 						\
    *ptr4 = TC_RGBA((((((*ptr4) >> 24) & 0xff) * msavalue + rvalue) >> 8),	\
		    (((((*ptr4) >> 16) & 0xff) * msavalue + gvalue) >> 8),	\
		    (((((*ptr4) >>  8) & 0xff) * msavalue + bvalue) >> 8));	\
  }

#include "../../linetemp.h"
}

/* Draw a flat-shaded, Z-less, alpha-blended, RGB line into an osmesa buffer. */
void cybFlatBlendRGBAZLineWriteDB(GLcontext * ctx, GLuint vert0, GLuint vert1, GLuint pvert)
{
  GLuint *db;
  struct vertex_buffer *VB;
  GLint avalue, msavalue, rvalue, gvalue, bvalue;

  db = dbRGBAGet(((amigaMesaContext) ctx->DriverCtx));
  VB = ctx->VB;
  avalue = VB->ColorPtr->data[pvert][3];
  msavalue = 255 - avalue;
  rvalue = VB->ColorPtr->data[pvert][0] * avalue;
  gvalue = VB->ColorPtr->data[pvert][1] * avalue;
  bvalue = VB->ColorPtr->data[pvert][2] * avalue;

#define INTERP_XY	1
#define INTERP_Z	1
#define CLIP_HACK	1
#define PLOT(X,Y)								\
  if (Z < *zPtr) {								\
    GLuint *ptr4 = dbRGBA(db, X, Y); 						\
    *ptr4 = TC_RGBA((((((*ptr4) >> 24) & 0xff) * msavalue + rvalue) >> 8),	\
		    (((((*ptr4) >> 16) & 0xff) * msavalue + gvalue) >> 8),	\
		    (((((*ptr4) >>  8) & 0xff) * msavalue + bvalue) >> 8));	\
    *zPtr = Z;									\
  }

#include "../../linetemp.h"
}

/*
 * Analyze context state to see if we can provide a fast line drawing
 * function, like those in lines.c.  Otherwise, return NULL.
 */
line_func test_cybChooseLineFunctionDB(GLcontext * ctx)
{
  GLboolean rgbmode = ctx->Visual->RGBAflag;

  if (ctx->Line.SmoothFlag)
    return NULL;
  if (ctx->Texture.Enabled)
    return NULL;
  if (ctx->Light.ShadeModel != GL_FLAT)
    return NULL;
  if (ctx->Line.Width != 1.0F)
    return NULL;
  if (ctx->Line.StippleFlag)
    return NULL;

  switch (ctx->RasterMask) {
    case 0:
      if (rgbmode)
	return cybFlatRGBALine;
      else
	return cybFlatCI32Line;
      break;
    case DEPTH_BIT:
      if ((ctx->Depth.Func == GL_LESS) &&
	  (ctx->Depth.Mask))
	if (rgbmode)
	  return cybFlatRGBAZLine;
	else
	  return cybFlatCI32ZLine;
      break;
    case BLEND_BIT:
      if ((ctx->Color.BlendSrc == GL_SRC_ALPHA) &&
	  (ctx->Color.BlendDst == GL_ONE_MINUS_SRC_ALPHA) &&
	  (ctx->Color.BlendEquation == GL_FUNC_ADD_EXT))
	return cybFlatBlendRGBALine;
      break;
    case DEPTH_BIT | BLEND_BIT:
      if ((ctx->Depth.Func == GL_LESS) &&
	  (ctx->Color.BlendSrc == GL_SRC_ALPHA) &&
	  (ctx->Color.BlendDst == GL_ONE_MINUS_SRC_ALPHA) &&
	  (ctx->Color.BlendEquation == GL_FUNC_ADD_EXT)) {
	if (ctx->Depth.Mask)
	  return cybFlatBlendRGBAZLineWrite;
	else
	  return cybFlatBlendRGBAZLine;
      }
      break;
    default:
      break;
  }

  return NULL;
}
