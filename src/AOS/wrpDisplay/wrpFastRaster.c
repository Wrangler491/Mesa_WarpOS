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

rastersetup_func wrpChooseRasterSetupFunction(GLcontext * ctx)
{
  struct TDdriver *TDdriver;

  DEBUGOUT(1, "wrpChooseRasterSetup()\n");

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;

  if (ctx->Depth.Test) {
    if (ctx->Light.ShadeModel == GL_SMOOTH) {
      if (ctx->Texture.Enabled) {
	if (ctx->Light.Model.TwoSide) {
	  TDdriver->CopyVertex = wrpCopyFlatTexZ;
	  return wrpSetupFlatTexZ;
	}

	TDdriver->CopyVertex = wrpCopyGouraudTexZ;
	return wrpSetupGouraudTexZ;
      }

      if (ctx->Fog.Enabled) {
	if (ctx->Light.Model.TwoSide) {
	  TDdriver->CopyVertex = wrpCopyFlatFogZ;
	  return wrpSetupFlatFogZ;
	}

	TDdriver->CopyVertex = wrpCopyGouraudFogZ;
	return wrpSetupGouraudFogZ;
      }

      if (ctx->Light.Model.TwoSide) {
	TDdriver->CopyVertex = wrpCopyFlatZ;
	return wrpSetupFlatZ;
      }

      TDdriver->CopyVertex = wrpCopyGouraudZ;
      return wrpSetupGouraudZ;
    }

    if (ctx->Texture.Enabled) {
      TDdriver->CopyVertex = wrpCopyFlatTexZ;
      return wrpSetupFlatTexZ;
    }

    if (ctx->Fog.Enabled) {
      TDdriver->CopyVertex = wrpCopyFlatFogZ;
      return wrpSetupFlatFogZ;
    }

    TDdriver->CopyVertex = wrpCopyFlatZ;
    return wrpSetupFlatZ;
  }

  if (ctx->Light.ShadeModel == GL_SMOOTH) {
    if (ctx->Texture.Enabled) {
      if (ctx->Light.Model.TwoSide) {
	TDdriver->CopyVertex = wrpCopyFlatTex;
	return wrpSetupFlatTex;
      }

      TDdriver->CopyVertex = wrpCopyGouraudTex;
      return wrpSetupGouraudTex;
    }

    if (ctx->Fog.Enabled) {
      if (ctx->Light.Model.TwoSide) {
	TDdriver->CopyVertex = wrpCopyFlatFog;
	return wrpSetupFlatFog;
      }

      TDdriver->CopyVertex = wrpCopyGouraudFog;
      return wrpSetupGouraudFog;
    }

    if (ctx->Light.Model.TwoSide) {
      TDdriver->CopyVertex = wrpCopyFlat;
      return wrpSetupFlat;
    }

    TDdriver->CopyVertex = wrpCopyGouraud;
    return wrpSetupGouraud;
  }

  if (ctx->Texture.Enabled) {
    TDdriver->CopyVertex = wrpCopyFlatTex;
    return wrpSetupFlatTex;
  }

  if (ctx->Fog.Enabled) {
    TDdriver->CopyVertex = wrpCopyFlatFog;
    return wrpSetupFlatFog;
  }

  TDdriver->CopyVertex = wrpCopyFlat;
  return wrpSetupFlat;
}
