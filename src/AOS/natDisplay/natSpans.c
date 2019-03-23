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

#include "natSpans8.c"
#include "natSpansD.c"
#include "natSpansG.c"

/**********************************************************************/
/*****        write spans of RGBA pixels                          *****/
/**********************************************************************/

void natWriteCI32Span(const GLcontext * ctx, GLuint n, GLint x, GLint y,
		      const GLuint index[],
		      const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLubyte *il;
  GLuint *ItoP;
  struct RastPort *rp;
  GLshort i, ant = 0;								/*  allocated then use fastversion  */

  amesa = (amigaMesaContext) ctx->DriverCtx;
  ItoP = amesa->ItoP;
  rp = amesa->rp;
  il = amesa->imageline;							/*  if imageline and temporary rastport have been */

  y = FIXy(y);
  x = FIXx(x);
  DEBUGOUT(1, "natWriteCI32Span(%d, %d, %d):\n", n, x, y);

  for (i = 0; i < n; i++) {							/*  draw pixel (x[i],y[i]) using index[i]  */
    if (*mask++) {
      ant++;
      *il++ = GetRGBPLocal(*iINDEXi);
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

void natWriteCI8Span(const GLcontext * ctx, GLuint n, GLint x, GLint y,
		     const GLubyte index[],
		     const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLubyte *il;
  GLuint *ItoP;
  struct RastPort *rp;
  GLshort ant = 0, i;								/*  allocated then use fastversion  */

  amesa = (amigaMesaContext) ctx->DriverCtx;
  ItoP = amesa->ItoP;
  rp = amesa->rp;
  il = amesa->imageline;							/*  if imageline and temporary rastport have been */

  y = FIXy(y);
  x = FIXx(x);
  DEBUGOUT(1, "natWriteCI8Span(%d, %d, %d): ", n, x, y);

  for (i = 0; i < n; i++) {							/*  draw pixel (x[i],y[i]) using index[i]  */
    if (*mask++) {
      ant++;
      *il++ = GetRGBPLocal(*iINDEXb);
    }
    else {
      if (ant) {
	DEBUGOUT(9, "  MyWritePixelLine8(%d, %d, %d)\n", n, x, y);
	MyWritePixelLine8(rp, x, y, ant, il = amesa->imageline, amesa->temprp);
	x += ant;
	ant = 0;
      }
      else
	x++;
    }
  }

  if (ant) {
    DEBUGOUT(9, "  MyWritePixelLine8(%d, %d, %d)\n", n, x, y);
    MyWritePixelLine8(rp, x, y, ant, amesa->imageline, amesa->temprp);
  }
}

void natWriteMonoCISpan(const GLcontext * ctx, GLuint n, GLint x, GLint y, const GLubyte mask[])
{
  amigaMesaContext amesa;
  struct RastPort *rp;
  GLshort s;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  rp = amesa->rp;

  y = FIXy(y);
  x = FIXx(x);
  DEBUGOUT(1, "natWriteMonoCISpan(%d, %d, %d): \n", n, x, y);

  s = 0;
  SetAPen(rp, amesa->pixel);
  while (s < n) {
    while (!*mask++ && s < n) {
      s++;
      x++;
    } mask--;

    if (s < n) {
      DEBUGOUT(9, "  Move(%d, %d)\n", x, y);
      Move(rp, x, y);
      while (*mask++ && s < n) {
	s++;
	x++;
      } mask--;
      DEBUGOUT(9, "  Draw(%d, %d)\n", x - 1, y);
      Draw(rp, x - 1, y);
    }
  }
}

/**********************************************************************/
/*****        write spans of RGBA pixels doublebuffered           *****/
/**********************************************************************/

void natWriteCI32SpanDB(const GLcontext * ctx, GLuint n, GLint x, GLint y,
			const GLuint index[],
			const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLubyte *db;
  GLuint *ItoP;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  db = dbPen(dbPenGet(amesa), x, y);
  ItoP = amesa->ItoP;

  DEBUGOUT(1, "natWriteCI32SpanDB(%d, %d, %d)\n", n, FIXx(x), FIXy(y));

  if (!mask) {
    /* draw pixel (x[i],y[i]) using index[i] */
	(GLshort)n--;
    for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--)
      *db++ = GetRGBPLocal(*INDEXi++);
  }
  else
    /* draw pixel (x[i],y[i]) using index[i] */
	(GLshort)n--;
    for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, db++, INDEXi++)		/* Slower */
      if (*mask++)
	*db = GetRGBPLocal(*INDEXi);
}

void natWriteCI8SpanDB(const GLcontext * ctx, GLuint n, GLint x, GLint y,
		       const GLubyte index[],
		       const GLubyte mask[])
{

  amigaMesaContext amesa;
  GLubyte *db;
  GLuint *ItoP;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  db = dbPen(dbPenGet(amesa), x, y);
  ItoP = amesa->ItoP;

  DEBUGOUT(1, "natWriteCI8SpanDB(%d, %d, %d)\n", n, FIXx(x), FIXy(y));

  if (!mask) {
    /* draw pixel (x[i],y[i]) using index[i] */
	(GLshort)n--;
    for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--)
      *db++ = GetRGBPLocal(*INDEXb++);
  }
  else
    /* draw pixel (x[i],y[i]) using index[i] */
	(GLshort)n--;
    for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, db++, INDEXb++)		/* Slower */
      if (*mask++)
	*db = GetRGBPLocal(*INDEXb);
}

void natWriteMonoCISpanDB(const GLcontext * ctx, GLuint n, GLint x, GLint y,
			  const GLubyte mask[])
{
  amigaMesaContext amesa;
  GLubyte *db;
  GLubyte col;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  db = dbPen(dbPenGet(amesa), x, y);

  DEBUGOUT(1, "natWriteMonoCISpanDB(%d, %d, %d)\n", n, FIXx(x), FIXy(y));

  col = amesa->pixel;
  if (!mask) {
    /* draw pixel (x[i],y[i]) using index[i] */
	(GLshort)n--;
    for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--)				/* Slower */
      *db++ = col;
  }
  else
    /* draw pixel (x[i],y[i]) using index[i] */
	(GLshort)n--;
    for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--, db++)
      if (*mask++)
	*db = col;
}

/**********************************************************************/
/*****        read spans of RGBA pixels doublebuffered            *****/
/**********************************************************************/

void natReadRGBASpanDB(const GLcontext * ctx, GLuint n, GLint x, GLint y,
		       GLubyte rgba[][4])
{
  /* TODO: samples-old/accum have problems with something, maybe this (check it) */
#if 0
  amigaMesaContext amesa;
  GLubyte *db;
  GLuint *PtoI;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  db = dbPen(dbPenGet(amesa), x, y);
  PtoI = amesa->PtoI;

  DEBUGOUT(1, "natReadRGBASpanDB(%d, %d, %d)\n", n, FIXx(x), FIXy(y));

  for (--(GLshort)n; (GLshort)n >= 0; (GLshort)n--)
    *RGBAi++ = GetIndexLocal(*db++) & 0xFFFFFF00;				/* clear alpha? */
#endif
}

void natReadCI32SpanDB(const GLcontext * ctx, GLuint n, GLint x, GLint y, GLuint index[])
{
  amigaMesaContext amesa;
  GLubyte *db;
  GLuint *PtoI;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  db = dbPen(dbPenGet(amesa), x, y);
  PtoI = amesa->PtoI;

  DEBUGOUT(1, "natReadCI32SpanDB(%d, %d, %d)\n", n, FIXx(x), FIXy(y));

	(GLshort)n--;
  for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--)
    *INDEXi++ = GetIndexLocal(*db++) & 0x000000FF;				/* clear rgb! */
}

/**********************************************************************/
/*****        read spans of RGBA pixels                           *****/
/**********************************************************************/

void natReadCI32Span(const GLcontext * ctx, GLuint n, GLint x, GLint y, GLuint index[])
{
  amigaMesaContext amesa;
  struct RastPort *rp;
  GLubyte *il;
  GLuint *PtoI;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  PtoI = amesa->PtoI;
  rp = amesa->rp;
  il = amesa->imageline;

  y = FIXy(y);
  x = FIXx(x);
  DEBUGOUT(1, "natReadCI32Span(%d, %d, %d)\n", n, x, y);

  DEBUGOUT(8, "  MyReadPixelLine8(%d, %d, %d)\n", n, x, y);
  MyReadPixelLine8(rp, x, y, n, il, amesa->temprp);
	(GLshort)n--;
  for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--)
    *INDEXi++ = GetIndexLocal(*il++) & 0x000000FF;				/* clear rgb! */
}

void natReadRGBASpan(const GLcontext * ctx, GLuint n, GLint x, GLint y,
		     GLubyte rgba[][4])
{
  amigaMesaContext amesa;
  struct RastPort *rp;
  GLubyte *il;
  GLuint *PtoI;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  PtoI = amesa->PtoI;
  rp = amesa->rp;
  il = amesa->imageline;

  y = FIXy(y);
  x = FIXx(x);
  DEBUGOUT(1, "natReadRGBASpan(%d, %d, %d)\n", n, x, y);

  /* bug in egcs (spilled register) */
  DEBUGOUT(8, "  MyReadPixelLine8(%d, %d, %d)\n", n, x, y);
  MyReadPixelLine8(rp, x, y, n, il, amesa->temprp);
	(GLshort)n--;
  for ((GLshort)n; (GLshort)n >= 0; (GLshort)n--)
    *RGBAi++ = GetIndexLocal(*il++) & 0xFFFFFF00;				/* clear alpha? */
}
