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

#include "natPixel8.c"
#include "natPixelD.c"
#include "natPixelG.c"

/**********************************************************************/
/*****        write arrays of RGBA pixels                         *****/
/**********************************************************************/

void natWriteCI32Pixel(const GLcontext * ctx, GLuint n, const GLint x[], const GLint y[],
			const GLuint index[], const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLuint *ItoP;
  struct RastPort *rp;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  ItoP = amesa->ItoP;
  rp = amesa->rp;

  DEBUGOUT(1, "natWriteCI32Pixel(%d)\n", n);

	(GLshort)n--;
  for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, INDEXi++, Xx++, Yy++) {
    if (*mask++) {
      SetAPen(rp, GetRGBPLocal(*INDEXi));
      DEBUGOUT(9, "  WritePixel(%d, %d)\n", FIXx(*Xx), FIXy(*Yy));
      WritePixel(rp, FIXx(*Xx), FIXy(*Yy));
    }
  }
}

void natWriteMonoCIPixel(const GLcontext * ctx, GLuint n,
			 const GLint x[], const GLint y[],
			 const GLubyte mask[])
{
  amigaMesaContext amesa;
  struct RastPort *rp;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  rp = amesa->rp;

  DEBUGOUT(1, "natWriteMonoCIPixel(0x%08x, %d)\n", mask, n);

  SetAPen(rp, amesa->pixel);
	(GLshort)n--;
  for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, Xx++, Yy++) {
    DEBUGOUT(9, " mask 0x%08x/%d, x %d, y %d\n", mask, *mask, FIXx(*Xx), FIXy(*Yy));
    if (*mask++) {
      /* write pixel x[i], y[i] using current index */
      DEBUGOUT(9, "  WritePixel(%d, %d)\n", FIXx(*Xx), FIXy(*Yy));
      WritePixel(rp, FIXx(*Xx), FIXy(*Yy));
    }
  }
}

/**********************************************************************/
/*****        write arrays of RGBA pixels doublebuffered          *****/
/**********************************************************************/

void natWriteCI32PixelDB(const GLcontext * ctx, GLuint n, const GLint x[], const GLint y[],
			  const GLuint index[], const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLuint *ItoP;
  GLubyte *db;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  ItoP = amesa->ItoP;
  db = dbPenGet(amesa);

  DEBUGOUT(1, "natWriteCI32PixelDB(%d)\n", n);

	(GLshort)n--;
  for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, INDEXi++, Xx++, Yy++)
    if (*mask++)
      *(dbPen(db, *Xx, *Yy)) = GetRGBPLocal(*INDEXi);
}

void natWriteMonoCIPixelDB(const GLcontext * ctx, GLuint n,
			    const GLint x[], const GLint y[],
			    const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLubyte *db;
  GLubyte col;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  db = dbPenGet(amesa);

  DEBUGOUT(1, "natWriteMonoCIPixelDB(%d)\n", n);

  col = amesa->pixel;
	(GLshort)n--;
  for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, Xx++, Yy++)
    if (*mask++)
      *(dbPen(db, *Xx, *Yy)) = col;
}

/**********************************************************************/
/*****                       Read arrays of pixels                          *****/
/**********************************************************************/
/* Read an array of color index pixels. */

void natReadRGBAPixel(const GLcontext * ctx, GLuint n, const GLint x[], const GLint y[],
		       GLubyte rgba[][4], const GLubyte mask[])
{
  amigaMesaContext amesa;
  struct RastPort *rp;
  GLuint *PtoI;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  rp = amesa->rp;
  PtoI = amesa->PtoI;

  DEBUGOUT(1, "natReadRGBAPixel(%d)\n", n);

	(GLshort)n--;
  for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, RGBAi++, Xx++, Yy++) {
    if (*mask++) {
      DEBUGOUT(9, "  ReadPixel(%d, %d)\n", FIXx(*Xx), FIXy(*Yy));
      *RGBAi = GetIndexLocal(ReadPixel(rp, FIXx(*Xx), FIXy(*Yy))) & 0xFFFFFF00;	/* clear alpha? */
    }
  }
}

void natReadCI32Pixel(const GLcontext * ctx, GLuint n, const GLint x[], const GLint y[],
		       GLuint index[], const GLubyte mask[])
{
  amigaMesaContext amesa;
  struct RastPort *rp;
  GLuint *PtoI;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  rp = amesa->rp;
  PtoI = amesa->PtoI;

  DEBUGOUT(1, "natReadCI32Pixel(%d)\n", n);

	(GLshort)n--;
  for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, INDEXi++, Xx++, Yy++) {
    if (*mask++) {
      /* index[i] = read_pixel x[i], y[i] */
      DEBUGOUT(9, "  ReadPixel(%d, %d)\n", FIXx(*Xx), FIXy(*Yy));
      *INDEXi = GetIndexLocal(ReadPixel(rp, FIXx(*Xx), FIXy(*Yy))) & 0xFF;	/* clear rgb! */
    }
  }
}

/**********************************************************************/
/***** Read arrays of pixels doublebuffered                       *****/
/**********************************************************************/
/* Read an array of color index pixels. */

void natReadRGBAPixelDB(const GLcontext * ctx, GLuint n, const GLint x[], const GLint y[],
			 GLubyte rgba[][4], const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLubyte *db;
  GLuint *PtoI;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  db = dbPenGet(amesa);
  PtoI = amesa->PtoI;

  DEBUGOUT(1, "natReadRGBAPixelDB(%d)\n", n);

	(GLshort)n--;
  for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, RGBAi++, Xx++, Yy++)
    if (*mask++)
      *RGBAi = GetIndexLocal(*(dbPen(db, *Xx, *Yy))) & 0xFFFFFF00;		/* clear alpha? */
}

void natReadCI32PixelDB(const GLcontext * ctx, GLuint n, const GLint x[], const GLint y[],
			 GLuint index[], const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLubyte *db;
  GLuint *PtoI;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  db = dbPenGet(amesa);
  PtoI = amesa->PtoI;

  DEBUGOUT(1, "natReadCI32PixelDB(%d)\n", n);

  /* index[i] = read_pixel x[i], y[i] */
	(GLshort)n--;
  for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, INDEXi++, Xx++, Yy++)
    if (*mask++)
      *INDEXi = GetIndexLocal(*(dbPen(db, *Xx, *Yy))) & 0xFF;			/* clear rgb! */
}
