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

void wrpSetupFog(GLcontext * ctx)
{
  struct TDdriver *TDdriver;
  W3D_Context *TDcontext;
  W3D_Fog fog;
  W3D_Float wscale;
  GLuint fogmode;

  DEBUGOUT(1, "wrpDepthTestSpan()\n");

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;
  wscale = TDdriver->wscale;

  if (ctx->Fog.Mode == GL_LINEAR) {
    fog.fog_start = (1 / ctx->Fog.Start) * wscale;
    fog.fog_end = (1 / ctx->Fog.End) * wscale;
  }
  else {
    fog.fog_start = 1.0;
    fog.fog_end = (1 / TDdriver->far) * wscale;
  }

  fog.fog_density = ctx->Fog.Density / wscale;

  if (ctx->Visual->RGBAflag) {
    fog.fog_color.r = ctx->Fog.Color[0];
    fog.fog_color.g = ctx->Fog.Color[1];
    fog.fog_color.b = ctx->Fog.Color[2];
  }
  else {
    GLuint index = ctx->Fog.Index;

    fog.fog_color.r = (W3D_Float) (TDdriver->Palette[index].r / 255.0);
    fog.fog_color.g = (W3D_Float) (TDdriver->Palette[index].g / 255.0);
    fog.fog_color.b = (W3D_Float) (TDdriver->Palette[index].b / 255.0);
  }

  switch (ctx->Fog.Mode) {
    case GL_LINEAR:
      fogmode = W3D_FOG_LINEAR;
      if (W3D_Query(TDcontext, W3D_Q_LINEAR, NULL) == W3D_NOT_SUPPORTED)
	fogmode = W3D_FOG_INTERPOLATED;
      break;
    case GL_EXP:
      fogmode = W3D_FOG_EXP;
      break;
    case GL_EXP2:
      fogmode = W3D_FOG_EXP_2;
      break;
    default:
      return;
  }

  W3D_SetFogParams(TDcontext, &fog, fogmode);
}
