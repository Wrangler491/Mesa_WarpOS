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

static GLint ilog2( GLint n );

typedef struct {
  W3D_Texture *texture;
  void *teximage;
  ULONG W3Dformat;
  ULONG mmask;
  int width;
  int height;
  ULONG minfilter;
  ULONG magfilter;
  ULONG wrap_s;
  ULONG wrap_t;
  W3D_Bool converted;
  W3D_Bool hasMipmaps;
  W3D_Bool MipMapFilter;
  W3D_Bool MipMapTexture;
  W3D_Bool dirtymm;
} W3Dtexobj;

typedef struct {
  GLubyte *conv_image;
  ULONG W3DMipmaplevel;
  W3D_Bool isW3DMap;
} W3Dteximg;

void wrpDeleteTexture(GLcontext * ctx, struct gl_texture_object *tObj)
{
  struct TDdriver *TDdriver;
  W3D_Context *TDcontext;
  W3Dtexobj *to = (W3Dtexobj *) tObj->DriverData;
  W3Dteximg *ti;
  GLint i;

  DEBUGOUT(1, "wrpDeleteTexture()\n");

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;

  if (to && to->texture)
    W3D_FreeTexObj(TDcontext, to->texture);

  for (i = 0; i < MAX_TEXTURE_LEVELS; i++) {
    if (tObj->Image[i]) {
      ti = ((W3Dteximg *) (tObj->Image[i]->DriverData));

      if (ti && ti->conv_image)
	FreeVec(ti->conv_image);
      if (ti)
	FreeVec(ti);

      tObj->Image[i]->DriverData = NULL;
    }
  }

  if (to) {
    FreeVec(to);
    tObj->DriverData = NULL;
  }
}

void wrpBindTexture(GLcontext * ctx, GLenum target,
		    struct gl_texture_object *tObj)
{
  struct gl_texture_image *timg, *timg0;
  struct TDdriver *TDdriver;
  W3D_Context *TDcontext;
  W3Dtexobj *to = (W3Dtexobj *) tObj->DriverData;
  W3Dteximg *ti, *ti0;
  GLint i, width, height;
  GLint level = 0;
  GLuint mask = 0xffffffff;
  void *teximage;
  void *mipmapptrs[MAX_TEXTURE_LEVELS];
  void **mptrs = mipmapptrs;

  DEBUGOUT(1, "wrpDeleteTexture()\n");

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;
  TDdriver->GLtex = NULL;

  if (!to) {
    if (!(to = tObj->DriverData = AllocVecPooled(amesaPool, sizeof(W3Dtexobj))))
      return;
    bzero(to, sizeof(W3Dtexobj));

    to->hasMipmaps = FALSE;
    to->minfilter = W3D_NEAREST;
    to->magfilter = W3D_LINEAR;
    to->wrap_s = W3D_REPEAT;
    to->wrap_t = W3D_REPEAT;
    to->MipMapFilter = FALSE;
    to->dirtymm = TRUE;
  }

  if (!to->texture) {
    if (!(timg0 = tObj->Image[0]))
      return;

    if (!(ti0 = (W3Dteximg *) (timg0->DriverData)))
      return;

    ti0->isW3DMap = TRUE;
    ti0->W3DMipmaplevel = 0;

    if (to->hasMipmaps) {
      width = timg0->Width;
      height = timg0->Height;

      if ((ilog2(width) < 0) || (ilog2(height) < 0))
	return;

      if ((width != height) && (!TstF(TDdriver->flags, TD_RECTTEX)))
	return;

      while ((width >> 1) && (height >> 1)) {
	width >>= 1;
	height >>= 1;
	level++;

	for (i = 0; i < MAX_TEXTURE_LEVELS; i++) {
	  timg = tObj->Image[i];
	  ti = (W3Dteximg *) (timg->DriverData);

	  if ((timg->Width == width) && (timg->Height == height)) {
	    ti->isW3DMap = TRUE;
	    ti->W3DMipmaplevel = level;
	    mask &= (~(1 << (level - 1)));

	    if (to->converted)
	      teximage = (void *)ti->conv_image;
	    else
	      teximage = (void *)timg->Data;

	    *mptrs++ = teximage;
	    break;
	  }
	}
      }
    }

    *mptrs++ = NULL;

    if (to->converted)
      teximage = (void *)ti0->conv_image;
    else
      teximage = (void *)timg0->Data;

    if (to->MipMapFilter || to->hasMipmaps) {
      GLuint rc;

      if (!(to->texture = W3D_AllocTexObjTags(TDcontext, &rc,
					      W3D_ATO_IMAGE, (ULONG)teximage,
					      W3D_ATO_FORMAT, to->W3Dformat,
					      W3D_ATO_WIDTH, timg0->Width,
					      W3D_ATO_HEIGHT, timg0->Height,
					      W3D_ATO_MIPMAP, mask,
					      W3D_ATO_MIPMAPPTRS, (ULONG)mptrs,
					      TAG_DONE, NULL))) {
	return;
      }

      to->MipMapTexture = TRUE;
    }
    else {
      if (!(to->texture = W3D_AllocTexObjTags(TDcontext, NULL,
					      W3D_ATO_IMAGE, (ULONG)teximage,
					      W3D_ATO_FORMAT, to->W3Dformat,
					      W3D_ATO_WIDTH, timg0->Width,
					      W3D_ATO_HEIGHT, timg0->Height,
					      TAG_DONE, NULL))) {
	return;
      }

      to->MipMapTexture = FALSE;
    }

    to->teximage = teximage;
    to->mmask = mask;
    to->width = timg0->Width;
    to->height = timg0->Height;

    W3D_SetFilter(TDcontext, to->texture, to->minfilter, to->magfilter);
    W3D_SetWrapMode(TDcontext, to->texture, to->wrap_s, to->wrap_t, NULL);
  }

  TDdriver->GLtex = tObj;
  TDdriver->TDtex = to->texture;
  TDdriver->texwidth = (W3D_Float) to->width;
  TDdriver->texheight = (W3D_Float) to->height;
}

void wrpTexParameter(GLcontext * ctx, GLenum target,
		     struct gl_texture_object *tObj,
		     GLenum pname, const GLfloat * params)
{
  struct TDdriver *TDdriver;
  W3D_Context *TDcontext;
  W3Dtexobj *to;
  GLenum param = (GLenum) (GLint) params[0];
  GLboolean newFilter = GL_FALSE;
  GLboolean newWrap = GL_FALSE;

  DEBUGOUT(1, "wrpDeleteTexture()\n");

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;

  if (target != GL_TEXTURE_2D)
    return;

  if (!(to = (W3Dtexobj *) tObj->DriverData)) {
    if (!(to = tObj->DriverData = AllocVecPooled(amesaPool, sizeof(W3Dtexobj))))
      return;
    bzero(to, sizeof(W3Dtexobj));

    to->hasMipmaps = FALSE;
    to->minfilter = W3D_NEAREST;
    to->magfilter = W3D_LINEAR;
    to->wrap_s = W3D_REPEAT;
    to->wrap_t = W3D_REPEAT;
    to->MipMapFilter = FALSE;
    to->dirtymm = TRUE;
  }

  switch (pname) {
    case GL_TEXTURE_MIN_FILTER:
      newFilter = GL_TRUE;
      to->dirtymm = FALSE;

      switch (param) {
	case GL_NEAREST:
	  to->MipMapFilter = FALSE;
	  to->minfilter = W3D_NEAREST;
	  break;
	case GL_LINEAR:
	  to->MipMapFilter = FALSE;
	  to->minfilter = W3D_LINEAR;
	  break;
	case GL_NEAREST_MIPMAP_NEAREST:
	  to->MipMapFilter = TRUE;
	  to->minfilter = W3D_NEAREST_MIP_NEAREST;
	  break;
	case GL_LINEAR_MIPMAP_NEAREST:
	  to->MipMapFilter = TRUE;
	  to->minfilter = W3D_LINEAR_MIP_NEAREST;
	  break;
	case GL_NEAREST_MIPMAP_LINEAR:
	  to->MipMapFilter = TRUE;
	  to->minfilter = W3D_NEAREST_MIP_LINEAR;
	  break;
	case GL_LINEAR_MIPMAP_LINEAR:
	  to->MipMapFilter = TRUE;
	  to->minfilter = W3D_LINEAR_MIP_LINEAR;
	default:
	  break;
      }
      break;

    case GL_TEXTURE_MAG_FILTER:
      newFilter = GL_TRUE;
      to->dirtymm = FALSE;
      switch (param) {
	case GL_NEAREST:
	  to->magfilter = W3D_NEAREST;
	  break;
	case GL_LINEAR:
	  to->magfilter = W3D_LINEAR;
	default:
	  break;
      }
      break;

    case GL_TEXTURE_WRAP_S:
      newWrap = GL_TRUE;
      switch (param) {
	case GL_CLAMP:
	  to->wrap_s = W3D_CLAMP;
	  break;
	case GL_REPEAT:
	  to->wrap_s = W3D_REPEAT;
	default:
	  break;
      }
      break;
    case GL_TEXTURE_WRAP_T:
      newWrap = GL_TRUE;
      switch (param) {
	case GL_CLAMP:
	  to->wrap_t = W3D_CLAMP;
	  break;
	case GL_REPEAT:
	  to->wrap_t = W3D_REPEAT;
	default:
	  break;
      }
    default:
      break;
  }

  if (to->texture) {
    if (newFilter) {
      if ((!(to->MipMapTexture)) && (to->MipMapFilter)) {
	W3D_Texture *wtobj = to->texture;

	W3D_FreeTexObj(TDcontext, to->texture);
	if (!(to->texture = W3D_AllocTexObjTags(TDcontext, NULL,
						W3D_ATO_IMAGE, (ULONG)to->teximage,
						W3D_ATO_FORMAT, to->W3Dformat,
						W3D_ATO_WIDTH, to->width,
						W3D_ATO_HEIGHT, to->height,
						W3D_ATO_MIPMAP, 0xffffffff,
						W3D_ATO_MIPMAPPTRS, NULL,
						TAG_DONE, NULL))) {
	  return;
	}

	to->MipMapTexture = TRUE;
	newWrap = TRUE;

	if (wtobj == TDdriver->TDtex)
	  TDdriver->TDtex = to->texture;
      }

      W3D_SetFilter(TDcontext, to->texture, to->minfilter, to->magfilter);
    }

    if (newWrap)
      W3D_SetWrapMode(TDcontext, to->texture, to->wrap_s, to->wrap_t, NULL);
  }
}

GLboolean wrpConvertTexture(W3D_Context * TDcontext, W3Dtexobj * to,
			    W3Dteximg * ti,
			    const struct gl_texture_image *image,
			    GLint internalformat, GLint xoffset, GLint yoffset,
			    GLsizei width, GLsizei height, int *bppix)
{
  switch (internalformat) {
    case GL_ALPHA:
    case GL_ALPHA4:
    case GL_ALPHA8:
    case GL_ALPHA12:
    case GL_ALPHA16:
      if (W3D_GetTexFmtInfo(TDcontext, W3D_A8, 0) & W3D_TEXFMT_UNSUPPORTED)
	return GL_FALSE;
      to->W3Dformat = W3D_A8;
      to->converted = FALSE;
      *bppix = 1;
      return GL_TRUE;
    case 1:
    case GL_LUMINANCE:
    case GL_LUMINANCE4:
    case GL_LUMINANCE8:
    case GL_LUMINANCE12:
    case GL_LUMINANCE16:
      if (W3D_GetTexFmtInfo(TDcontext, W3D_L8, 0) & W3D_TEXFMT_UNSUPPORTED)
	return GL_FALSE;
      to->W3Dformat = W3D_L8;
      to->converted = FALSE;
      *bppix = 1;
      return GL_TRUE;
    case 2:
    case GL_LUMINANCE_ALPHA:
    case GL_LUMINANCE4_ALPHA4:
    case GL_LUMINANCE6_ALPHA2:
    case GL_LUMINANCE8_ALPHA8:
    case GL_LUMINANCE12_ALPHA4:
    case GL_LUMINANCE12_ALPHA12:
    case GL_LUMINANCE16_ALPHA16:
      if (W3D_GetTexFmtInfo(TDcontext, W3D_L8A8, 0) & W3D_TEXFMT_UNSUPPORTED)
	return GL_FALSE;
      to->W3Dformat = W3D_L8A8;
      to->converted = FALSE;
      *bppix = 2;
      return GL_TRUE;
    case GL_INTENSITY:
    case GL_INTENSITY4:
    case GL_INTENSITY8:
    case GL_INTENSITY12:
    case GL_INTENSITY16:
      if (W3D_GetTexFmtInfo(TDcontext, W3D_I8, 0) & W3D_TEXFMT_UNSUPPORTED)
	return GL_FALSE;
      to->W3Dformat = W3D_I8;
      to->converted = FALSE;
      *bppix = 1;
      return GL_TRUE;
    case 3:
    case GL_RGB:
    case GL_R3_G3_B2:
    case GL_RGB4:
    case GL_RGB5:
    case GL_RGB8:
    case GL_RGB10:
    case GL_RGB12:
    case GL_RGB16:
      if (W3D_GetTexFmtInfo(TDcontext, W3D_R8G8B8, 0) & W3D_TEXFMT_UNSUPPORTED)
	return GL_FALSE;
      to->W3Dformat = W3D_R8G8B8;
      to->converted = FALSE;
      *bppix = 3;
      return GL_TRUE;
    case 4:
    case GL_RGBA:
    case GL_RGBA2:
    case GL_RGBA4:
    case GL_RGB5_A1:
    case GL_RGBA8:
    case GL_RGB10_A2:
    case GL_RGBA12:
    case GL_RGBA16:
      if (W3D_GetTexFmtInfo(TDcontext, W3D_R8G8B8A8, 0) & W3D_TEXFMT_UNSUPPORTED)
	return GL_FALSE;
      to->W3Dformat = W3D_R8G8B8A8;
      to->converted = FALSE;
      *bppix = 4;
      return GL_TRUE;
    case GL_COLOR_INDEX1_EXT:
    case GL_COLOR_INDEX2_EXT:
    case GL_COLOR_INDEX4_EXT:
    case GL_COLOR_INDEX8_EXT:
    case GL_COLOR_INDEX12_EXT:
    case GL_COLOR_INDEX16_EXT:
    default:
      return GL_FALSE;
  }
}

void wrpTexImage_generic(GLcontext * ctx, GLenum target,
			 struct gl_texture_object *tObj, GLint level,
			 GLint xoffset, GLint yoffset,
			 GLsizei width, GLsizei height,
			 GLint internalFormat,
			 struct gl_texture_image *image,
			 GLboolean fullUpdate)
{
  struct TDdriver *TDdriver;
  W3D_Context *TDcontext;
  W3Dtexobj *to;
  W3Dteximg *ti;
  ULONG wlevel = 0;
  GLint bppix;

  DEBUGOUT(1, "wrpDeleteTexture()\n");

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;

  if (target != GL_TEXTURE_2D)
    return;

  /* Currently I don't want to write an emulation of rectangular textures,
   * and they aren't anyway used much
   */
  if ((image->Width != image->Height) && (!TstF(TDdriver->flags, TD_RECTTEX)))
    return;

  /* this palette stuff annoys me... but actually almost no demos use
   * the paletted textures extension
   */
  if (image->Format == GL_COLOR_INDEX)
    return;

  if (!(to = (W3Dtexobj *) tObj->DriverData)) {
    if (!(to = tObj->DriverData = AllocVecPooled(amesaPool, sizeof(W3Dtexobj))))
      return;
    bzero(to, sizeof(W3Dtexobj));

    to->hasMipmaps = FALSE;
    to->minfilter = W3D_NEAREST;
    to->magfilter = W3D_LINEAR;
    to->wrap_s = W3D_REPEAT;
    to->wrap_t = W3D_REPEAT;
    to->MipMapFilter = FALSE;
    to->dirtymm = TRUE;
  }

  if (level)
    to->hasMipmaps = TRUE;

  if (!(ti = (W3Dteximg *) image->DriverData)) {
    /* TODO: this structure is not freed, if several images are assigned
     * to the same texture object
     */
    if (!(ti = image->DriverData = AllocVecPooled(amesaPool, sizeof(W3Dteximg))))
      return;
    bzero(ti, sizeof(W3Dteximg));

    if (level)
      ti->isW3DMap = FALSE;
    else
      ti->isW3DMap = TRUE;

    while (level) {
      int lw, lh, tolw, tolh;

      if (!(to->texture))
	break;

      if (((lw = ilog2(image->Width)) < 0) || ((lh = ilog2(image->Height)) < 0))
	break;

      if ((image->Width != image->Height) && (!TstF(TDdriver->flags, TD_RECTTEX)))
	break;

      tolw = ilog2(to->width);
      tolh = ilog2(to->height);

      if ((tolw - lw) != (tolh - lh))
	break;

      if (!((to->mmask) & (1 << ((tolw - lw) - 1))))				/* urgs */
	break;

      /* What the hell is all this? Well, I'd like to now, if this
       * image can serve as W3D mipmap level, in which case it
       * replaces the generated one by W3D */
      ti->isW3DMap = TRUE;
      wlevel = ti->W3DMipmaplevel = tolw - lw;

      break;
    }
  }

  /* if no filter was set up to now and a mipmap level was given, then set
   * the filter mode to the correct initial state
   */
  if (level && ti->isW3DMap && to->dirtymm)
    to->minfilter = W3D_NEAREST_MIP_LINEAR;

  if (!wrpConvertTexture(TDcontext, to, ti, image, internalFormat, xoffset, yoffset, width, height, &bppix))
    return;									/* unsupported texture format */

  if (to->texture && ti->isW3DMap) {
    void *img;

    if (to->converted)
      img = (void *)ti->conv_image;
    else
      img = (void *)image->Data;

    if (!level)
      to->teximage = img;

    /* case 1: texture was not a MipMap-Texture and a mipmap level was
     * created. reallocate texture as MipMap-Texture
     */
    if ((!(to->MipMapTexture)) && level) {
      W3D_Texture *wtobj = to->texture;

      W3D_FreeTexObj(TDcontext, to->texture);

      if (!(to->texture = W3D_AllocTexObjTags(TDcontext, NULL,
					      W3D_ATO_IMAGE, (ULONG)to->teximage,
					      W3D_ATO_FORMAT, to->W3Dformat,
					      W3D_ATO_WIDTH, to->width,
					      W3D_ATO_HEIGHT, to->height,
					      W3D_ATO_MIPMAP, ~0,
					      W3D_ATO_MIPMAPPTRS, NULL,
					      TAG_DONE, NULL))) {
	return;
      }

      to->mmask = ~0;
      to->MipMapTexture = TRUE;
      W3D_SetFilter(TDcontext, to->texture, to->minfilter, to->magfilter);
      W3D_SetWrapMode(TDcontext, to->texture, to->wrap_s, to->wrap_t, NULL);

      if (wtobj == TDdriver->TDtex) {
	TDdriver->TDtex = to->texture;
	TDdriver->texwidth = (W3D_Float) to->width;
	TDdriver->texheight = (W3D_Float) to->height;
      }
    }

    if (!level && ((image->Width != to->width) || (image->Height != to->height))) {
      W3D_Texture *wtobj = to->texture;

      W3D_FreeTexObj(TDcontext, to->texture);
      if (!(to->MipMapTexture)) {

        /* case 2: texture dimension has changed and texture is not a mipmap texture.
         * just reallocate the texture
         */
	if (!(to->texture = W3D_AllocTexObjTags(TDcontext, NULL,
						W3D_ATO_IMAGE, (ULONG)to->teximage,
						W3D_ATO_FORMAT, to->W3Dformat,
						W3D_ATO_WIDTH, image->Width,
						W3D_ATO_HEIGHT, image->Height,
						TAG_DONE, NULL))) {
	  return;
	}

	to->width = image->Width;
	to->height = image->Height;
      }
      else {
	GLuint error;

        /* case 3: texture dimension has changed and texture was a mipmap texture.
         * reallocate the texture and regenerate all mipmaps
         */
	if (!(to->texture = W3D_AllocTexObjTags(TDcontext, &error,
						W3D_ATO_IMAGE, (ULONG)to->teximage,
						W3D_ATO_FORMAT, to->W3Dformat,
						W3D_ATO_WIDTH, image->Width,
						W3D_ATO_HEIGHT, image->Height,
						W3D_ATO_MIPMAP, ~0,
						W3D_ATO_MIPMAPPTRS, NULL,
						TAG_DONE, NULL))) {
	  return;
	}

	to->width = image->Width;
	to->height = image->Height;
	to->mmask = ~0;
      }

      W3D_SetFilter(TDcontext, to->texture, to->minfilter, to->magfilter);
      W3D_SetWrapMode(TDcontext, to->texture, to->wrap_s, to->wrap_t, NULL);

      if (wtobj == TDdriver->TDtex) {
	TDdriver->TDtex = to->texture;
	TDdriver->texwidth = (W3D_Float) image->Width;
	TDdriver->texheight = (W3D_Float) image->Height;
      }
    }

    if (fullUpdate)
      W3D_UpdateTexImage(TDcontext, to->texture, img, ti->W3DMipmaplevel, NULL);
    else {
      GLuint bprow;
      void *imgptr;
      W3D_Scissor sc;

      imgptr = (void *)(((char *)img) + (yoffset * image->Width + xoffset) * bppix);

      sc.left = xoffset;
      sc.top = yoffset;
      sc.width = width;
      sc.height = height;

      bprow = image->Width * bppix;

      W3D_UpdateTexSubImage(TDcontext, to->texture, imgptr, ti->W3DMipmaplevel, NULL, &sc, bprow);
    }
  }

  if (!(TDdriver->GLtex))
    wrpBindTexture(ctx, GL_TEXTURE_2D, tObj);
}

void wrpTexSubImage(GLcontext * ctx, GLenum target,
		    struct gl_texture_object *tObj, GLint level,
		    GLint xoffset, GLint yoffset,
		    GLsizei width, GLsizei height,
		    GLint internalFormat,
		    const struct gl_texture_image *image)
{
  wrpTexImage_generic(ctx, target, tObj, level, xoffset, yoffset, width, height,
		      internalFormat, image, GL_FALSE);

}

void wrpTexImage(GLcontext * ctx, GLenum target,
		 struct gl_texture_object *tObj, GLint level,
		 GLint internalFormat,
		 const struct gl_texture_image *image)
{
  wrpTexImage_generic(ctx, target, tObj, level, 0, 0, image->Width, image->Height,
		      internalFormat, image, GL_TRUE);
}

void wrpTexEnv(GLcontext * ctx, GLenum pname, const GLfloat * param)
{
  struct TDdriver *TDdriver;
  W3D_Context *TDcontext;
  GLenum mode;

  DEBUGOUT(1, "wrpDeleteTexture()\n");

  TDdriver = ((amigaMesaContext) ctx->DriverCtx)->TDdriver;
  TDcontext = TDdriver->td_ctx;

  switch (pname) {
    case GL_TEXTURE_ENV_MODE:
      mode = (GLenum) (GLint) *param;

      switch (mode) {
	case GL_MODULATE:
	  TDdriver->envmode = W3D_MODULATE;
	  break;
	case GL_BLEND:
	  TDdriver->envmode = W3D_BLEND;
	  break;
	case GL_DECAL:
	  TDdriver->envmode = W3D_DECAL;
	  break;
	case GL_REPLACE:
	  TDdriver->envmode = W3D_REPLACE;
	default:
	  break;
      }
      break;
    case GL_TEXTURE_ENV_COLOR:
      TDdriver->envcolor.r = param[0];
      TDdriver->envcolor.g = param[1];
      TDdriver->envcolor.b = param[2];
      TDdriver->envcolor.a = param[3];
    default:
      break;
  }

  W3D_SetTexEnv(TDcontext, NULL, TDdriver->envmode, &TDdriver->envcolor);
}

/*
 * Return the largest k such that 2^k <= n.
 */
static GLint ilog2( GLint n )
{
   GLint k;

   if (n<=0) return 0;
   for (k=0; n>>=1; k++) ;
   return k;
}

