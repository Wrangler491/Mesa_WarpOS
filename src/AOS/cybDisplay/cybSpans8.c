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
/*****        read spans of RGBA pixels                           *****/
/**********************************************************************/

void cyb8ReadCI32Span(const GLcontext * ctx, GLuint n, GLint x, GLint y, GLuint index[])
{
  amigaMesaContext amesa;
  GLuint *il;
  GLuint *PtoI;
  struct RastPort *rp;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  PtoI = amesa->PtoI;
  rp = amesa->rp;
  il = (GLuint *) amesa->imageline;

  y = FIXy(y);
  x = FIXx(x);
  DEBUGOUT(1, "cybReadCI32Span(%d, %d, %d)\n", n, x, y);

  DEBUGOUT(8, "  ReadPixelArray(%d, %d, %d)\n", n, x, y);
  MyReadPixelArray(il, 0, 0, 4 * n, rp, x, y, n, 1, RECTFMT_RGBA);
	(GLshort)n--;
  for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--) {
    ULONG col = *il++;

    /* this means:
     *  -make a quick match and get a pen (color-quantization!)
     *  -convert the pen to index
     */
    *INDEXi++ = GetIndexLocal(PL8_RGBA(amesa, (GLubyte) (col >> 24),
					      (GLubyte) (col >> 16),
					      (GLubyte) (col >>  8))) & 0xFF;
  }
}

/**********************************************************************/
/*****        read spans of RGBA pixels doublebuffered            *****/
/**********************************************************************/

void cyb8ReadCI32SpanDB(const GLcontext * ctx, GLuint n, GLint x, GLint y, GLuint index[])
{
  amigaMesaContext amesa;
  GLuint *PtoI;
  GLuint *db;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  PtoI = amesa->PtoI;
  db = dbRGBA(dbRGBAGet(amesa), x, y);

  DEBUGOUT(1, "cybReadCI32SpanDB(%d, %d, %d)\n", n, FIXx(x), FIXy(y));

	(GLshort)n--;
  for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--) {
    ULONG col = *db++;

    /* this means:
     *  -make a quick match and get a pen (color-quantization!)
     *  -convert the pen to index
     */
    *INDEXi++ = GetIndexLocal(PL8_RGBA(amesa, (GLubyte) (col >> 24),
					      (GLubyte) (col >> 16),
					      (GLubyte) (col >>  8))) & 0xFF;
  }
}
