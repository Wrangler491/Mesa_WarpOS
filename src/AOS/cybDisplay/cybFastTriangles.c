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
#include "cybFastTrianglesDBn.c"
#include "cybFastTrianglesDB.c"
#endif

/**********************************************************************/
/*****            Accelerated point, line, polygon rendering      *****/
/**********************************************************************/

/*
 * Draw a filled polygon of a single color. If there is hardware/OS support
 * for polygon drawing use that here.   Otherwise, call a function in
 * polygon.c to do the drawing.
 */

void cybFastTriangleFunction(GLcontext * ctx, GLuint v0, GLuint v1, GLuint v2, GLuint pv)
{
  amigaMesaContext amesa;
  struct RastPort *rp;
  struct vertex_buffer *VB;
  int x, y;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  VB = ctx->VB;
  rp = amesa->rp;

  DEBUGOUT(1, "cybFastLineFunction\n");

  /* TODO: this is not the optimal case,
   * we should allocate the color here to
   * get the _exact_ color out of the allocated
   * pen
   */
  if (!(amesa->visual->flags & VISUAL_RGBMODE))
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

triangle_func cybChooseTriangleFunction(GLcontext * ctx)
{
  /*
   * Examine the current rendering state and return a pointer to a 
   * fast polygon-rendering function if possible. 
   */
  DEBUGOUT(1, "cybChooseTriangleFunction\n");

  if ((ctx->Line.Width == 1.0) &&
      (!ctx->Line.SmoothFlag) &&
      (!ctx->Line.StippleFlag) &&
      (ctx->Light.ShadeModel == GL_FLAT) &&
      (ctx->RasterMask == 0) &&							/* what means rastermask really? */
      (!ctx->Texture.Enabled))
    return cybFastTriangleFunction;
#ifdef	TRYTEST
  if (preset.tryTest)
    return test_cybChooseTriangleFunction(ctx);
#endif

  return NULL;
}

triangle_func cybChooseTriangleFunctionDB(GLcontext * ctx)
{
  DEBUGOUT(1, "cybChooseTriangleFunctionDB()\n");

#ifdef	TRYTEST
  if (preset.tryTest)
    return test_cybChooseTriangleFunctionDB(ctx);
#endif

  return NULL;
}

void cybFastQuadFunction(GLcontext * ctx, GLuint v0, GLuint v1, GLuint v2, GLuint v3, GLuint pv)
{
  amigaMesaContext amesa;
  struct RastPort *rp;
  struct vertex_buffer *VB;
  int x, y;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  VB = ctx->VB;
  rp = amesa->rp;

  DEBUGOUT(1, "cybFastLineFunction\n");

  /* TODO: this is not the optimal case,
   * we should allocate the color here to
   * get the _exact_ color out of the allocated
   * pen
   */
  if (!(amesa->visual->flags & VISUAL_RGBMODE))
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

quad_func cybChooseQuadFunction(GLcontext * ctx)
{
  /*
   * Examine the current rendering state and return a pointer to a 
   * fast polygon-rendering function if possible. 
   */
  DEBUGOUT(1, "cybChooseQuadFunction\n");

  if ((ctx->Line.Width == 1.0) &&
      (!ctx->Line.SmoothFlag) &&
      (!ctx->Line.StippleFlag) &&
      (ctx->Light.ShadeModel == GL_FLAT) &&
      (ctx->RasterMask == 0) &&							/* what means rastermask really? */
      (!ctx->Texture.Enabled))
    return cybFastQuadFunction;
#ifdef	TRYTEST
  if (preset.tryTest)
    return test_cybChooseQuadFunction(ctx);
#endif

  return NULL;
}

quad_func cybChooseQuadFunctionDB(GLcontext * ctx)
{
  DEBUGOUT(1, "cybChooseQuadFunctionDB()\n");

#ifdef	TRYTEST
  if (preset.tryTest)
    return test_cybChooseQuadFunctionDB(ctx);
#endif

  return NULL;
}

void cybFastRectFunction(GLcontext * ctx, GLint x, GLint y, GLint width, GLint height)
{
  amigaMesaContext amesa;
  struct RastPort *rp;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  rp = amesa->rp;

  DEBUGOUT(1, "cybFastLineFunction()\n");

  FillPixelArray(amesa->rp, FIXx(x),
			    FIXy(y) - height,
			    width,
			    height,
			    amesa->pixel >> 8);
}

rect_func cybChooseRectFunction(GLcontext * ctx)
{
  /*
   * Examine the current rendering state and return a pointer to a 
   * fast polygon-rendering function if possible. 
   */
  DEBUGOUT(1, "cybChooseRectFunction\n");

  if ((ctx->Line.Width == 1.0) &&
      (!ctx->Line.SmoothFlag) &&
      (!ctx->Line.StippleFlag) &&
      (ctx->Light.ShadeModel == GL_FLAT) &&
      (ctx->RasterMask == 0) &&							/* what means rastermask really? */
      (!ctx->Texture.Enabled))
    return cybFastRectFunction;
#ifdef	TRYTEST
  if (preset.tryTest)
    return test_cybChooseRectFunction(ctx);
#endif

  return NULL;
}

rect_func cybChooseRectFunctionDB(GLcontext * ctx)
{
  DEBUGOUT(1, "cybChooseRectFunctionDB()\n");

#ifdef	TRYTEST
  if (preset.tryTest)
    return test_cybChooseRectFunctionDB(ctx);
#endif

  return NULL;
}
