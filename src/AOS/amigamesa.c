/*
 * $Id: amigaMesa.c 3.00 1998/09/15 19:16:56 NielsF Exp NielsF $ 
 */

/*
 * Mesa 3-D graphics library
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
 * $Log: amigaMesa.c $
 * Revision 3.00  1998/09/15  19:16:56  NielsF
 * - Mesa 3.0 API
 * - fixed most of the bugs
 *
 * $Log: amigaMesa.c $
 * Revision 1.16  1997/06/25  19:16:56  StefanZ
 * New drawing rutines:
 * - Now in separate files
 * - cyberGfx added.
 *
 * Revision 1.15  1996/10/13  20:54:59  StefanZ
 * A few nasty bugfixes.
 *
 * Revision 1.14  1996/10/07  00:11:07  StefanZ
 * Mesa 2.0 Fixed
 *
 * Revision 1.13  1996/10/06  20:31:50  StefanZ
 * Source Bump before Mesa 2.0
 *
 * Revision 1.12  1996/08/14  22:17:32  StefanZ
 * New API to amigacalls (uses taglist)
 * Made it more flexible to add gfx-card support.
 * Fast doublebuff rutines
 * minor bugfixes
 *
 * Revision 1.10    1996/06/12  13:06:00  StefanZ
 * BugFix and encatments by Jorge Acereda (JAM)
 *
 * Revision 1.9  1996/06/11  15:53:02   StefanZ
 * Little speedup, VerryFast colorallocation by Stefan Burstöm
 *
 * Revision 1.8  1996/06/01  23:57:47   StefanZ
 * Started to use RCS to keep track of code.
 *
 * Revision 1.7  1996/05/21  23:08:42   StefanZ
 * A few bug and enforcer fixes
 *
 * Revision 1.6  1996/04/29  22:14:31   StefanZ
 * BugFixes reported by by Daniel Jönsson
 *
 * Revision 1.5  1996/03/14  23:54:33   StefanZ
 * Doublebuffer & Tmprastport seams to work (big speed improvment)
 * a fastpolydraw is also implemented
 *
 * Revision 1.4  1996/03/07  16:55:04   StefanZ
 * Much of the code works now (RGB mode is simulated) Doublebuffers... (didn't work)
 *
 * Revision 1.3  1996/02/29  02:12:45   StefanZ
 * First sight of colors (even the right ones) maglight.c works
 *
 * Revision 1.2  1996/02/25  13:11:16   StefanZ
 * First working version. Draws everything with the same color
 * (Colormaping is now urgent needed)
 *
 * Revision 1.1  1996/02/23  22:01:15   StefanZ
 * Made changes to match latest version of ddsample 1.5
 *
 * Revision 1.0  1996/02/21  11:01:15   StefanZ
 * File created from ddsample.c ver 1.3 and amesa.c ver 1.5
 * in a brave atempt to rebuild the amiga version
 *
 */

/*
 * TODO:
 * Dynamic allocate the vectorbuffer for polydrawing. (memory improvment)
 * implement shared list.
 * fix resizing bug.
 * some native asm rutine
 * fast asm line drawin in db mode
 * fast asm clear       in db mode
 * read buffer rutines  in db-mode
 * 
 * IDEAS:
 * Make the gl a sharedlibrary. (Have ben started look in /amiga)
 */

/*
 * Note that you'll usually have to flip Y coordinates since Mesa's
 * window coordinates start at the bottom and increase upward.  Most
 * window system's Y-axis increases downward
 *
 * See dd.h for more device driver info.
 * See the other device driver implementations for ideas.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <gl/gl.h>
#include "context.h"
#include "dd.h"
#include "xform.h"
#include "macros.h"
#include "vb.h"

#include <AOS/amigamesa.h>


/**********************************************************************/
/*****                Internal Data                               *****/
/**********************************************************************/

GLenum LastError;						/*  The last error generated */
#ifndef WARPOS
struct Library *CyberGfxBase = NULL;				/*  optional base address for cybergfx  */
struct Library *GfxBase = NULL;
struct Library *IntuitionBase = NULL;
struct Library *UtilityBase = NULL;
#else
#pragma pack(push,2)
struct Library *CyberGfxBase = NULL;				/*  optional base address for cybergfx  */
struct GfxBase *GfxBase = NULL;
struct IntuitionBase *IntuitionBase = NULL;
struct UtilityBase *UtilityBase = NULL;
#pragma pack(pop)
#endif
APTR amesaPool = NULL;
//#if !defined(NDEBUG) && !defined(NODEBUG)
LONG debugOutput = 0;
//#endif
int trueColor = 0;
BOOL tryCached = GL_FALSE;

#define MAXTAGS 100
long tagStore[MAXTAGS*2]={0};


/**********************************************************************/
/*****                Some Usefull code                           *****/
/**********************************************************************/

/*
 * The Drawing area is defined by:
 * 
 * CC.Viewport.X = x;
 * CC.Viewport.Width = width;
 * CC.Viewport.Y = y;
 * CC.Viewport.Height = height;
 */

/**********************************************************************/
/**********************************************************************/

void glConstructor(void)
{
  char *Flag;

  if (!(CyberGfxBase = OpenLibrary("cybergraphics.library", 39))) {
    DEBUGOUT(1, "Can't open cybergraphics.library version 39\n");
  }
  if (!(IntuitionBase = OpenLibrary("intuition.library", 39))) {
    printf("Can't open intuition.library version 39\n");
    exit(10);
  }
  if (!(GfxBase = OpenLibrary("graphics.library", 39))) {
    printf("Can't open graphics.library version 39\n");
    exit(10);
  }
  if (!(UtilityBase = OpenLibrary("utility.library", 39))) {
    printf("Can't open utility.library version 39\n");
    exit(10);
  }
#ifdef WARPOS
  if (!(amesaPool = CreatePool(MEMF_PUBLIC, 65536, 32768))) {
#else
  if (!(amesaPool = CreatePool(MEMF_ANY, 65536, 32768))) {
#endif
    printf("Can't create mempools for amesa\n");
    exit(10);
  }

#if !defined(NDEBUG) && !defined(NODEBUG)
  if((Flag = getenv("MESA_DEBUG"))) {
    if(!strncasecmp(Flag, "ON", 2)) {
      debugOutput = 1;
      if ((Flag[2] == ' ') &&
          (Flag[3] >= '0') &&
          (Flag[3] <= '9'))
        debugOutput = Flag[3] - '0';
      DEBUGOUT(1, "enable debugging stage %ld\n", debugOutput);
    }
    else {
      debugOutput = 0;
      DEBUGOUT(1, "disable debugging\n");
    }
  }
#endif

  if((Flag = getenv("MESA_TRUECOLOR"))) {
    if(!strncasecmp(Flag, "ON", 2)) {
      trueColor = 1;
      DEBUGOUT(1, "enable 332 truecolor colormap\n");

      if(!strncasecmp(Flag + 3, "MATCH", 2)) {
        trueColor = -1;
        DEBUGOUT(1, "disable 332 truecolor match by shift\n");
      }
      else {
        trueColor = 1;
        DEBUGOUT(1, "disable 332 truecolor match by shift\n");
      }
    }
    else {
      trueColor = 0;
      DEBUGOUT(1, "disable 332 truecolor colormap\n");
    }
  }

  if((Flag = getenv("MESA_PENCACHE"))) {
    if(!strncasecmp(Flag, "ON", 2)) {
      tryCached = GL_TRUE;
      DEBUGOUT(1, "enable penCache\n");
    }
    else {
      tryCached = GL_FALSE;
      DEBUGOUT(1, "disable penCache\n");
    }
  }
}

void glDestructor(void)
{
  if (CyberGfxBase) {
    CloseLibrary(CyberGfxBase);
    CyberGfxBase = 0;
  }
  if (IntuitionBase) {
    CloseLibrary(IntuitionBase);
    IntuitionBase = 0;
  }
  if (GfxBase) {
    CloseLibrary(GfxBase);
    GfxBase = 0;
  }
  if (UtilityBase) {
    CloseLibrary(UtilityBase);
    UtilityBase = 0;
  }
  if (amesaPool) {
    DeletePool(amesaPool);
    amesaPool = 0;
  }
}

  __asm__ (".section .init \n .long glConstructor, 340 \n .section .text\n");
  __asm__ (".section .fini \n .long glDestructor, 340 \n .section .text\n");

#if 0
asm("	.text; 	.stabs \"___CTOR_LIST__\",22,0,0,_constructor");
asm("	.text; 	.stabs \"___DTOR_LIST__\",22,0,0,_destructor");
#endif

/**********************************************************************/
/***** amigaMesaSet/GetOneColor                                       *****/
/**********************************************************************/

GLuint amigaMesaGetOneColor(struct amigamesa_context *amesa, GLuint index)
{
  
    //DEBUGOUT(4, "result: 0x%lx, ItoP: %x, index: %x\n", GetIndex(amesa->ItoP[(unsigned char)index]) >> 8, amesa->ItoP[(unsigned char)index], index);
    return GetRGBP(amesa, (unsigned char)index) >> 8;
  
}

void amigaMesaSetOneColor(struct amigamesa_context *amesa, GLuint index, GLfloat r, GLfloat g, GLfloat b)
{
  if (amesa->depth > 8)
    amesa->ItoP[(unsigned char)index] = TC_RGBA((int)(r * 255), (int)(g * 255), (int)(b * 255));
  else {
    amesa->ItoP[(unsigned char)index] = PLG_RGBA(amesa, (unsigned char)(r * 255), (unsigned char)(g * 255), (unsigned char)(b * 255));
    //DEBUGOUT(4, "r: %x, g: %x, b: %x, ItoP: %x, index: %x\n", (unsigned char)(r * 255), (unsigned char)(g * 255), (unsigned char)(b * 255), amesa->ItoP[(unsigned char)index], index);
  }
}

/**********************************************************************/
/*****                  amiga/Mesa API Functions                  *****/
/**********************************************************************/
/*
 * Implement the client-visible amiga/Mesa interface functions defined
 * in Mesa/include/GL/amigamesa.h
 *
 **********************************************************************/

struct amigamesa_visual *amigaMesaCreateVisualTags(long Tag1,...)
{
	printf("amigaMesaCreateVisualTags not supported!\n");
  return amigaMesaCreateVisual((struct TagItem *)&Tag1);
}

struct amigamesa_visual *amigaMesaCreateVisual(struct TagItem *tagList)
{
  struct amigamesa_visual *v;
  struct Window *actWindow;
  struct Screen *actScreen;
  struct RastPort *actRPort;
  GLint rbits = 8, gbits = 8, bbits = 8, abits = 0;
  GLint index_bits = 0;

  if (!(v = (struct amigamesa_visual *)AllocVecPooled(amesaPool, sizeof(struct amigamesa_visual)))) {
    LastError = AMESA_OUT_OF_MEM;
    return NULL;
  }
  DEBUGOUT(10, "amigaMesaCreateVisual(visual struct: 0x%08x)\n", v);
  bzero(v, sizeof(struct amigamesa_visual));

  if (GetTagData(AMA_RGBMode, 0xDEAD, tagList) == 0xDEAD) {
    if ((actWindow = (struct Window *)GetTagData(AMA_Window, NULL, tagList))) {
      actScreen = (struct Screen *)GetTagData(AMA_Screen, (unsigned long)actWindow->WScreen, tagList);
      actRPort = (struct RastPort *)GetTagData(AMA_RastPort, (unsigned long)actWindow->RPort, tagList);
    }
    else {
      actScreen = (struct Screen *)GetTagData(AMA_Screen, NULL, tagList);
      actRPort = (struct RastPort *)GetTagData(AMA_RastPort, NULL, tagList);
    }
	DEBUGOUT(10, "amigaMesaCreateVisual(Window struct: 0x%08x)\n", actWindow);
	DEBUGOUT(10, "amigaMesaCreateVisual(Screen struct: 0x%08x)\n", actScreen);
	DEBUGOUT(10, "amigaMesaCreateVisual(RPort struct: 0x%08x)\n", actRPort);

    if (actRPort) {
      if ((CyberGfxBase) && (GetCyberMapAttr(actRPort->BitMap, CYBRMATTR_ISCYBERGFX) == TRUE)) {
	int pixFmt;
	int pixDpt;

	pixFmt = GetCyberMapAttr(actRPort->BitMap, CYBRMATTR_PIXFMT);
	pixDpt = GetCyberMapAttr(actRPort->BitMap, CYBRMATTR_DEPTH);

	switch (pixFmt) {
	  case PIXFMT_LUT8:
	    index_bits = pixDpt;
	    v->flags &= ~VISUAL_ALPHACHANNEL;	//GL_FALSE;
		// set rbits,gbits,bbits to 0?
	    break;
	  case PIXFMT_RGB15:
	  case PIXFMT_BGR15:
	  case PIXFMT_RGB15PC:
	  case PIXFMT_BGR15PC:
	    rbits = gbits = bbits = 5;
	    abits = 0;
		v->flags &= ~VISUAL_ALPHACHANNEL;	//GL_FALSE;
	    break;
	  case PIXFMT_RGB16:
	  case PIXFMT_BGR16:
	  case PIXFMT_RGB16PC:
	  case PIXFMT_BGR16PC:
	    rbits = bbits = 5;
	    gbits = 6;
	    abits = 0;
		v->flags &= ~VISUAL_ALPHACHANNEL;	//GL_FALSE;
	    break;
	  case PIXFMT_RGB24:
	  case PIXFMT_BGR24:
		v->flags &= ~VISUAL_ALPHACHANNEL;	//GL_FALSE;
	    break;
	  case PIXFMT_ARGB32:
	  case PIXFMT_BGRA32:
	  case PIXFMT_RGBA32:
	    abits = 8;
		v->flags |= VISUAL_ALPHACHANNEL;	//GL_TRUE;
	    break;
	}
      } //is a cgx rastport
      else {
	index_bits = GetBitMapAttr(actRPort->BitMap, BMA_DEPTH);
	rbits = gbits = bbits = abits = 0;
      } //end of checking cgx rastport
    } //end of if actRport
  } //end of if AMA_RGBMode not in tagList
  else if (GetTagData(AMA_RGBMode, 0xDEAD, tagList) == GL_FALSE) {
	  	   DEBUGOUT(10, "amigaMesaCreateVisual(AMA_RGBMode == GL_FALSE)\n");
    if ((actRPort = (struct RastPort *)GetTagData(AMA_RastPort, NULL, tagList)))
      index_bits = GetBitMapAttr(actRPort->BitMap, BMA_DEPTH);
    else
      index_bits = 8;

    rbits = gbits = bbits = abits = 0;
  }  //end of AMA_RGBMode == GL_FALSE
  else {
	  //ie AMA_RGBMode == GL_TRUE and by implication assumed to be 24 bit, no alpha chann
	  DEBUGOUT(10, "amigaMesaCreateVisual(AMA_RGBMode == GL_TRUE)\n");
  }

  /*
   * Doublebuffer always configureable
  if (!(v->db_flag = GetTagData(AMA_DoubleBuf, GL_FALSE, tagList)))
    DEBUGOUT(1, "disable doublebuffering\n");
  else
    DEBUGOUT(1, "enable doublebuffering\n");
   */

  /*
   * RGBMode if no index_bits
  if (!(v->rgb_flag = index_bits == 0 ? GL_TRUE : GL_FALSE))
    DEBUGOUT(1, "disable rgbmode\n");
  else
    DEBUGOUT(1, "enable rgbmode\n");
   */

  /*
   * AlphaMode configureable if RGBMode (others make no sence)
   */
	if(index_bits) {
		v->flags |= VISUAL_ALPHACHANNEL;
	} else {
		v->flags &= ~VISUAL_ALPHACHANNEL;
	}
/*
  if (!(index_bits == 0 ? v->flags & VISUAL_ALPHACHANNEL : GetTagData(AMA_AlphaFlag, GL_FALSE, tagList)))
    DEBUGOUT(1, "disable alphachannel\n");
  else
    DEBUGOUT(1, "enable alphachannel\n");
*/
  /*
   * Create core visual 
   */
  v->gl_visual = gl_create_visual(v->flags & VISUAL_RGBMODE,
				  v->flags & VISUAL_ALPHACHANNEL,
				  v->flags & VISUAL_DOUBLEBUFFER,
				  GL_FALSE,			/*  stereo */
				  DEPTH_BITS,			/*  depth_size  */
				  STENCIL_BITS,			/*  stencil_size  */
				  ACCUM_BITS,			/*  accum_size  */
				  index_bits,
				  rbits, gbits, bbits, abits);

  //fill in other fields
  v->depthBits = DEPTH_BITS;
  v->accumBits = ACCUM_BITS;
  v->stencilBits = STENCIL_BITS;
  v->depth = index_bits ? index_bits : (rbits + gbits + bbits + abits);

  return v;
}

void amigaMesaDestroyVisual(struct amigamesa_visual *v)
{
  gl_destroy_visual(v->gl_visual);
  FreeVecPooled(amesaPool, (ULONG *)v);
}


enum AMesaChanged amigaMesaChangeVisualTags(struct amigamesa_visual *v, long Tag1,...)
{
	printf("amigaMesaChangeVisualTags not supported!\n");
	return amigaMesaChangeVisual(v, (struct TagItem *)&Tag1);
}

enum AMesaChanged amigaMesaChangeVisual(struct amigamesa_visual *v, struct TagItem *tagList)
{
	printf("amigaMesaChangeVisual called\n");
	return AMESA_VISUAL;
}

void amigaMesaGetVisualTags(struct amigamesa_visual *v, long Tag1,...)
{
	printf("amigaMesaGetVisualTags not supported!\n");
	return amigaMesaChangeVisual(v, (struct TagItem *)&Tag1);
}

void amigaMesaGetVisual(struct amigamesa_visual *v, struct TagItem *tagList)
{
	printf("amigaMesaGetVisual called\n");
	return;
}

/*****************************************************************************/
struct amigamesa_buffer *amigaMesaCreateBufferTags(struct amigamesa_visual *visual, long Tag1,...)
{
	printf("amigaMesaCreateBufferTags not supported!\n");
  return amigaMesaCreateBuffer(visual, (struct TagItem *)&Tag1);
}

struct amigamesa_buffer *amigaMesaCreateBuffer(struct amigamesa_visual *visual, struct TagItem *tagList)
{
  struct amigamesa_buffer *b;

  if (!(b = (struct amigamesa_buffer *)AllocVecPooled(amesaPool, sizeof(struct amigamesa_buffer)))) {
    LastError = AMESA_OUT_OF_MEM;
  }
  else {
    bzero(b, sizeof(struct amigamesa_buffer));
    b->gl_buffer = gl_create_framebuffer(visual->gl_visual);
    /* other stuff */
	b->attachedWindowID = (GLint)GetTagData(AMA_WindowID, 1, tagList);	//set WindowID, default = 1
  }
    DEBUGOUT(10, "amigaMesaCreateBuffer(buffer struct: 0x%08x)\n", b);
  return b;
}

void amigaMesaDestroyBuffer(struct amigamesa_buffer *b)
{
  gl_destroy_framebuffer(b->gl_buffer);
  FreeVecPooled(amesaPool, (ULONG *)b);
}

enum AMesaChanged amigaMesaChangeBufferTags(struct amigamesa_buffer *b, long Tag1,...)
{
	printf("amigaMesaChangeBufferTags not supported!\n");
	return amigaMesaChangeBuffer(b, (struct TagItem *)&Tag1);
}

enum AMesaChanged amigaMesaChangeBuffer(struct amigamesa_buffer *b, struct TagItem *tagList)
{
	printf("amigaMesaChangeBuffer called\n");
	return AMESA_BUFFER;
}

void amigaMesaGetBufferTags(struct amigamesa_buffer *b, long Tag1,...)
{
	printf("amigaMesaGetBufferTags not supported!\n");
	return amigaMesaChangeBuffer(b, (struct TagItem *)&Tag1);
}

void amigaMesaGetBuffer(struct amigamesa_buffer *b, struct TagItem *tagList)
{
	printf("amigaMesaGetBuffer called\n");
	return;
}

/*****************************************************************************/

struct amigamesa_context *amigaMesaCreateContextTags(long Tag1,...)
{
	va_list tagList;
	int n;
	ULONG val1, val2;
	
	va_start (tagList, Tag1);
	val1 = Tag1;
	tagStore[0] = val1;

	for(n=1;n<MAXTAGS*2;n+=2) {
		if(val1==TAG_END)
			break;
		val2 = va_arg(tagList, long);
		val1 = va_arg(tagList, long);
		tagStore[n] = val2;
		tagStore[n+1] = val1;
	}

	va_end(tagList);
	if(val1!=TAG_END) {
		printf("Max Tags exceeded!\n");
	}

	return amigaMesaCreateContext((struct TagItem *)tagStore);
  //return amigaMesaCreateContext((struct TagItem *)&Tag1);
}

struct amigamesa_context *amigaMesaCreateContext(struct TagItem *tagList)
{
  /*
   * Create a new amiga/Mesa context 
   * Be sure to initialize the following in the core Mesa context: 
   * DrawBuffer, ReadBuffer 
   */
  struct amigamesa_context *amesa;
  BOOL (*Init)(struct amigamesa_context *amesa, struct TagItem *tagList);

  /*
   * allocate amigamesa_context struct initialized to zeros 
   */
  if (!(amesa = (struct amigamesa_context *)AllocVecPooled(amesaPool, sizeof(struct amigamesa_context)))) {
    LastError = AMESA_OUT_OF_MEM;
    return (NULL);
  }
  bzero(amesa, sizeof(struct amigamesa_context));
    DEBUGOUT(10, "amigaMesaCreateContext(context struct: 0x%08x)\n", amesa);

  amesa->visual = (struct amigamesa_visual *)GetTagData(AMA_Visual, NULL, tagList);
  amesa->buffer = (struct amigamesa_buffer *)GetTagData(AMA_Buffer, NULL, tagList);
  /* amesa->memPool = amesaPool; */

  if (!amesa->visual) {
    if (!(amesa->visual = amigaMesaCreateVisual(tagList))) {
      LastError = AMESA_OUT_OF_MEM;
      return NULL;
    }
	amesa->visual->context = amesa;	//fill in bound context - query do this for provided visuals?
    amesa->flags |= RESOURCE_VISUAL;
  }

  if (!amesa->buffer) {
    if (!(amesa->buffer = amigaMesaCreateBuffer(amesa->visual, GetTagData(AMA_WindowID, 1, tagList)))) {
      LastError = AMESA_OUT_OF_MEM;
      return NULL;
    }
	amesa->buffer->context = amesa;	//fill in bound context - query do this for provided buffers?
    amesa->flags |= RESOURCE_BUFFER;
  }

  if((amesa->share = (struct amigamesa_context *)GetTagData(AMA_ShareGLContext, NULL, tagList)))
    DEBUGOUT(1, "use shared contexts\n");
  /*  sharetmp = amesa->share; */
  amesa->gl_ctx = gl_create_context(amesa->visual->gl_visual, amesa->share ? amesa->share : NULL, (void *)amesa, GL_FALSE);

  /*  drawMode = GetTagData(AMA_DrawMode,AMESA_AGA,tagList);  */
  /*if (amesa->visual->flags & VISUAL_DOUBLEBUFFER == GL_TRUE) {
    if (CyberGfxBase)
      Init = cybStandardInitDB;
    else
      Init = amigaStandardInitDB;
  }
  else {
    if (CyberGfxBase)
      Init = cybStandardInit;
    else
      Init = amigaStandardInit;
  }*/

	Init = cmnStandardInit;

  if (!(*Init)(amesa, tagList)) {
	    DEBUGOUT(10, "AmigaMesaCreateContext: cmnStandardInit returned GL_FALSE\n");
    amigaMesaDestroyContext(amesa);
    amesa = NULL;
  } else {
	  	    DEBUGOUT(10, "AmigaMesaCreateContext: cmnStandardInit returned GL_TRUE\n");
  }

  return amesa;
}

void amigaMesaDestroyContext(struct amigamesa_context *amesa)
{
  /*
   * destroy a amiga/Mesa context 
   */

  /*  if (amesa==amesa) */
  /*    amesa=NULL; */
  (*amesa->Dispose) (amesa);

  if (amesa->flags & RESOURCE_VISUAL)
    amigaMesaDestroyVisual(amesa->visual);
  if (amesa->flags & RESOURCE_BUFFER)
    amigaMesaDestroyBuffer(amesa->buffer);

  gl_destroy_context(amesa->gl_ctx);
  FreeVecPooled(amesaPool, (ULONG *)amesa);
}

enum AMesaChanged amigaMesaChangeContextTags(struct amigamesa_context *amesa, long Tag1,...)
{
	va_list tagList;
	int n;
	ULONG val1, val2;
	
	va_start (tagList, Tag1);
	val1 = Tag1;
	tagStore[0] = val1;

	for(n=1;n<MAXTAGS*2;n+=2) {
		if(val1==TAG_END)
			break;
		val2 = va_arg(tagList, long);
		val1 = va_arg(tagList, long);
		tagStore[n] = val2;
		tagStore[n+1] = val1;
	}

	va_end(tagList);
	if(val1!=TAG_END) {
		printf("Max Tags exceeded!\n");
	}

	return amigaMesaChangeContext(amesa, (struct TagItem *)tagStore);

	//return amigaMesaChangeContext(amesa, (struct TagItem *)&Tag1);
}

enum AMesaChanged amigaMesaChangeContext(struct amigamesa_context *amesa, struct TagItem *tagList)
{
	DEBUGOUT(10, "Entered amigaMesaChangeContext\n");
	if (GetTagData(AMA_PaletteMode, 0xDEAD, tagList) != 0xDEAD) {
		amesa->trueColor = tagList->ti_Data;
		DEBUGOUT(10, "amigaMesaChangeContext: changed PaletteMode to 0x%x\n",tagList->ti_Data);
	}

	if (GetTagData(AMA_PaletteCache, 0xDEAD, tagList) != 0xDEAD) {
		if(tagList->ti_Data == GL_TRUE) {
			amesa->flags |= PALETTE_CACHE;
			DEBUGOUT(10, "amigaMesaChangeContext: PaletteCache ON\n");
		} else {
			amesa->flags &= ~PALETTE_CACHE;
			DEBUGOUT(10, "amigaMesaChangeContext: PaletteCache OFF\n");
		}
	}

	if (GetTagData(AMA_PaletteDither, 0xDEAD, tagList) != 0xDEAD) {
		if(tagList->ti_Data == GL_TRUE) {
			amesa->flags |= PALETTE_DITHER;
			DEBUGOUT(10, "amigaMesaChangeContext: PaletteDither ON\n");
		} else {
			amesa->flags &= ~PALETTE_DITHER;
			DEBUGOUT(10, "amigaMesaChangeContext: PaletteDither OFF\n");
		}
	}

	return AMESA_CONTEXT;
}

void amigaMesaGetContextTags(struct amigamesa_context *amesa, long Tag1, ULONG *Tag1val, long Tag2)
{
	DEBUGOUT(10, "Entered amigaGetContextTags\n");
	if(Tag2!=TAG_DONE) {
		printf("Too many tags in amigaMesaGetContextTags!\n");
		return;
	}
	if(Tag1!=AMA_Buffer) {
		printf("Invalid tags in amigaMesaGetContextTags!\n");
		return;
	}
	*Tag1val = amesa->buffer;
	return;
	//return amigaMesaGetContext(amesa, (struct TagItem *)&Tag1);
}

/*
void amigaMesaGetContext(struct amigamesa_context *amesa, struct TagItem *tagList)
{
	if (GetTagData(AMA_Buffer, 0xDEAD, tagList) != 0xDEAD) {
		tagList->ti_Data = amesa->buffer;
	}
	return;
}
*/

/*****************************************************************************/

void amigaMesaMakeCurrent(struct amigamesa_context *amesa, struct amigamesa_buffer *b)
{
  /*
   * Make the specified context the current one 
   * the order of operations here is very important! 
   */

  /*  Current = amesa; */
  if (amesa && b) {
    (*amesa->InitDD)(amesa->gl_ctx);				/*  Call Driver_init_rutine  */
    gl_make_current(amesa->gl_ctx, b->gl_buffer);

    if (amesa->gl_ctx->Viewport.Width == 0) {
      /* initialize viewport to window size */
      glViewport(0, 0, amesa->width, amesa->height);
      amesa->gl_ctx->Scissor.Width = amesa->width;
      amesa->gl_ctx->Scissor.Height = amesa->height;
    }
  }
}

/*****************************************************************************/

void amigaMesaSwapBuffers(struct amigamesa_context *amesa)
{
  /* copy/swap back buffer to front if applicable */
  (*amesa->SwapBuffer)(amesa);
}

/*
 * This is on the drawingboard 
 * Mostly for when future changes the library is still intact
 */

/*****************************************************************************/

GLboolean amigaMesaSetDefs(struct TagItem *tagList)
{
  /*  struct TagItem *tag; */

  /*  tagValue=AMA_DrawMode; */
  /*  tag = FindTagItem(tagValue,tagList); */
  /*  if (tag) */
  /*    printf("Tag=0x%x, is 0x%x/n",tagValue,tag->ti_Data); */
  /*  else */
  /*    printf("Tag=0x%x is not specified/n",tagValue); */
  return FALSE;
}

/*****************************************************************************/

/*
 * Maybe a report error rutine ??? like:
 */
GLenum amigaMesaReportError(struct amigamesa_context *amesa)
{
  GLenum error;

  error = LastError;
  LastError = 0;
  return (error);
}

#undef DEBUGPRINT
