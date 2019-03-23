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

void wrpSetupFlat(GLcontext * ctx, GLuint start, GLuint end)
{
  struct TDdriver *TDdriver;
  W3D_Float *win;
  W3D_Float swap;
  W3D_Vertex *vptr;
  GLint i;

  /* raster setup function
   * flat shading, no texture mapping, no fogging, no ZBuffering
   */

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  win = (W3D_Float *) ctx->VB->Win.data[start];
  swap = TDdriver->height;
  vptr = &TDdriver->vbuffer[start];

  for (i = start; i < end; i++, vptr++) {
    vptr->x = (W3D_Float) (       *win++);
    vptr->y = (W3D_Float) (swap - *win++);
    win++;
  }
}

void wrpSetupFlatZ(GLcontext * ctx, GLuint start, GLuint end)
{
  struct TDdriver *TDdriver;
  W3D_Vertex *vptr;
  W3D_Float *win;
  W3D_Float swap;
  const W3D_Float invdepth = 1.0 / DEPTH_SCALE;
  GLint i;

  /* raster setup function
   * flat shading, no texture mapping, no fogging, ZBuffering
   */

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  win = (W3D_Float *) ctx->VB->Win.data[start];
  swap = TDdriver->height;
  vptr = &TDdriver->vbuffer[start];

  for (i = start; i < end; i++, vptr++) {
    vptr->x = (W3D_Float) (           *win++);
    vptr->y = (W3D_Float) (swap     - *win++);
    vptr->z = (W3D_Float) (invdepth * *win++);
  }
}

void wrpSetupGouraud(GLcontext * ctx, GLuint start, GLuint end)
{
  struct TDdriver *TDdriver;
  W3D_Vertex *vptr;
  W3D_Float *win;
  W3D_Float swap;
  GLubyte *col;
  GLint i;

  /* raster setup function
   * gouraud shading, no texture mapping, no fogging, no ZBuffering
   */

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  win = (W3D_Float *) ctx->VB->Win.data[start];
  swap = TDdriver->height;
  vptr = &TDdriver->vbuffer[start];
  col = ctx->VB->ColorPtr->data[start];

  for (i = start; i < end; i++, vptr++) {
    vptr->x = (W3D_Float) (       *win++);
    vptr->y = (W3D_Float) (swap - *win++);
    TC_Color(vptr->color, *col++, *col++, *col++, *col++);
    win++;
  }
}

void wrpSetupGouraudZ(GLcontext * ctx, GLuint start, GLuint end)
{
  struct TDdriver *TDdriver;
  W3D_Vertex *vptr;
  W3D_Float *win;
  W3D_Float swap;
  const W3D_Float invdepth = 1.0 / DEPTH_SCALE;
  GLubyte *col;
  GLint i;

  /* raster setup function
   * gouraud shading, no texture mapping, no fogging, ZBuffering
   */

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  win = (W3D_Float *) ctx->VB->Win.data[start];
  swap = TDdriver->height;
  vptr = &TDdriver->vbuffer[start];
  col = ctx->VB->ColorPtr->data[start];

  for (i = start; i < end; i++, vptr++) {
    vptr->x = (W3D_Float) (           *win++);
    vptr->y = (W3D_Float) (swap     - *win++);
    vptr->z = (W3D_Float) (invdepth * *win++);
    TC_Color(vptr->color, *col++, *col++, *col++, *col++);
  }
}

void wrpSetupFlatFog(GLcontext * ctx, GLuint start, GLuint end)
{
  struct TDdriver *TDdriver;
  W3D_Vertex *vptr;
  W3D_Float *win;
  W3D_Float swap;
  W3D_Float *clip;
  W3D_Float wscale;
  GLint i;

  /* raster setup function
   * flat shading, no texture mapping, fogging, no ZBuffering
   */

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  win = (W3D_Float *) ctx->VB->Win.data[start];
  swap = TDdriver->height;
  vptr = &TDdriver->vbuffer[start];
  clip = ctx->VB->ClipPtr->data[start];
  wscale = TDdriver->wscale;

  for (i = start; i < end; i++, vptr++, clip += 4) {
    vptr->x = (W3D_Float) (       *win++);
    vptr->y = (W3D_Float) (swap - *win++);
    vptr->w = (W3D_Float) (wscale / clip[3]);
    win++;
  }
}

void wrpSetupFlatFogZ(GLcontext * ctx, GLuint start, GLuint end)
{
  struct TDdriver *TDdriver;
  W3D_Vertex *vptr;
  W3D_Float *win;
  W3D_Float swap;
  const W3D_Float invdepth = 1.0 / DEPTH_SCALE;
  W3D_Float *clip;
  W3D_Float wscale;
  GLint i;

  /* raster setup function
   * flat shading, no texture mapping, fogging, ZBuffering
   */

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  win = (W3D_Float *) ctx->VB->Win.data[start];
  swap = TDdriver->height;
  vptr = &TDdriver->vbuffer[start];
  clip = ctx->VB->ClipPtr->data[start];
  wscale = TDdriver->wscale;

  for (i = start; i < end; i++, vptr++, clip += 4) {
    vptr->x = (W3D_Float) (           *win++);
    vptr->y = (W3D_Float) (swap     - *win++);
    vptr->z = (W3D_Float) (invdepth * *win++);
    vptr->w = (W3D_Float) (wscale / clip[3]);
  }
}

void wrpSetupGouraudFog(GLcontext * ctx, GLuint start, GLuint end)
{
  struct TDdriver *TDdriver;
  W3D_Vertex *vptr;
  W3D_Float *win;
  W3D_Float swap;
  W3D_Float *clip;
  W3D_Float wscale;
  GLubyte *col;
  GLint i;

  /* raster setup function
   * gouraud shading, no texture mapping, fogging, no ZBuffering
   */

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  win = (W3D_Float *) ctx->VB->Win.data[start];
  swap = TDdriver->height;
  vptr = &TDdriver->vbuffer[start];
  clip = ctx->VB->ClipPtr->data[start];
  wscale = TDdriver->wscale;
  col = ctx->VB->ColorPtr->data[start];

  for (i = start; i < end; i++, vptr++, clip += 4) {
    vptr->x = (W3D_Float) *win++;
    vptr->y = (W3D_Float) (swap - *win++);
    vptr->w = (W3D_Float) (wscale / clip[3]);
    TC_Color(vptr->color, *col++, *col++, *col++, *col++);
    win++;
  }
}

void wrpSetupGouraudFogZ(GLcontext * ctx, GLuint start, GLuint end)
{
  struct TDdriver *TDdriver;
  W3D_Vertex *vptr;
  W3D_Float *win;
  W3D_Float swap;
  const W3D_Float invdepth = 1.0 / DEPTH_SCALE;
  W3D_Float *clip;
  W3D_Float wscale;
  GLubyte *col;
  GLint i;

  /* raster setup function
   * gouraud shading, no texture mapping, fogging, ZBuffering
   */

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  win = (W3D_Float *) ctx->VB->Win.data[start];
  swap = TDdriver->height;
  vptr = &TDdriver->vbuffer[start];
  clip = ctx->VB->ClipPtr->data[start];
  wscale = TDdriver->wscale;
  col = ctx->VB->ColorPtr->data[start];

  for (i = start; i < end; i++, vptr++, clip += 4) {
    vptr->x = (W3D_Float) *win++;
    vptr->y = (W3D_Float) (swap - *win++);
    vptr->z = (W3D_Float) (*win++ * invdepth);
    vptr->w = (W3D_Float) (wscale / clip[3]);
    TC_Color(vptr->color, *col++, *col++, *col++, *col++);
  }
}

void wrpSetupFlatTex(GLcontext * ctx, GLuint start, GLuint end)
{
  struct TDdriver *TDdriver;
  W3D_Vertex *vptr;
  W3D_Float *win;
  W3D_Float swap;
  W3D_Float *texcoord;
  W3D_Float *clip;
  W3D_Float wscale;
  const W3D_Float one = 1.0;
  W3D_Float texwidth;
  W3D_Float texheight;
  GLint i;

  /* raster setup function
   * flat shading, texture mapping, no ZBuffering
   */

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  win = (W3D_Float *) ctx->VB->Win.data[start];
  swap = TDdriver->height;
  vptr = &TDdriver->vbuffer[start];
  clip = ctx->VB->ClipPtr->data[start];
  wscale = TDdriver->wscale;
  texwidth = TDdriver->texwidth;
  texheight = TDdriver->texheight;
  texcoord = ctx->VB->TexCoordPtr[0]->data[start];

  for (i = start; i < end; i++, vptr++, texcoord += 4, clip += 4) {
    W3D_Float tc3, invq;

    vptr->x = (W3D_Float) *win++;
    vptr->y = (W3D_Float) (swap - *win++);
    tc3 = texcoord[3];

    if (tc3 == one) {
      vptr->u = (W3D_Float) (texwidth * texcoord[0]);
      vptr->v = (W3D_Float) (texheight * texcoord[1]);
      vptr->w = (W3D_Float) (wscale / clip[3]);
    }
    else {
      invq = one / tc3;
      vptr->u = (W3D_Float) (texwidth * texcoord[0] * invq);
      vptr->v = (W3D_Float) (texheight * texcoord[1] * invq);
      vptr->w = (W3D_Float) (tc3 * wscale / clip[3]);
    }

    win++;
  }
}

void wrpSetupFlatTexZ(GLcontext * ctx, GLuint start, GLuint end)
{
  struct TDdriver *TDdriver;
  W3D_Vertex *vptr;
  W3D_Float *win;
  W3D_Float swap;
  const W3D_Float invdepth = 1.0 / DEPTH_SCALE;
  W3D_Float *texcoord;
  W3D_Float *clip;
  W3D_Float wscale;
  const W3D_Float one = 1.0;
  W3D_Float texwidth;
  W3D_Float texheight;
  GLint i;

  /* raster setup function
   * flat shading, texture mapping, ZBuffering
   */

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  win = (W3D_Float *) ctx->VB->Win.data[start];
  swap = TDdriver->height;
  vptr = &TDdriver->vbuffer[start];
  clip = ctx->VB->ClipPtr->data[start];
  wscale = TDdriver->wscale;
  texwidth = TDdriver->texwidth;
  texheight = TDdriver->texheight;
  texcoord = ctx->VB->TexCoordPtr[0]->data[start];

  for (i = start; i < end; i++, vptr++, texcoord += 4, clip += 4) {
    W3D_Float tc3, invq;

    vptr->x = (W3D_Float) *win++;
    vptr->y = (W3D_Float) (swap - *win++);
    vptr->z = (W3D_Float) (*win++ * invdepth);
    tc3 = texcoord[3];

    if (tc3 == one) {
      vptr->u = (W3D_Float) (texwidth * texcoord[0]);
      vptr->v = (W3D_Float) (texheight * texcoord[1]);
      vptr->w = (W3D_Float) (wscale / clip[3]);
    }
    else {
      invq = one / tc3;
      vptr->u = (W3D_Float) (texwidth * texcoord[0] * invq);
      vptr->v = (W3D_Float) (texheight * texcoord[1] * invq);
      vptr->w = (W3D_Float) (tc3 * wscale / clip[3]);
    }
  }
}

void wrpSetupGouraudTex(GLcontext * ctx, GLuint start, GLuint end)
{
  struct TDdriver *TDdriver;
  W3D_Vertex *vptr;
  W3D_Float *win;
  W3D_Float swap;
  W3D_Float *texcoord;
  W3D_Float *clip;
  W3D_Float wscale;
  const W3D_Float one = 1.0;
  W3D_Float texwidth;
  W3D_Float texheight;
  GLubyte *col;
  GLint i;

  /* raster setup function
   * gouraud shading, texture mapping, no ZBuffering
   */

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  win = (W3D_Float *) ctx->VB->Win.data[start];
  swap = TDdriver->height;
  vptr = &TDdriver->vbuffer[start];
  clip = ctx->VB->ClipPtr->data[start];
  wscale = TDdriver->wscale;
  texwidth = TDdriver->texwidth;
  texheight = TDdriver->texheight;
  texcoord = ctx->VB->TexCoordPtr[0]->data[start];
  col = ctx->VB->ColorPtr->data[start];

  for (i = start; i < end; i++, vptr++, texcoord += 4, clip += 4) {
    W3D_Float tc3, invq;

    vptr->x = (W3D_Float) *win++;
    vptr->y = (W3D_Float) (swap - *win++);
    tc3 = texcoord[3];

    if (tc3 == one) {
      vptr->u = (W3D_Float) (texwidth * texcoord[0]);
      vptr->v = (W3D_Float) (texheight * texcoord[1]);
      vptr->w = (W3D_Float) (wscale / clip[3]);
    }
    else {
      invq = one / tc3;
      vptr->u = (W3D_Float) (texwidth * texcoord[0] * invq);
      vptr->v = (W3D_Float) (texheight * texcoord[1] * invq);
      vptr->w = (W3D_Float) (tc3 * wscale / clip[3]);
    }

    TC_Color(vptr->color, *col++, *col++, *col++, *col++);
    win++;
  }
}

void wrpSetupGouraudTexZ(GLcontext * ctx, GLuint start, GLuint end)
{
  struct TDdriver *TDdriver;
  W3D_Vertex *vptr;
  W3D_Float *win;
  W3D_Float swap;
  const W3D_Float invdepth = 1.0 / DEPTH_SCALE;
  W3D_Float *texcoord;
  W3D_Float *clip;
  W3D_Float wscale;
  const W3D_Float one = 1.0;
  W3D_Float texwidth;
  W3D_Float texheight;
  GLubyte *col;
  GLint i;

  /* raster setup function
   * gouraud shading, texture mapping, ZBuffering
   */

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  win = (W3D_Float *) ctx->VB->Win.data[start];
  swap = TDdriver->height;
  vptr = &TDdriver->vbuffer[start];
  clip = ctx->VB->ClipPtr->data[start];
  wscale = TDdriver->wscale;
  texwidth = TDdriver->texwidth;
  texheight = TDdriver->texheight;
  texcoord = ctx->VB->TexCoordPtr[0]->data[start];
  col = ctx->VB->ColorPtr->data[start];

  for (i = start; i < end; i++, vptr++, texcoord += 4, clip += 4) {
    W3D_Float tc3, invq;

    vptr->x = (W3D_Float) *win++;
    vptr->y = (W3D_Float) (swap - *win++);
    vptr->z = (W3D_Float) (*win++ * invdepth);
    tc3 = texcoord[3];

    if (tc3 == one) {
      vptr->u = (W3D_Float) (texwidth * texcoord[0]);
      vptr->v = (W3D_Float) (texheight * texcoord[1]);
      vptr->w = (W3D_Float) (wscale / clip[3]);
    }
    else {
      invq = one / tc3;
      vptr->u = (W3D_Float) (texwidth * texcoord[0] * invq);
      vptr->v = (W3D_Float) (texheight * texcoord[1] * invq);
      vptr->w = (W3D_Float) (tc3 * wscale / clip[3]);
    }

    TC_Color(vptr->color, *col++, *col++, *col++, *col++);
  }
}
