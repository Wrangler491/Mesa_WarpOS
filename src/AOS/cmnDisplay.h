#ifndef	AMIGAMESAcmn_H
#define	AMIGAMESAcmn_H

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

GLboolean cmnStandardInit(amigaMesaContext amesa, struct TagItem *tagList);
GLboolean cmnStandardInitDB(amigaMesaContext amesa, struct TagItem *tagList);
void cmnStandardResize(GLcontext *ctx, GLuint *width, GLuint *height);
void cmnStandardResizeDB(GLcontext *ctx, GLuint *width, GLuint *height);
void cmnStandardSwapBuffer(amigaMesaContext amesa);
void cmnStandardSwapBufferDB(amigaMesaContext amesa);
void cmnStandardDispose(amigaMesaContext amesa);
void cmnStandardDisposeDB(amigaMesaContext amesa);

GLboolean cmnSetBuffer(GLcontext *ctx, GLenum mode);
#define	cmnSetBufferDB	cmnSetBuffer
void cmnFinish(void);
#define	cmnFinishDB	cmnFinish
void cmnRenderStart(GLcontext *ctx);
#define	cmnRenderStartDB	NULL
void cmnRenderFinish(GLcontext *ctx);
#define	cmnRenderFinishDB	NULL
void cmnDither(GLcontext *ctx, GLboolean enable);
#define	cmnDitherDB	cmnDither
void cmnFlush(GLcontext *ctx);
#define	cmnFlushDB	cmnFlush

#endif
