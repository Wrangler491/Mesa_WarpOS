/* $Id: fog.h,v 1.1.1.1.2.1 1999/11/25 16:51:24 keithw Exp $ */

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



#ifndef FOG_H
#define FOG_H


#include "types.h"


extern void gl_Fogfv( GLcontext *ctx, GLenum pname, const GLfloat *params );


extern void gl_fog_vertices( struct vertex_buffer *VB );

extern void gl_fog_rgba_pixels( const GLcontext *ctx,
                                GLuint n, const GLdepth z[],
                                GLubyte rgba[][4] );

extern void gl_fog_ci_pixels( const GLcontext *ctx,
                              GLuint n, const GLdepth z[], GLuint indx[] );

extern void gl_make_fog_coords( struct vertex_buffer *VB );

extern struct gl_pipeline_stage gl_fog_coord_stage;

extern void gl_init_fog( void );


#endif
