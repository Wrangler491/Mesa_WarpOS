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
 * glutReshapePositionWindow.c
 *
 * Version 1.0  28 Jun 1998
 * by Jarno van der Linden
 * jarno@kcbbs.gen.nz
 *
 */

#include "glutstuff.h"

void glutReshapeWindow(int width, int height)
{
  IGNORE_IN_GAME_MODE();

  if (glutstuff.curwin) {
    glutstuff.curwin->winwidth = width;
    glutstuff.curwin->winheight = height;

    glutstuff.curwin->needreshape = TRUE;
    glutstuff.curwin->fullscreen = FALSE;
  }
}

void glutPositionWindow(int x, int y)
{
  IGNORE_IN_GAME_MODE();

  if (glutstuff.curwin) {
    glutstuff.curwin->winx = x;
    glutstuff.curwin->winy = y;

    glutstuff.curwin->needposition = TRUE;
    glutstuff.curwin->fullscreen = FALSE;
  }
}

void glutFullScreen(void)
{
  IGNORE_IN_GAME_MODE();

  if (glutstuff.curwin)
    glutstuff.curwin->fullscreen = TRUE;
}
