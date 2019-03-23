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

#include "natFastLinesDB8.E"
#include "natFastLinesDBD.E"
#include "natFastLinesDBG.E"

/**********************************************************************/
/*****                   Optimized line rendering                 *****/
/**********************************************************************/

/* Draw a flat-shaded, RGB line into an osmesa buffer. */
void natFlatCI32LineDB(GLcontext * ctx, GLuint vert0, GLuint vert1, GLuint pvert)
{
  GLubyte *db;
  GLubyte pixel;

  db = dbPenGet(((amigaMesaContext) ctx->DriverCtx));
  pixel = GetRGBP(((amigaMesaContext) ctx->DriverCtx), ctx->VB->IndexPtr->data[pvert]);

#define INTERP_XY	1
#define CLIP_HACK	1
#define PLOT(X,Y)	*dbPen(db, X, Y) = pixel

#include "../../linetemp.h"
}

/* Draw a flat-shaded, Z-less, RGB line into an osmesa buffer. */
void natFlatCI32ZLineDB(GLcontext * ctx, GLuint vert0, GLuint vert1, GLuint pvert)
{
  GLubyte *db;
  GLubyte pixel;

  db = dbPenGet(((amigaMesaContext) ctx->DriverCtx));
  pixel = GetRGBP(((amigaMesaContext) ctx->DriverCtx), ctx->VB->IndexPtr->data[pvert]);

#define INTERP_XY	1
#define INTERP_Z	1
#define CLIP_HACK	1
#define PLOT(X,Y)		\
  if (Z < *zPtr) {		\
    *dbPen(db, X, Y) = pixel;	\
    *zPtr = Z;			\
  }

#include "../../linetemp.h"
}

/*
 * Analyze context state to see if we can provide a fast line drawing
 * function, like those in lines.c.  Otherwise, return NULL.
 */
line_func test_natChooseLineFunctionDB(GLcontext * ctx)
{
  GLboolean RGBMode = ctx->Visual->RGBAflag;
  palMode trueColor = ((amigaMesaContext)ctx->DriverCtx)->trueColor;

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
      if (RGBMode)
	return IS_SHIFT(trueColor) ? nat8FlatRGBALineDB :
	       IS_MATCH(trueColor) ? natGFlatRGBALineDB :
				     natDFlatRGBALineDB;
      else
	return natFlatCI32LineDB;
      break;
    case DEPTH_BIT:
      if ((ctx->Depth.Func == GL_LESS) &&
	  (ctx->Depth.Mask))
	if (RGBMode)
	  return IS_SHIFT(trueColor) ? nat8FlatRGBAZLineDB :
		 IS_MATCH(trueColor) ? natGFlatRGBAZLineDB :
				       natDFlatRGBAZLineDB;
	else
	  return natFlatCI32ZLineDB;
      break;
    case BLEND_BIT:
      if ((ctx->Color.BlendSrc == GL_SRC_ALPHA) &&
	  (ctx->Color.BlendDst == GL_ONE_MINUS_SRC_ALPHA) &&
	  (ctx->Color.BlendEquation == GL_FUNC_ADD_EXT))
	return IS_SHIFT(trueColor) ? nat8FlatBlendRGBALineDB :
	       IS_MATCH(trueColor) ? natGFlatBlendRGBALineDB :
				     natDFlatBlendRGBALineDB;
      break;
    case DEPTH_BIT | BLEND_BIT:
      if ((ctx->Depth.Func == GL_LESS) &&
	  (ctx->Color.BlendSrc == GL_SRC_ALPHA) &&
	  (ctx->Color.BlendDst == GL_ONE_MINUS_SRC_ALPHA) &&
	  (ctx->Color.BlendEquation == GL_FUNC_ADD_EXT)) {
	if (ctx->Depth.Mask)
	  return IS_SHIFT(trueColor) ? nat8FlatBlendRGBAZLineWriteDB :
		 IS_MATCH(trueColor) ? natGFlatBlendRGBAZLineWriteDB :
				       natDFlatBlendRGBAZLineWriteDB;
	else
	  return IS_SHIFT(trueColor) ? nat8FlatBlendRGBAZLineDB :
		 IS_MATCH(trueColor) ? natGFlatBlendRGBAZLineDB :
				       natDFlatBlendRGBAZLineDB;
      }
      break;
    default:
      break;
  }

  return NULL;
}
