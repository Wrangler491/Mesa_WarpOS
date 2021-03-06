/* $Id: all.h,v 1.2 1999/10/08 09:27:09 keithw Exp $ */

/*
 * Mesa 3-D graphics library
 * Version:  3.1
 * 
 * Copyright (C) 1999  Brian Paul   All Rights Reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */





/* The purpose of this file is to collect all the header files that Mesa
 * uses into a single header so that we can get new compilers that support
 * pre-compiled headers to compile much faster.
 * All we do is list all the internal headers used by Mesa in this one
 * main header file, and most compilers will pre-compile all these headers
 * and use them over and over again for each source module. This makes a
 * big difference for Win32 support, because the <windows.h> headers take
 * a *long* time to compile.
 */


#ifndef SRC_ALL_H
#define SRC_ALL_H


#ifndef PC_HEADER
  This is an error.  all.h should be included only if PC_HEADER is defined.
#endif

#ifdef HAVE_CONFIG_H
#include "conf.h"
#endif

#ifndef XFree86LOADER
#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>
#endif
#include "GL/gl.h"
#ifdef XFree86Server
#include "GL/osmesa.h"
#endif
#include "accum.h"
#include "alpha.h"
#include "alphabuf.h"
#include "api.h"
#include "asm-386.h"
#include "attrib.h"
#include "bitmap.h"
#include "blend.h"
#include "clip.h"
#include "colortab.h"
#include "context.h"
#include "config.h"
#include "copypix.h"
#include "dd.h"
#include "depth.h"
#include "dlist.h"
#include "drawpix.h"
#include "enable.h"
#include "eval.h"
#include "feedback.h"
#include "fixed.h"
#include "fog.h"
#include "get.h"
#include "hash.h"
#include "image.h"
#include "light.h"
#include "lines.h"
#include "logic.h"
#include "macros.h"
#include "masking.h"
#include "matrix.h"
#include "glmisc.h"
#include "mmath.h"
#include "pb.h"
#include "pixel.h"
#include "pointers.h"
#include "points.h"
#include "polygon.h"
#include "quads.h"
#include "rastpos.h"
#include "readpix.h"
#include "rect.h"
#include "scissor.h"
#include "shade.h"
#include "span.h"
#include "stencil.h"
#include "teximage.h"
#include "texobj.h"
#include "texstate.h"
#include "texture.h"
#include "triangle.h"
#include "types.h"
#include "varray.h"
#include "vb.h"
#include "vbfill.h"
#include "vbrender.h"
#include "vbxform.h"
#include "winpos.h"
#include "xform.h"
#include "zoom.h"
#ifdef XFree86Server
#include "GL/xf86glx.h"
#endif

#endif /*SRC_ALL_H*/
