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
 * glutAddMenuEntry.c
 *
 * Version 1.0  27 Jun 1998
 * by Jarno van der Linden
 * jarno@kcbbs.gen.nz
 *
 */

#include <stdlib.h>
#include "glutstuff.h"

void glutAddMenuEntry(const char *name, int value) {
  struct GlutMenu *actMenu;

  if ((actMenu = glutstuff.curmenu)) {
    struct GlutMenuEntry *gme;

    if ((gme = (struct GlutMenuEntry *)AllocVecPooled(glutPool, sizeof(struct GlutMenuEntry)))) {
      nAddTail(&actMenu->MenuEntries, &gme->EntryNode);

      gme->name = name;
      gme->EntryValue = value;
      gme->issubmenu = FALSE;
      gme->EntryMenu = 0;

      actMenu->needupdate = TRUE;
    }
    else
      DEBUGOUT(1, "failed to allocate menu entry\n");
  }
}
