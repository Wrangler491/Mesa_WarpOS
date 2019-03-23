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
/*****        write arrays of RGBA pixels                         *****/
/**********************************************************************/

void natGWriteRGBAPixel(const GLcontext * ctx, GLuint n, const GLint x[], const GLint y[],
			CONST GLubyte rgba[][4], const GLubyte mask[])
{
  amigaMesaContext amesa;
  struct RastPort *rp;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  rp = amesa->rp;

  DEBUGOUT(1, "natGWriteRGBAPixel(%d)\n", n);

	(GLshort)n--;
  for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, RGBAi++, Xx++, Yy++) {
    if (*mask++) {
      /* write pixel x[i], y[i] using rgba[i][0],rgba[i][1],rgba[i][2],rgba[i][3] */
      SetAPen(rp, PLG_RGBA(amesa, *RGBAb0, *RGBAb1, *RGBAb2));
      DEBUGOUT(9, "  WritePixel(%d, %d)\n", FIXx(*Xx), FIXy(*Yy));
      WritePixel(rp, FIXx(*Xx), FIXy(*Yy));
    }
  }
}

/**********************************************************************/
/*****        write arrays of RGBA pixels doublebuffered          *****/
/**********************************************************************/

void natGWriteRGBAPixelDB(const GLcontext * ctx, GLuint n, const GLint x[], const GLint y[],
			  CONST GLubyte rgba[][4], const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLubyte *db;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  db = dbPenGet(amesa);

  DEBUGOUT(1, "natGWriteRGBAPixelDB(%d)\n", n);

	(GLshort)n--;
  for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, RGBAi++, Xx++, Yy++)
    if (*mask++)
      *(dbPen(db, *Xx, *Yy)) = PLG_RGBA(amesa, *RGBAb0, *RGBAb1, *RGBAb2);
}
