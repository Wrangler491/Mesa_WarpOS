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

void wrpDrawPointFlat(GLcontext * ctx, GLuint first, GLuint last)
{
  struct TDdriver *TDdriver;
  struct vertex_buffer *VB = ctx->VB;
  W3D_Context *TDcontext;
  W3D_Point po;
  GLint i;

  DEBUGOUT(1, "wrpDrawPointFlat()\n");

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;

  LockHardware(TDdriver, TDcontext);

  po.tex = TDdriver->TDtex;
  po.pointsize = (W3D_Float) ctx->Point.Size;

  for (i = first; i <= last; i++) {
    W3D_Color TDcol;

    TC_Color(TDcol, VB->ColorPtr->data[first][0], VB->ColorPtr->data[first][1], VB->ColorPtr->data[first][2], VB->ColorPtr->data[first][3]);
    W3D_SetCurrentColor(TDcontext, &TDcol);

    TDdriver->CopyVertex(&po.v1, &TDdriver->vbuffer[i]);

    if (W3D_DrawPoint(TDcontext, &po) != W3D_SUCCESS)
      ClrF(TDdriver->flags, TD_ACTIVE);
  }

  SetF(TDdriver->flags, TD_DIRTY);
}

void wrpDrawPointSmooth(GLcontext * ctx, GLuint first, GLuint last)
{
  struct TDdriver *TDdriver;
  W3D_Context *TDcontext;
  W3D_Point po;
  GLint i;

  DEBUGOUT(1, "wrpDrawPointSmooth()\n");

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;

  LockHardware(TDdriver, TDcontext);

  po.tex = TDdriver->TDtex;
  po.pointsize = (W3D_Float) ctx->Point.Size;

  for (i = first; i <= last; i++) {
    TDdriver->CopyVertex(&po.v1, &TDdriver->vbuffer[i]);

    if (W3D_DrawPoint(TDcontext, &po) != W3D_SUCCESS)
      ClrF(TDdriver->flags, TD_ACTIVE);
  }

  SetF(TDdriver->flags, TD_DIRTY);
}

void wrpDrawPointSmoothTwo(GLcontext * ctx, GLuint first, GLuint last)
{
  struct TDdriver *TDdriver;
  struct vertex_buffer *VB = ctx->VB;
  W3D_Context *TDcontext;
  W3D_Point po;
  GLint i;

  DEBUGOUT(1, "wrpDrawPointSmoothTwo()\n");

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;

  LockHardware(TDdriver, TDcontext);

  po.tex = TDdriver->TDtex;
  po.pointsize = (W3D_Float) ctx->Point.Size;

  for (i = first; i <= last; i++) {
    TDdriver->CopyVertex(&po.v1, &TDdriver->vbuffer[i]);

    TC_Color(po.v1.color, VB->ColorPtr->data[i][0], VB->ColorPtr->data[i][1], VB->ColorPtr->data[i][2], VB->ColorPtr->data[i][3]);

    if (W3D_DrawPoint(TDcontext, &po) != W3D_SUCCESS)
      ClrF(TDdriver->flags, TD_ACTIVE);
  }

  SetF(TDdriver->flags, TD_DIRTY);
}

points_func wrpChoosePointsFunction(GLcontext * ctx)
{
  struct TDdriver *TDdriver;
  W3D_Context *TDcontext;

  DEBUGOUT(1, "wrpChoosePointsFunction()\n");

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;

  if (W3D_Query(TDcontext, W3D_Q_DRAW_POINT, NULL) != W3D_FULLY_SUPPORTED)
    return NULL;

  if (ctx->Point.SmoothFlag)
    if (W3D_Query(TDcontext, W3D_Q_ANTI_POINT, NULL) != W3D_FULLY_SUPPORTED)
      return NULL;

  if ((ctx->Color.AlphaEnabled) || (ctx->Color.BlendEnabled) ||
      (ctx->Fog.Enabled) || (ctx->Color.IndexLogicOpEnabled) ||
      (ctx->Color.ColorLogicOpEnabled))
    if (W3D_Query(TDcontext, W3D_Q_DRAW_POINT_FX, NULL) != W3D_FULLY_SUPPORTED)
      return NULL;

  if (ctx->Texture.Enabled)
    if (W3D_Query(TDcontext, W3D_Q_DRAW_POINT_TEX, NULL) != W3D_FULLY_SUPPORTED)
      return NULL;

  if (!(ctx->Point.Params[0] == 1.0 && ctx->Point.Params[1] == 0.0 &&
	ctx->Point.Params[2] == 0.0))
    return NULL;

  if (ctx->Point.Size != 1.0f)
    if (W3D_Query(TDcontext, W3D_Q_DRAW_POINT_X, NULL) != W3D_FULLY_SUPPORTED)
      return NULL;

  if (ctx->Light.ShadeModel == GL_SMOOTH) {
    if (ctx->Light.Model.TwoSide)
      return wrpDrawPointSmoothTwo;

    return wrpDrawPointSmooth;
  }

  return wrpDrawPointFlat;
}
