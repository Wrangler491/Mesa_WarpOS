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

#include "cybSpans8.c"
#include "cybSpansD.c"
#include "cybSpansG.c"

/**********************************************************************/
/*****        write spans of RGBA pixels                          *****/
/**********************************************************************/

/* Write RGBA pixels to an RGBA (or permuted) buffer. */
void cybWriteRGBASpan(const GLcontext * ctx, GLuint n, GLint x, GLint y,
		      CONST GLubyte rgba[][4], const GLubyte mask[])
{
  amigaMesaContext amesa;
  struct RastPort *rp;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  rp = amesa->rp;

  y = FIXy(y);
  x = FIXx(x);
  DEBUGOUT(1, "cybWriteRGBASpan(%d, %d, %d): ", n, x, y);

  if (!mask) {
    DEBUGOUT(1, "nomask\n");
    DEBUGOUT(2, " WritePixelArray(%d, %d, %d)\n", n, x, y);
    MyWritePixelArray(rgba, 0, 0, 4 * n, rp, x, y, n, 1, RECTFMT_RGBA);
  }
  else {
    GLshort i, ant = 0;
    GLuint *il;

    il = (GLuint *) amesa->imageline;						/* if imageline allocated then use fastversion */

    DEBUGOUT(1, "mask FAST\n");
    for (i = 0; i < n; i++) {							/* draw pixel (x[i],y[i]) */
      if (*mask++) {
	ant++;
	*il++ = *iRGBAi;
      }
      else {
	if (ant) {
	  DEBUGOUT(9, "  WritePixelArray(%d, %d, %d)\n", ant, x, y);
	il = (GLuint *)amesa->imageline;
	  MyWritePixelArray(il, 0, 0, 4 * ant, rp, x, y, ant, 1, RECTFMT_RGBA);
	  x += ant;
	  ant = 0;
	}
	else
	  x++;
      }
    }

    if (ant) {
      DEBUGOUT(9, "  WritePixelArray(%d, %d, %d)\n", ant, x, y);
      MyWritePixelArray(amesa->imageline, 0, 0, 4 * ant, rp, x, y, ant, 1, RECTFMT_RGBA);
    }
  }
}

/* Write RGB pixels to an RGBA (or permuted) buffer. */
void cybWriteRGBSpan(const GLcontext * ctx, GLuint n, GLint x, GLint y,
		     CONST GLubyte rgb[][3], const GLubyte mask[])
{
  amigaMesaContext amesa;
  struct RastPort *rp;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  rp = amesa->rp;

  y = FIXy(y);
  x = FIXx(x);
  DEBUGOUT(1, "cybWriteRGBSpan(%d, %d, %d): ", n, x, y);

  if (!mask) {
    DEBUGOUT(1, "nomask\n");
    DEBUGOUT(2, " WritePixelArray(%d, %d, %d)\n", n, x, y);
    MyWritePixelArray(rgb, 0, 0, 3 * n, rp, x, y, n, 1, RECTFMT_RGB);
  }
  else {									/* if imageline allocated then use fastversion */
    GLshort i, ant = 0;
    GLuint *il;

    il = (GLuint *) amesa->imageline;						/* if imageline allocated then use fastversion */

    RGBb--;									/* hacky RGBA to ARGB */
    for (i = 0; i < n; i++) {							/* draw pixel (x[i],y[i]) */
      if (*mask++) {
	ant++;
	*il++ = *iRGBi;
      }
      else {
	if (ant) {
	  DEBUGOUT(9, "  WritePixelArray(%d, %d, %d)\n", ant, x, y);
	il = (GLuint *)amesa->imageline;
	  MyWritePixelArray(il, 0, 0, 4 * ant, rp, x, y, ant, 1, RECTFMT_RGBA);
	  x += ant;
	  ant = 0;
	}
	else
	  x++;
      }
    }

    if (ant) {
      DEBUGOUT(9, "  WritePixelArray(%d, %d, %d)\n", ant, x, y);
      MyWritePixelArray(amesa->imageline, 0, 0, 4 * ant, rp, x, y, ant, 1, RECTFMT_RGBA);
    }
  }
}

/* TODO: convert whole code to WriteLUTPixelArray */
void cybWriteCI32Span(const GLcontext * ctx, GLuint n, GLint x, GLint y,
		      const GLuint index[],
		      const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLuint *il;
  GLuint *ItoP;
  struct RastPort *rp;
  GLshort i, ant = 0;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  ItoP = amesa->ItoP;
  rp = amesa->rp;
  il = (GLuint *) amesa->imageline;

  y = FIXy(y);
  x = FIXx(x);
  DEBUGOUT(1, "cybWriteCI32Span(%d, %d, %d)\n", n, x, y);

  if (!mask)
    DEBUGOUT(0, "cybWriteCI32Span: called with no mask!\n");
#if 0
  if (!mask) {
    /* TODO: check if this work */
    PENCONVb--;									/* hacky RGBA to XRGB */
    WriteLUTPixelArray(index, 0, 0, 1 * n, rp, ItoP, x, y, n, 1, CTABFMT_XRGB8);
  }
  else {
    GLshort s = 0;

    PENCONVb--;									/* hacky RGBA to XRGB */
    while (s < n) {
      while (!*mask++ && s < n) {
	s++;
	x++;
      } mask--;

      if (s < n) {
	j = 0;
	while (*mask++ && s < n) {
	  s++;
	  j++;
	} mask--;
	WriteLUTPixelArray(INDEXb + x, 0, 0, 1 * j, rp, ItoP, x, y, j, 1, CTABFMT_XRGB8);
	DEBUGOUT(9, "  WriteLUTPixelArray(%d, %d, %d)\n", j, x, y);
      }
    }
  }
#endif
    /*
     * if imageline have been
     * allocated then use fastversion 
     */
    for (i = 0; i < n; i++) {
      /* draw pixel (x[i],y[i]) using index[i] */
      if (*mask++) {
	ant++;
	*il++ = GetRGBPLocal(*iINDEXi);
      }
      else {
	if (ant) {
	  DEBUGOUT(9, "  WritePixelArray(%d, %d, %d)\n", ant, x, y);
	il = (GLuint *)amesa->imageline;
	  MyWritePixelArray(il, 0, 0, 4 * ant, rp, x, y, ant, 1, RECTFMT_RGBA);
	  x += ant;
	  ant = 0;
	}
	else
	  x++;
      }
    }

    if (ant) {
      DEBUGOUT(9, "  WritePixelArray(%d, %d, %d)\n", ant, x, y);
      MyWritePixelArray(amesa->imageline, 0, 0, 4 * ant, rp, x, y, ant, 1, RECTFMT_RGBA);
    }
}

/* TODO: convert whole code to WriteLUTPixelArray */
void cybWriteCI8Span(const GLcontext * ctx, GLuint n, GLint x, GLint y,
		     const GLubyte index[],
		     const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLuint *il;
  GLuint *ItoP;
  struct RastPort *rp;
  GLshort i, ant = 0;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  ItoP = amesa->ItoP;
  rp = amesa->rp;
  il = (GLuint *) amesa->imageline;

  y = FIXy(y);
  x = FIXx(x);
  DEBUGOUT(1, "cybWriteCI8Span(%d, %d, %d)\n", n, x, y);

  if (!mask)
    DEBUGOUT(0, "cybWriteCI8Span: called with no mask!\n");
#if 0
  if (!mask) {
    /* TODO: check if this work */
    PENCONVb--;									/* hacky RGBA to XRGB */
    WriteLUTPixelArray(index, 0, 0, 1 * n, rp, ItoP, x, y, n, 1, CTABFMT_XRGB8);
  }
  else {
    GLshort s = 0;

    PENCONVb--;									/* hacky RGBA to XRGB */
    while (s < n) {
      while (!*mask++ && s < n) {
	s++;
	x++;
      } mask--;

      if (s < n) {
	j = 0;
	while (*mask++ && s < n) {
	  s++;
	  j++;
	} mask--;
	WriteLUTPixelArray(INDEXb + x, 0, 0, 1 * j, rp, ItoP, x, y, j, 1, CTABFMT_XRGB8);
	DEBUGOUT(9, "  WriteLUTPixelArray(%d, %d, %d)\n", j, x, y);
      }
    }
  }
#endif
    /*
     * if imageline have been
     * allocated then use fastversion 
     */
    for (i = 0; i < n; i++) {
      /* draw pixel (x[i],y[i]) using index[i] */
      if (*mask++) {
	ant++;
	*il++ = GetRGBPLocal(*iINDEXb);
      }
      else {
	if (ant) {
	  DEBUGOUT(9, "  WritePixelArray(%d, %d, %d)\n", ant, x, y);
	il = (GLuint *)amesa->imageline;
	  MyWritePixelArray(il, 0, 0, 4 * ant, rp, x, y, ant, 1, RECTFMT_RGBA);
	  x += ant;
	  ant = 0;
	}
	else
	  x++;
      }
    }

    if (ant) {
      MyWritePixelArray(amesa->imageline, 0, 0, 4 * ant, rp, x, y, ant, 1, RECTFMT_RGBA);
      DEBUGOUT(9, "  WritePixelArray(%d, %d, %d)\n", ant, x, y);
    }
}

void cybWriteMonoCISpan(const GLcontext * ctx, GLuint n, GLint x, GLint y, const GLubyte mask[])
{
  amigaMesaContext amesa;
  struct RastPort *rp;
  GLshort s, j;
  GLuint col;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  rp = amesa->rp;

  y = FIXy(y);
  x = FIXx(x);
  DEBUGOUT(1, "cybWriteMonoCISpan(%d, %d, %d)\n", n, x, y);

  s = 0;
  col = amesa->pixel >> 8;
  while (s < n) {
    while (!*mask++ && s < n) {
      s++;
      x++;
    } mask--;

    if (s < n) {
      j = 0;
      while (*mask++ && s < n) {
	s++;
	j++;
      } mask--;
      FillPixelArray(rp, x, y, j, 1, col);
      DEBUGOUT(9, "  FillPixelArray(%d, %d, %d)\n", j, x, y);
    }
  }
}

/**********************************************************************/
/*****        write spans of RGBA pixels doublebuffered           *****/
/**********************************************************************/

void cybWriteRGBASpanDB(const GLcontext * ctx, GLuint n, GLint x, GLint y,
			CONST GLubyte rgba[][4], const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLuint *db;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  db = dbRGBA(dbRGBAGet(amesa), x, y);

  DEBUGOUT(1, "cybWriteRGBASpanDB(%d, %d, %d)\n", n, FIXx(x), FIXy(y));

  /* Slower */
  if (mask) {
    /* draw pixel (x[i],y[i]) using index[i] */
	(GLshort)n--;
    for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, db++, RGBAi++)
      if (*mask++)
	*db = *RGBAi;
  }
  else
    /* draw pixel (x[i],y[i]) using index[i] */
	(GLshort)n--;
    for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--)
      *db++ = *RGBAi++;
}

void cybWriteRGBSpanDB(const GLcontext * ctx, GLuint n, GLint x, GLint y,
		       CONST GLubyte rgb[][3], const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLuint *db;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  db = dbRGBA(dbRGBAGet(amesa), x, y);

  DEBUGOUT(1, "cybWriteRGBSpanDB(%d, %d, %d)\n", n, FIXx(x), FIXy(y));

  /* Slower */
  if (mask) {
    /* draw pixel (x[i],y[i]) using index[i] */
	(GLshort)n--;
    for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, db++, RGBb++,RGBb++,RGBb++)
      if (*mask++)
	*db = *RGBi;								/* hacky RGB to RGBA */
  }
  else
    /* draw pixel (x[i],y[i]) using index[i] */
	(GLshort)n--;
    for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, RGBb++,RGBb++,RGBb++)
      *db++ = *RGBi;								/* hacky RGB to RGBA */
}

void cybWriteCI32SpanDB(const GLcontext * ctx, GLuint n, GLint x, GLint y,
			const GLuint index[],
			const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLuint *db;
  GLuint *ItoP;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  ItoP = amesa->ItoP;
  db = dbRGBA(dbRGBAGet(amesa), x, y);

  DEBUGOUT(1, "cybWriteCI32SpanDB(%d, %d, %d)\n", n, FIXx(x), FIXy(y));

  /* Slower */
  if (mask) {
    /* draw pixel (x[i],y[i]) using index[i] */
	(GLshort)n--;
    for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, db++, INDEXi++)
      if (*mask++)
	*db = GetRGBPLocal(*INDEXi);
  }
  else
    /* draw pixel (x[i],y[i]) using index[i] */
	(GLshort)n--;
    for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--)
      *db++ = GetRGBPLocal(*INDEXi++);
}

void cybWriteCI8SpanDB(const GLcontext * ctx, GLuint n, GLint x, GLint y,
		       const GLubyte index[],
		       const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLuint *db;
  GLuint *ItoP;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  ItoP = amesa->ItoP;
  db = dbRGBA(dbRGBAGet(amesa), x, y);

  DEBUGOUT(1, "cybWriteCI8SpanDB(%d, %d, %d)\n", n, FIXx(x), FIXy(y));

  /* Slower */
  if (mask) {
    /* draw pixel (x[i],y[i]) using index[i] */
	(GLshort)n--;
    for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, db++, INDEXb++)
      if (*mask++)
	*db = GetRGBPLocal(*INDEXb);
  }
  else
    /* draw pixel (x[i],y[i]) using index[i] */
	(GLshort)n--;
    for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--)
      *db++ = GetRGBPLocal(*INDEXb++);
}

void cybWriteMonoCISpanDB(const GLcontext * ctx, GLuint n, GLint x, GLint y, const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLuint *db;
  GLuint col;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  db = dbRGBA(dbRGBAGet(amesa), x, y);

  DEBUGOUT(1, "cybWriteMonoCISpanDB(%d, %d, %d)\n", n, FIXx(x), FIXy(y));

  col = amesa->pixel;
  /* Slower */
  if (mask) {
    /* draw pixel (x[i],y[i]) using index[i] */
	(GLshort)n--;
    for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, db++)
      if (*mask++)
	*db = col;
  }
  else
    /* draw pixel (x[i],y[i]) using index[i] */
	(GLshort)n--;
    for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--)
      *db++ = col;
}

/**********************************************************************/
/*****        read spans of RGBA pixels                           *****/
/**********************************************************************/
/* Here we should check if the size of the colortable is <256 */

void cybReadRGBASpan(const GLcontext * ctx, GLuint n, GLint x, GLint y,
		     GLubyte rgba[][4])
{
  amigaMesaContext amesa;

  amesa = (amigaMesaContext) ctx->DriverCtx;

  y = FIXy(y);
  x = FIXx(x);
  DEBUGOUT(1, "cybReadRGBASpan(%d, %d, %d)\n", n, x, y);

  DEBUGOUT(2, " ReadPixelArray(%d, %d, %d)\n", n, x, y);
  MyReadPixelArray(rgba, 0, 0, n * 4, amesa->rp, x, y, n, 1, RECTFMT_RGBA);
}

/**********************************************************************/
/*****        read spans of RGBA pixels doublebuffered            *****/
/**********************************************************************/

void cybReadRGBASpanDB(const GLcontext * ctx, GLuint n, GLint x, GLint y,
		       GLubyte rgba[][4])
{
  amigaMesaContext amesa;
  GLuint *db;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  db = dbRGBA(dbRGBAGet(amesa), x, y);

  DEBUGOUT(1, "cybReadRGBASpanDB(%d, %d, %d)\n", n, FIXx(x), FIXy(y));

	(GLshort)n--;
  for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--)
    *RGBAi++ = *db++;
}
