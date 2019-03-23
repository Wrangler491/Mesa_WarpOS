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

void natDWriteRGBASpan(const GLcontext * ctx, GLuint n, GLint x, GLint y,
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
  DEBUGOUT(1, "natDWriteRGBASpan(%d, %d, %d): ", n, x, y);

  if (!mask) {
    GLshort s, xx;

    DEBUGOUT(1, "nomask\n");
    for (s = n - 1, xx = x; s >= 0; s--, xx++, RGBAi++)				/*  draw pixel (x[i],y[i]) */
      *il++ = PLD_RGBA(amesa, *RGBAb0, *RGBAb1, *RGBAb2, xx, y);		/* clear rgb? */
    DEBUGOUT(8, "  MyWritePixelLine8(%d, %d, %d)\n", n, x, y);
    MyWritePixelLine8(rp, x, y, n, il = amesa->imageline, amesa->temprp);
  }
  else {
    GLshort i, ant = 0, xx;

    DEBUGOUT(1, "mask\n");
    for (i = 0, xx = x; i < n; i++, xx++) {					/*  draw pixel (x[i],y[i])  */
      if (*mask++) {
	ant++;
	*il++ = PLD_RGBA(amesa, *iRGBAb0, *iRGBAb1, *iRGBAb2, xx, y);		/* clear rgb? */
      }
      else {
	if (ant) {
	  DEBUGOUT(9, "  MyWritePixelLine8(%d, %d, %d)\n", ant, x, y);
	  MyWritePixelLine8(rp, x, y, ant, il = amesa->imageline, amesa->temprp);
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

void natDWriteRGBSpan(const GLcontext * ctx, GLuint n, GLint x, GLint y,
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
  DEBUGOUT(1, "natDWriteRGBSpan(%d, %d, %d): ", n, x, y);

  if (!mask) {
    GLshort s, xx;

    DEBUGOUT(1, "nomask\n");
    for (s = n - 1, xx = x; s >= 0; s--, xx++, RGBb++,RGBb++,RGBb++)	//was +=3
      *il++ = PLD_RGBA(amesa, *RGBb0, *RGBb1, *RGBb2, xx, y);			/* clear rgb? */
    DEBUGOUT(8, "  MyWritePixelLine8(%d, %d, %d)\n", n, x, y);
    MyWritePixelLine8(rp, x, y, n, il = amesa->imageline, amesa->temprp);
  }
  else {
    GLshort i, ant = 0, xx;

    DEBUGOUT(1, "mask\n");
    for (i = 0, xx = x; i < n; i++, xx++) {					/*  draw pixel (x[i],y[i])  */
      if (*mask++) {
	ant++;
	*il++ = PLD_RGBA(amesa, *iRGBb0, *iRGBb1, *iRGBb2, xx, y);		/* clear rgb? */
      }
      else {
	if (ant) {
	  DEBUGOUT(9, "  MyWritePixelLine8(%d, %d, %d)\n", ant, x, y);
	  MyWritePixelLine8(rp, x, y, ant, il = amesa->imageline, amesa->temprp);
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

void natDWriteMonoCISpan(const GLcontext * ctx, GLuint n, GLint x, GLint y, const GLubyte mask[])
{
  amigaMesaContext amesa;
  struct RastPort *rp;
  GLshort s;
  GLubyte r, g, b;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  rp = amesa->rp;

  y = FIXy(y);
  x = FIXx(x);
  DEBUGOUT(1, "natWriteMonoCISpan(%d, %d, %d): \n", n, x, y);

  s = 0;
  r = amesa->pixel >> 24;
  g = amesa->pixel >> 16;
  b = amesa->pixel >>  8;

  while (s < n) {
    while (!*mask++ && s < n) {
      s++;
      x++;
    } mask--;

    if (s < n) {
      while (*mask++ && s < n) {
	SetAPen(rp, PLD_RGBA(amesa, r, g, b, x, y));
	WritePixel(rp, x, y);
	s++;
	x++;
      } mask--;
      SetAPen(rp, PLD_RGBA(amesa, r, g, b, x, y));
      WritePixel(rp, x, y);
    }
  }
}

/**********************************************************************/
/*****        write spans of RGBA pixels doublebuffered           *****/
/**********************************************************************/

void natDWriteRGBASpanDB(const GLcontext * ctx, GLuint n, GLint x, GLint y,
			 CONST GLubyte rgba[][4], const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLubyte *db;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  db = dbPen(dbPenGet(amesa), x, y);

  DEBUGOUT(1, "natDWriteRGBASpanDB(%d, %d, %d)\n", n, FIXx(x), FIXy(y));

  /* Slower */
  if (!mask) {
    /*  draw pixel (x[i],y[i]) */
	(GLshort)n--;
    for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, x++, RGBAi++)
      *db++ = PLD_RGBA(amesa, *RGBAb0, *RGBAb1, *RGBAb2, x, y);			/* clear rgb? */
  }
  else
    /* draw pixel (x[i],y[i]) using index[i] */
	(GLshort)n--;
    for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, x++, RGBAi++, db++)
      if (*mask++)
	*db = PLD_RGBA(amesa, *RGBAb0, *RGBAb1, *RGBAb2, x, y);			/* clear rgb? */
}

void natDWriteRGBSpanDB(const GLcontext * ctx, GLuint n, GLint x, GLint y,
			CONST GLubyte rgb[][3], const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLubyte *db;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  db = dbPen(dbPenGet(amesa), x, y);

  DEBUGOUT(1, "natDWriteRGBSpanDB(%d, %d, %d)\n", n, FIXx(x), FIXy(y));

  /* Slower */
  if (!mask) {
    /*  draw pixel (x[i],y[i]) */
	(GLshort)n--;
    for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, x++, RGBb++,RGBb++,RGBb++)
      *db++ = PLD_RGBA(amesa, *RGBb0, *RGBb1, *RGBb2, x, y);				/* clear rgb? */
  }
  else
    /* draw pixel (x[i],y[i]) using index[i] */
	(GLshort)n--;
    for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, x++, RGBb++,RGBb++,RGBb++, db++)
      if (*mask++)
	*db = PLD_RGBA(amesa, *RGBb0, *RGBb1, *RGBb2, x, y);				/* clear rgb? */
}

void natDWriteMonoCISpanDB(const GLcontext * ctx, GLuint n, GLint x, GLint y,
			  const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLubyte *db;
  GLubyte r, g, b;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  db = dbPen(dbPenGet(amesa), x, y);

  DEBUGOUT(1, "natWriteMonoCISpanDB(%d, %d, %d)\n", n, FIXx(x), FIXy(y));

  r = amesa->pixel >> 24;
  g = amesa->pixel >> 16;
  b = amesa->pixel >>  8;

  if (!mask) {
    /* draw pixel (x[i],y[i]) using index[i] */
	(GLshort)n--;
    for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, x++)						/* Slower */
      *db++ = PLD_RGBA(amesa, r, g, b, x, y);
  }
  else
    /* draw pixel (x[i],y[i]) using index[i] */
	(GLshort)n--;
    for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, x++, db++)
      if (*mask++)
	*db = PLD_RGBA(amesa, r, g, b, x, y);
}
