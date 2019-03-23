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

/*
 * $Log:  $
 */

#undef	NO_CONTEXT_AVAILABLE
#include <AOS/amigamesa.h>
#include "AOS/accessArray.h"

#define	MyWritePixelArray	WritePixelArray
#define	MyReadPixelArray	ReadPixelArray
#define	MyWriteLUTPixelArray	WriteLUTPixelArray

/**********************************************************************/
/*****                Miscellaneous device driver funcs           *****/
/**********************************************************************/

const char *cybRendererString(void)
{
  return "CyberGraphX standard\n";
}

const char *cybRendererStringDB(void)
{
  return "CyberGraphX fast\n";
}

void cybClearIndex(GLcontext * ctx, GLuint index)
{
  amigaMesaContext amesa;

  /*
   * implement glClearIndex 
   * usually just save the value in the context struct 
   */

  amesa = (amigaMesaContext) ctx->DriverCtx;

  DEBUGOUT(1, "cybClearIndex(%d)\n", index);

  amesa->clearpixel = GetRGBP(amesa, index);
}

void cybClearColor(GLcontext * ctx,
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

  DEBUGOUT(1, "cybClearColor(%d, %d, %d, %d)\n", r, g, b, a);

  amesa->clearpixel = MakeRGBP(amesa, (GLubyte) r, (GLubyte) g, (GLubyte) b, 0, 0);
}

GLbitfield cybClear(GLcontext * ctx, GLbitfield mask, GLboolean all,
		    GLint x, GLint y, GLint width, GLint height)
{
  amigaMesaContext amesa;

  /*
   * Clear the specified region of the color buffer using the clear color
   * or index as specified by one of the two functions above.
   * If all==GL_TRUE, clear whole buffer
   */

  amesa = (amigaMesaContext) ctx->DriverCtx;

  DEBUGOUT(1, "cybClear(%d, %d, %d, %d, %d)\n", all, x, y, width, height);

  if (all) {
    struct gl_viewport_attrib *vport = &ctx->Viewport;

    DEBUGOUT(2, " FillPixelArray(%d,%d,%d,%d);\n",
	     FIXx(vport->X),
	     FIXy(vport->Y) - vport->Height,
	     FIXx(vport->X) + vport->Width - 1,
	     FIXy(vport->Y) - 1);

    FillPixelArray(amesa->rp,
		   FIXx(vport->X),
		   FIXy(vport->Y) - vport->Height,
		   vport->Width,
		   vport->Height,
		   amesa->clearpixel >> 8);
  }
  else {
    DEBUGOUT(2, " FillPixelArray(%d,%d,%d,%d);\n",
	     FIXx(x),
	     FIXy(y) - height,
	     FIXx(x) + width - 1,
	     FIXy(y) - 1);

    FillPixelArray(amesa->rp, FIXx(x),
		   FIXy(y) - height,
		   width,
		   height,
		   amesa->clearpixel >> 8);
  }

  return mask & (~GL_COLOR_BUFFER_BIT);
}

GLbitfield cybClearDB(GLcontext * ctx, GLbitfield mask, GLboolean all,
		      GLint x, GLint y, GLint width, GLint height)
{
  amigaMesaContext amesa;
  GLuint *db;
  GLuint col;

  /*
   * Clear the specified region of the color buffer using the clear color
   * or index as specified by one of the two functions above.
   * If all==GL_TRUE, clear whole buffer
   */

  amesa = (amigaMesaContext) ctx->DriverCtx;

  DEBUGOUT(1, "cybClearDB(%d, %d, %d, %d, %d)\n", all, x, y, width, height);

  col = amesa->clearpixel;
  if (all) {
    GLint size = (amesa->FixedWidth * amesa->FixedHeight);

  /*db = dbRGBA(dbRGBAGet(amesa), 0, 0);*/
    db = (GLuint *)amesa->BackArray;						/* really clear all (Fixed), not only Real */

    while (--size >= 0)
      *db++ = col;
  }
  else {
    GLshort x1, y1, x2, y2;

    y2 = y + width;
    x2 = x + height;
    db = dbRGBAGet(amesa);

    for (y1 = y; y1 < y2; y1++) {
      GLuint *img = dbRGBA(db, y1, x);
      for (x1 = x; x1 < x2; x1++)
	*img++ = col;
    }
  }

  return mask & (~GL_COLOR_BUFFER_BIT);
}

void cybSetIndex(GLcontext * ctx, GLuint index)
{
  amigaMesaContext amesa;

  /*
   * Set the amesa color index. 
   */

  amesa = (amigaMesaContext) ctx->DriverCtx;

  DEBUGOUT(1, "cybSetIndex(%d)\n", index);

  amesa->pixel = GetRGBP(amesa, index);
}

void cybSetColor(GLcontext * ctx, GLubyte r, GLubyte g, GLubyte b, GLubyte a)
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

  DEBUGOUT(1, "cybSetColor(%d, %d, %d, %d)\n", r, g, b, a);

  amesa->pixel = MakeRGBP(amesa, (GLubyte) r, (GLubyte) g, (GLubyte) b, 0, 0);
}

GLboolean cybIndexMask(GLcontext * ctx, GLuint mask)
{
  amigaMesaContext amesa;

  /*
   * implement glIndexMask if possible, else return GL_FALSE 
   */

  amesa = (amigaMesaContext) ctx->DriverCtx;

  DEBUGOUT(1, "cybIndexMask(0x%x)\n", mask);

  amesa->rp->Mask = (GLubyte) mask;

  return (GL_TRUE);
}

GLboolean cybColorMask(GLcontext * ctx, GLboolean rmask, GLboolean gmask, GLboolean bmask, GLboolean amask)
{
  amigaMesaContext amesa;

  /*
   * implement glColorMask if possible, else return GL_FALSE 
   */

  amesa = (amigaMesaContext) ctx->DriverCtx;

  DEBUGOUT(1, "cybColorMask()\n");

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
GLboolean cybLogicOp(GLcontext * ctx, GLenum op)
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

  return GL_FALSE;
}

#include "cybDisplay/cybFastLines.c"
#include "cybDisplay/cybFastPixels.c"
#include "cybDisplay/cybFastPoints.c"
#include "cybDisplay/cybFastTriangles.c"

#include "cybDisplay/cybSpans.c"
#include "cybDisplay/cybPixel.c"

/**********************************************************************/
/*****                  amiga/Mesa Private Functions              *****/
/**********************************************************************/
/*
 * Initialize all the pointers in the DD struct.  Do this whenever   
 * a new context is made current or we change buffers via setBuffer! 
 */
void cybStandardDDPointers(GLcontext * ctx)
{
  palMode trueColor = ((amigaMesaContext) ctx->DriverCtx)->trueColor;

  DEBUGOUT(1, "cybStandardDDPointers()\n");

  ctx->Driver.RendererString = cybRendererString;

  ctx->Driver.UpdateState = cybStandardDDPointers;
  ctx->Driver.ClearIndex = cybClearIndex;
  ctx->Driver.ClearColor = cybClearColor;
  ctx->Driver.Clear = cybClear;

  ctx->Driver.Index = cybSetIndex;
  ctx->Driver.Color = cybSetColor;

  ctx->Driver.IndexMask = cybIndexMask;
  ctx->Driver.ColorMask = cybColorMask;
  ctx->Driver.LogicOp = cybLogicOp;

  ctx->Driver.SetBuffer = cmnSetBuffer;
  ctx->Driver.GetBufferSize = cmnStandardResize;

  ctx->Driver.RenderStart = cmnRenderStart;
  ctx->Driver.RenderFinish = cmnRenderFinish;
  ctx->Driver.Dither = NULL;
  ctx->Driver.Flush = NULL;							/* cmnFlush; */

  ctx->Driver.PointsFunc = cybChoosePointsFunction(ctx);
  ctx->Driver.LineFunc = cybChooseLineFunction(ctx);
  ctx->Driver.TriangleFunc = cybChooseTriangleFunction(ctx);
  ctx->Driver.QuadFunc = cybChooseQuadFunction(ctx);
  ctx->Driver.RectFunc = cybChooseRectFunction(ctx);

  ctx->Driver.DrawPixels = cybDirectDrawPixels;

  /* Pixel/span writing functions: */
  ctx->Driver.WriteRGBASpan = cybWriteRGBASpan;
  ctx->Driver.WriteRGBSpan = cybWriteRGBSpan;
  ctx->Driver.WriteCI32Span = cybWriteCI32Span;
  ctx->Driver.WriteCI8Span = cybWriteCI8Span;
  ctx->Driver.WriteMonoRGBASpan = cybWriteMonoCISpan;
  ctx->Driver.WriteMonoCISpan = cybWriteMonoCISpan;

  ctx->Driver.WriteRGBAPixels = cybWriteRGBAPixel;
  ctx->Driver.WriteCI32Pixels = cybWriteCI32Pixel;
  ctx->Driver.WriteMonoRGBAPixels = cybWriteMonoCIPixel;
  ctx->Driver.WriteMonoCIPixels = cybWriteMonoCIPixel;

  /* Pixel/span reading functions: */
  ctx->Driver.ReadRGBASpan = cybReadRGBASpan;
  ctx->Driver.ReadCI32Span = IS_SHIFT(trueColor) ? cyb8ReadCI32Span :
			     IS_MATCH(trueColor) ? cybGReadCI32Span :
						   cybDReadCI32Span;

  ctx->Driver.ReadRGBAPixels = cybReadRGBAPixel;
  ctx->Driver.ReadCI32Pixels = IS_SHIFT(trueColor) ? cyb8ReadCI32Pixel :
			       IS_MATCH(trueColor) ? cybGReadCI32Pixel :
						     cybDReadCI32Pixel;
}

void cybFasterDDPointers(GLcontext * ctx)
{
  palMode trueColor = ((amigaMesaContext) ctx->DriverCtx)->trueColor;

  DEBUGOUT(1, "cybFasterDDPointers()\n");

  ctx->Driver.RendererString = cybRendererStringDB;

  ctx->Driver.UpdateState = cybFasterDDPointers;
  ctx->Driver.ClearIndex = cybClearIndex;
  ctx->Driver.ClearColor = cybClearColor;
  ctx->Driver.Clear = cybClearDB;

  ctx->Driver.Index = cybSetIndex;
  ctx->Driver.Color = cybSetColor;

  ctx->Driver.IndexMask = cybIndexMask;
  ctx->Driver.ColorMask = cybColorMask;
  ctx->Driver.LogicOp = cybLogicOp;

  ctx->Driver.SetBuffer = cmnSetBufferDB;
  ctx->Driver.GetBufferSize = cmnStandardResizeDB;

  ctx->Driver.RenderStart = cmnRenderStartDB;
  ctx->Driver.RenderFinish = cmnRenderFinishDB;
  ctx->Driver.Dither = NULL;
  ctx->Driver.Flush = cmnFlushDB;

  ctx->Driver.PointsFunc = cybChoosePointsFunctionDB(ctx);
  ctx->Driver.LineFunc = cybChooseLineFunctionDB(ctx);
  ctx->Driver.TriangleFunc = cybChooseTriangleFunctionDB(ctx);
  ctx->Driver.QuadFunc = cybChooseQuadFunctionDB(ctx);
  ctx->Driver.RectFunc = cybChooseRectFunctionDB(ctx);

  ctx->Driver.DrawPixels = cybDirectDrawPixelsDB;

  /* Pixel/span writing functions: */
  ctx->Driver.WriteRGBASpan = cybWriteRGBASpanDB;
  ctx->Driver.WriteRGBSpan = cybWriteRGBSpanDB;
  ctx->Driver.WriteCI32Span = cybWriteCI32SpanDB;
  ctx->Driver.WriteCI8Span = cybWriteCI8SpanDB;
  ctx->Driver.WriteMonoRGBASpan = cybWriteMonoCISpanDB;				/* same  */
  ctx->Driver.WriteMonoCISpan = cybWriteMonoCISpanDB;

  ctx->Driver.WriteRGBAPixels = cybWriteRGBAPixelDB;
  ctx->Driver.WriteCI32Pixels = cybWriteCI32PixelDB;
  ctx->Driver.WriteMonoRGBAPixels = cybWriteMonoCIPixelDB;			/* same  */
  ctx->Driver.WriteMonoCIPixels = cybWriteMonoCIPixelDB;

  /* Pixel/span reading functions: */
  ctx->Driver.ReadRGBASpan = cybReadRGBASpanDB;
  ctx->Driver.ReadCI32Span = IS_SHIFT(trueColor) ? cyb8ReadCI32SpanDB :
			     IS_MATCH(trueColor) ? cybGReadCI32SpanDB :
						   cybDReadCI32SpanDB;

  ctx->Driver.ReadRGBAPixels = cybReadRGBAPixelDB;
  ctx->Driver.ReadCI32Pixels = IS_SHIFT(trueColor) ? cyb8ReadCI32PixelDB :
			       IS_MATCH(trueColor) ? cybGReadCI32PixelDB :
						     cybDReadCI32PixelDB;
}

#undef DEBUGPRINT
