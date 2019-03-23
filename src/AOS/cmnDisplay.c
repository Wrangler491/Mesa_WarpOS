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

#undef	NO_CONTEXT_AVAILABLE
#include <AOS/amigamesa.h>
#include "AOS/accessArray.h"

#define	MyWritePixelArray	WritePixelArray
#define	MyWriteChunkyPixels	WriteChunkyPixels

/**********************************************************************/

GLboolean cmnSetBuffer(GLcontext * ctx, GLenum mode)
{
  DEBUGOUT(1, "cmnSetBuffer()\n");

  /* Selects the color buffer(s) for reading and writing.
   * The following values must be accepted when applicable:
   *    GL_FRONT_LEFT - this buffer always exists
   *    GL_BACK_LEFT - when double buffering
   *    GL_FRONT_RIGHT - when using stereo
   *    GL_BACK_RIGHT - when using stereo and double buffering
   * The folowing values may optionally be accepted.  Return GL_TRUE
   * if accepted, GL_FALSE if not accepted.  In practice, only drivers
   * which can write to multiple color buffers at once should accept
   * these values.
   *    GL_FRONT - write to front left and front right if it exists
   *    GL_BACK - write to back left and back right if it exists
   *    GL_LEFT - write to front left and back left if it exists
   *    GL_RIGHT - write to right left and back right if they exist
   *    GL_FRONT_AND_BACK - write to all four buffers if they exist
   *    GL_NONE - disable buffer write in device driver.
   *
   * TODO implemed a set of buffers 
   */

  if (mode == GL_FRONT)
    return (GL_TRUE);
  else if (mode == GL_BACK)
    return (GL_TRUE);
  else
    return (GL_FALSE);
}

/**********************************************************************/

/* prototypes for natFast* */
void natFastTriangleFunction(GLcontext * ctx, GLuint v0, GLuint v1, GLuint v2, GLuint pv);
void natFastQuadFunction(GLcontext * ctx, GLuint v0, GLuint v1, GLuint v2, GLuint v3, GLuint pv);

/* These are called whenever glBegin() or glEnd() are called.
 * The device driver may do some sort of window locking/unlocking here.
 */
void cmnRenderStart(GLcontext *ctx) {
  amigaMesaContext amesa;
  const GLenum mode = 0;

  amesa = (amigaMesaContext) ctx->DriverCtx;

  DEBUGOUT(1, "cmnRenderStart(0x%08x)\n", amesa);

#if 0
  switch (mode) {
    case GL_TRIANGLES:
    case GL_TRIANGLE_STRIP:
    case GL_TRIANGLE_FAN:
    case GL_QUADS:
    case GL_QUAD_STRIP:
    case GL_POLYGON:
      /* 4 - 9 */
#endif
      if (!TstF(amesa->visual->flags, VISUAL_DOUBLEBUFFER) &&
          ((ctx->Driver.TriangleFunc == natFastTriangleFunction) ||
	   (ctx->Driver.QuadFunc     == natFastQuadFunction))) {
        /* non-db filled triangle- and quad-functions needs Areas */
        allocArea(amesa, amesa->rp);
        makeTempRaster(amesa, amesa->rp);					/* allocate temp raster */
      }
#if 0
    default:
      break;
  }
#endif

  amesa->lastAction = mode;
}

void cmnRenderFinish(GLcontext *ctx) {
  amigaMesaContext amesa;

  amesa = (amigaMesaContext) ctx->DriverCtx;

  DEBUGOUT(1, "cmnRenderFinish(0x%08x)\n", amesa);

#if 0
  switch (amesa->lastAction) {
    case GL_TRIANGLES:
    case GL_TRIANGLE_STRIP:
    case GL_TRIANGLE_FAN:
    case GL_QUADS:
    case GL_QUAD_STRIP:
    case GL_POLYGON:
      /* 4 - 9 */
#endif
      if (!TstF(amesa->visual->flags, VISUAL_DOUBLEBUFFER) &&
          ((ctx->Driver.TriangleFunc == natFastTriangleFunction) ||
	   (ctx->Driver.QuadFunc     == natFastQuadFunction))) {
        /* non-db filled triangle- and quad-functions needs Areas */
        destroyTempRaster(amesa, amesa->rp);				/* deallocate temp raster  */
        freeArea(amesa, amesa->rp);
      }
#if 0
    default:
      break;
  }
#endif

  amesa->lastAction = 0;
}

void cmnDither(GLcontext *ctx, GLboolean enable) {
  amigaMesaContext amesa;

  amesa = (amigaMesaContext) ctx->DriverCtx;

  DEBUGOUT(1, "cmnDither(0x%08x, %d)\n", amesa, enable);

  /* TODO: don't enable dither if it's off by TAGs */
#if 0
  if (enable) {
    SetF(amesa->flags, PALETTE_DITHER);
    if (!IS_DITHER(amesa->trueColor))
      MATCHtoDITHER(amesa->trueColor);
#ifdef	AOS_WARP3D
    if (Amesa->TDdriver)
      W3D_SetState(amesa->TDdriver->td_ctx, W3D_DITHERING, W3D_ENABLE );
#endif
  }
  else {
    ClrF(amesa->flags, PALETTE_DITHER);
    if (IS_DITHER(amesa->trueColor))
      DITHERtoMATCH(amesa->trueColor);
#ifdef	AOS_WARP3D
    if (amesa->TDdriver)
      W3D_SetState(amesa->TDdriver->td_ctx, W3D_DITHERING, W3D_DISABLE);
#endif
  }
#endif
}

void cmnFinish(void)
{
  /* implements glFinish if possible */
}

void cmnFlush(GLcontext * ctx)
{
  amigaMesaContext amesa;

  amesa = (amigaMesaContext) ctx->DriverCtx;

  DEBUGOUT(1, "cmnFlush(0x%08x)\n", amesa);

  /* implements glFlush if possible */
  if (amesa->SwapBuffer)
    (*amesa->SwapBuffer) (amesa);
}

/**********************************************************************/

void cmnStandardSwapBuffer(amigaMesaContext amesa)
{
  DEBUGOUT(1, "cmnStandardSwapBuffer(0x%08x)\n", amesa);

#ifdef	AOS_WARP3D
  if (Warp3DBase)
    wrpStandardSwapBufferDB(amesa);
#endif
  else if (amesa->rp != amesa->front_rp) {
    /* TODO: glLogicOp with minterms! */
    GLubyte minterm = 0xC0;
    struct gl_viewport_attrib *vport;

    vport = &amesa->gl_ctx->Viewport;

    DEBUGOUT(8, " ClipBlit(0x%08x, %d, %d, 0x%08x, %d, %d, %d, %d, %02x)\n",
	     amesa->rp,
	     FIXx(vport->X),
	     FIXy(vport->Y) - vport->Height,
	     amesa->front_rp,
	     FIXx(vport->X),
	     FIXy(vport->Y) - vport->Height,
	     vport->Width,
	     vport->Height,
	     minterm);
    ClipBlit(amesa->rp,
	     FIXx(vport->X),
	     FIXy(vport->Y) - vport->Height,					  /*  from  */
	     amesa->front_rp,
	     FIXx(vport->X),
	     FIXy(vport->Y) - vport->Height,					  /*  to  */
	     vport->Width,
	     vport->Height,							  /*  size  */
	     minterm);
  }
}

void cmnStandardSwapBufferDB(amigaMesaContext amesa)
{
  struct gl_viewport_attrib *vport;

  vport = &amesa->gl_ctx->Viewport;

  DEBUGOUT(1, "cmnStandardSwapBufferDB(0x%08x)\n", amesa);

  if (CyberGfxBase) {
#ifdef	AOS_WARP3D
    if (Warp3DBase)
      wrpStandardSwapBufferDB(amesa);
#endif
    /* TODO: check if WritePixelArray is faster than WriteChunkyPixels/WritePixelArray8 */
    else {
      GLshort rowSize, rowFormat;

      if (amesa->depth <= 8) {
	rowSize = amesa->FixedWidth;
	rowFormat = RECTFMT_LUT8;
      }
      else {
	rowSize = amesa->FixedWidth * 4;
	rowFormat = RECTFMT_RGBA;
      }

      DEBUGOUT(8, " WritePixelArray(0x%08x, %d, %d, %d, 0x%08x, %d, %d, %d, %d)\n",
			amesa->BackArray,
			FIXx(vport->X),
			FIXy(vport->Y) - vport->Height,
			rowSize,
			amesa->rp,
			FIXx(vport->X),
			FIXy(vport->Y) - vport->Height,
			vport->Width,
			vport->Height);
      MyWritePixelArray(amesa->BackArray,
			vport->X,
			amesa->height - vport->Height - vport->Y,
			rowSize,
			amesa->rp,
			FIXx(vport->X),
			FIXy(vport->Y) - vport->Height,
			vport->Width,
			vport->Height,
			rowFormat);
    }
  }
  else {
#if 0
    DEBUGOUT(8, " a) WritePixelArray(0x%08x, %d, %d, %d, %d, 0x%08x, 0x%08x)\n",
			amesa->rp,
			FIXx(vport->X),
		        FIXy(vport->Y) - vport->Height,
		        FIXx(vport->X) + vport->Width - 1,
		        FIXy(vport->Y) - 1,
		        amesa->BackArray,
		        amesa->temprp);
    WritePixelArray8(amesa->rp,
		     FIXx(vport->X),
		     FIXy(vport->Y) - vport->Height,
		     FIXx(vport->X) + vport->Width - 1,
		     FIXy(vport->Y) - 1,
		     amesa->BackArray,
		     amesa->temprp);
#else
    DEBUGOUT(8, " b) WriteChunkyPixels(0x%08x, %d, %d, %d, %d, 0x%08x, %d)\n",
			amesa->rp,
			FIXx(vport->X),
		        FIXy(vport->Y) - vport->Height,
		        FIXx(vport->X) + vport->Width - 1,
		        FIXy(vport->Y) - 1,
		        amesa->BackArray,
		        amesa->FixedWidth);
    MyWriteChunkyPixels(amesa->rp,
		        FIXx(vport->X),
		        FIXy(vport->Y) - vport->Height,
		        FIXx(vport->X) + vport->Width - 1,
		        FIXy(vport->Y) - 1,
		        amesa->BackArray,
		        amesa->FixedWidth);
#endif
  }
}

/**********************************************************************/
/*****               amiga/Mesa private init/despose/resize       *****/
/**********************************************************************/

void cmnStandardResize(GLcontext *ctx, GLuint *width, GLuint *height)
{
  amigaMesaContext amesa;

  amesa = (amigaMesaContext) ctx->DriverCtx;

  *width = amesa->width;
  *height = amesa->height;
  DEBUGOUT(1, "cmnStandardResize(0x%08x/0x%08x, %d, %d)\n", ctx, amesa, *width, *height);

  if (!((amesa->width == (LayerWidth(amesa->rp->Layer) - amesa->left - amesa->right)) &&
	(amesa->height == (LayerHeight(amesa->rp->Layer) - amesa->bottom - amesa->top)))) {
    if (CyberGfxBase && IsCyberModeID(GetVPModeID(&amesa->Screen->ViewPort)))
      amesa->depth = GetCyberMapAttr(amesa->rp->BitMap, CYBRMATTR_DEPTH);
    else
      amesa->depth = GetBitMapAttr(amesa->rp->BitMap, BMA_DEPTH);

    amesa->RealWidth = 
    amesa->FixedWidth = LayerWidth(amesa->rp->Layer);
    amesa->RealHeight = 
    amesa->FixedHeight = LayerHeight(amesa->rp->Layer);

    DEBUGOUT(2, "cmnStandardResize: resize %d/%d to %d/%d\n", *width, *height, amesa->RealWidth - amesa->left - amesa->right, amesa->RealHeight - amesa->bottom - amesa->top);
    *width = amesa->width = amesa->RealWidth - amesa->left - amesa->right;
    *height = amesa->height = amesa->RealHeight - amesa->bottom - amesa->top;

    amesa->gl_ctx->Color.DrawBuffer = GL_FRONT;
    if (TstF(amesa->visual->flags, VISUAL_DOUBLEBUFFER)) {
      destroyRPort(amesa, amesa->back_rp);
      amesa->gl_ctx->Color.DrawBuffer = GL_BACK;
      if (!(amesa->rp = amesa->back_rp = makeRPort(amesa, amesa->RealWidth, amesa->RealHeight, amesa->depth, 0, amesa->front_rp->BitMap)) == NULL) {
	amesa->rp = amesa->front_rp;
     /* amesa->visual->flags ^= VISUAL_DOUBLEBUFFER; */
	DEBUGOUT(0, "cmnStandardResize: Not enough memory to allocate back-Rastport\n");
      }
    }

#ifdef	AOS_WARP3D
    if (amesa->TDdriver) {
      struct TDdriver *TDdriver = amesa->TDdriver;
      W3D_Context *TDcontext = TDdriver->td_ctx;

      W3D_SetDrawRegion(TDcontext, amesa->rp->BitMap, 0, &TDdriver->scissor);
      ClrF(TDdriver->flags, TD_NOVMEM);

      if (TstF(TDdriver->flags, TD_ZBUFFER)) {
	W3D_FreeZBuffer(TDcontext);
	if (W3D_AllocZBuffer(TDcontext) != W3D_SUCCESS)
	  ClrF(TDdriver->flags, TD_ZBUFFER);
      }
    }
#endif

    ReallocOneLine("cmnStandardResize", amesa);
    if (amesa->depth <= 8)
      ReallocTempRPort("cmnStandardResize", amesa, 0);

    /* fast FIXx/FIXy/FIXxy */
    if (!makeFixedXY(amesa))
      Error("cmnStandardResize: Not enough memory to allocate FixedXY\n");

    DEBUGOUT(1, " amesa->RealWidth   = %d\n", amesa->RealWidth);
    DEBUGOUT(1, " amesa->RealHeight  = %d\n", amesa->RealHeight);
    DEBUGOUT(1, " amesa->FixedWidth  = %d\n", amesa->FixedWidth);
    DEBUGOUT(1, " amesa->FixedHeight = %d\n", amesa->FixedHeight);
    DEBUGOUT(1, " amesa->width       = %d\n", amesa->width);
    DEBUGOUT(1, " amesa->height      = %d\n", amesa->height);
    DEBUGOUT(1, " amesa->left        = %d\n", amesa->left);
    DEBUGOUT(1, " amesa->bottom      = %d\n", amesa->bottom);
    DEBUGOUT(1, " amesa->right       = %d\n", amesa->right);
    DEBUGOUT(1, " amesa->top         = %d\n", amesa->top);
    DEBUGOUT(1, " amesa->depth       = %d\n", amesa->depth);
  }
}

void cmnStandardResizeDB(GLcontext * ctx, GLuint * width, GLuint * height)
{
  amigaMesaContext amesa;

  amesa = (amigaMesaContext) ctx->DriverCtx;

  *width = amesa->width;
  *height = amesa->height;
  DEBUGOUT(1, "cmnStandardResizeDB(0x%08x/0x%08x, %d, %d)\n", ctx, amesa, *width, *height);

  if (!((amesa->width == (LayerWidth(amesa->rp->Layer) - amesa->left - amesa->right)) &&
	(amesa->height == (LayerHeight(amesa->rp->Layer) - amesa->bottom - amesa->top)))) {
    if (CyberGfxBase && IsCyberModeID(GetVPModeID(&amesa->Screen->ViewPort)))
      amesa->depth = GetCyberMapAttr(amesa->rp->BitMap, CYBRMATTR_DEPTH);
    else
      amesa->depth = GetBitMapAttr(amesa->rp->BitMap, BMA_DEPTH);

    amesa->RealWidth = LayerWidth(amesa->rp->Layer);
    amesa->RealHeight = LayerHeight(amesa->rp->Layer);
    if (amesa->depth <= 8)
      amesa->FixedWidth = natFixedMask(amesa->RealWidth);
    else
      amesa->FixedWidth = cybFixedMask(amesa->RealWidth);
    amesa->FixedHeight = amesa->RealHeight;

    DEBUGOUT(2, "cmnStandardResizeDB: resize %d/%d to %d/%d\n", *width, *height, amesa->RealWidth - amesa->left - amesa->right, amesa->RealHeight - amesa->bottom - amesa->top);
    *width = amesa->width = amesa->RealWidth - amesa->left - amesa->right;
    *height = amesa->height = amesa->RealHeight - amesa->bottom - amesa->top;

    amesa->gl_ctx->Color.DrawBuffer = GL_FRONT;
    if (TstF(amesa->visual->flags, VISUAL_DOUBLEBUFFER)) {
      amesa->gl_ctx->Color.DrawBuffer = GL_BACK;
      ReallocPenBackArray("cmnStandardResizeDB", amesa);
    }

    if (amesa->depth <= 8)
      ReallocTempRPort("cmnStandardResize", amesa, 0);

    /* fast FIXx/FIXy/FIXxy */
    if (!makeFixedXY(amesa))
      Error("cmnStandardResizeDB: Not enough memory to allocate FixedXY\n");

    DEBUGOUT(1, " amesa->RealWidth   = %d\n", amesa->RealWidth);
    DEBUGOUT(1, " amesa->RealHeight  = %d\n", amesa->RealHeight);
    DEBUGOUT(1, " amesa->FixedWidth  = %d\n", amesa->FixedWidth);
    DEBUGOUT(1, " amesa->FixedHeight = %d\n", amesa->FixedHeight);
    DEBUGOUT(1, " amesa->width       = %d\n", amesa->width);
    DEBUGOUT(1, " amesa->height      = %d\n", amesa->height);
    DEBUGOUT(1, " amesa->left        = %d\n", amesa->left);
    DEBUGOUT(1, " amesa->bottom      = %d\n", amesa->bottom);
    DEBUGOUT(1, " amesa->right       = %d\n", amesa->right);
    DEBUGOUT(1, " amesa->top         = %d\n", amesa->top);
    DEBUGOUT(1, " amesa->depth       = %d\n", amesa->depth);
  }
}

/**********************************************************************/

void cmnStandardDispose(amigaMesaContext amesa)
{
  DEBUGOUT(1, "cmnStandardDispose(0x%08x)\n", amesa);

#ifdef	AOS_WARP3D
  /* do Warp3D */
  if (amesa->TDdriver)
    wrpStandardDispose(amesa);
#endif

  Forbid();									/* atomize Dispose */

  /* in case of break before glEnd(), as it is not atomic this could be a leak */
  if (amesa->lastAction) {
    freeArea(amesa, amesa->rp);
    destroyTempRaster(amesa, amesa->rp);
  }

  FreeCMap(amesa, amesa->Screen);
  if (amesa->depth <= 8)
    freeTempRPort(amesa);
  FreeOneLine(amesa);

  if (amesa->back_rp) {
    if (amesa->rp == amesa->back_rp)
      amesa->rp = amesa->front_rp;
    destroyRPort(amesa, amesa->back_rp);
    amesa->back_rp = NULL;
  }

  Permit();
}

void cmnStandardDisposeDB(amigaMesaContext amesa)
{
  DEBUGOUT(1, "cmnStandardDisposeDB(0x%08x)\n", amesa);

#ifdef	AOS_WARP3D
  /* do Warp3D */
  if (amesa->TDdriver)
    wrpStandardDisposeDB(amesa);
#endif

  Forbid();									/* atomize DisposeDB */

  FreeCMap(amesa, amesa->Screen);
  if (amesa->depth <= 8)
    freeTempRPort(amesa);

  destroyPenBackArray(amesa, amesa->BackArray);
  amesa->BackArray = NULL;

  Permit();
}

/**********************************************************************/

GLboolean cmnStandardInit(amigaMesaContext amesa, struct TagItem *tagList)
{
  struct Screen *oldScreen = amesa->Screen;
  GLint newModeID;
  GLboolean TDMode = GL_FALSE;

  DEBUGOUT(1, "cmnStandardInit(0x%08x, 0x%08x)\n", amesa, tagList);

  if ((amesa->left = GetTagData(AMA_Left, amesa->left, tagList)) < 0)
    return GL_FALSE;
  if ((amesa->right = GetTagData(AMA_Right, amesa->right, tagList)) < 0)
    return GL_FALSE;
  if ((amesa->top = GetTagData(AMA_Top, amesa->top, tagList)) < 0)
    return GL_FALSE;
  if ((amesa->bottom = GetTagData(AMA_Bottom, amesa->bottom, tagList)) < 0)
    return GL_FALSE;

  if (!(amesa->window = (struct Window *)GetTagData(AMA_Window, (ULONG)amesa->window, tagList))) {
    if (!(amesa->Screen = (struct Screen *)GetTagData(AMA_Screen, (ULONG)amesa->Screen, tagList))) {
      DEBUGOUT(0, "cmnStandardInit: missing screen\n");
      LastError = AMESA_SCREEN_TAG_MISSING;
      return (GL_FALSE);
    }
    if (!(amesa->rp = (struct RastPort *)GetTagData(AMA_RastPort, (ULONG)amesa->rp, tagList))) {
      DEBUGOUT(0, "cmnStandardInit: missing rastport (don't use Screens rastport without verification!)\n");
      LastError = AMESA_RASTPORT_TAG_MISSING;
      return (GL_FALSE);
    }
  }
  else {
    amesa->rp = amesa->window->RPort;
    amesa->Screen = amesa->window->WScreen;

    if (amesa->left < amesa->window->BorderLeft)
      amesa->left = amesa->window->BorderLeft;
    if (amesa->right < amesa->window->BorderRight)
      amesa->right = amesa->window->BorderRight;
    if (amesa->top < amesa->window->BorderTop)
      amesa->top = amesa->window->BorderTop;
    if (amesa->bottom < amesa->window->BorderBottom)
      amesa->bottom = amesa->window->BorderBottom;
  }

	newModeID = GetVPModeID(&amesa->Screen->ViewPort);
  if (CyberGfxBase && IsCyberModeID(newModeID)) {
#ifdef	AOS_WARP3D
    if (Warp3DBase) {
      W3D_Driver *checkMode;
      int pixFmt;

      pixFmt = GetCyberMapAttr(amesa->rp->BitMap, CYBRMATTR_PIXFMT);
      if ((checkMode = W3D_TestMode(newModeID))) {
	switch (pixFmt) {
	  case PIXFMT_RGB15:	TDMode = (checkMode->formats & W3D_FMT_R5G5B5  );	break;
	  case PIXFMT_BGR15:	TDMode = (checkMode->formats & W3D_FMT_B5G5R5  );	break;
	  case PIXFMT_RGB15PC:	TDMode = (checkMode->formats & W3D_FMT_R5G5B5PC);	break;
	  case PIXFMT_BGR15PC:	TDMode = (checkMode->formats & W3D_FMT_B5G5R5PC);	break;
	  case PIXFMT_RGB16:	TDMode = (checkMode->formats & W3D_FMT_R5G6B5  );  	break;
	  case PIXFMT_BGR16:	TDMode = (checkMode->formats & W3D_FMT_B5G6R5  );	break;
	  case PIXFMT_RGB16PC:	TDMode = (checkMode->formats & W3D_FMT_R5G6B5PC);	break;
	  case PIXFMT_BGR16PC:	TDMode = (checkMode->formats & W3D_FMT_B5G6R5PC);	break;
	  case PIXFMT_RGB24:	TDMode = (checkMode->formats & W3D_FMT_R8G8B8  );	break;
	  case PIXFMT_BGR24:	TDMode = (checkMode->formats & W3D_FMT_B8G8R8  );	break;
	  case PIXFMT_ARGB32:	TDMode = (checkMode->formats & W3D_FMT_A8R8G8B8);	break;
	  case PIXFMT_BGRA32:	TDMode = (checkMode->formats & W3D_FMT_B8G8R8A8);	break;
	  case PIXFMT_RGBA32:	TDMode = (checkMode->formats & W3D_FMT_R8G8B8A8);	break;
	  case PIXFMT_LUT8:
	  default:									break;
	}
      }
    }
#endif

    amesa->depth = GetCyberMapAttr(amesa->rp->BitMap, CYBRMATTR_DEPTH);
  }
  else
    amesa->depth = GetBitMapAttr(amesa->rp->BitMap, BMA_DEPTH);

  amesa->RealWidth = 
  amesa->FixedWidth = LayerWidth(amesa->rp->Layer);
  amesa->RealHeight = 
  amesa->FixedHeight = LayerHeight(amesa->rp->Layer);

  if ((amesa->width = GetTagData(AMA_Width, amesa->RealWidth - amesa->left - amesa->right, tagList)) < 0)
    return GL_FALSE;
  if ((amesa->height = GetTagData(AMA_Height, amesa->RealHeight - amesa->bottom - amesa->top, tagList)) < 0)
    return GL_FALSE;

  if (!amesa->right)
    amesa->right = amesa->RealWidth - amesa->left - amesa->width;
  if (!amesa->top)
    amesa->top = amesa->RealHeight - amesa->bottom - amesa->height;

  amesa->front_rp = amesa->rp;
  amesa->back_rp = NULL;
/*amesa->rp = amesa->front_rp; */

/*amesa->gl_ctx->BufferWidth = amesa->width; */
/*amesa->gl_ctx->BufferHeight = amesa->height; */

  amesa->gl_ctx->Color.DrawBuffer = GL_FRONT;
  if (TstF(amesa->visual->flags, VISUAL_DOUBLEBUFFER)) {
    destroyRPort(amesa, amesa->back_rp);
    amesa->gl_ctx->Color.DrawBuffer = GL_BACK;
    if (!(amesa->rp = amesa->back_rp = makeRPort(amesa, amesa->RealWidth, amesa->RealHeight, amesa->depth, 0, amesa->front_rp->BitMap))) {
      amesa->rp = amesa->front_rp;
      amesa->gl_ctx->Color.DrawBuffer = GL_FRONT;
   /* amesa->visual->flags ^= VISUAL_DOUBLEBUFFER; */
      DEBUGOUT(0, "cmnStandardInit: Not enough memory to allocate back-Rastport\n");
    }
  }

#ifdef	AOS_WARP3D
  /* TDMode isn't true on missing Warp3DBase, allocate Warp3D-context */
  if (TDMode) {
    struct TDdriver *TDdriver;
    W3D_Context *TDcontext;

    if ((TDdriver = AllocVecPooled(amesaPool, sizeof(struct TDdriver)))) {
      if ((TDcontext = W3D_CreateContextTags(NULL,
					   W3D_CC_BITMAP,	amesa->rp->BitMap,	/* use the render-bitmap */
					   W3D_CC_YOFFSET,	0,
					   W3D_CC_DRIVERTYPE,	W3D_DRIVER_3DHW,
					   W3D_CC_GLOBALTEXENV,	TRUE,
					   W3D_CC_INDIRECT,	TRUE,
					   W3D_CC_FAST,		TRUE,
					   TAG_DONE, 		NULL))) {
	TDdriver->envmode = W3D_MODULATE;
	TDdriver->wscale = 1.0;
	TDdriver->far = 1.0;

	if (W3D_Query(TDcontext, W3D_Q_RECTTEXTURES, NULL) == W3D_FULLY_SUPPORTED)
	  SetF(TDdriver->flags, TD_RECTTEX);
	W3D_SetTexEnv(TDcontext, NULL, TDdriver->envmode, &TDdriver->envcolor);
	if (W3D_GetDriverState(TDcontext) == W3D_SUCCESS)
	  SetF(TDdriver->flags, TD_ACTIVE);

	if (TstF(amesa->flags, PALETTE_DITHER))
	  W3D_SetState(amesa->TDdriver->td_ctx, W3D_DITHERING, W3D_ENABLE );
	else
	  W3D_SetState(amesa->TDdriver->td_ctx, W3D_DITHERING, W3D_DISABLE);

	TDdriver->td_ctx = TDcontext;
	amesa->TDdriver = TDdriver;
      }
      else
	FreeVecPooled(amesaPool, (ULONG *)TDdriver);
    }
  }
#endif

  ReallocOneLine("cmnStandardInit", amesa);
  if (amesa->depth <= 8)
    ReallocTempRPort("cmnStandardInit", amesa, 0);

  /* CMaps are even allocated if we are in high-color modes to allow fast CI32 reading
   * if moved from one screen to another free old pens
   */
  if (oldScreen)
    FreeCMap(amesa, oldScreen);
  AllocCMap(amesa, amesa->Screen);
  /* rethink the ItoP/PtoI if the screen changes */
  if (oldScreen && (oldScreen != amesa->Screen))
    RethinkCMap(amesa);

  /* fast FIXx/FIXy/FIXxy */
  if (!makeFixedXY(amesa))
    Error("cmnStandardInit: Not enough memory to allocate FixedXY\n");

  amesa->pixel = 								/* current drawing pen  */
  amesa->clearpixel = MakeRGBP(amesa, 0, 0, 0, 0, 0);				/* current clear pen */

  amesa->InitDD = amesa->depth <= 8 ? natStandardDDPointers : cybStandardDDPointers;	/*  standard drawing */
  amesa->Dispose = cmnStandardDispose;
  amesa->SwapBuffer = cmnStandardSwapBuffer;
  (*amesa->InitDD) (amesa->gl_ctx);

  DEBUGOUT(1, " amesa->RealWidth   = %d\n", amesa->RealWidth);
  DEBUGOUT(1, " amesa->RealHeight  = %d\n", amesa->RealHeight);
  DEBUGOUT(1, " amesa->FixedWidth  = %d\n", amesa->FixedWidth);
  DEBUGOUT(1, " amesa->FixedHeight = %d\n", amesa->FixedHeight);
  DEBUGOUT(1, " amesa->width       = %d\n", amesa->width);
  DEBUGOUT(1, " amesa->height      = %d\n", amesa->height);
  DEBUGOUT(1, " amesa->left        = %d\n", amesa->left);
  DEBUGOUT(1, " amesa->bottom      = %d\n", amesa->bottom);
  DEBUGOUT(1, " amesa->right       = %d\n", amesa->right);
  DEBUGOUT(1, " amesa->top         = %d\n", amesa->top);
  DEBUGOUT(1, " amesa->depth       = %d\n", amesa->depth);

  return (GL_TRUE);
}

GLboolean cmnStandardInitDB(amigaMesaContext amesa, struct TagItem * tagList)
{
  struct Screen *oldScreen = amesa->Screen;

  DEBUGOUT(1, "cmnStandardInitDB(0x%08x, 0x%08x)\n", amesa, tagList);

  if ((amesa->left = GetTagData(AMA_Left, amesa->left, tagList)) < 0)
    return GL_FALSE;
  if ((amesa->right = GetTagData(AMA_Right, amesa->right, tagList)) < 0)
    return GL_FALSE;
  if ((amesa->top = GetTagData(AMA_Top, amesa->top, tagList)) < 0)
    return GL_FALSE;
  if ((amesa->bottom = GetTagData(AMA_Bottom, amesa->bottom, tagList)) < 0)
    return GL_FALSE;

  if (!(amesa->window = (struct Window *)GetTagData(AMA_Window, (ULONG)amesa->window, tagList))) {
    if (!(amesa->Screen = (struct Screen *)GetTagData(AMA_Screen, (ULONG)amesa->Screen, tagList))) {
      DEBUGOUT(0, "cmnStandardInitDB: missing screen\n");
      LastError = AMESA_SCREEN_TAG_MISSING;
      return (GL_FALSE);
    }
    if (!(amesa->rp = (struct RastPort *)GetTagData(AMA_RastPort, (ULONG)amesa->rp, tagList))) {
      DEBUGOUT(0, "cmnStandardInitDB: missing rastport (don't use Screens rastport without verification!)\n");
      LastError = AMESA_RASTPORT_TAG_MISSING;
      return (GL_FALSE);
    }
  }
  else {
    amesa->rp = amesa->window->RPort;
    amesa->Screen = amesa->window->WScreen;

    if (amesa->left < amesa->window->BorderLeft)
      amesa->left = amesa->window->BorderLeft;
    if (amesa->right < amesa->window->BorderRight)
      amesa->right = amesa->window->BorderRight;
    if (amesa->top < amesa->window->BorderTop)
      amesa->top = amesa->window->BorderTop;
    if (amesa->bottom < amesa->window->BorderBottom)
      amesa->bottom = amesa->window->BorderBottom;
  }

  if (CyberGfxBase && IsCyberModeID(GetVPModeID(&amesa->Screen->ViewPort)))
    amesa->depth = GetCyberMapAttr(amesa->rp->BitMap, CYBRMATTR_DEPTH);
  else
    amesa->depth = GetBitMapAttr(amesa->rp->BitMap, BMA_DEPTH);

  amesa->RealWidth = LayerWidth(amesa->rp->Layer);
  amesa->RealHeight = LayerHeight(amesa->rp->Layer);
  if (amesa->depth <= 8)
    amesa->FixedWidth = natFixedMask(amesa->RealWidth);
  else
    amesa->FixedWidth = cybFixedMask(amesa->RealWidth);
  amesa->FixedHeight = amesa->RealHeight;

  if ((amesa->width = GetTagData(AMA_Width, amesa->RealWidth - amesa->left - amesa->right, tagList)) < 0)
    return GL_FALSE;
  if ((amesa->height = GetTagData(AMA_Height, amesa->RealHeight - amesa->bottom - amesa->top, tagList)) < 0)
    return GL_FALSE;

  if (!amesa->right)
    amesa->right = amesa->RealWidth - amesa->left - amesa->width;
  if (!amesa->top)
    amesa->top = amesa->RealHeight - amesa->bottom - amesa->height;

  amesa->front_rp = amesa->rp;
  amesa->back_rp = NULL;
/*amesa->rp = amesa->front_rp; */

/*amesa->gl_ctx->BufferWidth = amesa->width; */
/*amesa->gl_ctx->BufferHeight = amesa->height; */

  amesa->gl_ctx->Color.DrawBuffer = GL_FRONT;
  if (TstF(amesa->visual->flags, VISUAL_DOUBLEBUFFER)) {
    amesa->gl_ctx->Color.DrawBuffer = GL_BACK;
    ReallocPenBackArray("cmnStandardInitDB", amesa);
  }

  if (amesa->depth <= 8)
    ReallocTempRPort("cmnStandardInitDB", amesa, 0);

  /* CMaps are even allocated if we are in high-color modes to allow fast CI32 reading */
  /* if moved from one screen to another free old pens */
  if (oldScreen)
    FreeCMap(amesa, oldScreen);
  AllocCMap(amesa, amesa->Screen);
  /* rethink the ItoP/PtoI if the screen changes */
  if (oldScreen && (oldScreen != amesa->Screen))
    RethinkCMap(amesa);

  /* fast FIXx/FIXy/FIXxy */
  if (!makeFixedXY(amesa))
    Error("cmnStandardInitDB: Not enough memory to allocate FixedXY\n");

  amesa->pixel = 								/* current drawing pen  */
  amesa->clearpixel = MakeRGBP(amesa, 0, 0, 0, 0, 0);				/* current clear pen */

  amesa->InitDD = amesa->depth <= 8 ? natFasterDDPointers : cybFasterDDPointers;	/*  fast drawing */
  amesa->Dispose = cmnStandardDisposeDB;
  amesa->SwapBuffer = cmnStandardSwapBufferDB;
  (*amesa->InitDD) (amesa->gl_ctx);

  DEBUGOUT(1, " amesa->RealWidth   = %d\n", amesa->RealWidth);
  DEBUGOUT(1, " amesa->RealHeight  = %d\n", amesa->RealHeight);
  DEBUGOUT(1, " amesa->FixedWidth  = %d\n", amesa->FixedWidth);
  DEBUGOUT(1, " amesa->FixedHeight = %d\n", amesa->FixedHeight);
  DEBUGOUT(1, " amesa->width       = %d\n", amesa->width);
  DEBUGOUT(1, " amesa->height      = %d\n", amesa->height);
  DEBUGOUT(1, " amesa->left        = %d\n", amesa->left);
  DEBUGOUT(1, " amesa->bottom      = %d\n", amesa->bottom);
  DEBUGOUT(1, " amesa->right       = %d\n", amesa->right);
  DEBUGOUT(1, " amesa->top         = %d\n", amesa->top);
  DEBUGOUT(1, " amesa->depth       = %d\n", amesa->depth);

  return (GL_TRUE);
}
