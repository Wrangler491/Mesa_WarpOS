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
/*****            Accelerated point, line, polygon rendering      *****/
/**********************************************************************/

void wrpDrawTriangleFlat(GLcontext * ctx, GLuint v1, GLuint v2,
			  GLuint v3, GLuint pv)
{
  struct TDdriver *TDdriver;
  struct vertex_buffer *VB;
  W3D_Context *TDcontext;
  W3D_Triangle tri;
  W3D_Color TDcol;

  DEBUGOUT(1, "wrpDrawTriangleFlat()\n");

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;
  VB = ctx->VB;

  LockHardware(TDdriver, TDcontext);

  TC_Color(TDcol, VB->ColorPtr->data[pv][0], VB->ColorPtr->data[pv][1], VB->ColorPtr->data[pv][2], VB->ColorPtr->data[pv][3]);
  W3D_SetCurrentColor(TDcontext, &TDcol);

  if (((v2 - 1 == v1) && (v3 - 2 == v1)) || ((v2 - 1 == v3) && (v1 - 2 == v3))) {
    /* optimization, very ugly style */
    W3D_Triangle *triptr = (W3D_Triangle *) (&TDdriver->vbuffer[v2 - 1]);
    W3D_Texture *texsave = triptr->tex;
    unsigned char *patternsave = triptr->st_pattern;

    triptr->tex = TDdriver->TDtex;

    if (!ctx->Polygon.StippleFlag)
      triptr->st_pattern = (unsigned char *)ctx->PolygonStipple;
    else
      triptr->st_pattern = NULL;

    if (W3D_DrawTriangle(TDcontext, triptr) != W3D_SUCCESS)
      ClrF(TDdriver->flags, TD_ACTIVE);
    SetF(TDdriver->flags, TD_DIRTY);

    triptr->tex = texsave;
    triptr->st_pattern = patternsave;
  }
  else {
    tri.tex = TDdriver->TDtex;

    if (!ctx->Polygon.StippleFlag)
      tri.st_pattern = (unsigned char *)ctx->PolygonStipple;
    else
      tri.st_pattern = NULL;

    TDdriver->CopyVertex(&tri.v1, &TDdriver->vbuffer[v1]);
    TDdriver->CopyVertex(&tri.v2, &TDdriver->vbuffer[v2]);
    TDdriver->CopyVertex(&tri.v3, &TDdriver->vbuffer[v3]);

    if (W3D_DrawTriangle(TDcontext, &tri) != W3D_SUCCESS)
      ClrF(TDdriver->flags, TD_ACTIVE);
    SetF(TDdriver->flags, TD_DIRTY);
  }
}

void wrpDrawTriangleSmooth(GLcontext * ctx, GLuint v1, GLuint v2,
			   GLuint v3, GLuint pv)
{
  struct TDdriver *TDdriver;
  W3D_Context *TDcontext;
  W3D_Triangle tri;

  DEBUGOUT(1, "wrpDrawTriangleFlat()\n");

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;

  LockHardware(TDdriver, TDcontext);

  if (((v2 - 1 == v1) && (v3 - 2 == v1)) || ((v2 - 1 == v3) && (v1 - 2 == v3))) {
    /* optimization, very ugly style */
    W3D_Triangle *triptr = (W3D_Triangle *) (&TDdriver->vbuffer[v2 - 1]);
    W3D_Texture *texsave = triptr->tex;
    unsigned char *patternsave = triptr->st_pattern;

    triptr->tex = TDdriver->TDtex;

    if (!ctx->Polygon.StippleFlag)
      triptr->st_pattern = (unsigned char *)ctx->PolygonStipple;
    else
      triptr->st_pattern = NULL;

    if (W3D_DrawTriangle(TDcontext, triptr) != W3D_SUCCESS)
      ClrF(TDdriver->flags, TD_ACTIVE);
    SetF(TDdriver->flags, TD_DIRTY);

    triptr->tex = texsave;
    triptr->st_pattern = patternsave;
  }
  else {
    tri.tex = TDdriver->TDtex;

    if (!ctx->Polygon.StippleFlag)
      tri.st_pattern = (unsigned char *)ctx->PolygonStipple;
    else
      tri.st_pattern = NULL;

    TDdriver->CopyVertex(&tri.v1, &TDdriver->vbuffer[v1]);
    TDdriver->CopyVertex(&tri.v2, &TDdriver->vbuffer[v2]);
    TDdriver->CopyVertex(&tri.v3, &TDdriver->vbuffer[v3]);

    if (W3D_DrawTriangle(TDcontext, &tri) != W3D_SUCCESS)
      ClrF(TDdriver->flags, TD_ACTIVE);
    SetF(TDdriver->flags, TD_DIRTY);
  }
}

void wrpDrawTriangleSmoothTwo(GLcontext * ctx, GLuint v1, GLuint v2,
			      GLuint v3, GLuint pv)
{
  struct TDdriver *TDdriver;
  struct vertex_buffer *VB = ctx->VB;
  W3D_Context *TDcontext;
  W3D_Triangle tri;

  DEBUGOUT(1, "wrpDrawTriangleFlat()\n");

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;

  LockHardware(TDdriver, TDcontext);

  if (((v2 - 1 == v1) && (v3 - 2 == v1)) || ((v2 - 1 == v3) && (v1 - 2 == v3))) {
    /* optimization, very ugly style */
    W3D_Triangle *triptr = (W3D_Triangle *) (&TDdriver->vbuffer[v2 - 1]);
    W3D_Texture *texsave = triptr->tex;
    unsigned char *patternsave = triptr->st_pattern;

    triptr->tex = TDdriver->TDtex;

    if (!ctx->Polygon.StippleFlag)
      triptr->st_pattern = (unsigned char *)ctx->PolygonStipple;
    else
      triptr->st_pattern = NULL;

    TC_Color(TDdriver->vbuffer[v1].color, VB->ColorPtr->data[v1][0], VB->ColorPtr->data[v1][1], VB->ColorPtr->data[v1][2], VB->ColorPtr->data[v1][3]);
    TC_Color(TDdriver->vbuffer[v2].color, VB->ColorPtr->data[v2][0], VB->ColorPtr->data[v2][1], VB->ColorPtr->data[v2][2], VB->ColorPtr->data[v2][3]);
    TC_Color(TDdriver->vbuffer[v3].color, VB->ColorPtr->data[v3][0], VB->ColorPtr->data[v3][1], VB->ColorPtr->data[v3][2], VB->ColorPtr->data[v3][3]);

    if (W3D_DrawTriangle(TDcontext, triptr) != W3D_SUCCESS)
      ClrF(TDdriver->flags, TD_ACTIVE);
    SetF(TDdriver->flags, TD_DIRTY);

    triptr->tex = texsave;
    triptr->st_pattern = patternsave;
  }
  else {
    tri.tex = TDdriver->TDtex;

    if (!ctx->Polygon.StippleFlag)
      tri.st_pattern = (unsigned char *)ctx->PolygonStipple;
    else
      tri.st_pattern = NULL;

    TDdriver->CopyVertex(&tri.v1, &TDdriver->vbuffer[v1]);
    TDdriver->CopyVertex(&tri.v2, &TDdriver->vbuffer[v2]);
    TDdriver->CopyVertex(&tri.v3, &TDdriver->vbuffer[v3]);

    TC_Color(tri.v1.color, VB->ColorPtr->data[v1][0], VB->ColorPtr->data[v1][1], VB->ColorPtr->data[v1][2], VB->ColorPtr->data[v1][3]);
    TC_Color(tri.v2.color, VB->ColorPtr->data[v2][0], VB->ColorPtr->data[v2][1], VB->ColorPtr->data[v2][2], VB->ColorPtr->data[v2][3]);
    TC_Color(tri.v3.color, VB->ColorPtr->data[v3][0], VB->ColorPtr->data[v3][1], VB->ColorPtr->data[v3][2], VB->ColorPtr->data[v3][3]);

    if (W3D_DrawTriangle(TDcontext, &tri) != W3D_SUCCESS)
      ClrF(TDdriver->flags, TD_ACTIVE);
    SetF(TDdriver->flags, TD_DIRTY);
  }
}

triangle_func wrpChooseTriangleFunction(GLcontext * ctx)
{
  struct TDdriver *TDdriver;
  W3D_Context *TDcontext;

  DEBUGOUT(1, "wrpChooseTriangle()\n");

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;

  if (W3D_Query(TDcontext, W3D_Q_DRAW_TRIANGLE, NULL) != W3D_FULLY_SUPPORTED)
    return NULL;

#if 0
  if ((ctx->Polygon.OffsetAny) || (ctx->RasterMask & FRONT_AND_BACK_BIT))
    return NULL;
#endif

  if (ctx->Polygon.SmoothFlag)
    if (W3D_Query(TDcontext,W3D_Q_ANTI_POLYGON,NULL) != W3D_FULLY_SUPPORTED)
      return NULL;

  if (ctx->Polygon.StippleFlag)
    if (W3D_Query(TDcontext, W3D_Q_DRAW_POLY_ST, NULL) != W3D_FULLY_SUPPORTED)
      return NULL;

  if (ctx->Light.ShadeModel == GL_SMOOTH) {
    if (ctx->Light.Model.TwoSide)
      return wrpDrawTriangleSmoothTwo;

    return wrpDrawTriangleSmooth;
  }

  return wrpDrawTriangleFlat;
}

/**********************************************************************/
/*****            Accelerated point, line, polygon rendering      *****/
/**********************************************************************/

void wrpDrawQuadFlat(GLcontext * ctx, GLuint v1, GLuint v2,
		      GLuint v3, GLuint v4, GLuint pv)
{
  struct TDdriver *TDdriver;
  struct vertex_buffer *VB = ctx->VB;
  W3D_Context *TDcontext;
  W3D_Triangles tri;
  W3D_Vertex w3dv[4];
  W3D_Color TDcol;

  DEBUGOUT(1, "wrpDrawQuadFlat()\n");

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;

  LockHardware(TDdriver, TDcontext);

  TC_Color(TDcol, VB->ColorPtr->data[pv][0], VB->ColorPtr->data[pv][1], VB->ColorPtr->data[pv][2], VB->ColorPtr->data[pv][3]);
  W3D_SetCurrentColor(TDcontext, &TDcol);

  if ((v2 - 1 == v1) && (v3 - 3 == v1) && (v4 - 2 == v1)) {
    tri.vertexcount = 4;
    tri.v = (&TDdriver->vbuffer[v1]);
    tri.tex = TDdriver->TDtex;

    if (!ctx->Polygon.StippleFlag)
      tri.st_pattern = (unsigned char *)ctx->PolygonStipple;
    else
      tri.st_pattern = NULL;

    if (W3D_DrawTriStrip(TDcontext, &tri) != W3D_SUCCESS)
      ClrF(TDdriver->flags, TD_ACTIVE);
    SetF(TDdriver->flags, TD_DIRTY);
  }
  else if ((v2 - 1 == v1) && (v3 - 2 == v1) && (v4 - 3 == v1)) {
    tri.vertexcount = 4;
    tri.v = (&TDdriver->vbuffer[v1]);
    tri.tex = TDdriver->TDtex;

    if (!ctx->Polygon.StippleFlag)
      tri.st_pattern = (unsigned char *)ctx->PolygonStipple;
    else
      tri.st_pattern = NULL;

    if (W3D_DrawTriFan(TDcontext, &tri) != W3D_SUCCESS)
      ClrF(TDdriver->flags, TD_ACTIVE);
    SetF(TDdriver->flags, TD_DIRTY);
  }
  else {
    tri.vertexcount = 4;
    tri.v = w3dv;
    tri.tex = TDdriver->TDtex;

    if (!ctx->Polygon.StippleFlag)
      tri.st_pattern = (unsigned char *)ctx->PolygonStipple;
    else
      tri.st_pattern = NULL;

    TDdriver->CopyVertex(&w3dv[0], &TDdriver->vbuffer[v1]);
    TDdriver->CopyVertex(&w3dv[1], &TDdriver->vbuffer[v2]);
    TDdriver->CopyVertex(&w3dv[2], &TDdriver->vbuffer[v4]);
    TDdriver->CopyVertex(&w3dv[3], &TDdriver->vbuffer[v3]);

    if (W3D_DrawTriStrip(TDcontext, &tri) != W3D_SUCCESS)
      ClrF(TDdriver->flags, TD_ACTIVE);
    SetF(TDdriver->flags, TD_DIRTY);
  }
}

void wrpDrawQuadSmooth(GLcontext * ctx, GLuint v1, GLuint v2,
			GLuint v3, GLuint v4, GLuint pv)
{
  struct TDdriver *TDdriver;
  W3D_Context *TDcontext;
  W3D_Triangles tri;
  W3D_Vertex w3dv[4];

  DEBUGOUT(1, "wrpDrawQuadSmooth()\n");

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;

  LockHardware(TDdriver, TDcontext);

  if ((v2 - 1 == v1) && (v3 - 3 == v1) && (v4 - 2 == v1)) {
    tri.vertexcount = 4;
    tri.v = (&TDdriver->vbuffer[v1]);
    tri.tex = TDdriver->TDtex;

    if (!ctx->Polygon.StippleFlag)
      tri.st_pattern = (unsigned char *)ctx->PolygonStipple;
    else
      tri.st_pattern = NULL;

    if (W3D_DrawTriStrip(TDcontext, &tri) != W3D_SUCCESS)
      ClrF(TDdriver->flags, TD_ACTIVE);
    SetF(TDdriver->flags, TD_DIRTY);
  }
  else if ((v2 - 1 == v1) && (v3 - 2 == v1) && (v4 - 3 == v1)) {
    tri.vertexcount = 4;
    tri.v = (&TDdriver->vbuffer[v1]);
    tri.tex = TDdriver->TDtex;

    if (!ctx->Polygon.StippleFlag)
      tri.st_pattern = (unsigned char *)ctx->PolygonStipple;
    else
      tri.st_pattern = NULL;

    if (W3D_DrawTriFan(TDcontext, &tri) != W3D_SUCCESS)
      ClrF(TDdriver->flags, TD_ACTIVE);
    SetF(TDdriver->flags, TD_DIRTY);
  }
  else {
    tri.vertexcount = 4;
    tri.v = w3dv;
    tri.tex = TDdriver->TDtex;

    if (!ctx->Polygon.StippleFlag)
      tri.st_pattern = (unsigned char *)ctx->PolygonStipple;
    else
      tri.st_pattern = NULL;

    TDdriver->CopyVertex(&w3dv[0], &TDdriver->vbuffer[v1]);
    TDdriver->CopyVertex(&w3dv[1], &TDdriver->vbuffer[v2]);
    TDdriver->CopyVertex(&w3dv[2], &TDdriver->vbuffer[v4]);
    TDdriver->CopyVertex(&w3dv[3], &TDdriver->vbuffer[v3]);

    if (W3D_DrawTriStrip(TDcontext, &tri) != W3D_SUCCESS)
      ClrF(TDdriver->flags, TD_ACTIVE);
    SetF(TDdriver->flags, TD_DIRTY);
  }
}

void wrpDrawQuadSmoothTwo(GLcontext * ctx, GLuint v1, GLuint v2,
			   GLuint v3, GLuint v4, GLuint pv)
{
  struct TDdriver *TDdriver;
  struct vertex_buffer *VB = ctx->VB;
  W3D_Context *TDcontext;
  W3D_Triangles tri;
  W3D_Vertex w3dv[4];

  DEBUGOUT(1, "wrpDrawQuadSmoothTwo()\n");

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;

  LockHardware(TDdriver, TDcontext);

  if ((v2 - 1 == v1) && (v3 - 3 == v1) && (v4 - 2 == v1)) {
    tri.vertexcount = 4;
    tri.v = (&TDdriver->vbuffer[v1]);
    tri.tex = TDdriver->TDtex;

    if (!ctx->Polygon.StippleFlag)
      tri.st_pattern = (unsigned char *)ctx->PolygonStipple;
    else
      tri.st_pattern = NULL;

    TC_Color(TDdriver->vbuffer[v1].color, VB->ColorPtr->data[v1][0], VB->ColorPtr->data[v1][1], VB->ColorPtr->data[v1][2], VB->ColorPtr->data[v1][3]);
    TC_Color(TDdriver->vbuffer[v2].color, VB->ColorPtr->data[v2][0], VB->ColorPtr->data[v2][1], VB->ColorPtr->data[v2][2], VB->ColorPtr->data[v2][3]);
    TC_Color(TDdriver->vbuffer[v3].color, VB->ColorPtr->data[v3][0], VB->ColorPtr->data[v3][1], VB->ColorPtr->data[v3][2], VB->ColorPtr->data[v3][3]);
    TC_Color(TDdriver->vbuffer[v4].color, VB->ColorPtr->data[v4][0], VB->ColorPtr->data[v4][1], VB->ColorPtr->data[v4][2], VB->ColorPtr->data[v4][3]);

    if (W3D_DrawTriStrip(TDcontext, &tri) != W3D_SUCCESS)
      ClrF(TDdriver->flags, TD_ACTIVE);
    SetF(TDdriver->flags, TD_DIRTY);
  }
  else if ((v2 - 1 == v1) && (v3 - 2 == v1) && (v4 - 3 == v1)) {
    tri.vertexcount = 4;
    tri.v = (&TDdriver->vbuffer[v1]);
    tri.tex = TDdriver->TDtex;

    if (!ctx->Polygon.StippleFlag)
      tri.st_pattern = (unsigned char *)ctx->PolygonStipple;
    else
      tri.st_pattern = NULL;

    TC_Color(TDdriver->vbuffer[v1].color, VB->ColorPtr->data[v1][0], VB->ColorPtr->data[v1][1], VB->ColorPtr->data[v1][2], VB->ColorPtr->data[v1][3]);
    TC_Color(TDdriver->vbuffer[v2].color, VB->ColorPtr->data[v2][0], VB->ColorPtr->data[v2][1], VB->ColorPtr->data[v2][2], VB->ColorPtr->data[v2][3]);
    TC_Color(TDdriver->vbuffer[v3].color, VB->ColorPtr->data[v3][0], VB->ColorPtr->data[v3][1], VB->ColorPtr->data[v3][2], VB->ColorPtr->data[v3][3]);
    TC_Color(TDdriver->vbuffer[v4].color, VB->ColorPtr->data[v4][0], VB->ColorPtr->data[v4][1], VB->ColorPtr->data[v4][2], VB->ColorPtr->data[v4][3]);

    if (W3D_DrawTriFan(TDcontext, &tri) != W3D_SUCCESS)
      ClrF(TDdriver->flags, TD_ACTIVE);
    SetF(TDdriver->flags, TD_DIRTY);
  }
  else {
    tri.vertexcount = 4;
    tri.v = w3dv;
    tri.tex = TDdriver->TDtex;

    if (!ctx->Polygon.StippleFlag)
      tri.st_pattern = (unsigned char *)ctx->PolygonStipple;
    else
      tri.st_pattern = NULL;

    TDdriver->CopyVertex(&w3dv[0], &TDdriver->vbuffer[v1]);
    TDdriver->CopyVertex(&w3dv[1], &TDdriver->vbuffer[v2]);
    TDdriver->CopyVertex(&w3dv[2], &TDdriver->vbuffer[v4]);
    TDdriver->CopyVertex(&w3dv[3], &TDdriver->vbuffer[v3]);

    TC_Color(w3dv[0].color, VB->ColorPtr->data[v1][0], VB->ColorPtr->data[v1][1], VB->ColorPtr->data[v1][2], VB->ColorPtr->data[v1][3]);
    TC_Color(w3dv[1].color, VB->ColorPtr->data[v2][0], VB->ColorPtr->data[v2][1], VB->ColorPtr->data[v2][2], VB->ColorPtr->data[v2][3]);
    TC_Color(w3dv[2].color, VB->ColorPtr->data[v3][0], VB->ColorPtr->data[v3][1], VB->ColorPtr->data[v3][2], VB->ColorPtr->data[v3][3]);
    TC_Color(w3dv[3].color, VB->ColorPtr->data[v4][0], VB->ColorPtr->data[v4][1], VB->ColorPtr->data[v4][2], VB->ColorPtr->data[v4][3]);

    if (W3D_DrawTriStrip(TDcontext, &tri) != W3D_SUCCESS)
      ClrF(TDdriver->flags, TD_ACTIVE);
    SetF(TDdriver->flags, TD_DIRTY);
  }
}

quad_func wrpChooseQuadFunction(GLcontext * ctx)
{
  struct TDdriver *TDdriver;
  W3D_Context *TDcontext;

  DEBUGOUT(1, "wrpChooseQuad()\n");

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;

  if (W3D_Query(TDcontext, W3D_Q_DRAW_TRIANGLE, NULL) != W3D_FULLY_SUPPORTED)
    return NULL;

#if 0
  if ((ctx->Polygon.OffsetAny) || (ctx->RasterMask & FRONT_AND_BACK_BIT))
    return NULL;
#endif

  if (ctx->Polygon.SmoothFlag) {
    if (W3D_Query(TDcontext,W3D_Q_ANTI_POLYGON,NULL) != W3D_FULLY_SUPPORTED)
      return NULL;
  }

  if (ctx->Polygon.StippleFlag) {
    if (W3D_Query(TDcontext, W3D_Q_DRAW_POLY_ST, NULL) != W3D_FULLY_SUPPORTED)
      return NULL;
  }

  if (ctx->Light.ShadeModel == GL_SMOOTH) {
    if (ctx->Light.Model.TwoSide)
      return wrpDrawQuadSmoothTwo;

    return wrpDrawQuadSmooth;
  }

  return wrpDrawQuadFlat;
}

/**********************************************************************/
/*****            Accelerated point, line, polygon rendering      *****/
/**********************************************************************/

/* draw triangle strips
 * this is never called, when two-sided lighting is enabled
 * this is not called when flat shading is enabled, except
 * when texture mapping is enabled and the env variable NOFLAT
 * is set (which results in wrong behaviour, if the constant
 * color is not always white and GL_DECAL/GL_MODULATE are used)
 */

void wrpDrawTriStrip(GLcontext * ctx, GLuint first, GLuint last)
{
  struct TDdriver *TDdriver;
  W3D_Context *TDcontext;
  W3D_Triangles tri;

  if (last - first < 2)
    return;

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;

  LockHardware(TDdriver, TDcontext);

  tri.vertexcount = last - first + 1;
  tri.v = (&TDdriver->vbuffer[first]);
  tri.tex = TDdriver->TDtex;

  if (!ctx->Polygon.StippleFlag)
    tri.st_pattern = (unsigned char *)ctx->PolygonStipple;
  else
    tri.st_pattern = NULL;

  if (W3D_DrawTriStrip(TDcontext, &tri) != W3D_SUCCESS)
    ClrF(TDdriver->flags, TD_ACTIVE);
  SetF(TDdriver->flags, TD_DIRTY);
}

tristrip_func wrpChooseTriStripFunction(GLcontext * ctx)
{
  struct TDdriver *TDdriver;
  W3D_Context *TDcontext;

  DEBUGOUT(1, "wrpChooseTriStrip()\n");

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;

  if (W3D_Query(TDcontext, W3D_Q_DRAW_TRIANGLE, NULL) != W3D_FULLY_SUPPORTED)
    return NULL;

#if 0
  if (ctx->Polygon.OffsetAny || (ctx->RasterMask & FRONT_AND_BACK_BIT))
    return NULL;
#endif

  if (ctx->Polygon.SmoothFlag) {
    if (W3D_Query(TDcontext, W3D_Q_ANTI_POLYGON,NULL) != W3D_FULLY_SUPPORTED)
      return NULL;
  }

  if (ctx->Polygon.StippleFlag) {
    if (W3D_Query(TDcontext, W3D_Q_DRAW_POLY_ST, NULL) != W3D_FULLY_SUPPORTED)
      return NULL;
  }

  if (ctx->Light.Model.TwoSide)
    return NULL;

  return wrpDrawTriStrip;
}

/**********************************************************************/
/*****            Accelerated point, line, polygon rendering      *****/
/**********************************************************************/

/* draw triangle fans
 * this is never called, when two-sided lighting is enabled
 * this is not called when flat shading is enabled, except
 * when texture mapping is enabled and the env variable NOFLAT
 * is set (which results in wrong behaviour, if the constant
 * color is not always white and GL_DECAL/GL_MODULATE are used)
 */

void wrpDrawTriFan(GLcontext * ctx, GLuint zero, GLuint first, GLuint last)
{
  struct TDdriver *TDdriver;
  W3D_Context *TDcontext;
  W3D_Triangles tri;
  W3D_Vertex vtemp;

  if (last - first < 1)
    return;

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;

  LockHardware(TDdriver, TDcontext);

  if (first - zero != 1) {
    TDdriver->CopyVertex(&vtemp, &TDdriver->vbuffer[first - 1]);
    TDdriver->CopyVertex(&TDdriver->vbuffer[first - 1], &TDdriver->vbuffer[zero]);
  }

  tri.vertexcount = last - first + 2;
  tri.v = (&TDdriver->vbuffer[first - 1]);
  tri.tex = TDdriver->TDtex;

  if (!ctx->Polygon.StippleFlag)
    tri.st_pattern = (unsigned char *)ctx->PolygonStipple;
  else
    tri.st_pattern = NULL;

  if (W3D_DrawTriFan(TDcontext, &tri) != W3D_SUCCESS)
    ClrF(TDdriver->flags, TD_ACTIVE);
  SetF(TDdriver->flags, TD_DIRTY);

  if (first - zero != 1)
    TDdriver->CopyVertex(&TDdriver->vbuffer[first - 1], &vtemp);
}

trifan_func wrpChooseTriFanFunction(GLcontext * ctx)
{
  struct TDdriver *TDdriver;
  W3D_Context *TDcontext;

  DEBUGOUT(1, "wrpChooseTriFan()\n");

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;

  if (W3D_Query(TDcontext, W3D_Q_DRAW_TRIANGLE, NULL) != W3D_FULLY_SUPPORTED)
    return NULL;

#if 0
  if (ctx->Polygon.OffsetAny || (ctx->RasterMask & FRONT_AND_BACK_BIT))
    return NULL;
#endif

  if (ctx->Polygon.SmoothFlag) {
    if (W3D_Query(TDcontext, W3D_Q_ANTI_POLYGON,NULL) != W3D_FULLY_SUPPORTED)
      return NULL;
  }

  if (ctx->Polygon.StippleFlag) {
    if (W3D_Query(TDcontext, W3D_Q_DRAW_POLY_ST, NULL) != W3D_FULLY_SUPPORTED)
      return NULL;
  }

  if (ctx->Light.Model.TwoSide)
    return NULL;

  return wrpDrawTriFan;
}
