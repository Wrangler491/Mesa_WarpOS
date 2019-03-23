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

void wrpDrawLineFlat(GLcontext * ctx, GLuint v1, GLuint v2, GLuint pv)
{
  struct TDdriver *TDdriver;
  struct vertex_buffer *VB;
  W3D_Context *TDcontext;
  W3D_Line line;
  W3D_Color TDcol;

  DEBUGOUT(1, "wrpDrawLineFlat()\n");

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;
  VB = ctx->VB;

  LockHardware(TDdriver, TDcontext);

  TC_Color(TDcol, VB->ColorPtr->data[pv][0], VB->ColorPtr->data[pv][1], VB->ColorPtr->data[pv][2], VB->ColorPtr->data[pv][3]);
  W3D_SetCurrentColor(TDcontext, &TDcol);

  if (v2 - 1 == v1) {
    /* optimization, very ugly style */
    W3D_Line *lineptr = (W3D_Line *) (&TDdriver->vbuffer[v1]);
    W3D_Texture *texsave = lineptr->tex;
    W3D_Float widthsave = lineptr->linewidth;
    W3D_Bool patternenable = lineptr->st_enable;
    unsigned short patternsave = lineptr->st_pattern;
    int factorsave = lineptr->st_factor;

    lineptr->tex = TDdriver->TDtex;

    if (!ctx->Line.StippleFlag) {
      lineptr->st_enable = TRUE;
      lineptr->st_pattern = (unsigned short)(ctx->Line.StipplePattern);
      lineptr->st_factor = (int)(ctx->Line.StippleFactor);
    }
    else
      lineptr->st_enable = FALSE;

    lineptr->linewidth = (W3D_Float) ctx->Line.Width;

    if (W3D_DrawLine(TDcontext, lineptr) != W3D_SUCCESS)
      ClrF(TDdriver->flags, TD_ACTIVE);
    SetF(TDdriver->flags, TD_DIRTY);

    lineptr->tex = texsave;
    lineptr->linewidth = widthsave;
    lineptr->st_enable = patternenable;
    lineptr->st_pattern = patternsave;
    lineptr->st_factor = factorsave;
  }
  else {
    line.tex = TDdriver->TDtex;

    if (!ctx->Line.StippleFlag) {
      line.st_enable = TRUE;
      line.st_pattern = (unsigned short)(ctx->Line.StipplePattern);
      line.st_factor = (int)(ctx->Line.StippleFactor);
    }
    else
      line.st_enable = FALSE;

    line.linewidth = (W3D_Float) ctx->Line.Width;

    TDdriver->CopyVertex(&line.v1, &TDdriver->vbuffer[v1]);
    TDdriver->CopyVertex(&line.v2, &TDdriver->vbuffer[v2]);

    if (W3D_DrawLine(TDcontext, &line) != W3D_SUCCESS)
      ClrF(TDdriver->flags, TD_ACTIVE);
    SetF(TDdriver->flags, TD_DIRTY);
  }
}

void wrpDrawLineSmooth(GLcontext * ctx, GLuint v1, GLuint v2, GLuint pv)
{
  struct TDdriver *TDdriver;
  struct vertex_buffer *VB;
  W3D_Context *TDcontext;
  W3D_Line line;

  DEBUGOUT(1, "wrpDrawLineSmooth()\n");

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;
  VB = ctx->VB;

  LockHardware(TDdriver, TDcontext);

  if (v2 - 1 == v1) {
    /* optimization, very ugly style */
    W3D_Line *lineptr = (W3D_Line *) (&TDdriver->vbuffer[v1]);
    W3D_Texture *texsave = lineptr->tex;
    W3D_Float widthsave = lineptr->linewidth;
    W3D_Bool patternenable = lineptr->st_enable;
    unsigned short patternsave = lineptr->st_pattern;
    int factorsave = lineptr->st_factor;

    lineptr->tex = TDdriver->TDtex;

    if (!ctx->Line.StippleFlag) {
      lineptr->st_enable = TRUE;
      lineptr->st_pattern = (unsigned short)(ctx->Line.StipplePattern);
      lineptr->st_factor = (int)(ctx->Line.StippleFactor);
    }
    else
      lineptr->st_enable = FALSE;

    lineptr->linewidth = (W3D_Float) ctx->Line.Width;

    if (W3D_DrawLine(TDcontext, lineptr) != W3D_SUCCESS)
      ClrF(TDdriver->flags, TD_ACTIVE);
    SetF(TDdriver->flags, TD_DIRTY);

    lineptr->tex = texsave;
    lineptr->linewidth = widthsave;
    lineptr->st_enable = patternenable;
    lineptr->st_pattern = patternsave;
    lineptr->st_factor = factorsave;
  }
  else {
    line.tex = TDdriver->TDtex;

    if (!ctx->Line.StippleFlag) {
      line.st_enable = TRUE;
      line.st_pattern = (unsigned short)(ctx->Line.StipplePattern);
      line.st_factor = (int)(ctx->Line.StippleFactor);
    }
    else
      line.st_enable = FALSE;

    line.linewidth = (W3D_Float) ctx->Line.Width;
    TDdriver->CopyVertex(&line.v1, &TDdriver->vbuffer[v1]);
    TDdriver->CopyVertex(&line.v2, &TDdriver->vbuffer[v2]);

    if (W3D_DrawLine(TDcontext, &line) != W3D_SUCCESS)
      ClrF(TDdriver->flags, TD_ACTIVE);
    SetF(TDdriver->flags, TD_DIRTY);
  }
}

void wrpDrawLineSmoothTwo(GLcontext * ctx, GLuint v1, GLuint v2, GLuint pv)
{
  struct TDdriver *TDdriver;
  struct vertex_buffer *VB;
  W3D_Context *TDcontext;
  W3D_Line line;

  DEBUGOUT(1, "wrpDrawLineSmoothTwo()\n");

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;
  VB = ctx->VB;

  LockHardware(TDdriver, TDcontext);

  line.tex = TDdriver->TDtex;

  if (!ctx->Line.StippleFlag) {
    line.st_enable = TRUE;
    line.st_pattern = (unsigned short)(ctx->Line.StipplePattern);
    line.st_factor = (int)(ctx->Line.StippleFactor);
  }
  else
    line.st_enable = FALSE;

  line.linewidth = (W3D_Float) ctx->Line.Width;
  TDdriver->CopyVertex(&line.v1, &TDdriver->vbuffer[v1]);
  TDdriver->CopyVertex(&line.v2, &TDdriver->vbuffer[v2]);

  TC_Color(line.v1.color, VB->ColorPtr->data[v1][0], VB->ColorPtr->data[v1][1], VB->ColorPtr->data[v1][2], VB->ColorPtr->data[v1][3]);
  TC_Color(line.v2.color, VB->ColorPtr->data[v2][0], VB->ColorPtr->data[v2][1], VB->ColorPtr->data[v2][2], VB->ColorPtr->data[v2][3]);

  if (W3D_DrawLine(TDcontext, &line) != W3D_SUCCESS)
    ClrF(TDdriver->flags, TD_ACTIVE);
  SetF(TDdriver->flags, TD_DIRTY);
}

line_func wrpChooseLineFunction(GLcontext * ctx)
{
  struct TDdriver *TDdriver;
  W3D_Context *TDcontext;

  DEBUGOUT(1, "wrpChooseLineFunction()\n");

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;

  if (W3D_Query(TDcontext, W3D_Q_DRAW_LINE, NULL) != W3D_FULLY_SUPPORTED)
    return NULL;

  if (ctx->Line.SmoothFlag) {
    if (W3D_Query(TDcontext, W3D_Q_ANTI_LINE, NULL) != W3D_FULLY_SUPPORTED)
      return NULL;
  }

  if ((ctx->Color.AlphaEnabled) || (ctx->Color.BlendEnabled) ||
      (ctx->Fog.Enabled) || (ctx->Color.IndexLogicOpEnabled) ||
      (ctx->Color.ColorLogicOpEnabled)) {
    if (W3D_Query(TDcontext, W3D_Q_DRAW_LINE_FX, NULL) != W3D_FULLY_SUPPORTED)
      return NULL;
  }

  if (ctx->Texture.Enabled)
    if (W3D_Query(TDcontext, W3D_Q_DRAW_LINE_TEX, NULL) != W3D_FULLY_SUPPORTED)
      return NULL;

  if (ctx->Line.Width != 1.0f)
    if (W3D_Query(TDcontext, W3D_Q_DRAW_LINE_X, NULL) != W3D_FULLY_SUPPORTED)
      return NULL;

  if (ctx->Line.StippleFlag)
    if (W3D_Query(TDcontext, W3D_Q_DRAW_LINE_ST, NULL) != W3D_FULLY_SUPPORTED)
      return NULL;

  if (ctx->Light.ShadeModel == GL_SMOOTH) {
    if (ctx->Light.Model.TwoSide)
      return wrpDrawLineSmoothTwo;

    return wrpDrawLineSmooth;
  }

  return wrpDrawLineFlat;
}

/**********************************************************************/
/*****            Accelerated point, line, polygon rendering      *****/
/**********************************************************************/

/* draw line strips
 * this is never called, when two-sided lighting is enabled
 * this is not called when flat shading is enabled, except
 * when texture mapping is enabled and the env variable NOFLAT
 * is set (which results in wrong behaviour, if the constant
 * color is not always white and GL_DECAL/GL_MODULATE are used)
 */

void wrpDrawLineStrip(GLcontext * ctx, GLuint first, GLuint last)
{
  struct TDdriver *TDdriver;
  W3D_Context *TDcontext;
  W3D_Lines line;

  DEBUGOUT(1, "wrpDrawLineStrip()\n");

  if (last - first < 1)
    return;

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;

  LockHardware(TDdriver, TDcontext);

  line.vertexcount = last - first + 1;
  line.v = (&TDdriver->vbuffer[first]);
  line.tex = TDdriver->TDtex;

  if (!ctx->Line.StippleFlag) {
    line.st_enable = TRUE;
    line.st_pattern = (unsigned short)(ctx->Line.StipplePattern);
    line.st_factor = (int)(ctx->Line.StippleFactor);
  }
  else
    line.st_enable = FALSE;

  if (W3D_DrawLineStrip(TDcontext, &line) != W3D_SUCCESS)
    ClrF(TDdriver->flags, TD_ACTIVE);
  SetF(TDdriver->flags, TD_DIRTY);
}

linestrip_func wrpChooseLineStripFunction(GLcontext * ctx)
{
  struct TDdriver *TDdriver;
  W3D_Context *TDcontext;

  DEBUGOUT(1, "wrpChooseLineStripFunction()\n");

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;

  if (W3D_Query(TDcontext, W3D_Q_DRAW_LINE, NULL) != W3D_FULLY_SUPPORTED)
    return NULL;

  if (ctx->Line.SmoothFlag)
    if (W3D_Query(TDcontext, W3D_Q_ANTI_LINE, NULL) != W3D_FULLY_SUPPORTED)
      return NULL;

  if ((ctx->Color.AlphaEnabled) || (ctx->Color.BlendEnabled) ||
      (ctx->Fog.Enabled) || (ctx->Color.IndexLogicOpEnabled) ||
      (ctx->Color.ColorLogicOpEnabled))
    if (W3D_Query(TDcontext, W3D_Q_DRAW_LINE_FX, NULL) != W3D_FULLY_SUPPORTED)
      return NULL;

  if (ctx->Texture.Enabled)
    if (W3D_Query(TDcontext, W3D_Q_DRAW_LINE_TEX, NULL) != W3D_FULLY_SUPPORTED)
      return NULL;

  if (ctx->Line.Width != 1.0f)
    if (W3D_Query(TDcontext, W3D_Q_DRAW_LINE_X, NULL) != W3D_FULLY_SUPPORTED)
      return NULL;

  if (ctx->Line.StippleFlag)
    if (W3D_Query(TDcontext, W3D_Q_DRAW_LINE_ST, NULL) != W3D_FULLY_SUPPORTED)
      return NULL;

  if (ctx->Light.Model.TwoSide)
    return NULL;

  return wrpDrawLineStrip;
}
