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
/*****                   Optimized line rendering                 *****/
/**********************************************************************/

/* Draw a flat-shaded, RGB line into an osmesa buffer. */
void natDFlatRGBALineDB(GLcontext * ctx, GLuint vert0, GLuint vert1, GLuint pvert)
{
  GLubyte *db;

  db = dbPenGet(((amigaMesaContext) ctx->DriverCtx));

#define INTERP_XY	1
#define CLIP_HACK	1
#define PLOT(X,Y)	*dbPen(db, X, Y) = PLD_RGBA(((amigaMesaContext) ctx->DriverCtx),\
						    ctx->VB->ColorPtr->data[pvert][0],		\
						    ctx->VB->ColorPtr->data[pvert][1],		\
						    ctx->VB->ColorPtr->data[pvert][2], X, Y)

#include "../../linetemp.h"
}

/* Draw a flat-shaded, Z-less, RGB line into an osmesa buffer. */
void natDFlatRGBAZLineDB(GLcontext * ctx, GLuint vert0, GLuint vert1, GLuint pvert)
{
  GLubyte *db;

  db = dbPenGet(((amigaMesaContext) ctx->DriverCtx));

#define INTERP_XY	1
#define INTERP_Z	1
#define CLIP_HACK	1
#define PLOT(X,Y)									\
  if (Z < *zPtr) {									\
    *dbPen(db, X, Y) = PLD_RGBA(((amigaMesaContext) ctx->DriverCtx),			\
						    ctx->VB->ColorPtr->data[pvert][0],		\
						    ctx->VB->ColorPtr->data[pvert][1],		\
						    ctx->VB->ColorPtr->data[pvert][2], X, Y)	\
    *zPtr = Z;										\
  }

#include "../../linetemp.h"
}

/* Draw a flat-shaded, alpha-blended, RGB line into an osmesa buffer. */
void natDFlatBlendRGBALineDB(GLcontext * ctx, GLuint vert0, GLuint vert1, GLuint pvert)
{
  amigaMesaContext amesa;
  GLubyte *db;
  GLuint *PtoI;
  struct vertex_buffer *VB;
  GLint avalue, msavalue, rvalue, gvalue, bvalue;

  amesa = ((amigaMesaContext) ctx->DriverCtx);
  db = dbPenGet(amesa);
  PtoI = amesa->PtoI;
  VB = ctx->VB;
  avalue = VB->ColorPtr->data[pvert][3];
  msavalue = 255 - avalue;
  rvalue = VB->ColorPtr->data[pvert][0] * avalue;
  gvalue = VB->ColorPtr->data[pvert][1] * avalue;
  bvalue = VB->ColorPtr->data[pvert][2] * avalue;

#define INTERP_XY	1
#define CLIP_HACK	1
#define PLOT(X,Y) {							\
    GLubyte *ptr4 = dbPen(db, X, Y);					\
    GLubyte *ptr3 = &PtoI[*ptr4];					\
    *ptr4 = PLD_RGBA(amesa, ((ptr3[0] * msavalue + rvalue) >> 8),	\
			    ((ptr3[1] * msavalue + gvalue) >> 8),	\
			    ((ptr3[2] * msavalue + bvalue) >> 8), X, Y);\
  }

#include "../../linetemp.h"
}

/* Draw a flat-shaded, Z-less, alpha-blended, RGB line into an osmesa buffer. */
void natDFlatBlendRGBAZLineDB(GLcontext * ctx, GLuint vert0, GLuint vert1, GLuint pvert)
{
  amigaMesaContext amesa;
  GLubyte *db;
  GLuint *PtoI;
  struct vertex_buffer *VB;
  GLint avalue, msavalue, rvalue, gvalue, bvalue;

  amesa = ((amigaMesaContext) ctx->DriverCtx);
  db = dbPenGet(amesa);
  PtoI = amesa->PtoI;
  VB = ctx->VB;
  avalue = VB->ColorPtr->data[pvert][3];
  msavalue = 255 - avalue;
  rvalue = VB->ColorPtr->data[pvert][0] * avalue;
  gvalue = VB->ColorPtr->data[pvert][1] * avalue;
  bvalue = VB->ColorPtr->data[pvert][2] * avalue;

#define INTERP_XY	1
#define INTERP_Z	1
#define CLIP_HACK	1
#define PLOT(X,Y)							\
  if (Z < *zPtr) {							\
    GLubyte *ptr4 = dbPen(db, X, Y); 					\
    GLubyte *ptr3 = &PtoI[*ptr4];					\
    *ptr4 = PLD_RGBA(amesa, ((ptr3[0] * msavalue + rvalue) >> 8),	\
			    ((ptr3[1] * msavalue + gvalue) >> 8),	\
			    ((ptr3[2] * msavalue + bvalue) >> 8), X, Y);\
  }

#include "../../linetemp.h"
}

/* Draw a flat-shaded, Z-less, alpha-blended, RGB line into an osmesa buffer. */
void natDFlatBlendRGBAZLineWriteDB(GLcontext * ctx, GLuint vert0, GLuint vert1, GLuint pvert)
{
  amigaMesaContext amesa;
  GLubyte *db;
  GLuint *PtoI;
  struct vertex_buffer *VB;
  GLint avalue, msavalue, rvalue, gvalue, bvalue;

  amesa = ((amigaMesaContext) ctx->DriverCtx);
  db = dbPenGet(amesa);
  PtoI = amesa->PtoI;
  VB = ctx->VB;
  avalue = VB->ColorPtr->data[pvert][3];
  msavalue = 255 - avalue;
  rvalue = VB->ColorPtr->data[pvert][0] * avalue;
  gvalue = VB->ColorPtr->data[pvert][1] * avalue;
  bvalue = VB->ColorPtr->data[pvert][2] * avalue;

#define INTERP_XY	1
#define INTERP_Z	1
#define CLIP_HACK	1
#define PLOT(X,Y)							\
  if (Z < *zPtr) {							\
    GLubyte *ptr4 = dbPen(db, X, Y); 					\
    GLubyte *ptr3 = &PtoI[*ptr4];					\
    *ptr4 = PLD_RGBA(amesa, ((ptr3[0] * msavalue + rvalue) >> 8),	\
			    ((ptr3[1] * msavalue + gvalue) >> 8),	\
			    ((ptr3[2] * msavalue + bvalue) >> 8), X, Y);\
    *zPtr = Z;								\
  }

#include "../../linetemp.h"
}
