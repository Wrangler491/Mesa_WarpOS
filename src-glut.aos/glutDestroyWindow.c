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
 * glutDestroyWindow.c
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
#include <stdlib.h>
#ifndef WARPOS
#include <inline/intuition.h>
#include <inline/graphics.h>
#include <inline/exec.h>
#include <inline/layers.h>
#include <inline/gadtools.h>
#else
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/exec.h>
#include <proto/layers.h>
#include <proto/gadtools.h>
#pragma pack(pop)
#endif
#include "glutstuff.h"

void StripIntuiMessages(struct MsgPort *mp, struct Window *win)
{
  struct IntuiMessage *msg;
  struct Node *succ;

  msg = (struct IntuiMessage *)mp->mp_MsgList.lh_Head;

  while ((succ = msg->ExecMessage.mn_Node.ln_Succ)) {
    if (msg->IDCMPWindow == win) {
      Remove(&msg->ExecMessage.mn_Node);
      ReplyMsg((struct Message *)msg);
    }
    msg = (struct IntuiMessage *)succ;
  }
}

void CloseWindowSafely(struct GlutWindow *win)
{
  if (win && win->window) {
    Forbid();
    StripIntuiMessages(win->window->UserPort, win->window);
    if (!glutstuff.GameMode) {							/* if in gamemode somethings closed, it MUST be the GameMode-window */
      win->window->UserPort = NULL;
      ModifyIDCMP(win->window, 0L);
    }
    Permit();

#ifdef USE_CLIP_LAYER_
    if (win->clipreg) {
      win->clipreg = InstallClipRegion(win->window->WLayer, win->clipreg);
      DisposeRegion(win->clipreg);
      win->clipreg = NULL;
    }
#endif
    CloseWindow(win->window);
  }
}

void glutDestroyWindow(int win)
{
  struct GlutWindow *actWindow;

  /* link out */
  if ((actWindow = stuffGetWin(win))) {
    struct Menu *menu;
    struct nnode *act;

    stuffLinkOutWin(actWindow);

    if (actWindow->window)
      ClearMenuStrip(actWindow->window);
    if ((menu = actWindow->menu)) {
      while (menu) {
        FreeMenus(menu->FirstItem);						/* TODO: cast */
        menu->FirstItem = NULL;
        menu = menu->NextMenu;
      }
      FreeMenus(actWindow->menu);
    }

    while ((act = nGetTail(&actWindow->WindowTimers))) {
      nRemove(&actWindow->WindowTimers, act);
      FreeVecPooled(glutPool, (ULONG *)act);					/* remove something out of the list, so we can't use nGetNext() safely */
    }

    while ((act = nGetTail(&actWindow->SubWindows)))
      glutDestroyWindow(((struct GlutWindow *)act)->WinID);			/* remove something out of the list, so we can't use nGetNext() safely */

    if (actWindow->vi)
      FreeVisualInfo(actWindow->vi);

    if (actWindow->context)
      amigaMesaDestroyContext(actWindow->context);
    if (actWindow->window)
      CloseWindowSafely(actWindow);

    FreeVecPooled(glutPool, (ULONG *)actWindow);
  }
  else
    DEBUGOUT(1, "somethings wrong in glutDestroyWindow(%d)\n", win);
}
