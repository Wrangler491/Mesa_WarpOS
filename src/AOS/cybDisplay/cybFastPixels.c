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

GLboolean cybDirectDrawPixels(GLcontext *ctx,
                              GLint x, GLint y, GLsizei width, GLsizei height,
                              GLenum format, GLenum type,
                              const struct gl_pixelstore_attrib *unpack,
                              const GLvoid *pixels ) {
  amigaMesaContext amesa;
  struct RastPort *rp;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  rp = amesa->rp;

  if ((ctx->RasterMask & (~(SCISSOR_BIT | WINCLIP_BIT))) == 0
      && ctx->Pixel.RedBias == 0.0 && ctx->Pixel.RedScale == 1.0
      && ctx->Pixel.GreenBias == 0.0 && ctx->Pixel.GreenScale == 1.0
      && ctx->Pixel.BlueBias == 0.0 && ctx->Pixel.BlueScale == 1.0
      && ctx->Pixel.AlphaBias == 0.0 && ctx->Pixel.AlphaScale == 1.0
      && ctx->Pixel.IndexShift == 0 && ctx->Pixel.IndexOffset == 0
      && ctx->Pixel.MapColorFlag == 0
      && unpack->Alignment == 1
      && !unpack->SwapBytes
      && !unpack->LsbFirst) {
    GLint skipSize;
    GLint skipPixels = unpack->SkipPixels;
    GLint skipRows = unpack->SkipRows;
    GLint rowLength;
    GLdepth zSpan[amesa->width];						/* only used when zooming */
    GLint zoomY0 = y;								/* save Y value of first row */

    GLboolean isZoom = GL_FALSE;
    if ((ctx->Pixel.ZoomX == 1.0F) &&
	(ctx->Pixel.ZoomY == 1.0F))
      isZoom = GL_TRUE;

    if (unpack->RowLength > 0)
      rowLength = unpack->RowLength;
    else
      rowLength = width;

    /* If we're not using pixel zoom then do all clipping calculations
     * now.  Otherwise, we'll let the gl_write_zoomed_*_span() functions
     * handle the clipping.
     */
    if (!isZoom) {
      /* horizontal clipping */
      if (x < ctx->Buffer->Xmin) {
	skipPixels += (ctx->Buffer->Xmin - x);
	width -= (ctx->Buffer->Xmin - x);
	x = ctx->Buffer->Xmin;
      }
      if (x + width > ctx->Buffer->Xmax)
	width -= (x + width - ctx->Buffer->Xmax - 1);
      if (width <= 0)
	return GL_TRUE;

      /* vertical clipping */
      if (y < ctx->Buffer->Ymin) {
	skipRows += (ctx->Buffer->Ymin - y);
	height -= (ctx->Buffer->Ymin - y);
	y = ctx->Buffer->Ymin;
      }
      if (y + height > ctx->Buffer->Ymax)
	height -= (y + height - ctx->Buffer->Ymax - 1);
      if (height <= 0)
	return GL_TRUE;
    }
    else {
      /* setup array of fragment Z value to pass to zoom function */
      GLdepth z = (GLdepth) (ctx->Current.RasterPos[2] * DEPTH_SCALE);
      GLint i;

      for (i = 0; i < width; i++)
	zSpan[i] = z;
    }

    /*
     * Ready to draw!
     * The window region at (x, y) of size (width, height)
     * will be written to.
     * We'll take pixel data from buffer pointed to by "pixels" but we'll
     * skip "skipRows" rows and skip "skipPixels" pixels/row.
     */
    skipSize = (skipRows * rowLength + skipPixels);

    if (format == GL_RGBA && type == GL_UNSIGNED_BYTE) {
      if (ctx->Visual->RGBAflag) {
	GLubyte *src = (GLubyte *) pixels + skipSize * 4;

	if (!isZoom)
	  /* no zooming */
	  MyWritePixelArray(src, 0, 0, rowLength * 4, rp, FIXx(x), FIXy(y), width, height, RECTFMT_RGBA);
	else {
	  /* with zooming */
	  while (--height >= 0) {
	    gl_write_zoomed_rgba_span(ctx, width, x, y, zSpan, (void *)src, zoomY0);

	    src += rowLength * 4;
	    y++;
	  }
	}
      }

      return GL_TRUE;
    }
    else if (format == GL_RGB && type == GL_UNSIGNED_BYTE) {
      if (ctx->Visual->RGBAflag) {
	GLubyte *src = (GLubyte *) pixels + skipSize * 3;

	if (!isZoom)
	  MyWritePixelArray(src, 0, 0, rowLength * 3, rp, FIXx(x), FIXy(y), width, height, RECTFMT_RGB);
	else {
	  /* with zooming */
	  while (--height >= 0) {
	    gl_write_zoomed_rgb_span(ctx, width, x, y, zSpan, (void *)src, zoomY0);

	    src += rowLength * 3;
	    y++;
	  }
	}
      }

      return GL_TRUE;
    }
    else if (format == GL_LUMINANCE && type == GL_UNSIGNED_BYTE) {
      if (ctx->Visual->RGBAflag) {
	GLubyte *src = (GLubyte *) pixels + skipSize * 1;

	if (!isZoom)
	  MyWritePixelArray(src, 0, 0, rowLength * 1, rp, FIXx(x), FIXy(y), width, height, RECTFMT_GREY8);
	else {
	  /* with zooming */
	  while (--height >= 0) {
	    GLshort i = width;
	    GLubyte *il = amesa->imageline;

	    while (--i >= 0) {
	      GLushort pel = *src++;
	      *(GLushort *)il++ = (pel << 8) | pel;	// was *((GLushort *)il)++
	      *il++ = pel;
	    }
	    gl_write_zoomed_rgb_span(ctx, width, x, y, zSpan, (void *)amesa->imageline, zoomY0);

	    src += rowLength - width;
	    y++;
	  }
	}
      }
      return GL_TRUE;
    }
    else if (format == GL_LUMINANCE_ALPHA && type == GL_UNSIGNED_BYTE) {
      if (ctx->Visual->RGBAflag) {
	GLubyte *src = (GLubyte *) pixels + skipSize * 2;

	if (!isZoom)
	  /* with zooming */
	  while (--height >= 0) {
	    GLshort i = width;
	    GLubyte *il = amesa->imageline;

	    while (--i >= 0) {
	      *il++ = *src++;
	      src++;
	    }
	    MyWritePixelArray(amesa->imageline, 0, 0, rowLength * 1, rp, FIXx(x), FIXy(y), width, height, RECTFMT_GREY8);	/* alpha isn't on amiga */

	    src += rowLength * 2 - width * 2;
	    y++;
	  }
	else {
	  /* with zooming */
	  while (--height >= 0) {
	    GLshort i = width;
	    GLuint *il = (GLuint *)amesa->imageline;

	    while (--i >= 0) {
	      GLuint pel = *src++;
	      *il++ = ((((pel << 8) | pel) << 8) | pel) << 8 | *src++;
	    }
	    gl_write_zoomed_rgba_span(ctx, width, x, y, zSpan, (void *)amesa->imageline, zoomY0);

	    src += rowLength * 2 - width * 2;
	    y++;
	  }
	}
      }

      return GL_TRUE;
    }
    else if (format == GL_COLOR_INDEX && type == GL_UNSIGNED_BYTE) {
      GLubyte *src = (GLubyte *) pixels + skipSize * 1;

      if (ctx->Visual->RGBAflag) {
	/* convert CI data to RGBA */
	if (!isZoom) {
	  /* no zooming */
	  if (!ctx->Pixel.MapItoRsize &&
	      !ctx->Pixel.MapItoGsize &&
	      !ctx->Pixel.MapItoBsize) {
	    /* no masking */
	    GLshort i;
	    ULONG xrgb[256];
	    const GLubyte *rMap = ctx->Pixel.MapItoR8;
	    const GLubyte *gMap = ctx->Pixel.MapItoG8;
	    const GLubyte *bMap = ctx->Pixel.MapItoB8;

	    for (i = 256 - 1; i >= 0; i--)
	      xrgb[i] = (((rMap[i] << 8) | gMap[i]) << 8) | bMap[i];

	    MyWriteLUTPixelArray(src, 0, 0, rowLength * 1, rp, xrgb, FIXx(x), FIXy(y), width, height, CTABFMT_XRGB8);
	  }
	  else {
	    /* with masking */
	    while (--height >= 0) {
	      gl_map_ci8_to_rgba(ctx, width, src, amesa->imageline);			/* masks */
	      MyWritePixelArray(amesa->imageline, 0, 0, 4 * width, rp, FIXx(x), FIXy(y), width, 1, RECTFMT_RGBA);

	      src += rowLength;
	      y++;
	    }
	  }

	  return GL_TRUE;
	}
	else {
	  /* with zooming */
	  while (--height >= 0) {
	    gl_map_ci8_to_rgba(ctx, width, src, amesa->imageline);			/* masks */
	    gl_write_zoomed_rgba_span(ctx, width, x, y, zSpan, (void *)amesa->imageline, zoomY0);

	    src += rowLength;
	    y++;
	  }

	  return GL_TRUE;
	}
      }
      else {
	/* write CI data to CI frame buffer */
	if (!isZoom) {
	  MyWriteLUTPixelArray(src, 0, 0, rowLength * 1, rp, (ULONG *)((GLubyte *)amesa->ItoP - 1), FIXx(x), FIXy(y), width, height, CTABFMT_XRGB8);

	  return GL_TRUE;
	}
	else {
	  /* with zooming */
	  return GL_FALSE;
	}
      }
    }
    else {
      /* can't handle this pixel format and/or data type here */
      return GL_FALSE;
    }
  }
  else {
    /* can't do direct render, have to use slow path */
    return GL_FALSE;
  }
}

GLboolean cybDirectDrawPixelsDB(GLcontext *ctx,
                                GLint x, GLint y, GLsizei width, GLsizei height,
                                GLenum format, GLenum type,
                                const struct gl_pixelstore_attrib *unpack,
                                const GLvoid *pixels ) {
  amigaMesaContext amesa;
  GLuint *db;

  amesa = (amigaMesaContext) ctx->DriverCtx;
  db = dbRGBA(dbRGBAGet(amesa), FIXx(x), FIXy(y));

  if ((ctx->RasterMask & (~(SCISSOR_BIT | WINCLIP_BIT))) == 0
      && ctx->Pixel.RedBias == 0.0 && ctx->Pixel.RedScale == 1.0
      && ctx->Pixel.GreenBias == 0.0 && ctx->Pixel.GreenScale == 1.0
      && ctx->Pixel.BlueBias == 0.0 && ctx->Pixel.BlueScale == 1.0
      && ctx->Pixel.AlphaBias == 0.0 && ctx->Pixel.AlphaScale == 1.0
      && ctx->Pixel.IndexShift == 0 && ctx->Pixel.IndexOffset == 0
      && ctx->Pixel.MapColorFlag == 0
      && unpack->Alignment == 1
      && !unpack->SwapBytes
      && !unpack->LsbFirst) {
    GLint skipSize;
    GLint skipPixels = unpack->SkipPixels;
    GLint skipRows = unpack->SkipRows;
    GLint rowLength;
    GLdepth zSpan[amesa->width];						/* only used when zooming */
    GLint zoomY0 = y;								/* save Y value of first row */

    GLboolean isZoom = GL_FALSE;
    if (ctx->Pixel.ZoomX == 1.0F && ctx->Pixel.ZoomY == 1.0F)
      isZoom = GL_TRUE;

    if (unpack->RowLength > 0)
      rowLength = unpack->RowLength;
    else
      rowLength = width;

    /* If we're not using pixel zoom then do all clipping calculations
     * now.  Otherwise, we'll let the gl_write_zoomed_*_span() functions
     * handle the clipping.
     */
    if (!isZoom) {
      /* horizontal clipping */
      if (x < ctx->Buffer->Xmin) {
	skipPixels += (ctx->Buffer->Xmin - x);
	width -= (ctx->Buffer->Xmin - x);
	x = ctx->Buffer->Xmin;
      }
      if (x + width > ctx->Buffer->Xmax)
	width -= (x + width - ctx->Buffer->Xmax - 1);
      if (width <= 0)
	return GL_TRUE;

      /* vertical clipping */
      if (y < ctx->Buffer->Ymin) {
	skipRows += (ctx->Buffer->Ymin - y);
	height -= (ctx->Buffer->Ymin - y);
	y = ctx->Buffer->Ymin;
      }
      if (y + height > ctx->Buffer->Ymax)
	height -= (y + height - ctx->Buffer->Ymax - 1);
      if (height <= 0)
	return GL_TRUE;
    }
    else {
      /* setup array of fragment Z value to pass to zoom function */
      GLdepth z = (GLdepth) (ctx->Current.RasterPos[2] * DEPTH_SCALE);
      GLint i;

      for (i = 0; i < width; i++)
	zSpan[i] = z;
    }

    /*
     * Ready to draw!
     * The window region at (x, y) of size (width, height)
     * will be written to.
     * We'll take pixel data from buffer pointed to by "pixels" but we'll
     * skip "skipRows" rows and skip "skipPixels" pixels/row.
     */
    skipSize = (skipRows * rowLength + skipPixels);

    if (format == GL_RGBA && type == GL_UNSIGNED_BYTE) {
      if (ctx->Visual->RGBAflag) {
	GLubyte *src = (GLubyte *) pixels + skipSize * 4;

	if (!isZoom) {
	  /* no zooming */
	  while (--height >= 0) {
	    GLshort i = width;

	    while (--i >= 0)
	      *db++ = *src++;

	    src += rowLength * 4 - width * 4;
	    db += amesa->FixedWidth - width;
	  }
	}
	else {
	  /* with zooming */
	  while (--height >= 0) {
	    gl_write_zoomed_rgba_span(ctx, width, x, y, zSpan, (void *)src, zoomY0);

	    src += rowLength * 4;
	    y++;
	  }
	}
      }
      return GL_TRUE;
    }
    else if (format == GL_RGB && type == GL_UNSIGNED_BYTE) {
      if (ctx->Visual->RGBAflag) {
	GLubyte *src = (GLubyte *) pixels + skipSize * 3;

	if (!isZoom) {
	  /* no zooming */
	  while (--height >= 0) {
	    GLshort i = width;

	    while (--i >= 0) {
	      *db++ = *((GLuint *)src);
	      src += 3;
	    }

	    src += rowLength * 3 - width * 3;
	    db += amesa->FixedWidth - width;
	  }
	}
	else {
	  /* with zooming */
	  while (--height >= 0) {
	    gl_write_zoomed_rgb_span(ctx, width, x, y, zSpan, (void *)src, zoomY0);

	    src += rowLength * 3;
	    y++;
	  }
	}
      }
      return GL_TRUE;
    }
    else if (format == GL_LUMINANCE && type == GL_UNSIGNED_BYTE) {
      if (ctx->Visual->RGBAflag) {
	GLubyte *src = (GLubyte *) pixels + skipSize * 1;

	if (!isZoom) {
	  /* no zooming */
	  while (--height >= 0) {
	    GLshort i = width;

	    while (--i >= 0) {
	      GLuint grey = *src++;
	      *db++ = ((((grey << 8) | grey) << 8) | grey) << 8;
	    }

	    src += rowLength * 1 - width * 1;
	    db += amesa->FixedWidth - width;
	  }
	}
	else {
	  /* with zooming */
	  while (--height >= 0) {
	    GLshort i = width;
	    GLubyte *il = amesa->imageline;

	    while (--i >= 0) {
	      GLushort pel = *src++;
	      *(GLushort *)il++ = (pel << 8) | pel;
	      *il++ = pel;
	    }
	    gl_write_zoomed_rgb_span(ctx, width, x, y, zSpan, (void *)amesa->imageline, zoomY0);

	    src += rowLength - width;
	    y++;
	  }
	}
      }
      return GL_TRUE;
    }
    else if (format == GL_LUMINANCE_ALPHA && type == GL_UNSIGNED_BYTE) {
      if (ctx->Visual->RGBAflag) {
	GLubyte *src = (GLubyte *) pixels + skipSize * 2;

	if (!isZoom) {
	  /* no zooming */
	  while (--height >= 0) {
	    GLshort i = width;

	    while (--i >= 0) {
	      GLuint grey = *src++;
	      *db++ = ((((grey << 8) | grey) << 8) | grey) << 8;
	      src++;
	    }

	    src += rowLength * 2 - width * 2;
	    db += amesa->FixedWidth - width;
	  }
	}
	else {
	  /* with zooming */
	  while (--height >= 0) {
	    GLshort i = width;
	    GLuint *il = (GLuint *)amesa->imageline;

	    while (--i >= 0) {
	      GLuint pel = *src++;
	      *il++ = ((((pel << 8) | pel) << 8) | pel) << 8 | *src++;
	    }
	    gl_write_zoomed_rgba_span(ctx, width, x, y, zSpan, (void *)amesa->imageline, zoomY0);

	    src += rowLength * 2 - width * 2;
	    y++;
	  }
	}
      }

      return GL_TRUE;
    }
    else if (format == GL_COLOR_INDEX && type == GL_UNSIGNED_BYTE) {
      GLubyte *src = (GLubyte *) pixels + skipSize * 1;

      if (ctx->Visual->RGBAflag) {
	/* convert CI data to RGBA */
	if (!isZoom) {
	  /* no zooming */
	  if (!ctx->Pixel.MapItoRsize &&
	      !ctx->Pixel.MapItoGsize &&
	      !ctx->Pixel.MapItoBsize) {
	    /* no masking */
	    GLshort i;
	    GLuint rgba[256];
	    const GLubyte *rMap = ctx->Pixel.MapItoR8;
	    const GLubyte *gMap = ctx->Pixel.MapItoG8;
	    const GLubyte *bMap = ctx->Pixel.MapItoB8;

	    for (i = 256 - 1; i >= 0; i--)
	      rgba[i] = ((((rMap[i] << 8) | gMap[i]) << 8) | bMap[i]) << 8;

	    while (--height >= 0) {
	      i = width;

	      while (--i >= 0)
	        *db++ = rgba[*src++];

	      src += rowLength * 1 - width * 1;
	      db += amesa->FixedWidth - width;
	    }
	  }
	  else {
	    /* with masking */
	    while (--height >= 0) {
	      GLshort i = width;
	      GLuint *il = (GLuint *)amesa->imageline;

	      gl_map_ci8_to_rgba(ctx, width, src, il);				/* masks */
	      while (--i >= 0)
	        *db++ = *il++;

	      src += rowLength;
	      db += amesa->FixedWidth - width;
	    }
	  }

	  return GL_TRUE;
	}
	else {
	  /* with zooming */
	  while (--height >= 0) {
	    gl_map_ci8_to_rgba(ctx, width, src, amesa->imageline);			/* masks */
	    gl_write_zoomed_rgba_span(ctx, width, x, y, zSpan, (void *)amesa->imageline, zoomY0);
	    src += rowLength;
	    y++;
	  }

	  return GL_TRUE;
	}
      }
      else {
	/* write CI data to CI frame buffer */
	if (!isZoom) {
	  GLuint *rgba = amesa->ItoP;

	  while (--height >= 0) {
	    GLshort i = width;

	    while (--i >= 0)
	      *db++ = rgba[*src++];

	    src += rowLength * 1 - width * 1;
	    db += amesa->FixedWidth - width;
	  }

	  return GL_TRUE;
	}
	else {
	  /* with zooming */
	  return GL_FALSE;
	}
      }
    }
    else {
      /* can't handle this pixel format and/or data type here */
      return GL_FALSE;
    }
  }
  else {
    /* can't do direct render, have to use slow path */
    return GL_FALSE;
  }
}
