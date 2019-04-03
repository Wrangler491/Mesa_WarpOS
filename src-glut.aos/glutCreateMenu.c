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
 * glutCreateMenu.c
 *
 * Version 1.0  27 Jun 1998
 * by Jarno van der Linden
 * jarno@kcbbs.gen.nz
 *
 */

#include <stdlib.h>
#include "glutstuff.h"

int glutCreateMenu(void (*func) (int value))
{
  struct GlutMenu *gm;

  if ((gm = AllocVecPooled(glutPool, sizeof(struct GlutMenu)))) {
    nNewList(&gm->MenuEntries);
  
    gm->menufunc = func;
    gm->needupdate = TRUE;

    stuffLinkInMenu(gm);

    DEBUGOUT(2, "%d (%lx) = glutCreateMenu()\n", gm->MenuID, gm);
    return gm->MenuID;
  }

  DEBUGOUT(1, "failed to allocate menu\n");
  return 0;
}
