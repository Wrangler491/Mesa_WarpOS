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
 * glutIconifying.c
 *
 * Version 1.0  27 Jun 1998
 * by Jarno van der Linden
 * jarno@kcbbs.gen.nz
 *
 */

#include "glutstuff.h"

void glutPopWindow(void) {
  IGNORE_IN_GAME_MODE();

  if (glutstuff.curwin) {
    if (glutstuff.curwin->push)
      glutstuff.curwin->push = FALSE;
    else
      glutstuff.curwin->pop = TRUE;
  }
}

void glutPushWindow(void) {
  IGNORE_IN_GAME_MODE();

  if (glutstuff.curwin) {
    if (glutstuff.curwin->pop)
      glutstuff.curwin->pop = FALSE;
    else
      glutstuff.curwin->push = TRUE;
  }
}

void glutIconifyWindow(void) {
  IGNORE_IN_GAME_MODE();

  if (glutstuff.curwin)
    glutstuff.curwin->iconify = TRUE;
}

void glutShowWindow(void) {
  IGNORE_IN_GAME_MODE();

  if (glutstuff.curwin) {
    if (glutstuff.curwin->show)
      glutstuff.curwin->show = FALSE;
    else
      glutstuff.curwin->hide = TRUE;
  }
}

void glutHideWindow(void) {
  IGNORE_IN_GAME_MODE();

  if (glutstuff.curwin) {
    if (glutstuff.curwin->show)
      glutstuff.curwin->show = FALSE;
    else
      glutstuff.curwin->hide = TRUE;
  }
}

void glutSetIconTitle(const char *title) {
  IGNORE_IN_GAME_MODE();
  										/* TODO: workbench.library icons */
}
