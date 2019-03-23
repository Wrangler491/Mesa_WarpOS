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
#include "cybFastLinesDBn.c"
#include "cybFastLinesDB.c"
#endif

/**********************************************************************/
/*****            Accelerated point, line, polygon rendering      *****/
/**********************************************************************/

/*
 * Examine the current rendering state and return a pointer to a 
 * fast line-rendering function if possible. 
 */

void cybFastLineFunction(GLcontext * ctx, GLuint v0, GLuint v1, GLuint pv)
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
  DEBUGOUT(8, " Move(%d, %d)\n", x, y);
  Move(rp, x, y);

  x = FIXx((int)(VB->Win.data[v1][0]));
  y = FIXy((int)(VB->Win.data[v1][1]));
  DEBUGOUT(8, " Draw(%d, %d)\n", x, y);
  Draw(rp, x, y);
}

line_func cybChooseLineFunction(GLcontext * ctx)
{
  DEBUGOUT(1, "cybChooseLineFunction()\n");

  if ((ctx->Line.Width == 1.0) &&
      (!ctx->Line.SmoothFlag) &&
      (!ctx->Line.StippleFlag) &&
      (ctx->Light.ShadeModel == GL_FLAT) &&
      (ctx->RasterMask == 0) &&							/* what means rastermask really? */
      (!ctx->Texture.Enabled))
    return cybFastLineFunction;
#ifdef	TRYTEST
  if (preset.tryTest)
    return test_cybChooseLineFunction(ctx);
#endif

  return NULL;
}

line_func cybChooseLineFunctionDB(GLcontext * ctx)
{
  DEBUGOUT(1, "cybChooseLineFunctionDB()\n");

#ifdef	TRYTEST
  if (preset.tryTest)
    return test_cybChooseLineFunctionDB(ctx);
#endif

  return NULL;
}
