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
 * glutGetSetWindow.c
 *
 * Version 1.0  28 Jun 1998
 * by Jarno van der Linden
 * jarno@kcbbs.gen.nz
 *
 */

#ifndef WARPOS
#include <inline/intuition.h>
#else
#pragma pack(push,2)
#include <proto/intuition.h>
#pragma pack(pop)
#endif
#include "glutstuff.h"

void glutSetWindow(int win)
{
  struct GlutWindow *gw;

  if ((gw = stuffGetWin(win))) {
    stuffMakeCurrent(gw);
    ActivateWindow(gw->window);
    glutstuff.activeWindow = gw;
  }
  else
    DEBUGOUT(1, "somethings wrong in glutSetWindow(%d)\n", win);
}

void glutSetWindowTitle(const char *title)
{
  IGNORE_IN_GAME_MODE();

  if (glutstuff.curwin && glutstuff.curwin->window)
    SetWindowTitles(glutstuff.curwin->window, title, (UBYTE *) ~ 0);
  else
    DEBUGOUT(1, "no window to set title to\n");
}

int glutGetWindow(void)
{
  return glutstuff.curwin ?
	 glutstuff.curwin->WinID : 0;
}

void glutSetCursor(int cursor)
{
  if (glutstuff.curwin)
    glutstuff.curwin->cursor = cursor;					/* TODO: different pointer images */
  else
    DEBUGOUT(1, "no window to set cursor to\n");
}

void glutWarpPointer(int x, int y)
{
  if (glutstuff.curwin) {
  										/* TODO: MouseMove */
  }
  else
    DEBUGOUT(1, "no window to warp pointer from\n");
}
