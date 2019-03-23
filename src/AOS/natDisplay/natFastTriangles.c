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

#ifdef	TRYTEST
#include "natFastTrianglesDBn.c"
#include "natFastTrianglesDB.c"
#endif

/**********************************************************************/
/*****            Accelerated point, line, polygon rendering      *****/
/**********************************************************************/

/*
 * Draw a filled polygon of a single color. If there is hardware/OS support
 * for polygon drawing use that here.   Otherwise, call a function in
 * polygon.c to do the drawing.
 */

void natFastTriangleFunction(GLcontext * ctx, GLuint v0, GLuint v1, GLuint v2, GLuint pv)
{
  amigaMesaContext amesa;
  struct RastPort *rp;
  struct vertex_buffer *VB;
  int x, y;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  VB = ctx->VB;
  rp = amesa->rp;

  DEBUGOUT(1, "natFastLineFunction\n");

  if (!TstF(amesa->visual->flags, VISUAL_RGBMODE))
    SetAPen(rp, GetRGBP(amesa, VB->IndexPtr->data[pv]));
  else
    SetAPen(rp, PL_RGBA(amesa, VB->ColorPtr->data[pv][0], VB->ColorPtr->data[pv][1], VB->ColorPtr->data[pv][2], 0, 0));

  x = FIXx((int)(VB->Win.data[v0][0]));
  y = FIXy((int)(VB->Win.data[v0][1]));
  DEBUGOUT(8, " AreaMove(%d, %d)\n", x, y);
  AreaMove(rp, x, y);

  x = FIXx((int)(VB->Win.data[v1][0]));
  y = FIXy((int)(VB->Win.data[v1][1]));
  DEBUGOUT(8, " AreaDraw(%d, %d)\n", x, y);
  AreaDraw(rp, x, y);

  x = FIXx((int)(VB->Win.data[v2][0]));
  y = FIXy((int)(VB->Win.data[v2][1]));
  DEBUGOUT(8, " AreaDraw(%d, %d)\n", x, y);
  AreaDraw(rp, x, y);
 
  AreaEnd(rp);
}

triangle_func natChooseTriangleFunction(GLcontext * ctx)
{
  /*
   * Examine the current rendering state and return a pointer to a 
   * fast polygon-rendering function if possible. 
   */

  DEBUGOUT(1, "natChooseTriangleFunction()\n");

  if (!IS_DITHER((amigaMesaContext) ctx->DriverCtx) &&
      (ctx->Line.Width == 1.0) &&
      (!ctx->Line.SmoothFlag) &&
      (!ctx->Line.StippleFlag) &&
      (ctx->Light.ShadeModel == GL_FLAT) &&
      (ctx->RasterMask == 0) &&							/* what means rastermask really? */
      (!ctx->Texture.Enabled))
    return natFastTriangleFunction;
#ifdef	TRYTEST
  if (preset.tryTest)
    return test_natChooseTriangleFunction(ctx);
#endif

#if 0
  DEBUGOUT(0, "Line.Width = %g\nLine.SmoothFlag = %d\nLine.StippleFlag = %d\nLight.ShadeModel = %x\nRasterMask = %x\nTexture.Enabled = %d\n",
	  ctx->Line.Width, ctx->Line.SmoothFlag, ctx->Line.StippleFlag, ctx->Light.ShadeModel, ctx->RasterMask, ctx->Texture.Enabled);
#endif

  return NULL;
}

triangle_func natChooseTriangleFunctionDB(GLcontext * ctx)
{
  DEBUGOUT(1, "natChooseTriangleFunctionDB()\n");

#ifdef	TRYTEST
  if (preset.tryTest)
    return test_natChooseTriangleFunctionDB(ctx);
#endif

  return NULL;
}

void natFastQuadFunction(GLcontext * ctx, GLuint v0, GLuint v1, GLuint v2, GLuint v3, GLuint pv)
{
  amigaMesaContext amesa;
  struct RastPort *rp;
  struct vertex_buffer *VB;
  int x, y;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  VB = ctx->VB;
  rp = amesa->rp;

  DEBUGOUT(1, "natFastQuadFunction\n");

  if (!TstF(amesa->visual->flags, VISUAL_RGBMODE))
    SetAPen(rp, GetRGBP(amesa, VB->IndexPtr->data[pv]));
  else
    SetAPen(rp, PL_RGBA(amesa, VB->ColorPtr->data[pv][0], VB->ColorPtr->data[pv][1], VB->ColorPtr->data[pv][2], 0, 0));

  x = FIXx((int)(VB->Win.data[v0][0]));
  y = FIXy((int)(VB->Win.data[v0][1]));
  DEBUGOUT(8, " AreaMove(%d, %d)\n", x, y);
  AreaMove(rp, x, y);

  x = FIXx((int)(VB->Win.data[v1][0]));
  y = FIXy((int)(VB->Win.data[v1][1]));
  DEBUGOUT(8, " AreaDraw(%d, %d)\n", x, y);
  AreaDraw(rp, x, y);

  x = FIXx((int)(VB->Win.data[v2][0]));
  y = FIXy((int)(VB->Win.data[v2][1]));
  DEBUGOUT(8, " AreaDraw(%d, %d)\n", x, y);
  AreaDraw(rp, x, y);

  x = FIXx((int)(VB->Win.data[v3][0]));
  y = FIXy((int)(VB->Win.data[v3][1]));
  DEBUGOUT(8, " AreaDraw(%d, %d)\n", x, y);
  AreaDraw(rp, x, y);

  AreaEnd(rp);
}

quad_func natChooseQuadFunction(GLcontext * ctx)
{
  /*
   * Examine the current rendering state and return a pointer to a 
   * fast polygon-rendering function if possible. 
   */

  DEBUGOUT(1, "natChooseQuadFunction()\n");

  if (!IS_DITHER((amigaMesaContext) ctx->DriverCtx) &&
      (ctx->Line.Width == 1.0) &&
      (!ctx->Line.SmoothFlag) &&
      (!ctx->Line.StippleFlag) &&
      (ctx->Light.ShadeModel == GL_FLAT) &&
      (ctx->RasterMask == 0) &&							/* what means rastermask really? */
      (!ctx->Texture.Enabled))
    return natFastQuadFunction;
#ifdef	TRYTEST
  if (preset.tryTest)
    return test_natChooseQuadFunction(ctx);
#endif

#if 0
  DEBUGOUT(0, "Line.Width = %g\nLine.SmoothFlag = %d\nLine.StippleFlag = %d\nLight.ShadeModel = %x\nRasterMask = %x\nTexture.Enabled = %d\n",
	  ctx->Line.Width, ctx->Line.SmoothFlag, ctx->Line.StippleFlag, ctx->Light.ShadeModel, ctx->RasterMask, ctx->Texture.Enabled);
#endif

  return NULL;
}

quad_func natChooseQuadFunctionDB(GLcontext * ctx)
{
  DEBUGOUT(1, "natChooseQuadFunctionDB()\n");

#ifdef	TRYTEST
  if (preset.tryTest)
    return test_natChooseQuadFunctionDB(ctx);
#endif

  return NULL;
}

void natFastRectFunction(GLcontext * ctx, GLint x, GLint y, GLint width, GLint height)
{
  amigaMesaContext amesa;
  struct RastPort *rp;

  amesa = (amigaMesaContext) ctx->DriverCtx;

  DEBUGOUT(1, "natFastRectFunction()\n");

  SetAPen(amesa->rp, amesa->pixel);
  RectFill(amesa->rp, FIXx(x),
		      FIXy(y) - height,
		      FIXx(x) + width - 1,
		      FIXy(y) - 1);
}

rect_func natChooseRectFunction(GLcontext * ctx)
{
  /*
   * Examine the current rendering state and return a pointer to a 
   * fast polygon-rendering function if possible. 
   */

  DEBUGOUT(1, "natChooseRectFunction()\n");

  if (!IS_DITHER((amigaMesaContext) ctx->DriverCtx) &&
      (ctx->Line.Width == 1.0) &&
      (!ctx->Line.SmoothFlag) &&
      (!ctx->Line.StippleFlag) &&
      (ctx->Light.ShadeModel == GL_FLAT) &&
      (ctx->RasterMask == 0) &&							/* what means rastermask really? */
      (!ctx->Texture.Enabled))
    return natFastRectFunction;
#ifdef	TRYTEST
  if (preset.tryTest)
    return test_natChooseRectFunction(ctx);
#endif

#if 0
  DEBUGOUT(0, "Line.Width = %g\nLine.SmoothFlag = %d\nLine.StippleFlag = %d\nLight.ShadeModel = %x\nRasterMask = %x\nTexture.Enabled = %d\n",
	  ctx->Line.Width, ctx->Line.SmoothFlag, ctx->Line.StippleFlag, ctx->Light.ShadeModel, ctx->RasterMask, ctx->Texture.Enabled);
#endif

  return NULL;
}

rect_func natChooseRectFunctionDB(GLcontext * ctx)
{
  DEBUGOUT(1, "natChooseRectFunctionDB()\n");

#ifdef	TRYTEST
  if (preset.tryTest)
    return test_natChooseRectFunctionDB(ctx);
#endif

  return NULL;
}
