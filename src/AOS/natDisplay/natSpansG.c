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
/*****        write spans of RGBA pixels                          *****/
/**********************************************************************/

void natGWriteRGBASpan(const GLcontext * ctx, GLuint n, GLint x, GLint y,
		       CONST GLubyte rgba[][4], const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLubyte *il;
  struct RastPort *rp;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  rp = amesa->rp;
  il = amesa->imageline;							/*  if imageline allocated then use fastversion  */

  y = FIXy(y);
  x = FIXx(x);
  DEBUGOUT(1, "natGWriteRGBASpan(%d, %d, %d): ", n, x, y);

  if (!mask) {
    GLshort s;

    DEBUGOUT(1, "nomask\n");
    for (s = n - 1; s >= 0; s--, RGBAi++)					/*  draw pixel (x[i],y[i]) */
      *il++ = PLG_RGBA(amesa, *RGBAb0, *RGBAb1, *RGBAb2);			/* clear rgb? */
    DEBUGOUT(8, "  MyWritePixelLine8(%d, %d, %d)\n", n, x, y);
	il = amesa->imageline;
    MyWritePixelLine8(rp, x, y, n, il, amesa->temprp);
  }
  else {
    GLshort i, ant = 0;

    DEBUGOUT(1, "mask\n");
    for (i = 0; i < n; i++) {							/*  draw pixel (x[i],y[i])  */
      if (*mask++) {
	ant++;
	*il++ = PLG_RGBA(amesa, *iRGBAb0, *iRGBAb1, *iRGBAb2);			/* clear rgb? */
      }
      else {
	if (ant) {
	  DEBUGOUT(9, "  MyWritePixelLine8(%d, %d, %d)\n", ant, x, y);
	  il = amesa->imageline;
	  MyWritePixelLine8(rp, x, y, ant, il, amesa->temprp);
	  x += ant;
	  ant = 0;
	}
	else
	  x++;
      }
    }

    if (ant) {
      DEBUGOUT(9, "  MyWritePixelLine8(%d, %d, %d)\n", ant, x, y);
      MyWritePixelLine8(rp, x, y, ant, amesa->imageline, amesa->temprp);
    }
  }
}

void natGWriteRGBSpan(const GLcontext * ctx, GLuint n, GLint x, GLint y,
		      CONST GLubyte rgb[][3], const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLubyte *il;
  struct RastPort *rp;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  rp = amesa->rp;
  il = amesa->imageline;							/*  if imageline allocated then use fastversion  */

  y = FIXy(y);
  x = FIXx(x);
  DEBUGOUT(1, "natGWriteRGBSpan(%d, %d, %d): ", n, x, y);

  if (!mask) {
    GLshort s;

    DEBUGOUT(1, "nomask\n");
    for (s = n - 1; s >= 0; s--, RGBb++,RGBb++,RGBb++)
      *il++ = PLG_RGBA(amesa, *RGBb0, *RGBb1, *RGBb2);				/* clear rgb? */
    DEBUGOUT(8, "  MyWritePixelLine8(%d, %d, %d)\n", n, x, y);
	il = amesa->imageline;
    MyWritePixelLine8(rp, x, y, n, il, amesa->temprp);
  }
  else {
    GLshort i, ant = 0;

    DEBUGOUT(1, "mask\n");
    for (i = 0; i < n; i++) {							/*  draw pixel (x[i],y[i])  */
      if (*mask++) {
	ant++;
	*il++ = PLG_RGBA(amesa, *iRGBb0, *iRGBb1, *iRGBb2);			/* clear rgb? */
      }
      else {
	if (ant) {
	  DEBUGOUT(9, "  MyWritePixelLine8(%d, %d, %d)\n", ant, x, y);
	  il = amesa->imageline;
	  MyWritePixelLine8(rp, x, y, ant, il, amesa->temprp);
	  x += ant;
	  ant = 0;
	}
	else
	  x++;
      }
    }

    if (ant) {
      DEBUGOUT(9, "  MyWritePixelLine8(%d, %d, %d)\n", ant, x, y);
      MyWritePixelLine8(rp, x, y, ant, amesa->imageline, amesa->temprp);
    }
  }
}

/**********************************************************************/
/*****        write spans of RGBA pixels doublebuffered           *****/
/**********************************************************************/

void natGWriteRGBASpanDB(const GLcontext * ctx, GLuint n, GLint x, GLint y,
			 CONST GLubyte rgba[][4], const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLubyte *db;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  db = dbPen(dbPenGet(amesa), x, y);

  DEBUGOUT(1, "natGWriteRGBASpanDB(%d, %d, %d)\n", n, FIXx(x), FIXy(y));

  /* Slower */
  if (!mask) {
    /*  draw pixel (x[i],y[i]) */
	(GLshort)n--;
    for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, RGBAi++)
      *db++ = PLG_RGBA(amesa, *RGBAb0, *RGBAb1, *RGBAb2);			/* clear rgb? */
  }
  else
    /* draw pixel (x[i],y[i]) using index[i] */
	(GLshort)n--;
    for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, RGBAi++, db++)
      if (*mask++)
	*db = PLG_RGBA(amesa, *RGBAb0, *RGBAb1, *RGBAb2);			/* clear rgb? */
}

void natGWriteRGBSpanDB(const GLcontext * ctx, GLuint n, GLint x, GLint y,
			CONST GLubyte rgb[][3], const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLubyte *db;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  db = dbPen(dbPenGet(amesa), x, y);

  DEBUGOUT(1, "natGWriteRGBSpanDB(%d, %d, %d)\n", n, FIXx(x), FIXy(y));

  /* Slower */
  if (!mask) {
    /*  draw pixel (x[i],y[i]) */
	(GLshort)n--;
    for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, RGBb++,RGBb++,RGBb++)
      *db++ = PLG_RGBA(amesa, *RGBb0, *RGBb1, *RGBb2);				/* clear rgb? */
  }
  else
    /* draw pixel (x[i],y[i]) using index[i] */
	(GLshort)n--;
    for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, RGBb++,RGBb++,RGBb++, db++)
      if (*mask++)
	*db = PLG_RGBA(amesa, *RGBb0, *RGBb1, *RGBb2);				/* clear rgb? */
}
