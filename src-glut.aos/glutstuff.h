#if !defined(NDEBUG) && !defined(NODEBUG)
#define	MESA_DEBUG
#endif

/*
 * Amiga GLUT graphics library toolkit
 * Version:  1.1
 * Copyright (C) 1998 Jarno van der Linden
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 * glutstuff.h
 *
 * Version 1.0  27 Jun 1998
 * by Jarno van der Linden
 * jarno@kcbbs.gen.nz
 *
 * Version 1.1  02 Aug 1998
 * by Jarno van der Linden
 * jarno@kcbbs.gen.nz
 *
 * - Quantizer plugin support added
 *
 */

#ifdef WARPOS
#pragma pack(push,2)
#endif
#include <exec/types.h>
#include <intuition/intuition.h>
#pragma pack(pop)

#include <GL/glut.h>
#include <GL/amigamesa.h>

#ifdef WARPOS
#include <liblists.h>
#endif

#define	USE_CLIP_LAYER

/* public funtions of MesaGL */
extern APTR glutPool;

#ifndef WARPOS
APTR AllocVecPooled(register APTR poolHeader __asm__("a0"), register ULONG byteSize __asm__("d0"));
void FreeVecPooled(register APTR poolHeader __asm__("a0"), register ULONG *memory __asm__("a1"));
#else
APTR AllocVecPooled(APTR poolHeader, ULONG byteSize);
void FreeVecPooled(APTR poolHeader, ULONG *memory);
#endif

/* Menus */
#define IGNORE_IN_GAME_MODE(args...) \
  { if (glutstuff.GameMode) return args; }

struct GlutMenuEntry {
  struct nnode EntryNode;
#define	EntryValue	EntryNode.data
  struct GlutMenu *EntryMenu;

  const char *name;
  BOOL issubmenu;
};

struct GlutMenu {
  struct nnode MenuNode;
#define	MenuID		MenuNode.data
  struct nlist MenuEntries;
#define	NumEntries	MenuEntries.nodes

  void (*menufunc) (int value);

  BOOL needupdate;
};

struct GlutTimer {
  struct nnode TimerNode;
#define	TimerValue	TimerNode.data

  ULONG secs, micros;
  void (*timerfunc) (int value);
};

struct GlutWindow {
  struct nnode WindowNode;					/* bind the windows together */
#define	WinID		WindowNode.data
  struct nlist SubWindows;					/* bind all subwindows of this window together */
#define	NumChildren	SubWindows.nodes
  struct nnode SubWindowNode;					/* a subwindow only have one parent, not more */
  struct GlutWindow *SubWindowParent;				/* direct link backwards */
  struct nlist WindowTimers;

  struct Window *window;
#ifdef USE_CLIP_LAYER
  struct Region *clipreg;
#endif
  amigaMesaContext context;

  APTR vi;

  struct Menu *menu;
  struct GlutMenu *leftmenu, *middlemenu, *rightmenu;

  UWORD qualifiers;
  int ignorekeyrepeat, repeatmode;
  int mousex, mousey;
  int joystickpoll, cursor;						/* TODO */

  int winx, winy;							/* Shape that we want */
  int winwidth, winheight;
  int wincurx, wincury;							/* Shape that we believe it to currently be */
  int wincurwidth, wincurheight;

  void (*displayfunc) (void);
  void (*overlaydisplayfunc) (void);
  void (*keyboardfunc) (unsigned char key, int x, int y);
  void (*keyboardupfunc) (unsigned char key, int x, int y);		/* TODO */
  void (*reshapefunc) (int width, int height);
  void (*visibilityfunc) (int state);
  void (*entryfunc) (int state);
  void (*windowstatusfunc) (int status);				/* TODO */
  void (*specialfunc) (int key, int x, int y);
  void (*specialupfunc) (int key, int x, int y);			/* TODO */
  void (*mousefunc) (int button, int state, int x, int y);
  void (*joystickfunc) (unsigned int buttonMask, int x, int y, int z);	/* TODO */
  void (*motionfunc) (int x, int y);
  void (*passivemotionfunc) (int x, int y);

  BOOL needredisplay,
	needreshape, needreshapegui,
	needposition, needpositiongui,
	needvisibility, visible,
	needleftmenu, needmiddlemenu, needrightmenu,
	fullscreen, lastoutwindow,
	push, pop, show, hide, iconify;
};

struct GlutGameMode {
  struct MsgPort *backuped_port;
  struct GlutWindow *backuped_window;
  
  struct Screen *GameScreen;
  struct GlutWindow FakeyWindow;
};

struct GlutStuff {
  struct MsgPort *msgport;

  struct GlutWindow *activeWindow;						/* window set by glutSetWindow() */
  struct GlutWindow *curwin;
  struct nlist Windows;
  int nextwinid;

  struct GlutMenu *activeMenu;							/* window set by glutSetMenu() */
  struct GlutMenu *curmenu;
  struct nlist Menues;
  int nextmenuid;

  /* stuff for the glutInitWindow* functions */
  int initposx, initposy,
      initwidth, initheight,
      scrwidth, scrheight;
  unsigned int initdisplaymode;

  char *pubscreenname;

  /* basic overwriteable modes */
  int doublemode,
      rgbamode,
      stereomode;
  palMode palmode;

  GLboolean rgba,
	    alpha,
	    db,
	    accum,
	    depth,
	    stencil,
	    multisample,
	    stereo,
	    luminance;

  void (*idlefunc) (void);
  void (*menustatefunc) (int state);
  void (*menustatusfunc) (int status, int x, int y);

  ULONG basetime_secs, basetime_micros;
  BOOL havebasetime;

  /* stuff for the glutGameModeString functions */
  int gameposx, gameposy;
  int gamewidth, gameheight;

  struct GlutGameMode *GameMode;
};

/* deactive debugging-output only on request */
//#if !defined(NDEBUG) && !defined(NODEBUG)
extern LONG debugOutputGLUT;
#include <stdio.h>
#define DEBUGOUT(level,str,args...) ({	\
  if(debugOutputGLUT >= level) {	\
 /* Printf(str,"gl  : "); */		\
 /* Printf(str,##args);	*/		\
    fprintf(stderr,"glut: ");		\
    fprintf(stderr,str,##args);		\
    fflush(stderr);			\
  }					\
})
//#else
//#define DEBUGOUT(level,str,args...) ({})
//#define	DEBUGOUT_REMOVED
//#endif

#define Error(str,args...) ({		\
/*Printf("Fatal error: "); */		\
/*Printf(str,##args); */		\
  fprintf(stderr,"Fatal error: ");	\
  fprintf(stderr,str,##args);		\
  fflush(stderr);			\
  abort();				\
})

extern struct GlutStuff glutstuff;
#ifndef WARPOS
extern struct Library *IntuitionBase;
extern struct Library *GfxBase;
extern struct Library *DOSBase;
#else
#pragma pack(push,2)
#endif
extern struct Library *GadToolsBase;
extern struct Library *LayersBase;
extern struct ExecBase *SysBase;
#pragma pack(pop)

int stuffGetNewWinID(void);
struct GlutWindow *stuffGetWin(int winid);
void stuffLinkInWin(struct GlutWindow *gw);
void stuffLinkInSubWin(struct GlutWindow *gw, struct GlutWindow *gwn);
void stuffLinkOutWin(struct GlutWindow *gw);
void stuffMakeCurrent(struct GlutWindow *gw);

int stuffGetNewMenuID(void);
struct GlutMenu *stuffGetMenu(int menuid);
void stuffLinkInMenu(struct GlutMenu *gm);
void stuffLinkOutMenu(struct GlutMenu *gm);
void stuffMakeCurrentMenu(struct GlutMenu *gm);

struct GlutMenuEntry *stuffGetMenuEntry(int entry, struct GlutMenu *gm);
void stuffLinkInMenuEntry(struct GlutMenuEntry *gme, struct GlutMenu *gm);
void stuffLinkOutMenuEntry(struct GlutMenuEntry *gme, struct GlutMenu *gm);

#define	LayerWidth(l)		(l->bounds.MaxX - l->bounds.MinX + 1)
#define	LayerHeight(l)		(l->bounds.MaxY - l->bounds.MinY + 1)
#define	InnerWidth(w)		(w->Width - w->BorderLeft - w->BorderRight)
#define	InnerHeight(w)		(w->Height - w->BorderTop - w->BorderBottom)

#ifdef USE_CLIP_LAYER
struct Region *clipWindow(struct Window *win, LONG minX, LONG minY, LONG maxX, LONG maxY);
struct Region *clipWindowToBorders(struct Window *win);
#endif
