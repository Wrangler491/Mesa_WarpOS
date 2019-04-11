#ifndef	AMIGAMESAint_H
#define	AMIGAMESAint_H

#if !defined(NDEBUG) && !defined(NODEBUG)
#define	MESA_DEBUG
#endif

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

#define	NO_CONTEXT_LIBRARIES		/* do not pass library-bases over contexts */
#define	REPLACE_ALLOC			/* replace malloc/calloc/free with pool-functions (only in libMesaGL) */
//#define	PREVENT_LUT		/* use real calculation instead of LUTs */

/************************************************************************
 *
 * basic os-dependent
 */
#ifdef WARPOS
#pragma pack(push,2)
#endif
#include <cybergraphx/cybergraphics.h>
#include <Warp3D/Warp3D.h>
#include <exec/memory.h>
#include <exec/types.h>
#include <hardware/blit.h>
#include <intuition/intuition.h>
#include <intuition/screens.h>
#ifdef WARPOS
#pragma pack(pop)
#endif

#if 0
/*
 * A is the blitmask (temporary memory passed to Blt#? as TempA)
 * B is the source-bitmap
 * C is the destination bitmap
 */

#define	LOGIC_CLEAR	
#define	LOGIC_SET	
#define	LOGIC_COPY	(ABC | ABNC)
#define	LOGIC_COPY_INV	(ANBC | ANBNC)
#define	LOGIC_NOOP	
#define	LOGIC_INV	
#define	LOGIC_AND	(ABC)
#define	LOGIC_NAND	(ANBC)
#define	LOGIC_OR	(ABC | ANBC | ABNC) | (NABC | NANBC | NABNC)
#define	LOGIC_NOR	(ANBC | ABC | ANBNC) | (NANBC | NABC | NANBNC)
#define	LOGIC_XOR	(ANBC | ABNC) | (NANBC | NABNC)
#define	LOGIC_XNOR	(ABC | ANBNC) | (NABC | NANBNC)
#endif

/************************************************************************
 *
 * inlines
 */
#ifndef WARPOS
#include <inline/cybergraphx.h>
#include <inline/Warp3D.h>
#include <inline/dos.h>
#include <inline/exec.h>
#include <inline/graphics.h>
#include <inline/intuition.h>
#include <inline/utility.h>
#else
#pragma pack(push,2)
#include <proto/cybergraphics.h>
#include <Warp3D/Warp3D_protos.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <proto/utility.h>
#pragma pack(pop)
#endif

/************************************************************************
 *
 * MESA_AOS kernel structures
 */
#include <assert.h>
#include <GL/gl.h>
#include "types.h"
#include "context.h"
#include "dd.h"
#include "xform.h"
#include "macros.h"
#include "vb.h"

#define	AOS_KERNEL
#define	AOS_WARP3D

/*****************************************************************************
 *
 * private palette-definitions
 */
#define	PALETTE_DEFS
#include "AOS/palettes.h"
#undef	PALETTE_DEFS

/*****************************************************************************
 *
 * private visual
 */
struct amigamesa_context;

struct amigamesa_visual {
  struct amigamesa_context *context;				/* to which context are we bound to? */
  GLvisual *gl_visual;

  GLushort flags;
#define	VISUAL_DOUBLEBUFFER	(1 << 0)			/* double buffered? */
#define	VISUAL_STEREO		(1 << 1)			/* stereo mode? */
#define	VISUAL_RGBMODE		(1 << 2)			/* RGB mode?  */
#define	VISUAL_ALPHACHANNEL	(1 << 3)			/* Alphacolor? */

#define	VISUAL_AUTORGB		(1 << 8)			/* autodetect RGB mode? */

  GLshort depthBits, accumBits, stencilBits;			/* number of bits per extra-channel */
  GLshort depth;						/* bits per pixel (1, 8, 24, etc) */
};

/*****************************************************************************
 *
 * private buffer
 *
 * TODO: check for size compatibility of buffer vs. context if AMA_Buffer!!!
 */
struct amigamesa_buffer {
  struct amigamesa_context *context;				/* to which context are we bound to? */
  GLframebuffer *gl_buffer;					/* The depth, stencil, accum, etc buffers */

  GLint attachedWindowID;
};

/*****************************************************************************
 *
 * private context
 */
typedef void (*rastersetup_func) (GLcontext *, GLuint, GLuint);
typedef	void (*linestrip_func) (GLcontext *, GLuint, GLuint);
typedef void (*tristrip_func) (GLcontext *, GLuint, GLuint);
typedef void (*trifan_func) (GLcontext *, GLuint, GLuint, GLuint);

struct amigamesa_context {
  GLcontext *gl_ctx;						/* the core library context */
  GLcontext *share;						/* share this context with this */

  struct amigamesa_visual *visual;				/* the visual context */
  struct amigamesa_buffer *buffer;				/* the buffer context */

  void *mempool;						/* special mempool for this context, with it's death all mem is freed */

  GLenum lastAction;						/* the last call to glBegin was this */

  GLushort flags;						/* 0x1 = own visuel 0x2 = own buffer */
#define	RESOURCE_VISUAL		(1 <<  0)			/* contexts visual is it's own */
#define	RESOURCE_BUFFER		(1 <<  1)			/* contexts buffer is it's own */

#define	PALETTE_CACHE		(1 <<  8)			/* try to cache pens for speedup pen-determination */
#define	PALETTE_DITHER		(1 <<  9)			/* dither all values before GetPen */

  GLuint pixel;							/* current color index or RGBA pixel value */
  GLuint clearpixel;						/* pixel for clearing the color buffers */

  /* the things this context is bind to */
  struct Window *window;					/* Not neaded if in dubbelbuff needed */
  struct RastPort *rp;						/* current rastport */
  struct Screen *Screen;					/* current screen */

  /* single/double-buffering resources */
  struct RastPort *front_rp;					/* front rastport */
  struct RastPort *back_rp;					/* back rastport (NULL if SB or RGB) */
  struct TmpRas *tmpras;					/* tmpras rastport */
  struct RastPort *temprp;
  GLubyte *BackArray;						/* a pen Array big as drawing area for use in dubelbuff mode */
  GLubyte *imageline;						/* One Line for WritePixelRow renders */
  struct TDdriver {
    W3D_Context *td_ctx;

    struct gl_texture_object *GLtex;
    W3D_Texture *TDtex;

    GLuint flags, envmode, failed;
#define TD_ACTIVE	(1 <<  0)				/* hardware driver active/inactive */
#define TD_ZBUFFER	(1 <<  1)				/* Z buffer allocated */
#define TD_SBUFFER	(1 <<  2)				/* Stencil buffer allocated */
#define TD_RECTTEX	(1 <<  3)				/* Rectangular textures supported */
#define TD_STATS	(1 <<  4)				/* statistics enabled */
#define TD_NOCLAMP	(1 <<  5)				/* use HW even if clamping not supported */
#define TD_NICETEX	(1 <<  6)				/* highquality texturing */
#define TD_NICEFOG	(1 <<  7)				/* highquality fogging */
#define TD_DIRTY	(1 <<  8)				/* flush necessary before Zbuffer access */
#define TD_NOVMEM	(1 <<  9)				/* no VMEM for textures */
#define TD_LOCKED	(1 << 10)				/* in locked state  */

    W3D_Color Palette[palNumColsMax];				/* the actual palette-tables */

    W3D_Vertex vbuffer[VB_SIZE];
    W3D_Float height;
    W3D_Float wscale;
    W3D_Float woffset;
    W3D_Float near;
    W3D_Float far;
    W3D_Scissor scissor;
    W3D_Float texwidth;
    W3D_Float texheight;
    W3D_Color envcolor;

    linestrip_func LineStripFunc;
    tristrip_func TriStripFunc;
    trifan_func TriFanFunc;

    void (*CopyVertex) (W3D_Vertex *dest, W3D_Vertex *source);
  } *TDdriver;							/* Warp3Ddriver */

  GLshort depth;						/* bits per pixel (1, 8, 24, etc) */

  GLshort width, height;					/* drawable area */
  GLshort left, bottom;						/* offsets due to window border */
  GLshort right, top;						/* offsets due to window border */
  GLshort RealWidth, RealHeight;				/* the drawingareas real size */
  GLshort FixedWidth, FixedHeight;				/* The internal buffer real size speeds up the drawing a bit */

  /* provide fast FIXx/FIXy/FIXxy macros */
  GLshort FixedX, FixedY;					/* */
  GLint *FixedXY, **FixedXYdb;					/* */

  /* ItoP[] is changed if auxSetOneColor(index,r,g,b); is called */
  palMode trueColor;						/* the palette allocation-mode */
  GLshort PaletteCols;						/* the actual palette-colors */
  struct ColorEntry Palette[palNumColsMax];			/* the actual palette-tables */
  GLshort (*MatchErrors)[ERROR_RANGE][4];			/* the actual match-error-tables */
  struct ColorCache penCache[256];				/* the cached pens */
  GLuint ItoP[256];						/* when allocating index 13 with a color ItoP[13] is the actual pen color */
  GLuint PtoI[256];						/* PtoI[ItoP[13]] is index 13 */

  void (*InitDD) (GLcontext *);					/* keep track of witch drawing rutines should be used */
  void (*Dispose) (struct amigamesa_context *);			/* Use this when AmigaMesaDestroyContext is called */
  void (*SwapBuffer) (struct amigamesa_context *);		/* Use this when AmigaMesaSwapBuffers is called */

#define	NO_CONTEXT_LIBRARIES
#ifndef	NO_CONTEXT_LIBRARIES
  /* speeds up inlines needn't to restore a4 as they get the library-bases
   * out of the context not a4! */
  struct ExecBase *aSysBase;
  struct Library *aDOSBase;
  struct Library *aGfxBase;
  struct Library *aUtilityBase;
  struct Library *aCyberGfxBase;
  struct Library *aWarp3DBase;
#ifndef	NO_CONTEXT_AVAILABLE
#define	SysBase		(amesa->aSysBase)
#define	DOSBase		(amesa->aDOSBase)
#define	GfxBase		(amesa->aGfxBase)
#define	UtilityBase	(amesa->aUtilityBase)
#define	CyberGfxBase	(amesa->aCyberGfxBase)
#define	Warp3DBase	(amesa->aWarp3DBase)
#endif
#endif
};

/* set a flag */
#define	SetF(field, flag)	((field) |= (flag))
/* clear a flag */
#define	ClrF(field, flag)	((field) &= ~(flag))
/* swap a flag */
#define	SwpF(field, flag)	((field) ^= (flag))
/* test a flag */
#define	TstF(field, flag)	((field) & (flag))

/* convert a flag to a boolean */
#define	FtoB(field, flag)	(TstF(field, flag) ? GL_TRUE : GL_FALSE)

/*****************************************************************************
 *
 * private externals
 */
#ifdef WARPOS
#pragma pack(push,2)
#endif
#if defined(NO_CONTEXT_AVAILABLE) || defined(NO_CONTEXT_LIBRARIES)
extern struct ExecBase *SysBase;
extern struct Library *CyberGfxBase;
#ifdef AOS_WARP3D
extern struct Library *Warp3DBase;
#endif

#ifndef WARPOS
extern struct Library *DOSBase;
extern struct Library *GfxBase;
extern struct Library *UtilityBase;
#else
extern struct DosLibrary *DOSBase;
extern struct GfxBase *GfxBase;
extern struct UtilityBase *UtilityBase;
#pragma pack(pop)
#endif
#endif

//#define	TRYTEST	1
extern struct presets {
  palMode trueColor;
  GLboolean tryCached, tryTest, Dither;
} preset;
extern APTR amesaPool;

/*****************************************************************************
 *
 * includes
 */
#include <GL/amigamesa.h>

#include "AOS/amesatools.h"
#include "AOS/mempools.h"

#include "AOS/cmnDisplay.h"
#include "AOS/cybDisplay.h"
#include "AOS/natDisplay.h"
#include "AOS/wrpDisplay.h"

/*****************************************************************************
 *
 * private palette-prototypes
 */
#define	PALETTE_PROTOS
#include "AOS/palettes.h"
#undef	PALETTE_PROTOS

/*****************************************************************************
 *
 * Note that you'll usually have to flip Y coordinates since Mesa's
 * window coordinates start at the bottom and increase upward.  Most
 * window system's Y-axis increases downward
 *
 * See dd.h for more device driver info.
 * See the other device driver implementations for ideas.
 */
#define	LayerWidth(l)		(l->bounds.MaxX - l->bounds.MinX + 1)
#define	LayerHeight(l)		(l->bounds.MaxY - l->bounds.MinY + 1)
#define	natFixedMask(width)	(((width) + 15) & 0xFFFFFFF0)	/* 16byte aligned */
#define	cybFixedMask(width)	(((width) +  3) & 0xFFFFFFFC)	/*  4byte aligned */
#ifdef	PREVENT_LUT
#define FIXx(x)			(amesa->left + (x))
#define FIXy(y)			(amesa->FixedHeight - amesa->bottom - (y))
#define FIXxy(x,y)		(amesa->FixedWidth * FIXy(y) + FIXx(x))
#define	dbPenGet(am)		((GLubyte *)((am)->BackArray))
#define	dbPen(db, x, y)		(((GLubyte *)(db)) + FIXxy((x), (y)))
#define	dbRGBAGet(am)		((GLuint *)((am)->BackArray))
#define	dbRGBA(db, x, y)	(((GLuint *)(db)) + FIXxy((x), (y)))
#else
#define FIXx(x)			(amesa->FixedX + (x))
#define FIXy(y)			(amesa->FixedY - (y))
#define FIXxy(x,y)		(amesa->FixedXY[(y)] + (x))
#define	dbPenGet(am)		((GLubyte *)((am)->FixedXYdb))	/*  (GLubyte **) */
#define	dbPen(db, x, y)		(((GLubyte **)(db))[(y)] + (x))	/* *(GLubyte **) */
#define	dbRGBAGet(am)		((GLuint *)((am)->FixedXYdb))	/*  (GLuint **) */
#define	dbRGBA(db, x, y)	(((GLuint **)(db))[(y)] + (x))	/* *(GLuint **) */
#endif

/*****************************************************************************
 *
 * ???
 */

#define MAX_POLYGON 300

/*****************************************************************************
 *
 * debugging output
 */
/* deactive debugging-output only on request */
//#if !defined(NDEBUG) && !defined(NODEBUG)
extern LONG debugOutput;
#include <stdio.h>
#define DEBUGOUT(level,str,args...) ({	\
  if(debugOutput >= level) {		\
 /* Printf(str,"gl  : "); */		\
 /* Printf(str,##args);	*/		\
    fprintf(stderr,"gl  : ");		\
    fprintf(stderr,str,##args);		\
    fflush(stderr);			\
  }					\
})
//#else
//#define DEBUGOUT(level,str,args...) ({})
//#define	DEBUGOUT_REMOVED
//#endif

#include <stdio.h>
#define Error(str,args...) ({		\
/*Printf("Fatal error: "); */		\
/*Printf(str,##args); */		\
  fprintf(stderr,"Fatal error: ");	\
  fprintf(stderr,str,##args);		\
  fflush(stderr);			\
  abort();				\
})

#endif
