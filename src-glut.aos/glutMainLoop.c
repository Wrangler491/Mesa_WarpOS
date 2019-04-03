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
 * glutMainLoop.c
 *
 * Version 1.0  27 Jun 1998
 * by Jarno van der Linden
 * jarno@kcbbs.gen.nz
 *
 */

#ifdef WARPOS
#pragma pack(push,2)
#endif
extern struct ExecBase *SysBase;
#ifndef WARPOS
#include <inline/intuition.h>
#include <inline/gadtools.h>
#include <inline/exec.h>
#include <inline/dos.h>
#include <inline/graphics.h>
#include <inline/layers.h>
#else
#include <proto/intuition.h>
#include <proto/gadtools.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/graphics.h>
#include <proto/layers.h>
#endif
#include <libraries/gadtools.h>
#pragma pack(pop)

#include <stdlib.h>

#include "glutstuff.h"

extern void RedoMenu(int button, struct GlutMenu *glutmenu);

static int ConvRaw(UWORD code, UWORD qual)
{
  switch (code) {
    case CURSORLEFT:
      return GLUT_KEY_LEFT;
    case CURSORUP:
      return GLUT_KEY_UP;
    case CURSORRIGHT:
      return GLUT_KEY_RIGHT;
    case CURSORDOWN:
      return GLUT_KEY_DOWN;
    case 0x50:
      return GLUT_KEY_F1;
    case 0x51:
      return GLUT_KEY_F2;
    case 0x52:
      return GLUT_KEY_F3;
    case 0x53:
      return GLUT_KEY_F4;
    case 0x54:
      return GLUT_KEY_F5;
    case 0x55:
      return GLUT_KEY_F6;
    case 0x56:
      return GLUT_KEY_F7;
    case 0x57:
      return GLUT_KEY_F8;
    case 0x58:
      return GLUT_KEY_F9;
    case 0x59:
      return GLUT_KEY_F10;
  }

  return code;
}

static int ConvToButton(UWORD code, UWORD qual)
{
  switch (code) {
    case SELECTUP:
    case SELECTDOWN:
      if (qual & IEQUALIFIER_RCOMMAND)
	return GLUT_MIDDLE_BUTTON;
      if (qual & IEQUALIFIER_RALT)
	return GLUT_RIGHT_BUTTON;
      return GLUT_LEFT_BUTTON;
    case MIDDLEUP:
    case MIDDLEDOWN:
      return GLUT_MIDDLE_BUTTON;
    case MENUUP:
    case MENUDOWN:
      return GLUT_RIGHT_BUTTON;
  }

  return code;
}

static int ConvToButtonState(UWORD code)
{
  switch (code) {
    case SELECTUP:
    case MIDDLEUP:
    case MENUUP:
      return GLUT_UP;
    case SELECTDOWN:
    case MIDDLEDOWN:
    case MENUDOWN:
      return GLUT_DOWN;
  }

  return code;
}

void glutMainLoop(void)
{
  BOOL wanttoquit = FALSE;

  while (1) {
    struct GlutWindow *actWindow;
    struct GlutMenu *actMenu;
    struct IntuiMessage *msg;
    struct Window *win;
    BOOL idleing = TRUE;

    if(!glutstuff.Windows.nodes) {
      DEBUGOUT(1, "no windows open to process\n");
      return;
    }

    /* Apply changes
     */
    actWindow = (struct GlutWindow *)&glutstuff.Windows;
    while ((actWindow = (struct GlutWindow *)nGetNext(&actWindow->WindowNode))) {
      win = actWindow->window;
      stuffMakeCurrent(actWindow);

      if (actWindow->WindowTimers.nodes) {
        struct GlutTimer *gt = (struct GlutTimer *)nGetHead(&actWindow->WindowTimers);

	while(gt) {
	  struct GlutTimer *new = (struct GlutTimer *)nGetNext(&gt->TimerNode);
	  ULONG secs, micros;

	  CurrentTime(&secs, &micros);						/* better for accuracity */
	  if ((secs > gt->secs) || ((secs == gt->secs) && (micros > gt->micros))) {
	    DEBUGOUT(2, "do timer (%ld:%ld), set for (%ld:%ld)\n", secs, micros, gt->secs, gt->micros);

	    idleing = FALSE;
	    if (gt->timerfunc)							/* be paranoid */
	      (*gt->timerfunc) (gt->TimerValue);

	    nRemove(&actWindow->WindowTimers, &gt->TimerNode);
	    FreeVecPooled(glutPool, (ULONG *)gt);
	  }
	  else
	    DEBUGOUT(2, "skip timer (%ld:%ld), wait for (%ld:%ld)\n", secs, micros, gt->secs, gt->micros);

	  gt = new;
	}
      }

      /* Hide/Show/Iconify */							/* TODO: workbench.library icons */
      /*  then break if hide or iconify */
      /*  continue if show */
      if (actWindow->pop) {
        WindowToFront(win);
        actWindow->pop = FALSE;
        actWindow->push = FALSE;
      }
      else if (actWindow->push) {
        WindowToBack(win);
        actWindow->push = FALSE;
        actWindow->pop = FALSE;
      }

      if (actWindow->needposition) {
	actWindow->needposition = FALSE;
	actWindow->needpositiongui = FALSE;
	/* Let the change be made through the gui */
	ChangeWindowBox(win, actWindow->winx, actWindow->winy, actWindow->winwidth, actWindow->winheight);
	actWindow->fullscreen = FALSE;		/* BUG?: could prevent fullscreen-mode */
      }

      if (actWindow->needpositiongui) {
	actWindow->needpositiongui = FALSE;
	actWindow->fullscreen = FALSE;		/* BUG?: could prevent fullscreen-mode */
      }

      if (actWindow->needreshape) {
	actWindow->needreshape = FALSE;
	actWindow->needreshapegui = FALSE;				/* Reshaping and redisplaying is useless */
	actWindow->needredisplay = FALSE;				/* this round. Change is inevitable */
	/* Let the change be made through the gui */
	ChangeWindowBox(win, actWindow->winx, actWindow->winy, actWindow->winwidth, actWindow->winheight);
	actWindow->fullscreen = FALSE;		/* BUG?: could prevent fullscreen-mode */
      }

      if (actWindow->needreshapegui) {
	actWindow->needreshapegui = FALSE;
	if (actWindow->reshapefunc)
	  (*actWindow->reshapefunc) (InnerWidth(win), InnerHeight(win));
	actWindow->fullscreen = FALSE;		/* BUG?: could prevent fullscreen-mode */
      }

      if (actWindow->fullscreen) {
       /*
        * TODO: make real fullscreen
        *  close window, open borderless window on old screen, rearrange contexts
        *  pop the window to front if something overlays it
        * ALTERNATIVE:
        *  try to make window borderless on the run
        */
	actWindow->winx = 0;
	actWindow->winy = 0;
	actWindow->winwidth = win->WScreen->Width;
	actWindow->winheight = win->WScreen->Height;
	ChangeWindowBox(win, actWindow->winx,
			     actWindow->winy,
			     actWindow->winwidth,
			     actWindow->winheight);
	WindowToFront(win);
      }

      if (actWindow->needredisplay) {
	actWindow->needredisplay = FALSE;
	if (actWindow->displayfunc)
	  (*actWindow->displayfunc) ();
      }

      if (actWindow->needvisibility) {
	actWindow->needvisibility = FALSE;
	if (actWindow->visibilityfunc)
	  (*actWindow->visibilityfunc) (actWindow->visible ? GLUT_VISIBLE : GLUT_NOT_VISIBLE);
      }

      if ((actWindow->leftmenu && actWindow->leftmenu->needupdate) || actWindow->needleftmenu) {
	actWindow->needleftmenu = FALSE;
	RedoMenu(GLUT_LEFT_BUTTON, actWindow->leftmenu);
      }

      if ((actWindow->middlemenu && actWindow->middlemenu->needupdate) || actWindow->needmiddlemenu) {
	actWindow->needmiddlemenu = FALSE;
	RedoMenu(GLUT_MIDDLE_BUTTON, actWindow->middlemenu);
      }

      if ((actWindow->rightmenu && actWindow->rightmenu->needupdate) || actWindow->needrightmenu) {
	actWindow->needrightmenu = FALSE;
	RedoMenu(GLUT_RIGHT_BUTTON, actWindow->rightmenu);
      }
    }

    if (glutstuff.Menues.nodes) {
      actMenu = (struct GlutMenu *)&glutstuff.Menues;
      while ((actMenu = (struct GlutMenu *)nGetNext(&actMenu->MenuNode))) {
	stuffMakeCurrentMenu(actMenu);
	actMenu->needupdate = FALSE;
      }
    }

    /* Wait for something to happen
     */
    if (glutstuff.idlefunc == NULL)
      Wait(1L << glutstuff.msgport->mp_SigBit);

    /* Handle all messages (if any)
     */
    while ((msg = (struct IntuiMessage *)GetMsg(glutstuff.msgport))) {
      struct GlutMenuEntry *actEntry;
      struct MenuItem *item;
      UWORD menuNumber;
      struct IntuiMessage cmsg = *msg;

      ReplyMsg(&msg->ExecMessage);

      stuffMakeCurrent((struct GlutWindow *)cmsg.IDCMPWindow->UserData);
      if (glutstuff.curwin != (struct GlutWindow *)cmsg.IDCMPWindow->UserData) {
        Printf("serious error found in stuffMakeCurrent(), quitting ...\n");
        continue;
      }
      actWindow = glutstuff.curwin;

      if (actWindow->window != cmsg.IDCMPWindow) {
        Printf("serious error found in stuffMakeCurrent(), quitting ...\n");
        continue;
      }
      win = actWindow->window;

      actWindow->qualifiers = cmsg.Qualifier;
      switch (cmsg.Class) {
	case IDCMP_REFRESHWINDOW:							/* received only in DB-mode, non-DB-modes are NoCareRefresh */
	  BeginRefresh(win);
	  amigaMesaSwapBuffers(actWindow->context);					/* put current contents into damaged region */
	  EndRefresh(win, TRUE);
	  break;
	case IDCMP_CHANGEWINDOW:
	  idleing = FALSE;
	  actWindow->winx = win->LeftEdge;
	  actWindow->winy = win->TopEdge;
	  actWindow->winwidth = InnerWidth(win);
	  actWindow->winheight = InnerHeight(win);
	  if ((InnerWidth(win) != actWindow->wincurwidth) ||
	      (InnerHeight(win) != actWindow->wincurheight)) {
#ifdef USE_CLIP_LAYER_
	    if (actWindow->clipreg)							/* Get rid of last clipping region */
	      DisposeRegion(InstallClipRegion(win->WLayer, NULL));
	    clipWindowToBorders(win);
#endif
	    actWindow->wincurwidth = InnerWidth(win);
	    actWindow->wincurheight = InnerHeight(win);
	    actWindow->needreshapegui = TRUE;
	    actWindow->needredisplay = TRUE;
	  }
	  if ((win->LeftEdge != actWindow->wincurx) ||
	      (win->TopEdge != actWindow->wincury)) {
	    actWindow->wincurx = win->LeftEdge;
	    actWindow->wincury = win->TopEdge;
	    actWindow->needpositiongui = TRUE;
	  }
	  break;
	case IDCMP_VANILLAKEY:
	  if (cmsg.Code & IECODE_UP_PREFIX) {
	    if (actWindow->keyboardupfunc) {
	      if (!((cmsg.Qualifier & IEQUALIFIER_REPEAT) &&
		    (actWindow->ignorekeyrepeat ||
		     (actWindow->repeatmode == GLUT_KEY_REPEAT_OFF)))) {
	        idleing = FALSE;
	        (*actWindow->keyboardupfunc) (ConvRaw(cmsg.Code, cmsg.Qualifier), cmsg.MouseX, cmsg.MouseY);
	      }
	    }
	  }
	  else if (actWindow->keyboardfunc) {
	    if (!((cmsg.Qualifier & IEQUALIFIER_REPEAT) &&
		  (actWindow->ignorekeyrepeat ||
		   (actWindow->repeatmode == GLUT_KEY_REPEAT_OFF)))) {
	      idleing = FALSE;
	      (*actWindow->keyboardfunc) (ConvRaw(cmsg.Code, cmsg.Qualifier), cmsg.MouseX, cmsg.MouseY);
	    }
	  }
	  break;
	case IDCMP_RAWKEY:
	  if (cmsg.Code & IECODE_UP_PREFIX) {
	    if (actWindow->specialupfunc) {
	      if (!((cmsg.Qualifier & IEQUALIFIER_REPEAT) &&
		    (actWindow->ignorekeyrepeat ||
		     (actWindow->repeatmode == GLUT_KEY_REPEAT_OFF)))) {
	        idleing = FALSE;
	        (*actWindow->specialupfunc) (ConvRaw(cmsg.Code, cmsg.Qualifier), cmsg.MouseX, cmsg.MouseY);
	      }
	    }
	  }
	  else if (actWindow->specialfunc) {
	    if (!((cmsg.Qualifier & IEQUALIFIER_REPEAT) &&
		  (actWindow->ignorekeyrepeat ||
		   (actWindow->repeatmode == GLUT_KEY_REPEAT_OFF)))) {
	      idleing = FALSE;
	      (*actWindow->specialfunc) (ConvRaw(cmsg.Code, cmsg.Qualifier), cmsg.MouseX, cmsg.MouseY);
	    }
	  }
	  break;
	case IDCMP_MENUPICK:
	  menuNumber = cmsg.Code;
	  while (menuNumber != MENUNULL) {
	    item = ItemAddress(actWindow->menu, menuNumber);
	    actEntry = (struct GlutMenuEntry *)GTMENUITEM_USERDATA(item);
	    stuffMakeCurrentMenu(actEntry->EntryMenu);
	    if (glutstuff.curmenu->menufunc) {
	      idleing = FALSE;
	      (*glutstuff.curmenu->menufunc) (actEntry->EntryValue);
	    }
	    menuNumber = item->NextSelect;
	  }
	  break;
	case IDCMP_CLOSEWINDOW:
	  if (wanttoquit) {
	    /* User wants to quit, but ESC doesn't do anything
	     * Panic, and return in the hope that it will drop
	     * through to an end-of-program call.
	     * (Note that we don't use exit(), as we may want
	     * to put all this in a run-time library).
	     */
	    return;
	  }
	  if (actWindow->keyboardfunc) {
	    idleing = FALSE;
	    (*actWindow->keyboardfunc) (27, 0, 0);
	  }
	  wanttoquit = TRUE;
	  break;
	case IDCMP_MOUSEBUTTONS:
	  if (actWindow->mousefunc) {
	    idleing = FALSE;
	    (*actWindow->mousefunc) (ConvToButton(cmsg.Code, cmsg.Qualifier), ConvToButtonState(cmsg.Code), cmsg.MouseX, cmsg.MouseY);
	  }
	  break;
	case IDCMP_INTUITICKS:
	  if ((actWindow->mousex != cmsg.MouseX) ||
	      (actWindow->mousey != cmsg.MouseY)) {
	    actWindow->mousex = cmsg.MouseX;
	    actWindow->mousey = cmsg.MouseY;

	    if ((cmsg.Qualifier & IEQUALIFIER_LEFTBUTTON) &&
		(actWindow->motionfunc)) {
	      idleing = FALSE;
	      (*actWindow->motionfunc) (cmsg.MouseX, cmsg.MouseY);
	    }
	    else if (actWindow->passivemotionfunc) {
	      idleing = FALSE;
	      (*actWindow->passivemotionfunc) (cmsg.MouseX, cmsg.MouseY);
	    }

	    if (actWindow->entryfunc) {
	      if ((actWindow->mousex < actWindow->winx) ||
		  (actWindow->mousey < actWindow->winy) ||
		  (actWindow->mousex > (actWindow->winx + win->Width)) ||
		  (actWindow->mousey > (actWindow->winy + win->Height))) {
	        /* out of window region */
	        if (!actWindow->lastoutwindow) {
		  idleing = FALSE;
	          (*actWindow->entryfunc) (GLUT_LEFT);
	          actWindow->lastoutwindow = TRUE;
	        }
	      }
	      else {
	        /* in to window region */
	        if (actWindow->lastoutwindow) {
		  idleing = FALSE;
	          (*actWindow->entryfunc) (GLUT_ENTERED);
	          actWindow->lastoutwindow = FALSE;
	        }
	      }
	    }
	  }
	  break;
      }
    }

#if 0    
    if (actWindow->windowstatusfunc) {
      struct GlutWindow *gwo;
      
      for (gwo = (struct GlutWindow *)nGetHead(&glutstuff.Windows); gwo; gwo = (struct GlutWindow *)nGetNext(&gwo->WindowNode)) {
        /* skip us, compare only others */
        if(gwo != actWindow) {
          /* ... dynamic rectangle list */
        }
      }
    }
#endif

    /* If nothing is happening, call the idle function
     */
    if (idleing && glutstuff.idlefunc)
      (*glutstuff.idlefunc) ();
  }
}
