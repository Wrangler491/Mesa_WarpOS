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
/*****            Accelerated point, line, polygon rendering      *****/
/**********************************************************************/

void cybFastPointsFunction(GLcontext * ctx, GLuint first, GLuint last)
{
  amigaMesaContext amesa;
  struct vertex_buffer *VB;
  struct RastPort *rp;
  GLubyte *mask;
  GLuint i;

  amesa = ((amigaMesaContext) ctx->DriverCtx);
  VB = ctx->VB;
  rp = amesa->rp;

  DEBUGOUT(1, "cybFastPointsFunction: ");

  mask = VB->ClipMask;
  if (!(amesa->visual->flags & VISUAL_RGBMODE)) {
    GLuint *ItoP = amesa->ItoP;

    DEBUGOUT(1, "VB.IndexColor\n");
    for (i = first; i <= last; i++) {
      if (!*mask++) {
	int x, y;

	x = FIXx((GLint) (VB->Win.data[i][0]));
	y = FIXy((GLint) (VB->Win.data[i][1]));

	WriteRGBPixel(rp, x, y, GetRGBPLocal(VB->IndexPtr->data[i]) >> 8);
	DEBUGOUT(9, " WriteRGBPixel(%d, %d)\n", x, y);
      }
    }
  }
  else {
    DEBUGOUT(1, "VB.RGBColor\n");

    for (i = first; i <= last; i++) {
      if (!*mask++) {
	int x, y;

	x = FIXx((GLint) (VB->Win.data[i][0]));
	y = FIXy((GLint) (VB->Win.data[i][1]));

	WriteRGBPixel(rp, x, y, (*((GLuint *) VB->ColorPtr->data[i])) >> 8);
	DEBUGOUT(9, " WriteRGBPixel(%d, %d)\n", x, y);
      }
    }
  }
}

points_func cybChoosePointsFunction(GLcontext * ctx)
{
  /*
   * Examine the current rendering state and return a pointer to a 
   * fast point-rendering function if possible. 
   */

  DEBUGOUT(1, "cybChoosePointsFunction\n");

  if ((ctx->Point.Size == 1.0) &&
      (!ctx->Point.SmoothFlag) &&
      (ctx->RasterMask == 0) &&							/* what means rastermask really? */
      (!ctx->Texture.Enabled))
    return cybFastPointsFunction;

  return NULL;
}

points_func cybChoosePointsFunctionDB(GLcontext * ctx)
{
  DEBUGOUT(1, "cybChoosePointsFunctionDB\n");

  return NULL;
}
