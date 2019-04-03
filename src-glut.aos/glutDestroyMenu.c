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
 * glutDestroyMenu.c
 *
 * Version 1.0  27 Jun 1998
 * by Jarno van der Linden
 * jarno@kcbbs.gen.nz
 *
 */

#include <stdlib.h>
#include "glutstuff.h"

void glutDestroyMenu(int menu)
{
  struct GlutMenu *actMenu;

  if ((actMenu = glutstuff.curmenu)) {
    struct GlutMenu *gm;

    if ((gm = stuffGetMenu(menu))) {
      stuffMakeCurrentMenu(gm);

      DEBUGOUT(3, "destroy %d entries\n", gm->NumEntries);
      while (gm->NumEntries)
        glutRemoveMenuItem(1);
      stuffLinkOutMenu(gm);

      FreeVecPooled(glutPool, (ULONG *)gm);
      if(gm != actMenu) {
        DEBUGOUT(3, "other menu restored\n");
        stuffMakeCurrentMenu(actMenu);
      }
      else
	glutstuff.curmenu = NULL;
    }
    else
      DEBUGOUT(1, "somethings wrong in glutDestroyMenu(%d)\n", menu);
  }
}
