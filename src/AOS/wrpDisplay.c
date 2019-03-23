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

/*
 * $Log:  $
 */

#undef	NO_CONTEXT_AVAILABLE
#include <AOS/amigamesa.h>
#include "AOS/accessArray.h"
#include "AOS/cybDisplay/intern.h"

#ifdef	AOS_WARP3D
#define	LockHardware(td, td_ctx)	\
    if (!TstF(td->flags, TD_LOCKED)) {	\
      W3D_LockHardware(td_ctx);		\
      SetF(td->flags, TD_LOCKED);	\
    }

#define	UnLockHardware(td, td_ctx)	\
    if (TstF(td->flags, TD_LOCKED)) {	\
      W3D_UnLockHardware(td_ctx);	\
      ClrF(td->flags, TD_LOCKED);	\
    }

#define	FlushHardware(td, td_ctx)	\
  if (TstF(td->flags, TD_DIRTY)) {	\
    W3D_Flush(td_ctx);			\
    ClrF(td->flags, TD_DIRTY);		\
  }

/**********************************************************************/
/*****                Miscellaneous device driver funcs           *****/
/**********************************************************************/

const char *wrpRendererString(void)
{
  return "Warp3D CyberGraphX standard\n";
}

const char *wrpRendererStringDB(void)
{
  return "Warp3D CyberGraphX fast\n";
}

GLbitfield wrpClearTD(GLcontext * ctx, GLbitfield mask,
		      GLboolean all, GLint x, GLint y, GLint width, GLint height)
{
  struct TDdriver *TDdriver;
  W3D_Context *TDcontext;

  DEBUGOUT(1, "wrpClearTD()\n");

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;

  if (TstF(TDdriver->flags, TD_ACTIVE) &&
      TstF(TDdriver->flags, TD_ZBUFFER | TD_SBUFFER)) {
    LockHardware(TDdriver, TDcontext);

    if (TstF(TDdriver->flags, TD_ZBUFFER) &&
	TstF(mask, GL_DEPTH_BUFFER_BIT)) {
      if (!((ctx->Visual->DepthBits) || !ctx->Depth.Mask)) {
	W3D_Double clearvalue = (W3D_Double) ctx->Depth.Clear;

	W3D_ClearZBuffer(TDcontext, &clearvalue);
	SetF(TDdriver->flags, TD_DIRTY);
	ClrF(mask, GL_DEPTH_BUFFER_BIT);
      }
    }

    if (TstF(TDdriver->flags, TD_SBUFFER) &&
	TstF(mask, GL_STENCIL_BUFFER_BIT)) {
      if (!(ctx->Visual->StencilBits == 0)) {
        GLuint sclr = ctx->Stencil.Clear;

	W3D_ClearStencilBuffer(TDcontext, (ULONG *)&sclr);
	SetF(TDdriver->flags, TD_DIRTY);
	ClrF(mask, GL_STENCIL_BUFFER_BIT);
      }
    }
  }
  else
    W3D_Flush(TDcontext);

  return mask;
}

GLbitfield wrpClear(GLcontext * ctx, GLbitfield mask, GLboolean all,
		    GLint x, GLint y, GLint width, GLint height)
{
  mask = cybClear(ctx, mask, all, x, y, width, height);
  return wrpClearTD(ctx, mask, all, x, y, width, height);
}

void wrpSetIndex(GLcontext * ctx, GLuint index)
{
  amigaMesaContext amesa;

  /*
   * Set the amesa color index. 
   */

  amesa = (amigaMesaContext) ctx->DriverCtx;

  DEBUGOUT(1, "wrpSetIndex(%d)\n", index);

  cybSetIndex(ctx, index);
  W3D_SetCurrentColor(amesa->TDdriver->td_ctx, &amesa->TDdriver->Palette[amesa->pixel & 0xFF]);
  W3D_SetCurrentPen(amesa->TDdriver->td_ctx, amesa->pixel & 0xFF);
}

void wrpSetColor(GLcontext * ctx, GLubyte r, GLubyte g, GLubyte b, GLubyte a)
{
  amigaMesaContext amesa;
  W3D_Color TDcol;

  /*
   * Set the current RGBA color. 
   * r is in 0..255.RedScale 
   * g is in 0..255.GreenScale 
   * b is in 0..255.BlueScale 
   * a is in 0..255.AlphaScale 
   */

  amesa = (amigaMesaContext) ctx->DriverCtx;

  DEBUGOUT(1, "wrpSetColor(%d, %d, %d, %d)\n", r, g, b, a);

  cybSetColor(ctx, r, g, b, a);
  TC_Color(TDcol, r, g, b, a);
  W3D_SetCurrentColor(amesa->TDdriver->td_ctx, &TDcol);
  W3D_SetCurrentPen(amesa->TDdriver->td_ctx, amesa->pixel & 0x000000FF);
}

/*
 * OPTIONAL FUNCTION: 
 * Implements glLogicOp if possible.  Return GL_TRUE if the device driver
 * can perform the operation, otherwise return GL_FALSE.  If GL_FALSE
 * is returned, the logic op will be done in software by Mesa.
 */
GLboolean wrpLogicOp(GLcontext * ctx, GLenum op)
{
  switch (op) {
    case GL_CLEAR:
    case GL_SET:
    case GL_COPY:
    case GL_COPY_INVERTED:
    case GL_NOOP:
    case GL_INVERT:
    case GL_AND:
    case GL_NAND:
    case GL_OR:
    case GL_NOR:
    case GL_XOR:
    case GL_EQUIV:
    case GL_AND_REVERSE:
    case GL_AND_INVERTED:
    case GL_OR_REVERSE:
    case GL_OR_INVERTED:
    default:
      break;
  }

  return GL_TRUE;
}

#include "wrpDisplay/wrpFastLines.c"
#include "wrpDisplay/wrpFastPixels.c"
#include "wrpDisplay/wrpFastPoints.c"
#include "wrpDisplay/wrpFastTriangles.c"

#include "wrpDisplay/wrpSetup.c"
#include "wrpDisplay/wrpVertex.c"
#include "wrpDisplay/wrpFastRaster.c"

#include "wrpDisplay/wrpDepth.c"
#include "wrpDisplay/wrpFog.c"
#include "wrpDisplay/wrpTexture.c"

GLint wrpGetParameteri(const GLcontext * ctx, GLint param)
{
#if 0
  switch (param) {
    case DD_MAX_TEXTURE_SIZE:
      return MAX_TEXTURE_SIZE;
    case DD_MAX_TEXTURES:
      return MAX_TEX_SETS;
    case DD_MAX_TEXTURE_COORD_SETS:
      return MAX_TEX_COORD_SETS;
    case DD_HAVE_HARDWARE_FOG:
      return 1;
  }
#endif
  return 0;
}

void wrpNearFar(GLcontext * ctx, GLfloat nearVal, GLfloat farVal)
{
  struct TDdriver *TDdriver;

  DEBUGOUT(1, "wrpNearFar()\n");

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;

  TDdriver->near = nearVal;
  TDdriver->far = farVal;
  if (nearVal < 1)
    TDdriver->wscale = 1.0;
  else
    TDdriver->wscale = nearVal;

  wrpSetupFog(ctx);
}

/**********************************************************************/

void wrpStandardSwapBuffer(amigaMesaContext amesa)
{
  struct TDdriver *TDdriver = amesa->TDdriver;
  W3D_Context *TDcontext = TDdriver->td_ctx;

  DEBUGOUT(1, "wrpStandardSwapBuffer(0x%08x)\n", amesa);

  switch (W3D_Flush(TDcontext)) {
    case W3D_NOGFXMEM:	SetF(TDdriver->flags, TD_NOVMEM);	break;
    case W3D_NOTVISIBLE:ClrF(TDdriver->flags, TD_ACTIVE);	break;
    default:							break;
  }	

  W3D_FlushFrame(TDcontext);
  UnLockHardware(TDdriver, TDcontext);

  if (!TstF(TDdriver->flags, TD_ACTIVE) &&
      (W3D_GetDriverState(TDdriver->td_ctx) == W3D_SUCCESS)) {
    SetF(TDdriver->flags, TD_ACTIVE);
    SetF(amesa->gl_ctx->NewState, NEW_DRVSTATE0);
  }

  (*amesa->SwapBuffer) (amesa);
  W3D_SetDrawRegion(TDcontext, amesa->rp->BitMap, 0, &TDdriver->scissor);
}

void wrpStandardDispose(amigaMesaContext amesa)
{
  struct TDdriver *TDdriver;

  DEBUGOUT(1, "wrpStandardDispose(0x%08x)\n", amesa);

  if ((TDdriver = amesa->TDdriver)) {
    struct gl_texture_object *TDtex = amesa->gl_ctx->Shared->TexObjectList;

    W3D_Flush(TDdriver->td_ctx);
    while (TDtex) {
      wrpDeleteTexture(amesa->gl_ctx, TDtex);
      TDtex = TDtex->Next;
    }

    if (TstF(TDdriver->flags, TD_ZBUFFER))
      W3D_FreeZBuffer(TDdriver->td_ctx);

    W3D_DestroyContext(TDdriver->td_ctx);
    FreeVecPooled(amesaPool, (ULONG *)TDdriver);
    amesa->TDdriver = NULL;
  }
}

/**********************************************************************/
/*****                  amiga/Mesa Private Functions              *****/
/**********************************************************************/

GLboolean wrpContextSupported(GLcontext * ctx)
{
  amigaMesaContext amesa;
  struct TDdriver *TDdriver;
  W3D_Context *TDcontext;

  DEBUGOUT(1, "wrpContextSupported()\n");

  amesa = (amigaMesaContext) ctx->DriverCtx;
  TDdriver = amesa->TDdriver;
  TDcontext = TDdriver->td_ctx;

  if (!(ctx->NewState & (~NEW_LIGHTING)))
    return GL_TRUE;

  /* TODO: remove, as soon as flat shading works correctly */
  if (amesa->depth <= 8)
    return GL_FALSE;

  /* color index mode currently not supported (would need some
   * adaptions to the color conversion tables)
   */
  if (!TstF(amesa->visual->flags, VISUAL_RGBMODE))
    return GL_FALSE;

  /* depth buffer allocation */
  if (ctx->Depth.Test)
    return GL_FALSE;

  /* general check: HW ready? */
  if (!TstF(TDdriver->flags, TD_ACTIVE))
    return GL_FALSE;

  /* TODO: implement general check, as soon as available */
#if 0
  if (W3D_Query(TDcontext, W3D_Q_DRAW_TRIANGLE, NULL) == W3D_NOTHING_SUPPORTED)
    return GL_FALSE;
#endif
  if (ctx->Color.DrawBuffer == GL_NONE)
    return GL_FALSE;

  /* texture mapping check */
  /* TODO: support 3d textures */
  if (ctx->Texture.Enabled && ctx->Texture.Enabled != TEXTURE0_2D)
    return GL_FALSE;

  if (ctx->Texture.Enabled) {
    struct gl_texture_object *curTex = ctx->Texture.Unit[0].Current;
    GLenum envmode = ctx->Texture.Unit[0].EnvMode;

    if (TstF(TDdriver->flags, TD_NOVMEM))
      return GL_FALSE;

    if (!curTex->Image[0])
      return GL_FALSE;

    if (W3D_Query(TDcontext, W3D_Q_TEXMAPPING, NULL) != W3D_FULLY_SUPPORTED)
      return GL_FALSE;

    if (ctx->Hint.PerspectiveCorrection == GL_FASTEST) {
      if (((curTex->WrapS == GL_REPEAT) || (curTex->WrapT == GL_REPEAT)) &&
	(W3D_Query(TDcontext, W3D_Q_LINEAR_REPEAT, NULL) != W3D_FULLY_SUPPORTED))
	return GL_FALSE;

      if (!TstF(TDdriver->flags, TD_NOCLAMP)) {
	if (((curTex->WrapS != GL_REPEAT) ||
	     (curTex->WrapT != GL_REPEAT)) &&
	    (W3D_Query(TDcontext, W3D_Q_LINEAR_CLAMP, NULL) != W3D_FULLY_SUPPORTED))
	  return GL_FALSE;
      }

      if ((curTex->WrapS != curTex->WrapT) &&
	  (W3D_Query(TDcontext, W3D_Q_WRAP_ASYM, NULL) != W3D_FULLY_SUPPORTED))
	return GL_FALSE;

      if (curTex->Image[0]->Width > W3D_Query(TDcontext, W3D_Q_MAXTEXWIDTH, NULL))
	return GL_FALSE;

      if (curTex->Image[0]->Height > W3D_Query(TDcontext, W3D_Q_MAXTEXHEIGHT, NULL))
	return GL_FALSE;
    }
    else {
      if (W3D_Query(TDcontext, W3D_Q_PERSPECTIVE, NULL) != W3D_FULLY_SUPPORTED)
	return GL_FALSE;

      if (((curTex->WrapS == GL_REPEAT) || (curTex->WrapT == GL_REPEAT)) &&
	  (W3D_Query(TDcontext, W3D_Q_PERSP_REPEAT, NULL) != W3D_FULLY_SUPPORTED))
	return GL_FALSE;

      if (!TstF(TDdriver->flags, TD_NOCLAMP)) {
	if (((curTex->WrapS != GL_REPEAT) || (curTex->WrapT != GL_REPEAT)) &&
	  (W3D_Query(TDcontext, W3D_Q_PERSP_CLAMP, NULL) != W3D_FULLY_SUPPORTED))
	  return GL_FALSE;
      }

      if ((curTex->WrapS != curTex->WrapT) &&
	  (W3D_Query(TDcontext, W3D_Q_WRAP_ASYM, NULL) != W3D_FULLY_SUPPORTED))
	return GL_FALSE;

      if (curTex->Image[0]->Width > W3D_Query(TDcontext, W3D_Q_MAXTEXWIDTH_P, NULL))
	return GL_FALSE;

      if (curTex->Image[0]->Height > W3D_Query(TDcontext, W3D_Q_MAXTEXHEIGHT_P, NULL))
	return GL_FALSE;
    }

    if (((curTex->MinFilter == GL_LINEAR) ||
	 (curTex->MinFilter == GL_LINEAR_MIPMAP_NEAREST) ||
	 (curTex->MinFilter == GL_LINEAR_MIPMAP_LINEAR) ||
	 (curTex->MagFilter == GL_LINEAR)) &&
	(W3D_Query(TDcontext, W3D_Q_BILINEARFILTER, NULL) != W3D_FULLY_SUPPORTED))
      return GL_FALSE;

    if (((curTex->MinFilter == GL_NEAREST_MIPMAP_NEAREST) ||
	 (curTex->MinFilter == GL_NEAREST_MIPMAP_LINEAR) ||
	 (curTex->MinFilter == GL_LINEAR_MIPMAP_NEAREST) ||
	 (curTex->MinFilter == GL_LINEAR_MIPMAP_LINEAR)) &&
	(W3D_Query(TDcontext, W3D_Q_MIPMAPPING, NULL) != W3D_FULLY_SUPPORTED))
      return GL_FALSE;

    if (((curTex->MinFilter == GL_NEAREST_MIPMAP_LINEAR) ||
	 (curTex->MinFilter == GL_LINEAR_MIPMAP_LINEAR)) &&
	(W3D_Query(TDcontext, W3D_Q_MMFILTER, NULL) != W3D_FULLY_SUPPORTED))
      return GL_FALSE;

    if ((envmode == GL_REPLACE) &&
	(W3D_Query(TDcontext, W3D_Q_ENV_REPLACE, NULL) != W3D_FULLY_SUPPORTED))
      return GL_FALSE;

    if ((envmode == GL_DECAL) &&
	(W3D_Query(TDcontext, W3D_Q_ENV_DECAL, NULL) != W3D_FULLY_SUPPORTED))
      return GL_FALSE;

    if ((envmode == GL_MODULATE) &&
	(W3D_Query(TDcontext, W3D_Q_ENV_MODULATE, NULL) != W3D_FULLY_SUPPORTED))
      return GL_FALSE;

    if ((envmode == GL_BLEND) &&
	(W3D_Query(TDcontext, W3D_Q_ENV_BLEND, NULL) != W3D_FULLY_SUPPORTED))
      return GL_FALSE;

    if ((curTex->Image[0]->Width != curTex->Image[0]->Height) && (!TstF(TDdriver->flags, TD_RECTTEX)))
      return GL_FALSE;

    if (!TDdriver->GLtex)
      return GL_FALSE;
    else {
      struct gl_texture_image *TDintex;
      struct gl_texture_object *TDouttex = TDdriver->GLtex;
      GLuint GLformat = 0, TDformat = 0;
      GLenum format;
      GLshort i = MAX_TEXTURE_LEVELS;

      if (!(TDintex = TDouttex->Image[0]))
	return GL_FALSE;

      switch (format = TDintex->Format) {
	case GL_ALPHA:		  GLformat = W3D_A8;		break;
	case GL_LUMINANCE:	  GLformat = W3D_L8;		break;
	case GL_LUMINANCE_ALPHA:  GLformat = W3D_L8A8;		break;
	case GL_INTENSITY:	  GLformat = W3D_I8;		break;
	case GL_RGB:		  GLformat = W3D_R8G8B8;	break;
	case GL_RGBA:		  GLformat = W3D_A8R8G8B8;	break;
	case GL_COLOR_INDEX:
	default:		  return GL_FALSE;		break;
      }

      if (amesa->depth <= 8)
	TDformat = W3D_CHUNKY;

      if (W3D_GetTexFmtInfo(TDcontext, GLformat, TDformat) & W3D_TEXFMT_UNSUPPORTED)
	return GL_FALSE;

      /* all mipmap levels must have the same format */
      while (--i >= 0)
	if (TDouttex->Image[i] && (TDouttex->Image[i]->Format != format))
	  return GL_FALSE;

      if (!(((W3Dtexobj *) (TDouttex->DriverData))->texture))
	return GL_FALSE;							/* no W3D texture object exists */
    }
  }

  /* gouraud / flat shading check */
  if (ctx->Light.ShadeModel == GL_SMOOTH) {
    if (W3D_Query(TDcontext, W3D_Q_GOURAUDSHADING, NULL) != W3D_FULLY_SUPPORTED)
      return GL_FALSE;
  }
  else {
    if (W3D_Query(TDcontext, W3D_Q_FLATSHADING, NULL) != W3D_FULLY_SUPPORTED)
      return GL_FALSE;
  }

  /* depth buffer check */
  if (ctx->Depth.Test) {
    if (W3D_Query(TDcontext, W3D_Q_ZBUFFER, NULL) != W3D_FULLY_SUPPORTED)
      return GL_FALSE;

    if (W3D_Query(TDcontext, W3D_Q_ZCOMPAREMODES, NULL) != W3D_FULLY_SUPPORTED)
      return GL_FALSE;

    if (ctx->Depth.Mask) {
      if (W3D_Query(TDcontext, W3D_Q_ZBUFFERUPDATE, NULL) != W3D_FULLY_SUPPORTED)
	return GL_FALSE;
    }
  }

  /* alpha test check */
  if (ctx->Color.AlphaEnabled) {
    if (W3D_Query(TDcontext, W3D_Q_ALPHATEST, NULL) != W3D_FULLY_SUPPORTED)
      return GL_FALSE;

    if (W3D_Query(TDcontext, W3D_Q_ALPHATESTMODES, NULL) != W3D_FULLY_SUPPORTED)
      return GL_FALSE;
  }

  /* blending check */
  if (ctx->Color.BlendEnabled) {
    GLuint TDsfactor = 0, TDdfactor = 0;

    if (W3D_Query(TDcontext, W3D_Q_BLENDING, NULL) == W3D_NOT_SUPPORTED)
      return GL_FALSE;

    switch (ctx->Color.BlendSrcRGB) {
      case GL_ZERO:			TDsfactor = W3D_ZERO;				break;
      case GL_ONE:			TDsfactor = W3D_ONE;				break;
      case GL_DST_COLOR:		TDsfactor = W3D_DST_COLOR;			break;
      case GL_ONE_MINUS_DST_COLOR:	TDsfactor = W3D_ONE_MINUS_DST_COLOR;		break;
      case GL_SRC_ALPHA:		TDsfactor = W3D_SRC_ALPHA;			break;
      case GL_ONE_MINUS_SRC_ALPHA:	TDsfactor = W3D_ONE_MINUS_SRC_ALPHA;		break;
      case GL_DST_ALPHA:		TDsfactor = W3D_DST_ALPHA;			break;
      case GL_ONE_MINUS_DST_ALPHA:	TDsfactor = W3D_ONE_MINUS_DST_ALPHA;		break;
      case GL_SRC_ALPHA_SATURATE:	TDsfactor = W3D_SRC_ALPHA_SATURATE;		break;
      case GL_CONSTANT_COLOR:		TDsfactor = W3D_CONSTANT_COLOR;			break;
      case GL_ONE_MINUS_CONSTANT_COLOR:	TDsfactor = W3D_ONE_MINUS_CONSTANT_COLOR;	break;
      case GL_CONSTANT_ALPHA:		TDsfactor = W3D_CONSTANT_ALPHA;			break;
      case GL_ONE_MINUS_CONSTANT_ALPHA:	TDsfactor = W3D_ONE_MINUS_CONSTANT_ALPHA;	break;
      default:				return GL_FALSE;				break;
    }

    switch (ctx->Color.BlendDstRGB) {
      case GL_ZERO:			TDdfactor = W3D_ZERO;				break;
      case GL_ONE:			TDdfactor = W3D_ONE;				break;
      case GL_SRC_COLOR:		TDdfactor = W3D_SRC_COLOR;			break;
      case GL_ONE_MINUS_SRC_COLOR:	TDdfactor = W3D_ONE_MINUS_SRC_COLOR;		break;
      case GL_SRC_ALPHA:		TDdfactor = W3D_SRC_ALPHA;			break;
      case GL_ONE_MINUS_SRC_ALPHA:	TDdfactor = W3D_ONE_MINUS_SRC_ALPHA;		break;
      case GL_DST_ALPHA:		TDdfactor = W3D_DST_ALPHA;			break;
      case GL_ONE_MINUS_DST_ALPHA:	TDdfactor = W3D_ONE_MINUS_DST_ALPHA;		break;
      case GL_CONSTANT_COLOR:		TDdfactor = W3D_CONSTANT_COLOR;			break;
      case GL_ONE_MINUS_CONSTANT_COLOR:	TDdfactor = W3D_ONE_MINUS_CONSTANT_COLOR;	break;
      case GL_CONSTANT_ALPHA:		TDdfactor = W3D_CONSTANT_ALPHA;			break;
      case GL_ONE_MINUS_CONSTANT_ALPHA:	TDdfactor = W3D_ONE_MINUS_CONSTANT_ALPHA;	break;
      default:				return GL_FALSE;				break;
    }

    if (W3D_SetBlendMode(TDcontext, TDsfactor, TDdfactor) != W3D_SUCCESS)
      return GL_FALSE;

    if (W3D_Query(TDcontext, W3D_Q_BLEND_DECAL_FOG, NULL) != W3D_FULLY_SUPPORTED) {
      GLenum envmode = ctx->Texture.Unit[0].EnvMode;

      if (ctx->Texture.Enabled && ctx->Fog.Enabled && (envmode == GL_DECAL))
	return GL_FALSE;
    }
  }

  /* fogging check */
  if (ctx->Fog.Enabled) {
    if (W3D_Query(TDcontext, W3D_Q_FOGGING, NULL) != W3D_FULLY_SUPPORTED)
      return GL_FALSE;

    switch (ctx->Fog.Mode) {
      case GL_LINEAR:
	if (W3D_Query(TDcontext, W3D_Q_LINEAR, NULL) != W3D_FULLY_SUPPORTED) {
	  if (TstF(TDdriver->flags, TD_NICEFOG))
	    return GL_FALSE;
	  else {
	    if (W3D_Query(TDcontext, W3D_Q_INTERPOLATED, NULL) != W3D_FULLY_SUPPORTED)
	      return GL_FALSE;
	  }
	}
	break;
      case GL_EXP:
	if (W3D_Query(TDcontext, W3D_Q_EXPONENTIAL, NULL) != W3D_FULLY_SUPPORTED)
	  return GL_FALSE;
	break;
      case GL_EXP2:
	if (W3D_Query(TDcontext, W3D_Q_S_EXPONENTIAL, NULL) != W3D_FULLY_SUPPORTED)
	  return GL_FALSE;
      default:
	break;
    }
  }

  /* scissor check */
  if (ctx->Scissor.Enabled) {
    if (W3D_Query(TDcontext, W3D_Q_SCISSOR, NULL) != W3D_FULLY_SUPPORTED)
      return GL_FALSE;
  }

  /* logic op check */
  if ((ctx->Color.IndexLogicOpEnabled) || (ctx->Color.ColorLogicOpEnabled)) {
    if (W3D_Query(TDcontext, W3D_Q_LOGICOP, NULL) != W3D_FULLY_SUPPORTED)
      return GL_FALSE;
  }

  /* masking check */
  if (!(((*(GLuint *) ctx->Color.ColorMask == 0xffffffff) ||
	 (*(GLuint *) ctx->Color.ColorMask == 0xffffff00)) &&
	((ctx->Color.IndexMask & 0xff) == 0xff))) {
    if (W3D_Query(TDcontext, W3D_Q_MASKING, NULL) != W3D_FULLY_SUPPORTED)
      return GL_FALSE;
  }

  /* stencil check */
  if (ctx->Stencil.Enabled) {
    if (W3D_Query(TDcontext, W3D_Q_STENCILBUFFER, NULL) != W3D_FULLY_SUPPORTED)
      return GL_FALSE;

    if (W3D_Query(TDcontext, W3D_Q_STENCIL_FUNC, NULL) != W3D_FULLY_SUPPORTED)
      return GL_FALSE;

    if (W3D_Query(TDcontext, W3D_Q_STENCIL_MASK, NULL) != W3D_FULLY_SUPPORTED)
      return GL_FALSE;

    if (W3D_Query(TDcontext, W3D_Q_STENCIL_SFAIL, NULL) != W3D_FULLY_SUPPORTED)
      return GL_FALSE;

    if (W3D_Query(TDcontext, W3D_Q_STENCIL_DPFAIL, NULL) != W3D_FULLY_SUPPORTED)
      return GL_FALSE;

    if (W3D_Query(TDcontext, W3D_Q_STENCIL_DPPASS, NULL) != W3D_FULLY_SUPPORTED)
      return GL_FALSE;

    if (ctx->Stencil.WriteMask != ((1 << STENCIL_BITS) - 1)) {
      if (W3D_Query(TDcontext, W3D_Q_STENCIL_WRMASK, NULL) != W3D_FULLY_SUPPORTED)
	return GL_FALSE;
    }
  }

  /* TODO: support specular highlighting completely */
  if (ctx->Light.Model.ColorControl == GL_SEPARATE_SPECULAR_COLOR)
    return GL_FALSE;

  return GL_TRUE;
}

/**********************************************************************/

void wrpSetupDriver(GLcontext * ctx)
{
  amigaMesaContext amesa;
  struct TDdriver *TDdriver;
  W3D_Context *TDcontext;

  DEBUGOUT(1, "wrpSetupDriver()\n");

  amesa = (amigaMesaContext) ctx->DriverCtx;
  TDdriver = amesa->TDdriver;
  TDcontext = TDdriver->td_ctx;

  /* texmapping setup */
  /* TODO: support 3d textures */
  W3D_WaitIdle(TDcontext);

  if (ctx->Texture.Enabled) {
    W3D_SetState(TDcontext, W3D_TEXMAPPING, W3D_ENABLE);

    if (ctx->Hint.PerspectiveCorrection == GL_FASTEST)
      W3D_SetState(TDcontext, W3D_PERSPECTIVE, W3D_DISABLE);
    else {
      if (!(TDdriver->flags & 0x20000000))
	W3D_SetState(TDcontext, W3D_PERSPECTIVE, W3D_ENABLE);
      else
	W3D_SetState(TDcontext, W3D_PERSPECTIVE, W3D_DISABLE);
    }
  }
  else
    W3D_SetState(TDcontext, W3D_TEXMAPPING, W3D_DISABLE);

  /* gouraud / flat shading setup */
  if (ctx->Light.ShadeModel == GL_SMOOTH)
    W3D_SetState(TDcontext, W3D_GOURAUD, W3D_ENABLE);
  else
    W3D_SetState(TDcontext, W3D_GOURAUD, W3D_DISABLE);

  /* depth buffer setup */
  if (ctx->Depth.Test) {
    ULONG zmode = 0;

    if (!(TDdriver->flags & 0x80000000))
      W3D_SetState(TDcontext, W3D_ZBUFFER, W3D_ENABLE);

    if (!(TDdriver->flags & 0x40000000)) {
      if (ctx->Depth.Mask)
	W3D_SetState(TDcontext, W3D_ZBUFFERUPDATE, W3D_ENABLE);
      else
	W3D_SetState(TDcontext, W3D_ZBUFFERUPDATE, W3D_DISABLE);
    }

    switch (ctx->Depth.Func) {
      case GL_NEVER:	zmode = W3D_Z_NEVER;	break;
      case GL_LESS:	zmode = W3D_Z_LESS;	break;
      case GL_GEQUAL:	zmode = W3D_Z_GEQUAL;	break;
      case GL_LEQUAL:	zmode = W3D_Z_LEQUAL;	break;
      case GL_GREATER:	zmode = W3D_Z_GREATER;	break;
      case GL_NOTEQUAL:	zmode = W3D_Z_NOTEQUAL;	break;
      case GL_EQUAL:	zmode = W3D_Z_EQUAL;	break;
      case GL_ALWAYS:	zmode = W3D_Z_ALWAYS;	break;
      default:					break;
    }

    W3D_SetZCompareMode(TDcontext, zmode);
  }
  else {
    W3D_SetState(TDcontext, W3D_ZBUFFER, W3D_DISABLE);
    W3D_SetState(TDcontext, W3D_ZBUFFERUPDATE, W3D_DISABLE);
  }

  /* alpha test setup */
  if (ctx->Color.AlphaEnabled) {
    W3D_Float ref;
    GLuint amode = 0;

    W3D_SetState(TDcontext, W3D_ALPHATEST, W3D_ENABLE);
    ref = (((GLfloat) ctx->Color.AlphaRef) - 0.5) / 255.0;

    switch (ctx->Color.AlphaFunc) {
      case GL_NEVER:	amode = W3D_A_NEVER;	break;
      case GL_LESS:	amode = W3D_A_LESS;	break;
      case GL_GEQUAL:	amode = W3D_A_GEQUAL;	break;
      case GL_LEQUAL:	amode = W3D_A_LEQUAL;	break;
      case GL_GREATER:	amode = W3D_A_GREATER;	break;
      case GL_NOTEQUAL:	amode = W3D_A_NOTEQUAL;	break;
      case GL_EQUAL:	amode = W3D_A_EQUAL;	break;
      case GL_ALWAYS:	amode = W3D_A_ALWAYS;	break;
      default:					break;
    }

    W3D_SetAlphaMode(TDcontext, amode, &ref);
  }
  else
    W3D_SetState(TDcontext, W3D_ALPHATEST, W3D_DISABLE);

  /* blending check */
  if (ctx->Color.BlendEnabled) {
    GLuint srcfactor = 0, destfactor = 0;

    W3D_SetState(TDcontext, W3D_BLENDING, W3D_ENABLE);

    switch (ctx->Color.BlendSrcRGB) {
      case GL_ZERO:			srcfactor = W3D_ZERO;				break;
      case GL_ONE:			srcfactor = W3D_ONE;				break;
      case GL_DST_COLOR:		srcfactor = W3D_DST_COLOR;			break;
      case GL_ONE_MINUS_DST_COLOR:	srcfactor = W3D_ONE_MINUS_DST_COLOR;		break;
      case GL_SRC_ALPHA:		srcfactor = W3D_SRC_ALPHA;			break;
      case GL_ONE_MINUS_SRC_ALPHA:	srcfactor = W3D_ONE_MINUS_SRC_ALPHA;		break;
      case GL_DST_ALPHA:		srcfactor = W3D_DST_ALPHA;			break;
      case GL_ONE_MINUS_DST_ALPHA:	srcfactor = W3D_ONE_MINUS_DST_ALPHA;		break;
      case GL_SRC_ALPHA_SATURATE:	srcfactor = W3D_SRC_ALPHA_SATURATE;		break;
      case GL_CONSTANT_COLOR:		srcfactor = W3D_CONSTANT_COLOR;			break;
      case GL_ONE_MINUS_CONSTANT_COLOR:	srcfactor = W3D_ONE_MINUS_CONSTANT_COLOR;	break;
      case GL_CONSTANT_ALPHA:		srcfactor = W3D_CONSTANT_ALPHA;			break;
      case GL_ONE_MINUS_CONSTANT_ALPHA:	srcfactor = W3D_ONE_MINUS_CONSTANT_ALPHA;	break;
      default:										break;
    }

    switch (ctx->Color.BlendDstRGB) {
      case GL_ZERO:			destfactor = W3D_ZERO;				break;
      case GL_ONE:			destfactor = W3D_ONE;				break;
      case GL_SRC_COLOR:		destfactor = W3D_SRC_COLOR;			break;
      case GL_ONE_MINUS_SRC_COLOR:	destfactor = W3D_ONE_MINUS_SRC_COLOR;		break;
      case GL_SRC_ALPHA:		destfactor = W3D_SRC_ALPHA;			break;
      case GL_ONE_MINUS_SRC_ALPHA:	destfactor = W3D_ONE_MINUS_SRC_ALPHA;		break;
      case GL_DST_ALPHA:		destfactor = W3D_DST_ALPHA;			break;
      case GL_ONE_MINUS_DST_ALPHA:	destfactor = W3D_ONE_MINUS_DST_ALPHA;		break;
      case GL_CONSTANT_COLOR:		destfactor = W3D_CONSTANT_COLOR;		break;
      case GL_ONE_MINUS_CONSTANT_COLOR:	destfactor = W3D_ONE_MINUS_CONSTANT_COLOR;	break;
      case GL_CONSTANT_ALPHA:		destfactor = W3D_CONSTANT_ALPHA;		break;
      case GL_ONE_MINUS_CONSTANT_ALPHA:	destfactor = W3D_ONE_MINUS_CONSTANT_ALPHA;	break;
      default:										break;
    }

    W3D_SetBlendMode(TDcontext, srcfactor, destfactor);
  }
  else
    W3D_SetState(TDcontext, W3D_BLENDING, W3D_DISABLE);

  /* fogging check */
  if (ctx->Fog.Enabled) {
    W3D_SetState(TDcontext, W3D_FOGGING, W3D_ENABLE);

    wrpSetupFog(ctx);
  }
  else
    W3D_SetState(TDcontext, W3D_FOGGING, W3D_DISABLE);

  /* antialiasing setup */
  if (ctx->Point.SmoothFlag)
    W3D_SetState(TDcontext, W3D_ANTI_POINT, W3D_ENABLE);
  else
    W3D_SetState(TDcontext, W3D_ANTI_POINT, W3D_DISABLE);

  if (ctx->Line.SmoothFlag)
    W3D_SetState(TDcontext, W3D_ANTI_LINE, W3D_ENABLE);
  else
    W3D_SetState(TDcontext, W3D_ANTI_LINE, W3D_DISABLE);

  if (ctx->Polygon.SmoothFlag)
    W3D_SetState(TDcontext, W3D_ANTI_POLYGON, W3D_ENABLE);
  else
    W3D_SetState(TDcontext, W3D_ANTI_POLYGON, W3D_DISABLE);

  /* dithering setup */
  if (!(ctx->NoDither) && (ctx->Color.DitherFlag))
    W3D_SetState(TDcontext, W3D_DITHERING, W3D_ENABLE);
  else
    W3D_SetState(TDcontext, W3D_DITHERING, W3D_DISABLE);

  /* scissor setup */
  if (ctx->Scissor.Enabled) {
    TDdriver->scissor.left = ctx->Scissor.X;
    TDdriver->scissor.top = amesa->height - ctx->Scissor.Y - ctx->Scissor.Height;
    TDdriver->scissor.width = ctx->Scissor.Width;
    TDdriver->scissor.height = ctx->Scissor.Height;

    W3D_SetState(TDcontext, W3D_SCISSOR, W3D_ENABLE);
    W3D_SetScissor(TDcontext, &TDdriver->scissor);
  }
  else {
    TDdriver->scissor.left = 0;
    TDdriver->scissor.top = 0;
    TDdriver->scissor.width = amesa->width;
    TDdriver->scissor.height = amesa->height;

    W3D_SetState(TDcontext, W3D_SCISSOR, W3D_DISABLE);
    /* small workaround to fix W3DV2 Prototype problem */
    W3D_SetScissor(TDcontext, &TDdriver->scissor);
  }

  /* logic op setup */
  if ((ctx->Color.IndexLogicOpEnabled) || (ctx->Color.ColorLogicOpEnabled)) {
    GLuint operation = 0;

    W3D_SetState(TDcontext, W3D_LOGICOP, W3D_ENABLE);
    switch (ctx->Color.LogicOp) {
      case GL_CLEAR:		operation = W3D_LO_CLEAR;		break;
      case GL_SET:		operation = W3D_LO_SET;			break;
      case GL_COPY:		operation = W3D_LO_COPY;		break;
      case GL_COPY_INVERTED:	operation = W3D_LO_COPY_INVERTED;	break;
      case GL_NOOP:		operation = W3D_LO_NOOP;		break;
      case GL_INVERT:		operation = W3D_LO_INVERT;		break;
      case GL_AND:		operation = W3D_LO_AND;			break;
      case GL_NAND:		operation = W3D_LO_NAND;		break;
      case GL_OR:		operation = W3D_LO_OR;			break;
      case GL_NOR:		operation = W3D_LO_NOR;			break;
      case GL_XOR:		operation = W3D_LO_XOR;			break;
      case GL_EQUIV:		operation = W3D_LO_EQUIV;		break;
      case GL_AND_REVERSE:	operation = W3D_LO_AND_REVERSE;		break;
      case GL_AND_INVERTED:	operation = W3D_LO_AND_INVERTED;	break;
      case GL_OR_REVERSE:	operation = W3D_LO_OR_REVERSE;		break;
      case GL_OR_INVERTED:	operation = W3D_LO_OR_INVERTED;		break;
      default:								break;
    }

    W3D_SetLogicOp(TDcontext, operation);
  }
  else
    W3D_SetState(TDcontext, W3D_LOGICOP, W3D_DISABLE);

  /* masking setup */
  if (ctx->Visual->RGBAflag) {
    W3D_Bool r, g, b, a;

    r = (ctx->Color.ColorMask[RCOMP] == 0xff);
    g = (ctx->Color.ColorMask[GCOMP] == 0xff);
    b = (ctx->Color.ColorMask[BCOMP] == 0xff);
    a = (ctx->Color.ColorMask[ACOMP] == 0xff);

    W3D_SetColorMask(TDcontext, r, g, b, a);
  }
  else
    W3D_SetPenMask(TDcontext, (ULONG) ctx->Color.IndexMask);

  /* stencil setup */
  if (ctx->Stencil.Enabled) {
    ULONG stfunc = 0, stwmask, stref, stmask;
    ULONG stop_fail = 0, stop_zfail = 0, stop_zpass = 0;
    
    W3D_SetState(TDcontext, W3D_STENCILBUFFER, W3D_ENABLE);
    stref = ctx->Stencil.Ref;
    stmask = ctx->Stencil.ValueMask;
    stwmask = ctx->Stencil.WriteMask;

    switch (ctx->Stencil.Function) {
      case GL_NEVER:	stfunc = W3D_ST_NEVER;		break;
      case GL_LESS:	stfunc = W3D_ST_LESS;		break;
      case GL_LEQUAL:	stfunc = W3D_ST_LEQUAL;		break;
      case GL_GREATER:	stfunc = W3D_ST_GREATER;	break;
      case GL_GEQUAL:	stfunc = W3D_ST_GEQUAL;		break;
      case GL_EQUAL:	stfunc = W3D_ST_EQUAL;		break;
      case GL_NOTEQUAL:	stfunc = W3D_ST_NOTEQUAL;	break;
      case GL_ALWAYS:	stfunc = W3D_ST_ALWAYS;		break;
      default:						break;
    }

    switch (ctx->Stencil.FailFunc) {
      case GL_KEEP:	stop_fail = W3D_ST_KEEP;	break;
      case GL_ZERO:	stop_fail = W3D_ST_ZERO;	break;
      case GL_REPLACE:	stop_fail = W3D_ST_REPLACE;	break;
      case GL_INCR:	stop_fail = W3D_ST_INCR;	break;
      case GL_DECR:	stop_fail = W3D_ST_DECR;	break;
      case GL_INVERT:	stop_fail = W3D_ST_INVERT;	break;
      default:						break;
    }

    switch (ctx->Stencil.ZFailFunc) {
      case GL_KEEP:	stop_zfail = W3D_ST_KEEP;	break;
      case GL_ZERO:	stop_zfail = W3D_ST_ZERO;	break;
      case GL_REPLACE:	stop_zfail = W3D_ST_REPLACE;	break;
      case GL_INCR:	stop_zfail = W3D_ST_INCR;	break;
      case GL_DECR:	stop_zfail = W3D_ST_DECR;	break;
      case GL_INVERT:	stop_zfail = W3D_ST_INVERT;	break;
      default:						break;
    }

    switch (ctx->Stencil.ZPassFunc) {
      case GL_KEEP:	stop_zpass = W3D_ST_KEEP;	break;
      case GL_ZERO:	stop_zpass = W3D_ST_ZERO;	break;
      case GL_REPLACE:	stop_zpass = W3D_ST_REPLACE;	break;
      case GL_INCR:	stop_zpass = W3D_ST_INCR;	break;
      case GL_DECR:	stop_zpass = W3D_ST_DECR;	break;
      case GL_INVERT:	stop_zpass = W3D_ST_INVERT;	break;
      default:						break;
    }

    W3D_SetStencilFunc(TDcontext,stfunc,stref,stmask);
    W3D_SetStencilOp(TDcontext,stop_fail,stop_zfail,stop_zpass);
    W3D_SetWriteMask(TDcontext,stwmask);
  }
  else
    W3D_SetState(TDcontext,W3D_STENCILBUFFER,W3D_DISABLE);

  /* TODO: support specular highlighting as soon as supported by W3D */
}

/*
 * Initialize all the pointers in the DD struct.  Do this whenever   
 * a new TDcontext is made current or we change buffers via setBuffer! 
 */
void wrpStandardDDPointers(GLcontext * ctx)
{
  amigaMesaContext amesa;
  struct TDdriver *TDdriver;
  W3D_Context *TDcontext;
  palMode trueColor;
  GLboolean ctxSupport = GL_TRUE;

  DEBUGOUT(1, "wrpStandardDDPointers()\n");

  amesa = (amigaMesaContext) ctx->DriverCtx;
  TDdriver = amesa->TDdriver;
  TDcontext = TDdriver->td_ctx;
  trueColor = amesa->trueColor;

  ctx->Driver.RendererString = wrpRendererString;

  ctx->Driver.UpdateState = wrpStandardDDPointers;
  ctx->Driver.ClearIndex = cybClearIndex;
  ctx->Driver.ClearColor = cybClearColor;
  ctx->Driver.Clear = wrpClear;

  ctx->Driver.Index = wrpSetIndex;
  ctx->Driver.Color = wrpSetColor;

  ctx->Driver.IndexMask = cybIndexMask;
  ctx->Driver.ColorMask = cybColorMask;
  ctx->Driver.LogicOp = wrpLogicOp;

  ctx->Driver.SetBuffer = cmnSetBuffer;
  ctx->Driver.GetBufferSize = cmnStandardResize;

  ctx->Driver.RenderStart = cmnRenderStart;
  ctx->Driver.RenderFinish = cmnRenderFinish;
  ctx->Driver.Dither = NULL;
  ctx->Driver.Flush = cmnFlush;

  if ((ctxSupport = wrpContextSupported(ctx))) {
    ctx->Driver.PointsFunc = wrpChoosePointsFunction(ctx);
    ctx->Driver.LineFunc = wrpChooseLineFunction(ctx);
    ctx->Driver.TriangleFunc = wrpChooseTriangleFunction(ctx);
    ctx->Driver.QuadFunc = wrpChooseQuadFunction(ctx);
/*  ctx->Driver.RectFunc = wrpChooseRectFunction(ctx); */
    ctx->Driver.RasterSetup = wrpChooseRasterSetupFunction(ctx);

    TDdriver->LineStripFunc = wrpChooseLineStripFunction(ctx);
    TDdriver->TriStripFunc = wrpChooseTriStripFunction(ctx);
    TDdriver->TriFanFunc = wrpChooseTriFanFunction(ctx);
  }

  /* Pixel/span writing functions: */
  ctx->Driver.WriteRGBASpan = cybWriteRGBASpan;
  ctx->Driver.WriteRGBSpan = cybWriteRGBSpan;
  ctx->Driver.WriteCI32Span = cybWriteCI32Span;
  ctx->Driver.WriteCI8Span = cybWriteCI8Span;
  ctx->Driver.WriteMonoRGBASpan = cybWriteMonoCISpan;
  ctx->Driver.WriteMonoCISpan = cybWriteMonoCISpan;

  ctx->Driver.WriteRGBAPixels = cybWriteRGBAPixel;
  ctx->Driver.WriteCI32Pixels = cybWriteCI32Pixel;
  ctx->Driver.WriteMonoRGBAPixels = cybWriteMonoCIPixel;
  ctx->Driver.WriteMonoCIPixels = cybWriteMonoCIPixel;

  /* Pixel/span reading functions: */
  ctx->Driver.ReadRGBASpan = cybReadRGBASpan;
  ctx->Driver.ReadCI32Span = IS_SHIFT(trueColor) ? cyb8ReadCI32Span :
			     IS_MATCH(trueColor) ? cybGReadCI32Span :
						   cybDReadCI32Span;

  ctx->Driver.ReadRGBAPixels = cybReadRGBAPixel;
  ctx->Driver.ReadCI32Pixels = IS_SHIFT(trueColor) ? cyb8ReadCI32Pixel :
			       IS_MATCH(trueColor) ? cybGReadCI32Pixel :
						     cybDReadCI32Pixel;

  /* internal Warp3D-support */
/*ctx->Driver.GetParameteri = wrpGetParameteri; */
  ctx->Driver.NearFar = wrpNearFar;
  ctx->Driver.TexEnv = wrpTexEnv;
  ctx->Driver.TexImage = wrpTexImage;
  ctx->Driver.TexSubImage = wrpTexSubImage;
  ctx->Driver.TexParameter = wrpTexParameter;
  ctx->Driver.BindTexture = wrpBindTexture;
  ctx->Driver.DeleteTexture = wrpDeleteTexture;

  W3D_Hint(TDcontext, W3D_H_TEXMAPPING,		W3D_H_NICE);
  W3D_Hint(TDcontext, W3D_H_MIPMAPPING,		W3D_H_NICE);
  W3D_Hint(TDcontext, W3D_H_BILINEARFILTER,	W3D_H_NICE);
  W3D_Hint(TDcontext, W3D_H_MMFILTER,		W3D_H_NICE);
  W3D_Hint(TDcontext, W3D_H_PERSPECTIVE,	W3D_H_NICE);

  if (ctx->Visual->DepthBits) {
    if (W3D_AllocZBuffer(TDcontext) != W3D_SUCCESS)
      /* TODO: CyberGfx-FallBack */;
    else
      SetF(TDdriver->flags, TD_ZBUFFER);
  }

  if (ctxSupport) {
    if (ctx->NewState & (~NEW_LIGHTING))
      wrpSetupDriver(ctx);
    TDdriver->height = (W3D_Float) amesa->height;
  }

  if (ctx->Buffer && !ctx->Buffer->Depth) {
    gl_alloc_depth_buffer(ctx);
    gl_clear_depth_buffer(ctx);
  }

  W3D_Flush(TDdriver->td_ctx);
}

#undef DEBUGPRINT
#endif
