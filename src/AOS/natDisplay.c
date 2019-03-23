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

#undef	NO_CONTEXT_AVAILABLE
#include <AOS/amigamesa.h>
#include "AOS/accessArray.h"

#define	MyWritePixelLine8	WritePixelLine8
#define	MyReadPixelLine8	ReadPixelLine8

/**********************************************************************/
/*****                Miscellaneous device driver funcs           *****/
/**********************************************************************/

const char *natRendererString(void)
{
  return "Native standard\n";
}

const char *natRendererStringDB(void)
{
  return "Native fast\n";
}

void natClearIndex(GLcontext * ctx, GLuint index)
{
  amigaMesaContext amesa;

  /*
   * implement glClearIndex 
   * usually just save the value in the context struct 
   */
  amesa = (amigaMesaContext) ctx->DriverCtx;

  DEBUGOUT(1, "natClearIndex(%d)\n", index);

  /* RGBP (same as RGBA if P is ignored, same as CI8 if RGB is ignored) */
  amesa->clearpixel = GetRGBP(amesa, index);
}

void natClearColor(GLcontext * ctx,
		   GLubyte r, GLubyte g, GLubyte b, GLubyte a)
{
  amigaMesaContext amesa;

  /*
   * implement glClearColor 
   * color components are floats in [0,1] 
   * usually just save the value in the context struct 
   *
   * @@@ TODO FREE COLOR IF NOT USED 
   */
  amesa = (amigaMesaContext) ctx->DriverCtx;

  DEBUGOUT(1, "natClearColor(%d, %d, %d, %d)\n", r, g, b, a);

  /* TODO: make a pattern of 2*16 size and fill		*
   * the area with the pattern istead of this color	*/
  amesa->clearpixel = MakeRGBP(amesa, (GLubyte) r, (GLubyte) g, (GLubyte) b, 0, 0);
}

GLbitfield natClear(GLcontext * ctx, GLbitfield mask, GLboolean all,
		    GLint x, GLint y, GLint width, GLint height)
{
  amigaMesaContext amesa;

  /*
   * Clear the specified region of the color buffer using the clear color
   * or index as specified by one of the two functions above.
   * If all==GL_TRUE, clear whole buffer
   */

  amesa = (amigaMesaContext) ctx->DriverCtx;

  DEBUGOUT(1, "natClear(%d, %d, %d, %d, %d)\n", all, FIXx(x), FIXy(y), width, height);

  SetAPen(amesa->rp, amesa->clearpixel);
  if (all) {
    struct gl_viewport_attrib *vport = &ctx->Viewport;

    DEBUGOUT(8, " RectFill(%d,%d,%d,%d);\n",
	     FIXx(vport->X),
	     FIXy(vport->Y) - vport->Height,
	     FIXx(vport->X) + vport->Width - 1,
	     FIXy(vport->Y) - 1);

    RectFill(amesa->rp, FIXx(vport->X),
	     FIXy(vport->Y) - vport->Height,
	     FIXx(vport->X) + vport->Width - 1,
	     FIXy(vport->Y) - 1);
  }
  else {
    DEBUGOUT(8, " RectFill(%d,%d,%d,%d);\n",
	     FIXx(x),
	     FIXy(y) - height,
	     FIXx(x) + width - 1,
	     FIXy(y) - 1);

    RectFill(amesa->rp, FIXx(x),
	     FIXy(y) - height,
	     FIXx(x) + width - 1,
	     FIXy(y) - 1);
  }

  return mask & (~GL_COLOR_BUFFER_BIT);
}

GLbitfield natClearDB(GLcontext * ctx, GLbitfield mask, GLboolean all,
		      GLint x, GLint y, GLint width, GLint height)
{
  amigaMesaContext amesa;
  GLubyte *db;
  GLubyte col;

  /*
   * Clear the specified region of the color buffer using the clear color
   * or index as specified by one of the two functions above.
   * If all==GL_TRUE, clear whole buffer
   */

  amesa = (amigaMesaContext) ctx->DriverCtx;
  col = amesa->clearpixel;

  DEBUGOUT(1, "natClearDB(%d, %d, %d, %d, %d)\n", all, FIXx(x), FIXy(y), width, height);

  if (all) {
    GLint size = amesa->FixedWidth * amesa->FixedHeight;

  /*db = dbPen(dbPenGet(amesa), 0, 0);*/
    db = amesa->BackArray;							/* really clear all (Fixed), not only Real */
    while (--size >= 0)
      *db++ = col;
  }
  else {
    GLshort x1, y1, x2, y2;

    y2 = y + width;
    x2 = x + height;
    db = dbPenGet(amesa);

    for (y1 = y; y1 < y2; y1++) {
      GLubyte *img = dbPen(db, y1, x);
      for (x1 = x; x1 < x2; x1++)
	*img++ = col;
    }
  }

  return mask & (~GL_COLOR_BUFFER_BIT);
}

void natSetIndex(GLcontext * ctx, GLuint index)
{
  amigaMesaContext amesa;

  /*
   * Set the amesa color index. 
   */

  amesa = (amigaMesaContext) ctx->DriverCtx;

  DEBUGOUT(1, "natSetIndex(%d)\n", index);

  /* RGBP (same as RGBA if P is ignored, same as CI8 if RGB is ignored) */
  amesa->pixel = GetRGBP(amesa, index);
}

void natSetColor(GLcontext * ctx, GLubyte r, GLubyte g, GLubyte b, GLubyte a)
{
  amigaMesaContext amesa;

  /*
   * Set the current RGBA color. 
   * r is in 0..255.RedScale 
   * g is in 0..255.GreenScale 
   * b is in 0..255.BlueScale 
   * a is in 0..255.AlphaScale 
   */

  amesa = (amigaMesaContext) ctx->DriverCtx;

  DEBUGOUT(1, "natSetColor(%d, %d, %d, %d)\n", r, g, b, a);

  /* MonoCI functions need the rgb-value in case of dithering */
  amesa->pixel = MakeRGBP(amesa, (GLubyte) r, (GLubyte) g, (GLubyte) b, 0, 0);
}

GLboolean natIndexMask(GLcontext * ctx, GLuint mask)
{
  amigaMesaContext amesa;

  /*
   * implement glIndexMask if possible, else return GL_FALSE 
   */

  amesa = (amigaMesaContext) ctx->DriverCtx;

  DEBUGOUT(1, "natIndexMask(0x%x)\n", mask);

  amesa->rp->Mask = (GLubyte) mask;

  return (GL_TRUE);
}

GLboolean natColorMask(GLcontext * ctx, GLboolean rmask, GLboolean gmask, GLboolean bmask, GLboolean amask)
{
  amigaMesaContext amesa;

  /*
   * implement glColorMask if possible, else return GL_FALSE 
   */

  amesa = (amigaMesaContext) ctx->DriverCtx;

  DEBUGOUT(1, "natColorMask()\n");

  if (rmask || gmask || bmask)
    return GL_TRUE;

  return GL_FALSE;
}

/*
 * OPTIONAL FUNCTION: 
 * Implements glLogicOp if possible.  Return GL_TRUE if the device driver
 * can perform the operation, otherwise return GL_FALSE.  If GL_FALSE
 * is returned, the logic op will be done in software by Mesa.
 */
GLboolean natLogicOp(GLcontext * ctx, GLenum op)
{
  /* TODO: minterms etc. */
  switch (op) {
    case GL_CLEAR:
    case GL_SET:
    case GL_COPY:
    case GL_COPY_INVERTED:
    case GL_NOOP:
    case GL_INVERT:
    case GL_AND:
    case GL_NAND:
    case GL_OR:
    case GL_NOR:
    case GL_XOR:
    case GL_EQUIV:
    case GL_AND_REVERSE:
    case GL_AND_INVERTED:
    case GL_OR_REVERSE:
    case GL_OR_INVERTED:
    default:
      break;
  }

  return (GL_FALSE);
}

#include "natDisplay/natFastLines.c"
#include "natDisplay/natFastPoints.c"
#include "natDisplay/natFastPixels.c"
#include "natDisplay/natFastTriangles.c"

#include "natDisplay/natSpans.c"
#include "natDisplay/natPixel.c"

/**********************************************************************/
/*****                  amiga/Mesa Private Functions              *****/
/**********************************************************************/
/*
 * Initialize all the pointers in the DD struct.  Do this whenever   
 * a new context is made current or we change buffers via setBuffer! 
 */
void natStandardDDPointers(GLcontext * ctx)
{
  palMode trueColor = ((amigaMesaContext) ctx->DriverCtx)->trueColor;

  DEBUGOUT(1, "natStandardDDPointers()\n");

  ctx->Driver.RendererString = natRendererString;

  ctx->Driver.UpdateState = natStandardDDPointers;
  ctx->Driver.ClearIndex = natClearIndex;
  ctx->Driver.ClearColor = natClearColor;
  ctx->Driver.Clear = natClear;

  ctx->Driver.Index = natSetIndex;
  ctx->Driver.Color = natSetColor;

  ctx->Driver.IndexMask = natIndexMask;
  ctx->Driver.ColorMask = natColorMask;

  ctx->Driver.SetBuffer = cmnSetBuffer;
  ctx->Driver.GetBufferSize = cmnStandardResize;

  ctx->Driver.RenderStart = cmnRenderStart;
  ctx->Driver.RenderFinish = cmnRenderFinish;
  ctx->Driver.Dither = cmnDither;
  ctx->Driver.Flush = NULL;							/* cmnFlush; */

  ctx->Driver.PointsFunc = natChoosePointsFunction(ctx);
  ctx->Driver.LineFunc = natChooseLineFunction(ctx);
  ctx->Driver.TriangleFunc = natChooseTriangleFunction(ctx);
  ctx->Driver.QuadFunc = natChooseQuadFunction(ctx);
  ctx->Driver.RectFunc = natChooseRectFunction(ctx);

  ctx->Driver.DrawPixels = NULL;

  /* Pixel/span writing functions: */
  ctx->Driver.WriteRGBASpan = IS_SHIFT(trueColor) ? nat8WriteRGBASpan :
			      IS_MATCH(trueColor) ? natGWriteRGBASpan :
						    natDWriteRGBASpan;
  ctx->Driver.WriteRGBSpan = IS_SHIFT(trueColor) ? nat8WriteRGBSpan :
			     IS_MATCH(trueColor) ? natGWriteRGBSpan :
						   natDWriteRGBSpan;
  ctx->Driver.WriteCI32Span = natWriteCI32Span;
  ctx->Driver.WriteCI8Span = natWriteCI8Span;
  ctx->Driver.WriteMonoRGBASpan =
  ctx->Driver.WriteMonoCISpan = IS_DITHER(trueColor) ? natDWriteMonoCISpan :
						       natWriteMonoCISpan;

  ctx->Driver.WriteRGBAPixels = IS_SHIFT(trueColor) ? nat8WriteRGBAPixel :
			        IS_MATCH(trueColor) ? natGWriteRGBAPixel :
						      natDWriteRGBAPixel;
  ctx->Driver.WriteCI32Pixels = natWriteCI32Pixel;
  ctx->Driver.WriteMonoRGBAPixels =
  ctx->Driver.WriteMonoCIPixels = IS_DITHER(trueColor) ? natDWriteMonoCIPixel :
							 natWriteMonoCIPixel;

  /* Pixel/span reading functions: */
  ctx->Driver.ReadRGBASpan = natReadRGBASpan;
  ctx->Driver.ReadCI32Span = natReadCI32Span;

  ctx->Driver.ReadRGBAPixels = natReadRGBAPixel;
  ctx->Driver.ReadCI32Pixels = natReadCI32Pixel;
}

void natFasterDDPointers(GLcontext * ctx)
{
  palMode trueColor = ((amigaMesaContext) ctx->DriverCtx)->trueColor;

  DEBUGOUT(1, "natFasterDDPointers()\n");

  ctx->Driver.RendererString = natRendererStringDB;

  ctx->Driver.UpdateState = natFasterDDPointers;
  ctx->Driver.ClearIndex = natClearIndex;
  ctx->Driver.ClearColor = natClearColor;
  ctx->Driver.Clear = natClearDB;

  ctx->Driver.Index = natSetIndex;
  ctx->Driver.Color = natSetColor;

  ctx->Driver.IndexMask = natIndexMask;
  ctx->Driver.ColorMask = natColorMask;

  ctx->Driver.SetBuffer = cmnSetBufferDB;
  ctx->Driver.GetBufferSize = cmnStandardResizeDB;

  ctx->Driver.RenderStart = cmnRenderStartDB;
  ctx->Driver.RenderFinish = cmnRenderFinishDB;
  ctx->Driver.Dither = cmnDitherDB;
  ctx->Driver.Flush = cmnFlushDB;

  ctx->Driver.PointsFunc = natChoosePointsFunctionDB(ctx);
  ctx->Driver.LineFunc = natChooseLineFunctionDB(ctx);
  ctx->Driver.TriangleFunc = natChooseTriangleFunctionDB(ctx);
  ctx->Driver.QuadFunc = natChooseQuadFunctionDB(ctx);
  ctx->Driver.RectFunc = natChooseRectFunctionDB(ctx);

  ctx->Driver.DrawPixels = NULL;

  /* Pixel/span writing functions: */
  ctx->Driver.WriteRGBASpan = IS_SHIFT(trueColor) ? nat8WriteRGBASpanDB :
			      IS_MATCH(trueColor) ? natGWriteRGBASpanDB :
						    natDWriteRGBASpanDB;
  ctx->Driver.WriteRGBSpan = IS_SHIFT(trueColor) ? nat8WriteRGBSpanDB :
			     IS_MATCH(trueColor) ? natGWriteRGBSpanDB :
						   natDWriteRGBSpanDB;
  ctx->Driver.WriteCI32Span = natWriteCI32SpanDB;
  ctx->Driver.WriteCI8Span = natWriteCI8SpanDB;
  ctx->Driver.WriteMonoRGBASpan =
  ctx->Driver.WriteMonoCISpan = IS_DITHER(trueColor) ? natDWriteMonoCISpanDB :
						       natWriteMonoCISpanDB;

  ctx->Driver.WriteRGBAPixels = IS_SHIFT(trueColor) ? nat8WriteRGBAPixelDB :
			        IS_MATCH(trueColor) ? natGWriteRGBAPixelDB :
						      natDWriteRGBAPixelDB;
  ctx->Driver.WriteCI32Pixels = natWriteCI32PixelDB;
  ctx->Driver.WriteMonoRGBAPixels =
  ctx->Driver.WriteMonoCIPixels = IS_DITHER(trueColor) ? natDWriteMonoCIPixelDB :
							 natWriteMonoCIPixelDB;

  /* Pixel/span reading functions: */
  ctx->Driver.ReadRGBASpan = natReadRGBASpanDB;
  ctx->Driver.ReadCI32Span = natReadCI32SpanDB;

  ctx->Driver.ReadRGBAPixels = natReadRGBAPixelDB;
  ctx->Driver.ReadCI32Pixels = natReadCI32PixelDB;
}

#undef DEBUGPRINT
