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
 * glutstuff.c
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

#include <signal.h>
#include <stdlib.h>
#ifndef WARPOS
#include <inline/dos.h>
#include <inline/exec.h>
#include <inline/graphics.h>
#include <inline/layers.h>
#include <inline/intuition.h>
#include <exec/memory.h>
#else
#pragma pack(push,2)
#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/layers.h>
#include <proto/intuition.h>
#include <exec/memory.h>
#pragma pack(pop)
#endif
#include "glutstuff.h"

#ifdef WARPOS
#include <liblists.c>
#endif

/**********************************************************************/

/* TODO: maybe we share GfxBase, IntuitionBase and amesaPool with aMesa */
struct Library *GadToolsBase = NULL;
#ifndef WARPOS
struct Library *GfxBase = NULL;
struct Library *DOSBase = NULL;
struct Library *IntuitionBase = NULL;
struct Library *LayersBase = NULL;
#else
//removed to avoid clash with libMesaGL.a
//struct GfxBase *GfxBase = NULL;
//struct DosLibrary *DOSBase = NULL;
//struct IntuitionBase *IntuitionBase = NULL;
struct Library *LayersBase = NULL;
#endif
APTR glutPool = NULL;
//#if !defined(NDEBUG) && !defined(NODEBUG)
LONG debugOutputGLUT = 100;
//#endif

struct GlutStuff glutstuff;

void glutShutDown(int code)
{
  exit(0);
}

void glutConstructor(void)
{
  char Flag[256];
  short int FlagLen;

  if (!(DOSBase = OpenLibrary("dos.library", 39))) {
    if ((DOSBase = OpenLibrary("dos.library", 0))) {
      Printf("Can't open dos.library version 39\n");
      CloseLibrary(DOSBase);
    }
    exit(10);
  }
  if (!(IntuitionBase = OpenLibrary("intuition.library", 39))) {
    Printf("Can't open intuition.library version 39\n");
    exit(10);
  }
  if (!(GadToolsBase = OpenLibrary("gadtools.library", 39))) {
    Printf("Can't open gadtools.library version 39\n");
    exit(10);
  }
  if (!(GfxBase = OpenLibrary("graphics.library", 39))) {
    Printf("Can't open graphics.library version 39\n");
    exit(10);
  }
  if(!(LayersBase = OpenLibrary("layers.library", 39))) {
    Printf("Can't open layers.library version 39\n");
    exit(10);
  }
#ifdef WARPOS
  if (!(glutPool = CreatePool(MEMF_PUBLIC | MEMF_CLEAR, 65536, 32768))) {
#else
  if (!(glutPool = CreatePool(MEMF_ANY | MEMF_CLEAR, 65536, 32768))) {
#endif
    Printf("Can't create mempools for glut\n");
    exit(10);
  }

  signal(SIGINT, glutShutDown);
  signal(SIGTERM, glutShutDown);

  bzero(&glutstuff, sizeof(glutstuff));

#if !defined(NDEBUG) && !defined(NODEBUG)
  if((FlagLen = GetVar("GLUT_DEBUG", Flag, 255, 0)) > 0) {
    Flag[(FlagLen < 256 ? FlagLen : 255)] = '\0';
    if(!strncasecmp(Flag, "ON", 2)) {
      debugOutputGLUT = 1;
      if ((Flag[2] == ' ') &&
          (Flag[3] >= '0') &&
          (Flag[3] <= '9'))
        debugOutputGLUT = Flag[3] - '0';
      DEBUGOUT(1, "enable debugging stage %ld\n", debugOutputGLUT);
    }
    else {
      debugOutputGLUT = 0;
      DEBUGOUT(1, "disable debugging\n");
    }
  }
#endif

  if((FlagLen = GetVar("MESA_GLUTOVERWRITE", Flag, 255, 0)) > 0) {
    char *FFlow = Flag;

    Flag[(FlagLen < 256 ? FlagLen : 255)] = '\0';

    while(*FFlow != '\0') {
      if(!strncasecmp(FFlow, "DOUBLE_OFF", 10)) {
        glutstuff.doublemode = -1;
        DEBUGOUT(1, "force doublebuffering off\n");
      }
      else if(!strncasecmp(FFlow, "DOUBLE_ON", 9)) {
        glutstuff.doublemode = 1;
        DEBUGOUT(1, "force doublebuffering on\n");
      }
      else if(!strncasecmp(FFlow, "RGBA_OFF", 8)) {
        glutstuff.rgbamode = -1;
        DEBUGOUT(1, "force RGBA off\n");
      }
      else if(!strncasecmp(FFlow, "RGBA_ON", 7)) {
        glutstuff.rgbamode = 1;
        DEBUGOUT(1, "force RGBA on\n");
      }
      else if(!strncasecmp(FFlow, "STEREO_OFF", 8)) {
        glutstuff.stereomode = -1;
        DEBUGOUT(1, "force stereo off\n");
      }
      else if(!strncasecmp(FFlow, "STEREO_ON", 7)) {
        glutstuff.stereomode = 1;
        DEBUGOUT(1, "force stereo on\n");
      }
      while(*FFlow != '\0')
        if(*FFlow++ == ' ')
          break;
    }
  }

  if(!(glutstuff.msgport = CreateMsgPort()))
    exit (10);

  nNewList(&glutstuff.Windows);
  glutstuff.nextwinid = 1;

  nNewList(&glutstuff.Menues);
  glutstuff.nextmenuid = 1;

  glutstuff.initposx = -1;
  glutstuff.initposy = -1;
  glutstuff.initwidth = 300;
  glutstuff.initheight = 300;

  glutstuff.pubscreenname = "Mesa";

  glutstuff.rgba = GL_TRUE;
  glutstuff.depth = GL_TRUE;							/* depth isn't the default! */

  glutstuff.initdisplaymode = GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH;

  glutstuff.palmode = AMESA_WEIGHTED_MATCH;
}

void glutDestructor(void)
{
  struct nnode *act;

  while ((act = nGetTail(&glutstuff.Windows)))
    glutDestroyWindow(((struct GlutWindow *)act)->WinID);			/* remove something out of the list, so we can't use nGetNext() safely */

  while ((act = nGetTail(&glutstuff.Menues)))
    glutDestroyMenu(((struct GlutMenu *)act)->MenuID);				/* remove something out of the list, so we can't use nGetNext() safely */

  if (glutstuff.msgport)
    DeleteMsgPort(glutstuff.msgport);

  if(LayersBase) {
    CloseLibrary(LayersBase);
    LayersBase = 0;
  }
  if (GfxBase) {
    CloseLibrary(GfxBase);
    GfxBase = 0;
  }
  if (GadToolsBase) {
    CloseLibrary(GadToolsBase);
    GadToolsBase = 0;
  }
  if (IntuitionBase) {
    CloseLibrary(IntuitionBase);
    IntuitionBase = 0;
  }
  if (DOSBase) {
    CloseLibrary(DOSBase);
    DOSBase = 0;
  }
  if (glutPool) {
    DeletePool(glutPool);
    glutPool = 0;
  }
}

#if 0
asm("	.text; 	.stabs \"___CTOR_LIST__\",22,0,0,_constructor");
asm("	.text; 	.stabs \"___DTOR_LIST__\",22,0,0,_destructor");
#endif

/******************************************************************************/

void stuffMakeCurrent(struct GlutWindow *gw)
{
  if (gw && (glutstuff.curwin != gw)) {
    glutstuff.curwin = gw;
  //ActivateWindow(gw->window);				/* message-loop calls this, so don't use it here */
  //glutstuff.activeWindow = gw;

    if (gw->context) {
      amigaMesaBuffer buffer = NULL;

      amigaMesaGetContextTags(gw->context, AMA_Buffer, &buffer, TAG_DONE);
      if (buffer)
        amigaMesaMakeCurrent(gw->context, buffer);
    }
  }

  DEBUGOUT(4, "stuffMakeCurrent(0x%08x (%d))\n", gw, gw->WinID);
}

int stuffGetNewWinID(void)
{
  return glutstuff.nextwinid++;
}

struct GlutWindow *stuffGetWin(int winid)
{
  struct GlutWindow *act = NULL;
  
  if (glutstuff.Windows.nodes) {
    act = (struct GlutWindow *)&glutstuff.Windows;
    while((act = (struct GlutWindow *)nGetNext(&act->WindowNode))) {
      DEBUGOUT(4, "%d != %d\n", ((struct GlutWindow *)act)->WinID, winid);
      if (((struct GlutWindow *)act)->WinID == winid)
	break;
    }
    if (act && ((struct GlutWindow *)act)->WinID != winid)
      act = NULL;
  }

  DEBUGOUT(4, "0x%08x = stuffGetWindow(%d)\n", act, winid);
  return act;
}

void stuffLinkInWin(struct GlutWindow *gw)
{
  if (gw) {
    gw->WinID = stuffGetNewWinID();
  //dEnqueue(&glutstuff.Windows, &gw->WindowNode);
    nAddTail(&glutstuff.Windows, &gw->WindowNode);	/* ever growing list */
    stuffMakeCurrent(gw);
  }

  DEBUGOUT(4, "%d (of %d) = stuffLinkInWin(0x%08x)\n", gw->WinID, glutstuff.Windows.nodes, gw);
}

void stuffLinkInSubWin(struct GlutWindow *gw, struct GlutWindow *gwn)
{
  if (gw && gwn) {
    gwn->SubWindowParent = gw;
  //dEnqueue(&gw->SubWindows, &gwn->SubWindowNode);
    nAddTail(&gw->SubWindows, &gwn->SubWindowNode);	/* ever growing list */
  }

  DEBUGOUT(4, "%d (of %d) = stuffLinkInSubWin(0x%08x, 0x%08x)\n", gwn->WinID, gw->SubWindows.nodes, gw, gwn);
}

void stuffLinkOutWin(struct GlutWindow *gw)
{
  if (gw) {
    nRemove(&glutstuff.Windows, &gw->WindowNode);
    if (gw->SubWindowParent)				/* remove window from subwindow-list */
      nRemove(&gw->SubWindowParent->SubWindows, &gw->SubWindowNode);

    if (glutstuff.curwin == gw)
      glutstuff.curwin = glutstuff.activeWindow;
    if (glutstuff.curwin == gw)
      glutstuff.curwin = NULL;
  }

  DEBUGOUT(4, "stuffLinkOutWindow(0x%08x)\n", gw);
}

int stuffGetNewMenuID(void)
{
  return glutstuff.nextmenuid++;
}

struct GlutMenu *stuffGetMenu(int menuid)
{
  struct GlutMenu *act = NULL;
  
  if (glutstuff.Menues.nodes) {
    act = (struct GlutMenu *)&glutstuff.Menues;
    while((act = (struct GlutMenu *)nGetNext(&act->MenuNode))) {
      DEBUGOUT(4, "%d != %d\n", ((struct GlutMenu *)act)->MenuID, menuid);
      if (((struct GlutMenu *)act)->MenuID == menuid)
	break;
    }
    if (act && ((struct GlutMenu *)act)->MenuID != menuid)
      act = NULL;
  }

  DEBUGOUT(4, "0x%08x = stuffGetMenu(%d)\n", act, menuid);
  return act;
}

void stuffLinkInMenu(struct GlutMenu *gm)
{
  if (gm) {
    gm->MenuID = stuffGetNewMenuID();
  //dEnqueue(&glutstuff.Menues, &gm->MenuNode);
    nAddTail(&glutstuff.Menues, &gm->MenuNode);		/* ever growing list */
    stuffMakeCurrentMenu(gm);
  }

  DEBUGOUT(4, "%d (of %d) = stuffLinkInMenu(0x%08x)\n", gm->MenuID, glutstuff.Menues.nodes, gm);
}

void stuffLinkOutMenu(struct GlutMenu *gm)
{
  if (gm) {
    nRemove(&glutstuff.Menues, &gm->MenuNode);

    if (glutstuff.curmenu == gm)
      glutstuff.curmenu = glutstuff.activeMenu;
    if (glutstuff.curmenu == gm)
      glutstuff.curmenu = NULL;
  }

  DEBUGOUT(4, "stuffLinkOutMenu(0x%08x)\n", gm);
}

void stuffMakeCurrentMenu(struct GlutMenu *gm)
{
  glutstuff.curmenu = gm;
//glutstuff.activeMenu = gm;
}

struct GlutMenuEntry *stuffGetMenuEntry(int entry, struct GlutMenu *gm)	/* is 0 == first or 1? */
{
  struct GlutMenuEntry *gme = NULL;
  short int e = entry;

  if (gm && (gm->MenuEntries.nodes)) {
    gme = (struct GlutMenuEntry *)&gm->MenuEntries;
    while (--e >= 0)
      gme = (struct GlutMenuEntry *)nGetNext(&gme->EntryNode);
  }

  DEBUGOUT(4, "0x%08x = stuffGetMenuEntry(%d, 0x%08x)\n", gme, entry, gm);
  return gme;
}

void stuffLinkInMenuEntry(struct GlutMenuEntry *gme, struct GlutMenu *gm)
{
  if (gm && gme) {
    nAddTail(&gm->MenuEntries, &gme->EntryNode);
    gme->EntryMenu = gm;
  }

  DEBUGOUT(4, "%d = stuffLinkInMenuEntry(0x%08x, 0x%08x)\n", gm->MenuEntries.nodes, gme, gm);
}

void stuffLinkOutMenuEntry(struct GlutMenuEntry *gme, struct GlutMenu *gm)
{
  if (gm && gme) {
    nRemove(&gm->MenuEntries, &gme->EntryNode);
    gme->EntryMenu = NULL;
  }

  DEBUGOUT(4, "stuffLinkOutMenuEntry(0x%08x, 0x%08x)\n", gme, gm);
}

#ifdef USE_CLIP_LAYER_
/*
 * clipWindow()
 * Clip a window to a specified rectangle (given by upper left and
 * lower right corner.)  the removed region is returned so that it
 * may be re-installed later.
 */
static struct Region *clipWindow(struct Window *win, LONG minX, LONG minY, LONG maxX, LONG maxY)
{
  struct Region *new_region;
  struct Rectangle my_rectangle;

  /* set up the limits for the clip */
  my_rectangle.MinX = minX;
  my_rectangle.MinY = minY;
  my_rectangle.MaxX = maxX;
  my_rectangle.MaxY = maxY;

  /* get a new region and OR in the limits. */
  if ((new_region = NewRegion())) {
    if (!OrRectRegion(new_region, &my_rectangle)) {
      DisposeRegion(new_region);
      new_region = NULL;
    }
  }

  /*
   * Install the new region, and return any existing region.
   * If the above allocation and region processing failed, then
   * new_region will be NULL and no clip region will be installed.
   */
  return (InstallClipRegion(win->WLayer, new_region));
}

/*
 * clipWindowToBorders()
 * clip a window to its borders.
 * The removed region is returned so that it may be re-installed later.
 */
struct Region *clipWindowToBorders(struct Window *win)
{
  return (clipWindow(win, win->BorderLeft, win->BorderTop, win->Width - win->BorderRight - 1, win->Height - win->BorderBottom - 1));
}
#endif
