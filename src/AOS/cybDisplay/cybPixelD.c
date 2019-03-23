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
/*****        read arrays of RGBA pixels                          *****/
/**********************************************************************/

void cybDReadCI32Pixel(const GLcontext * ctx, GLuint n, const GLint x[], const GLint y[],
			GLuint index[], const GLubyte mask[])
{
  amigaMesaContext amesa;
  struct RastPort *rp;
  GLuint *PtoI;
  GLuint col;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  rp = amesa->rp;
  PtoI = amesa->PtoI;

  DEBUGOUT(1, "cybReadCI32Pixel(%d)\n", n);

	(GLshort)n--;
  for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, INDEXi++, Xx++, Yy++) {
    if (*mask++) {
      /* index[i] = read_pixel x[i], y[i] */
      col = ReadRGBPixel(rp, FIXx(*Xx), FIXy(*Yy));
      DEBUGOUT(9, "  ReadRGBPixel(%d, %d)\n", FIXx(*Xx), FIXy(*Yy));

      /* this means:
       *  -make a quick match and get a pen (color-quantization!)
       *  -convert the pen to index
       */
      *INDEXi = GetIndexLocal(PLD_RGBA(amesa, (GLubyte) (col >> 24),
					      (GLubyte) (col >> 16),
					      (GLubyte) (col >>  8), FIXx(*Xx), FIXy(*Yy))) & 0xFF;
    }
  }
}

/**********************************************************************/
/*****        read arrays of RGBA pixels doublebuffered           *****/
/**********************************************************************/

void cybDReadCI32PixelDB(const GLcontext * ctx, GLuint n, const GLint x[], const GLint y[],
			  GLuint index[], const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLuint *db;
  GLuint *PtoI;
  GLuint col;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  db = dbRGBAGet(amesa);
  PtoI = amesa->PtoI;

  DEBUGOUT(1, "cybReadCI32PixelDB(%d)\n", n);

	(GLshort)n--;
  for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, INDEXi++, Xx++, Yy++) {
    if (*mask++) {
      /* index[i] = read_pixel x[i], y[i] */
      col = *(dbRGBA(db, *Xx, *Yy));

      /* this means:
       *  -make a quick match and get a pen (color-quantization!)
       *  -convert the pen to index
       */
      *INDEXi = GetIndexLocal(PLD_RGBA(amesa, (GLubyte) (col >> 24),
					      (GLubyte) (col >> 16),
					      (GLubyte) (col >>  8), FIXx(*Xx), FIXy(*Yy))) & 0xFF;
    }
  }
}
