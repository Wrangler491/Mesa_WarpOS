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

#include "natFastLinesDBn8.E"
#include "natFastLinesDBnD.E"
#include "natFastLinesDBnG.E"

/**********************************************************************/
/*****                   Optimized line rendering                 *****/
/**********************************************************************/

/* Draw a flat-shaded, RGB line into an osmesa buffer. */
void natFlatCI32Line(GLcontext * ctx, GLuint vert0, GLuint vert1, GLuint pvert)
{
  struct RastPort *rp;

  rp = ((amigaMesaContext) ctx->DriverCtx)->rp;

  SetAPen(rp, GetRGBP(((amigaMesaContext) ctx->DriverCtx), ctx->VB->IndexPtr->data[pvert]));

#define INTERP_XY	1
#define CLIP_HACK	1
#define PLOT(X,Y)	WritePixel(rp, X, Y)

#include "../../linetemp.h"
}

/* Draw a flat-shaded, Z-less, RGB line into an osmesa buffer. */
void natFlatCI32ZLine(GLcontext * ctx, GLuint vert0, GLuint vert1, GLuint pvert)
{
  struct RastPort *rp;

  rp = ((amigaMesaContext) ctx->DriverCtx)->rp;

  SetAPen(rp, GetRGBP(((amigaMesaContext) ctx->DriverCtx), ctx->VB->IndexPtr->data[pvert]));

#define INTERP_XY	1
#define INTERP_Z	1
#define CLIP_HACK	1
#define PLOT(X,Y)		\
  if (Z < *zPtr) {		\
    WritePixel(rp, X, Y);	\
    *zPtr = Z;			\
  }

#include "../../linetemp.h"
}

/*
 * Analyze context state to see if we can provide a fast line drawing
 * function, like those in lines.c.  Otherwise, return NULL.
 */
line_func test_natChooseLineFunction(GLcontext * ctx)
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
	return IS_SHIFT(trueColor) ? nat8FlatRGBALine :
	       IS_MATCH(trueColor) ? natGFlatRGBALine :
				     natDFlatRGBALine;
      else
	return natFlatCI32Line;
      break;
    case DEPTH_BIT:
      if ((ctx->Depth.Func == GL_LESS) &&
	  (ctx->Depth.Mask))
	if (RGBMode)
	  return IS_SHIFT(trueColor) ? nat8FlatRGBAZLine :
		 IS_MATCH(trueColor) ? natGFlatRGBAZLine :
				       natDFlatRGBAZLine;
	else
	  return natFlatCI32ZLine;
      break;
    case BLEND_BIT:
      if ((ctx->Color.BlendSrc == GL_SRC_ALPHA) &&
	  (ctx->Color.BlendDst == GL_ONE_MINUS_SRC_ALPHA) &&
	  (ctx->Color.BlendEquation == GL_FUNC_ADD_EXT))
	return IS_SHIFT(trueColor) ? nat8FlatBlendRGBALine :
	       IS_MATCH(trueColor) ? natGFlatBlendRGBALine :
				     natDFlatBlendRGBALine;
      break;
    case DEPTH_BIT | BLEND_BIT:
      if ((ctx->Depth.Func == GL_LESS) &&
	  (ctx->Color.BlendSrc == GL_SRC_ALPHA) &&
	  (ctx->Color.BlendDst == GL_ONE_MINUS_SRC_ALPHA) &&
	  (ctx->Color.BlendEquation == GL_FUNC_ADD_EXT)) {
	if (ctx->Depth.Mask)
	  return IS_SHIFT(trueColor) ? nat8FlatBlendRGBAZLineWrite :
		 IS_MATCH(trueColor) ? natGFlatBlendRGBAZLineWrite :
				       natDFlatBlendRGBAZLineWrite;
	else
	  return IS_SHIFT(trueColor) ? nat8FlatBlendRGBAZLine :
		 IS_MATCH(trueColor) ? natGFlatBlendRGBAZLine :
				       natDFlatBlendRGBAZLine;
      }
      break;
    default:
      break;
  }

  return NULL;
}
