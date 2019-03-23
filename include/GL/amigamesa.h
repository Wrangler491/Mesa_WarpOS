/*
 * amigamesa.h 
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
 * $Id: Amigamesa.h 3.10 1999/06/22 18:51:18 NielsF Exp NielsF $
 * 
 * $Log: amigaMesa.h $
 * Revision 3.10  1999/06/22  18:51:18  NielsF
 * - Mesa 3.1 API
 * - fixed most of the new introduced bugs
 *
 * $Log: amigaMesa.h $
 * Revision 3.00  1998/09/15  19:16:56  NielsF
 * - Mesa 3.0 API
 * - fixed most of the bugs
 *
 * $Log: Amigamesa.h $
 * Revision 1.5  1997/06/25  19:16:45  StefanZ
 * *** empty log message ***
 *
 * Revision 1.3  1996/10/06  20:35:11  StefanZ
 * Source bump before Mesa 2.0
 *
 * Revision 1.2  1996/08/14  22:16:31  StefanZ
 * New Api to amigaspecific functions, Added suport for gfx-cards
 *
 * Revision 1.1  1996/06/02  00:15:03   StefanZ
 * Initial revision
 *
 * Revision 1.0  1996/02/21  11:09:45   brianp
 * A copy of amesa.h version 1.4 in a brave atempt to make a amiga interface
 *
 */

#ifndef AMIGAMESA_H
#define AMIGAMESA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <GL/gl.h>

#ifndef UTILITY_TAGITEM_H
#include <utility/tagitem.h>
#endif

  enum AMesaError {
    AMESA_OUT_OF_MEM,

    AMESA_RASTPORT_TAG_MISSING,
    AMESA_SCREEN_TAG_MISSING,
    AMESA_WINDOW_TAG_MISSING
  };

  enum AMesaChanged {
    /* fail GL_FALSE */
    AMESA_FAILED     =       -1 ,						/* TODO: all bits set that are unused upto made number negative */
    /* negative nothing */
    AMESA_NOTHING    =        0 ,
    /* positive bitfield */
    AMESA_BUFFER     =  (1 << 0),						/* gl_buffer changed */
    AMESA_VISUAL     =  (1 << 1),						/* gl_visual changed (implies gl_context/gl_buffer changed) */
    AMESA_CONTEXT    =  (1 << 2),						/* gl_context changed */
    AMESA_SHARED     =  (1 << 3),						/* gl_shared_context changed (implies gl_context changed) */
    AMESA_DIMENSIONS =  (1 << 4),						/* context-dimensions changed */
    AMESA_BINDING    =  (1 << 5)						/* contexts rastport (or window/screen) changed */
  };

#ifndef	AOS_KERNEL
  /* first part defines amigaMesa-specifig tag
   * second part defines the choosen palette (aka modelling of available colors)
   * third part defines methog to fetch data (aka encoding of given colors
   *
   * at the moment dithering is only available in RGBMode, that means if we are
   * in index-mode the index-pixels are directly (without dither) mapped to
   * pen-pixels, it could be an improvement to offer dithering even for index-modes
   */
  typedef enum PaletteModeID {
    /* dithering */
    AMESA_HPCR_DITHER = -8,							/* hp-color-recovery */
    AMESA_GREY_DITHER = -4,
    AMESA_WEIGHTED_DITHER = -2,							/* default standard 0 */
    AMESA_TRUECOLOR_DITHER = -1,						/* matches below 0 */
    /* matching */
    AMESA_HPCR_MATCH = 8,							/* hp-color-recovery */
    AMESA_GREY_MATCH = 4,
    AMESA_WEIGHTED_MATCH = 2,							/* default standard 0 */
    AMESA_TRUECOLOR_MATCH = 1,							/* matches below 0 */
    /* shifting */
    AMESA_TRUECOLOR_SHIFT = 0,							/* shifts above 0 */
    /* invalid */
    AMESA_DEADFOOD = 0xDEAD							/* dead man */
  } palMode;

 /*
  * This is the Amiga/Mesa context structure.  This usually contains
  * info about what window/buffer we're rendering too, the current
  * drawing color, etc.
  */
  struct amigamesa_context {
    /* everything is private!!! */
  };

  struct amigamesa_visual {
    /* everything is private!!! */
  };

  struct amigamesa_buffer {
    /* everything is private!!! */
  };
#endif

  typedef struct amigamesa_context *amigaMesaContext;
  typedef struct amigamesa_visual  *amigaMesaVisual;
  typedef struct amigamesa_buffer  *amigaMesaBuffer;

/**********************************************************************/
/*****                  Amiga/Mesa API Functions                  *****/
/**********************************************************************/

  /* buffers */
  amigaMesaBuffer amigaMesaCreateBufferTags(amigaMesaVisual, long Tag1,...);
  amigaMesaBuffer amigaMesaCreateBuffer(amigaMesaVisual, struct TagItem *);
  void amigaMesaDestroyBuffer(amigaMesaBuffer);

  enum AMesaChanged amigaMesaChangeBufferTags(amigaMesaBuffer, long Tag1,...);
  enum AMesaChanged amigaMesaChangeBuffer(amigaMesaBuffer, struct TagItem *);

  void amigaMesaGetBufferTags(amigaMesaBuffer, long Tag1,...);
  void amigaMesaGetBuffer(amigaMesaBuffer, struct TagItem *);

  /* visuals */
  amigaMesaVisual amigaMesaCreateVisualTags(long Tag1,...);
  amigaMesaVisual amigaMesaCreateVisual(struct TagItem *);
  void amigaMesaDestroyVisual(amigaMesaVisual);

  enum AMesaChanged amigaMesaChangeVisualTags(amigaMesaVisual, long Tag1,...);
  enum AMesaChanged amigaMesaChangeVisual(amigaMesaVisual, struct TagItem *);

  void amigaMesaGetVisualTags(amigaMesaVisual, long Tag1,...);
  void amigaMesaGetVisual(amigaMesaVisual, struct TagItem *);

  /* contexts */
  amigaMesaContext amigaMesaCreateContextTags(long Tag1,...);
  amigaMesaContext amigaMesaCreateContext(struct TagItem *);
  void amigaMesaDestroyContext(amigaMesaContext);

  enum AMesaChanged amigaMesaChangeContextTags(amigaMesaContext, long Tag1,...);
  enum AMesaChanged amigaMesaChangeContext(amigaMesaContext, struct TagItem *);

  void amigaMesaGetContextTags(amigaMesaContext, long Tag1,...);
  void amigaMesaGetContext(amigaMesaContext, struct TagItem *);

  /* misc */
  void amigaMesaSetOneColor(amigaMesaContext, GLuint, GLfloat r, GLfloat g, GLfloat b);
  GLuint amigaMesaGetOneColor(amigaMesaContext, GLuint);
  GLint amigaMesaNumColors(amigaMesaContext);

  void amigaMesaMakeCurrent(amigaMesaContext, amigaMesaBuffer);
  void amigaMesaSwapBuffers(amigaMesaContext);

  extern GLenum LastError;							/* Last Error generated */

  /* This is on the drawingboard */
  GLboolean amigaMesaSetDefs(struct TagItem *tagList);
  GLenum amigaMesaReportError(amigaMesaContext amesa);

/**********************************************************************/
/*****                amigaMesa TAGS (Set/Get)                    *****/
/**********************************************************************/

 /*
  * Amiga Mesa Attribute tag ID's.  These are used in the ti_Tag field of
  * TagItem arrays passed to amigaMesaSetDefs() and amigaMesaCreateContext()
  */
#define AMA_Dummy		(TAG_USER + 32)

 /*
  * Offset to use. WARNING AMA_Left, AMA_Bottom Specifies the low left corner
  * of the drawing area in deltapixles from the lowest left corner
  * typical AMA_Left,window->BorderLeft
  * AMA_Bottom,window->BorderBottom + 1
  * This is since ALL gl drawing actions is specified with this point as 0,0
  * and with y positive uppwards (like in real graphs).
  * 
  * Untuched (defult) will result in 
  * AMA_Left=0;
  * AMA_Bottom=0;
  */
#define AMA_Left		(AMA_Dummy + 0x0001)
#define AMA_Bottom		(AMA_Dummy + 0x0002)
#define AMA_Right		(AMA_Dummy + 0x0003)
#define AMA_Top			(AMA_Dummy + 0x0004)

 /*
  * Size in pixels of drawing area if others than the whole rastport.
  * All internal drawingbuffers will be in this size
  * 
  * Untuched (defult) will result in 
  * AMA_Width =rp->BitMap->BytesPerRow*8;
  * AMA_Height=rp->BitMap->Rows;
  */
#define AMA_Width		(AMA_Dummy + 0x0010)
#define AMA_Height		(AMA_Dummy + 0x0011)

#define AMA_PaletteMode		(AMA_Dummy + 0x0020)
#define AMA_PaletteCache	(AMA_Dummy + 0x0021)
#define AMA_PaletteDither	(AMA_Dummy + 0x0022)

#define AMA_Screen		(AMA_Dummy + 0x0030)
#define AMA_Window		(AMA_Dummy + 0x0031)
#define AMA_RastPort		(AMA_Dummy + 0x0032)

 /** booleans **/
 /*
  * AMA_DoubleBuf: If specified it uses double Buffering (change buffer with
  * amigaMesaSwapBuffers()) Turn this on as much as posible
  * it will result in smother looking and faster rendering
  * Defult value: GL_FALSE
  * AMA_RGBMode: If specified it uses 24bit when drawing (on non 24bit displays it
  * it emuletes 24bit)
  * Defult value: GL_TRUE
  * AMA_AlphaFlag: Alphachanel ?
  * Defule value: GL_FALSE
  */
#define AMA_DoubleBuffer	(AMA_Dummy + 0x0040)
#define AMA_RGBMode		(AMA_Dummy + 0x0041)
#define AMA_AlphaChannel	(AMA_Dummy + 0x0042)
#define AMA_Stereo		(AMA_Dummy + 0x0043)

#define AMA_BitsDepth		(AMA_Dummy + 0x0044)
#define AMA_BitsAccum		(AMA_Dummy + 0x0045)
#define AMA_BitsStencil		(AMA_Dummy + 0x0046)

#define	AMA_No3D		(AMA_Dummy + 0x0047)

 /** Special **/
 /*
  * AMA_ShareGLContext: Set the "friend" context (use multiple contexts) 
  * See the GL maual or Mesa to get more info
  * AMA_Visual: If you want to implement your own amigamesa_visual 
  * AMA_Buffer: If you want to implement your own amigamesa_buffer
  * AMA_WindowID: A windowID to use when I alloc AMA_Buffer for you if
  * you didn't supply one.(defult=1)
  */

#define AMA_ShareGLContext	(AMA_Dummy + 0x0050)
#define AMA_Visual		(AMA_Dummy + 0x0051)
#define AMA_Buffer		(AMA_Dummy + 0x0052)
#define AMA_WindowID		(AMA_Dummy + 0x0053)
#define AMA_GLContext		(AMA_Dummy + 0x0054)

 /** GetOnly **/
 /*
  * AMA_Depth: get the current context-depth
  */

#define	AMA_VisualDepth		(AMA_Dummy + 0x0060)
#define	AMA_ContextDepth	(AMA_Dummy + 0x0061)
#define	AMA_VisualGLVisual	(AMA_Dummy + 0x0062)

#ifdef __cplusplus
}

#endif
#endif
