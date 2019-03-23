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
#include "natFastLinesDBn.c"
#include "natFastLinesDB.c"
#endif

/**********************************************************************/
/*****            Accelerated point, line, polygon rendering      *****/
/**********************************************************************/

void natFastLineFunction(GLcontext * ctx, GLuint v0, GLuint v1, GLuint pv)
{
  amigaMesaContext amesa;
  struct RastPort *rp;
  struct vertex_buffer *VB;
  int x, y;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  VB = ctx->VB;
  rp = amesa->rp;

  DEBUGOUT(1, "natFastLineFunction\n");

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

line_func natChooseLineFunction(GLcontext * ctx)
{
  /*
   * Examine the current rendering state and return a pointer to a 
   * fast line-rendering function if possible. 
   */

  DEBUGOUT(1, "natChooseLineFunction()\n");

  if (!IS_DITHER((amigaMesaContext) ctx->DriverCtx) &&
      (ctx->Line.Width == 1.0) &&
      (!ctx->Line.SmoothFlag) &&
      (!ctx->Line.StippleFlag) &&
      (ctx->Light.ShadeModel == GL_FLAT) &&
      (ctx->RasterMask == 0) &&							/* what means rastermask really? */
      (!ctx->Texture.Enabled))
    return natFastLineFunction;
#ifdef	TRYTEST
  else if (preset.tryTest)
    return test_natChooseLineFunction(ctx);
#endif

  return NULL;
}

line_func natChooseLineFunctionDB(GLcontext * ctx)
{
  DEBUGOUT(1, "natChooseLineFunctionDB()\n");

#ifdef	TRYTEST
  if (preset.tryTest)
    return test_natChooseLineFunctionDB(ctx);
#endif

  return NULL;
}
