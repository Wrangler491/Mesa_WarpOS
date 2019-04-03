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
 * glutChangeToMenuSubEntry.c
 *
 * Version 1.0  27 Jun 1998
 * by Jarno van der Linden
 * jarno@kcbbs.gen.nz
 *
 */

#include "glutstuff.h"

void glutChangeToMenuEntry(int entry, const char *name, int value)
{
  struct GlutMenuEntry *gme;

  if (glutstuff.curmenu) {
    if((gme = stuffGetMenuEntry(entry, glutstuff.curmenu))) {
      gme->name = name;
      gme->EntryValue = value;
      gme->issubmenu = FALSE;
      gme->EntryMenu = 0;

      glutstuff.curmenu->needupdate = TRUE;
    }
    else
      DEBUGOUT(1, "somethings wrong in glutChangeToMenuEntry(%d, %s, %d)\n", entry, name, value);
  }
}

void glutChangeToSubMenu(int entry, const char *name, int menu)
{
  struct GlutMenuEntry *gme;

  if (glutstuff.curmenu) {
    if ((gme = stuffGetMenuEntry(entry, glutstuff.curmenu))) {
      gme->name = name;
      gme->EntryValue = menu;
      gme->issubmenu = TRUE;
      gme->EntryMenu = stuffGetMenu(menu);

      glutstuff.curmenu->needupdate = TRUE;
    }
    else
      DEBUGOUT(1, "somethings wrong in glutChangeToSubMenu(%d, %s, %d)\n", entry, name, menu);
  }
}
