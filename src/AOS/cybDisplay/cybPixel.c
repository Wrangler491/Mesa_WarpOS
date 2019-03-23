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

#include "cybPixel8.c"
#include "cybPixelD.c"
#include "cybPixelG.c"

/**********************************************************************/
/*****        write arrays of RGBA pixels                         *****/
/**********************************************************************/

void cybWriteRGBAPixel(const GLcontext * ctx, GLuint n, const GLint x[], const GLint y[],
			CONST GLubyte rgba[][4], const GLubyte mask[])
{
  amigaMesaContext amesa;
  struct RastPort *rp;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  rp = amesa->rp;

  DEBUGOUT(1, "cybWriteRGBAPixel(%d)\n", n);

  RGBAb--;									/* hacky RGBA to RGB */
	(GLshort)n--;
  for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, RGBAi++, Xx++, Yy++) {
    if (*mask++) {
      /* write pixel x[i], y[i] using red[i],green[i],blue[i],alpha[i] */
      WriteRGBPixel(rp, FIXx(*Xx), FIXy(*Yy), *RGBAi);
      DEBUGOUT(9, "  WriteRGBPixel(%d, %d)\n", FIXx(*Xx), FIXy(*Yy));
    }
  }
}

void cybWriteCI32Pixel(const GLcontext * ctx, GLuint n, const GLint x[], const GLint y[],
			const GLuint index[], const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLuint *ItoP;
  struct RastPort *rp;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  rp = amesa->rp;
  ItoP = amesa->ItoP;

  DEBUGOUT(1, "cybWriteCI32Pixel(%d)\n", n);

  I2Pb--;									/* hacky RGBA to RGB */
	(GLshort)n--;
  for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, INDEXi++, Xx++, Yy++) {
    if (*mask++) {
      WriteRGBPixel(rp, FIXx(*Xx), FIXy(*Yy), GetRGBPLocal(*INDEXi));
      DEBUGOUT(9, "  WriteRGBPixel(%d, %d)\n", FIXx(*Xx), FIXy(*Yy));
    }
  }
}

void cybWriteMonoCIPixel(const GLcontext * ctx, GLuint n,
			  const GLint x[], const GLint y[],
			  const GLubyte mask[])
{
  amigaMesaContext amesa;
  struct RastPort *rp;
  GLuint col;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  rp = amesa->rp;

  DEBUGOUT(1, "cybWriteMonoCIPixel(%d)\n", n);

  col = amesa->pixel >> 8;
	(GLshort)n--;
  for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, Xx++, Yy++) {
    if (*mask++) {
      /* write pixel x[i], y[i] using current index */
      WriteRGBPixel(rp, FIXx(*Xx), FIXy(*Yy), col);
      DEBUGOUT(9, "  WriteRGBPixel(%d, %d)\n", FIXx(*Xx), FIXy(*Yy));
    }
  }
}

/**********************************************************************/
/*****        write arrays of RGBA pixels doublebuffered          *****/
/**********************************************************************/

void cybWriteRGBAPixelDB(const GLcontext * ctx, GLuint n, const GLint x[], const GLint y[],
			  CONST GLubyte rgba[][4], const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLuint *db;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  db = dbRGBAGet(amesa);

  DEBUGOUT(1, "cybWriteRGBAPixelDB(%d)\n", n);

	(GLshort)n--;
  for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, RGBAi++, Xx++, Yy++)
    if (*mask++)
      *(dbRGBA(db, *Xx, *Yy)) = *RGBAi;
}

void cybWriteCI32PixelDB(const GLcontext * ctx, GLuint n, const GLint x[], const GLint y[],
			  const GLuint index[], const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLuint *db;
  GLuint *ItoP;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  ItoP = amesa->ItoP;
  db = dbRGBAGet(amesa);

  DEBUGOUT(1, "cybWriteCI32PixelDB(%d)\n", n);

	(GLshort)n--;
  for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, INDEXi++, Xx++, Yy++)
    if (*mask++)
      *(dbRGBA(db, *Xx, *Yy)) = GetRGBPLocal(*INDEXi);
}

void cybWriteMonoCIPixelDB(const GLcontext * ctx, GLuint n,
			    const GLint x[], const GLint y[],
			    const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLuint *db;
  GLuint col;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  db = dbRGBAGet(amesa);

  DEBUGOUT(1, "cybWriteMonoCIPixelDB(%d)\n", n);

  col = amesa->pixel;
	(GLshort)n--;
  for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, Xx++, Yy++)
    if (*mask++)
      *(dbRGBA(db, *Xx, *Yy)) = col;
}

/**********************************************************************/
/*****        read arrays of RGBA pixels                          *****/
/**********************************************************************/
/* Read an array of color index pixels. */

void cybReadRGBAPixel(const GLcontext * ctx, GLuint n, const GLint x[], const GLint y[],
		       GLubyte rgba[][4], const GLubyte mask[])
{
  amigaMesaContext amesa;
  struct RastPort *rp;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  rp = amesa->rp;

  DEBUGOUT(1, "cybReadRGBAPixel(%d)\n", n);

//RGBAb--;                                      /* hacky RGB to RGBA (write not allowed) */
	(GLshort)n--;
  for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, RGBAi++, Xx++, Yy++) {
    if (*mask++) {
      *RGBAi = ReadRGBPixel(rp, FIXx(*Xx), FIXy(*Yy)) << 8;
      DEBUGOUT(9, "  ReadRGBPixel(%d, %d)\n", FIXx(*Xx), FIXy(*Yy));
    }
  }
}

/**********************************************************************/
/*****        read arrays of RGBA pixels doublebuffered           *****/
/**********************************************************************/

void cybReadRGBAPixelDB(const GLcontext * ctx, GLuint n, const GLint x[], const GLint y[],
			 GLubyte rgba[][4], const GLubyte mask[])
{
  amigaMesaContext amesa = (amigaMesaContext) ctx->DriverCtx;
  GLuint *db;

  db = dbRGBAGet(amesa);

  DEBUGOUT(1, "cybReadRGBAPixelDB(%d)\n", n);

	(GLshort)n--;
  for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, RGBAi++, Xx++, Yy++)
    if (*mask++)
      *RGBAi = *(dbRGBA(db, *Xx, *Yy));
}
