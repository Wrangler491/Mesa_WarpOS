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

#define	rup(pos)	(GLdepth) ((pos) * MAX_DEPTH + 0.5)
#define	rdown(pos)	(((W3D_Float) (pos) + 0.5) / MAX_DEPTH)

GLuint wrpDepthTestSpan(GLcontext * ctx,
			GLuint n, GLint x, GLint y, const GLdepth z[],
			GLubyte mask[])
{
  amigaMesaContext amesa;
  struct TDdriver *TDdriver;
  W3D_Context *TDcontext;
  W3D_Double zf[MAX_WIDTH];
  W3D_Double *zptr = zf;
  GLubyte *m = mask;
  GLuint i;
  GLuint passed = 0;

  DEBUGOUT(1, "wrpDepthTestSpan()\n");

  amesa = (amigaMesaContext) ctx->DriverCtx;
  TDdriver = amesa->TDdriver;
  TDcontext = TDdriver->td_ctx;

  x = FIXx(x);
  y = FIXy(y);

  FlushHardware(TDdriver, TDcontext);

  /* these depth tests are really slow (VRAM accesses) therefore
   * GL applications, which can't be handled by the installed
   * 3D hardware should be run with hardware acceleration disabled
   */

  /* switch cases ordered from most frequent to less frequent */
  switch (ctx->Depth.Func) {
    case GL_LESS:
      if (ctx->Depth.Mask) {
	/* Update Z buffer */
	W3D_ReadZSpan(TDcontext, x, y, n, zf);
	for (i = 0; i < n; i++, zptr++, m++) {
	  if (*m) {
	    if (z[i] < rup(*zptr)) {
	      /* pass */
	      *zptr = rdown(z[i]);
	      passed++;
	    }
	    else
	      /* fail */
	      *m = 0;
	  }
	}
	W3D_WriteZSpan(TDcontext, x, y, n, zf, (UBYTE *) mask);
      }
      else {
	/* Don't update Z buffer */
	W3D_ReadZSpan(TDcontext, x, y, n, zf);
	for (i = 0; i < n; i++, zptr++, m++) {
	  if (*m) {
	    if (z[i] < rup(*zptr))
	      passed++;
	    else
	      *m = 0;
	  }
	}
      }
      break;
    case GL_LEQUAL:
      if (ctx->Depth.Mask) {
	/* Update Z buffer */
	W3D_ReadZSpan(TDcontext, x, y, n, zf);
	for (i = 0; i < n; i++, zptr++, m++) {
	  if (*m) {
	    if (z[i] <= rup(*zptr)) {
	      *zptr = rdown(z[i]);
	      passed++;
	    }
	    else {
	      *m = 0;
	    }
	  }
	}
	W3D_WriteZSpan(TDcontext, x, y, n, zf, (UBYTE *) mask);
      }
      else {
	/* Don't update Z buffer */
	W3D_ReadZSpan(TDcontext, x, y, n, zf);
	for (i = 0; i < n; i++, zptr++, m++) {
	  if (*m) {
	    if (z[i] <= rup(*zptr))
	      passed++;
	    else
	      *m = 0;
	  }
	}
      }
      break;
    case GL_GEQUAL:
      if (ctx->Depth.Mask) {
	/* Update Z buffer */
	W3D_ReadZSpan(TDcontext, x, y, n, zf);
	for (i = 0; i < n; i++, zptr++, m++) {
	  if (*m) {
	    if (z[i] >= rup(*zptr)) {
	      *zptr = rdown(z[i]);
	      passed++;
	    }
	    else
	      *m = 0;
	  }
	}
	W3D_WriteZSpan(TDcontext, x, y, n, zf, (UBYTE *) mask);
      }
      else {
	/* Don't update Z buffer */
	W3D_ReadZSpan(TDcontext, x, y, n, zf);
	for (i = 0; i < n; i++, zptr++, m++) {
	  if (*m) {
	    if (z[i] >= rup(*zptr))
	      passed++;
	    else
	      *m = 0;
	  }
	}
      }
      break;
    case GL_GREATER:
      if (ctx->Depth.Mask) {
	/* Update Z buffer */
	W3D_ReadZSpan(TDcontext, x, y, n, zf);
	for (i = 0; i < n; i++, zptr++, m++) {
	  if (*m) {
	    if (z[i] > rup(*zptr)) {
	      *zptr = rdown(z[i]);
	      passed++;
	    }
	    else
	      *m = 0;
	  }
	}
	W3D_WriteZSpan(TDcontext, x, y, n, zf, (UBYTE *) mask);
      }
      else {
	/* Don't update Z buffer */
	W3D_ReadZSpan(TDcontext, x, y, n, zf);
	for (i = 0; i < n; i++, zptr++, m++) {
	  if (*m) {
	    if (z[i] > rup(*zptr))
	      passed++;
	    else
	      *m = 0;
	  }
	}
      }
      break;
    case GL_NOTEQUAL:
      if (ctx->Depth.Mask) {
	/* Update Z buffer */
	W3D_ReadZSpan(TDcontext, x, y, n, zf);
	for (i = 0; i < n; i++, zptr++, m++) {
	  if (*m) {
	    if (z[i] != rup(*zptr)) {
	      *zptr = rdown(z[i]);
	      passed++;
	    }
	    else
	      *m = 0;
	  }
	}
	W3D_WriteZSpan(TDcontext, x, y, n, zf, (UBYTE *) mask);
      }
      else {
	/* Don't update Z buffer */
	W3D_ReadZSpan(TDcontext, x, y, n, zf);
	for (i = 0; i < n; i++, zptr++, m++) {
	  if (*m) {
	    if (z[i] != rup(*zptr))
	      passed++;
	    else
	      *m = 0;
	  }
	}
      }
      break;
    case GL_EQUAL:
      if (ctx->Depth.Mask) {
	/* Update Z buffer */
	W3D_ReadZSpan(TDcontext, x, y, n, zf);
	for (i = 0; i < n; i++, zptr++, m++) {
	  if (*m) {
	    if (z[i] == rup(*zptr)) {
	      *zptr = rdown(z[i]);
	      passed++;
	    }
	    else
	      *m = 0;
	  }
	}
	W3D_WriteZSpan(TDcontext, x, y, n, zf, (UBYTE *) mask);
      }
      else {
	/* Don't update Z buffer */
	W3D_ReadZSpan(TDcontext, x, y, n, zf);
	for (i = 0; i < n; i++, zptr++, m++) {
	  if (*m) {
	    if (z[i] == rup(*zptr))
	      passed++;
	    else
	      *m = 0;
	  }
	}
      }
      break;
    case GL_ALWAYS:
      if (ctx->Depth.Mask) {
	/* Update Z buffer */
	W3D_ReadZSpan(TDcontext, x, y, n, zf);
	for (i = 0; i < n; i++, zptr++, m++) {
	  if (*m) {
	    *zptr = rdown(z[i]);
	    passed++;
	  }
	}
	W3D_WriteZSpan(TDcontext, x, y, n, zf, (UBYTE *) mask);
      }
      else
	/* Don't update Z buffer or mask */
	passed = n;
      break;
    case GL_NEVER:
      bzero(mask, n * sizeof(GLubyte));
    default:
      break;
  }										/*switch */

  return passed;
}

void wrpDepthTestPixels(GLcontext * ctx,
			GLuint n, const GLint x[], const GLint y[],
			const GLdepth z[], GLubyte mask[])
{
  amigaMesaContext amesa;
  struct TDdriver *TDdriver;
  W3D_Context *TDcontext;
  W3D_Double f;
  GLuint i;

  DEBUGOUT(1, "wrpSetupDriver()\n");

  amesa = (amigaMesaContext) ctx->DriverCtx;
  TDdriver = amesa->TDdriver;
  TDcontext = TDdriver->td_ctx;

  FlushHardware(TDdriver, TDcontext);

  /* these depth tests are really slow (VRAM accesses) therefore
   * GL applications, which can't be handled by the installed
   * 3D hardware should be run with hardware acceleration disabled
   */

  /* switch cases ordered from most frequent to less frequent */
  switch (ctx->Depth.Func) {
    case GL_LESS:
      if (ctx->Depth.Mask) {
	/* Update Z buffer */
	for (i = 0; i < n; i++) {
	  if (mask[i]) {
	    W3D_ReadZPixel(TDcontext, FIXx(x[i]), FIXy(y[i]), &f);
	    if (z[i] < rup(f)) {
	      f = rdown(z[i]);
	      W3D_WriteZPixel(TDcontext, FIXx(x[i]), FIXy(y[i]), &f);
	    }
	    else
	      mask[i] = 0;
	  }
	}
      }
      else {
	/* Don't update Z buffer */
	for (i = 0; i < n; i++) {
	  if (mask[i]) {
	    W3D_ReadZPixel(TDcontext, FIXx(x[i]), FIXy(y[i]), &f);
	    if (z[i] >= rup(f))
	      mask[i] = 0;
	  }
	}
      }
      break;
    case GL_LEQUAL:
      if (ctx->Depth.Mask) {
	/* Update Z buffer */
	for (i = 0; i < n; i++) {
	  if (mask[i]) {
	    W3D_ReadZPixel(TDcontext, FIXx(x[i]), FIXy(y[i]), &f);
	    if (z[i] <= rup(f)) {
	      f = rdown(z[i]);
	      W3D_WriteZPixel(TDcontext, FIXx(x[i]), FIXy(y[i]), &f);
	    }
	    else
	      mask[i] = 0;
	  }
	}
      }
      else {
	/* Don't update Z buffer */
	for (i = 0; i < n; i++) {
	  if (mask[i]) {
	    W3D_ReadZPixel(TDcontext, FIXx(x[i]), FIXy(y[i]), &f);
	    if (z[i] > rup(f))
	      mask[i] = 0;
	  }
	}
      }
      break;
    case GL_GEQUAL:
      if (ctx->Depth.Mask) {
	/* Update Z buffer */
	for (i = 0; i < n; i++) {
	  if (mask[i]) {
	    W3D_ReadZPixel(TDcontext, FIXx(x[i]), FIXy(y[i]), &f);
	    if (z[i] >= rup(f)) {
	      f = rdown(z[i]);
	      W3D_WriteZPixel(TDcontext, FIXx(x[i]), FIXy(y[i]), &f);
	    }
	    else
	      mask[i] = 0;
	  }
	}
      }
      else {
	/* Don't update Z buffer */
	for (i = 0; i < n; i++) {
	  if (mask[i]) {
	    W3D_ReadZPixel(TDcontext, FIXx(x[i]), FIXy(y[i]), &f);
	    if (z[i] < rup(f))
	      mask[i] = 0;
	  }
	}
      }
      break;
    case GL_GREATER:
      if (ctx->Depth.Mask) {
	/* Update Z buffer */
	for (i = 0; i < n; i++) {
	  if (mask[i]) {
	    W3D_ReadZPixel(TDcontext, FIXx(x[i]), FIXy(y[i]), &f);
	    if (z[i] > rup(f)) {
	      f = rdown(z[i]);
	      W3D_WriteZPixel(TDcontext, FIXx(x[i]), FIXy(y[i]), &f);
	    }
	    else
	      mask[i] = 0;
	  }
	}
      }
      else {
	/* Don't update Z buffer */
	for (i = 0; i < n; i++) {
	  if (mask[i]) {
	    W3D_ReadZPixel(TDcontext, FIXx(x[i]), FIXy(y[i]), &f);
	    if (z[i] <= rup(f))
	      mask[i] = 0;
	  }
	}
      }
      break;
    case GL_NOTEQUAL:
      if (ctx->Depth.Mask) {
	/* Update Z buffer */
	for (i = 0; i < n; i++) {
	  if (mask[i]) {
	    W3D_ReadZPixel(TDcontext, FIXx(x[i]), FIXy(y[i]), &f);
	    if (z[i] != rup(f)) {
	      f = rdown(z[i]);
	      W3D_WriteZPixel(TDcontext, FIXx(x[i]), FIXy(y[i]), &f);
	    }
	    else
	      mask[i] = 0;
	  }
	}
      }
      else {
	/* Don't update Z buffer */
	for (i = 0; i < n; i++) {
	  if (mask[i]) {
	    W3D_ReadZPixel(TDcontext, FIXx(x[i]), FIXy(y[i]), &f);
	    if (z[i] == rup(f))
	      mask[i] = 0;
	  }
	}
      }
      break;
    case GL_EQUAL:
      if (ctx->Depth.Mask) {
	/* Update Z buffer */
	for (i = 0; i < n; i++) {
	  if (mask[i]) {
	    W3D_ReadZPixel(TDcontext, FIXx(x[i]), FIXy(y[i]), &f);
	    if (z[i] == rup(f)) {
	      f = rdown(z[i]);
	      W3D_WriteZPixel(TDcontext, FIXx(x[i]), FIXy(y[i]), &f);
	    }
	    else
	      mask[i] = 0;
	  }
	}
      }
      else {
	/* Don't update Z buffer */
	for (i = 0; i < n; i++) {
	  if (mask[i]) {
	    W3D_ReadZPixel(TDcontext, FIXx(x[i]), FIXy(y[i]), &f);
	    if (z[i] != rup(f))
	      mask[i] = 0;
	  }
	}
      }
      break;
    case GL_ALWAYS:
      if (ctx->Depth.Mask) {
	/* Update Z buffer */
	for (i = 0; i < n; i++) {
	  if (mask[i]) {
	    f = rdown(z[i]);
	    W3D_WriteZPixel(TDcontext, FIXx(x[i]), FIXy(y[i]), &f);
	  }
	}
      }
      break;
    case GL_NEVER:
      /* depth test never passes */
      bzero(mask, n * sizeof(GLubyte));
    default:
      break;
  }										/*switch */
}

void wrpReadDepthSpanFloat(GLcontext * ctx,
			   GLuint n, GLint x, GLint y, GLfloat depth[])
{
  amigaMesaContext amesa;
  struct TDdriver *TDdriver;
  W3D_Context *TDcontext;
  W3D_Double z[MAX_WIDTH];
  GLint i;

  DEBUGOUT(1, "wrpSetupDriver()\n");

  amesa = (amigaMesaContext) ctx->DriverCtx;
  TDdriver = amesa->TDdriver;
  TDcontext = TDdriver->td_ctx;

  FlushHardware(TDdriver, TDcontext);

  W3D_ReadZSpan(TDcontext, FIXx(x), FIXy(y), n, z);
  for (i = 0; i < n; i++)
    depth[i] = (float)z[i];
}

void wrpReadDepthSpanInt(GLcontext * ctx,
			 GLuint n, GLint x, GLint y, GLdepth depth[])
{
  amigaMesaContext amesa;
  struct TDdriver *TDdriver;
  W3D_Context *TDcontext;
  W3D_Double z[MAX_WIDTH];
  GLint i;

  DEBUGOUT(1, "wrpSetupDriver()\n");

  amesa = (amigaMesaContext) ctx->DriverCtx;
  TDdriver = amesa->TDdriver;
  TDcontext = TDdriver->td_ctx;

  FlushHardware(TDdriver, TDcontext);

  /* implicitly assumes, that GLfloat == float! */
  W3D_ReadZSpan(TDcontext, FIXx(x), FIXy(y), n, z);
  for (i = 0; i < n; i++)
    depth[i] = rup(z[i]);
}
