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
 * glutGetModifiers.c
 *
 * Version 1.0  27 Jun 1998
 * by Jarno van der Linden
 * jarno@kcbbs.gen.nz
 *
 */

#ifdef WARPOS
#pragma pack(push,2)
#endif
#include <devices/inputevent.h>
#pragma pack(pop)
#include "glutstuff.h"

int glutGetModifiers(void)
{
  int mods = 0;

  if (glutstuff.curwin) {
    DEBUGOUT(2, "qualifier: %x\n", glutstuff.curwin->qualifiers);
    if (glutstuff.curwin->qualifiers & IEQUALIFIER_LSHIFT)
      mods |= GLUT_ACTIVE_SHIFT;
    if (glutstuff.curwin->qualifiers & IEQUALIFIER_RSHIFT)
      mods |= GLUT_ACTIVE_SHIFT;
    if (glutstuff.curwin->qualifiers & IEQUALIFIER_CONTROL)
      mods |= GLUT_ACTIVE_CTRL;
    if (glutstuff.curwin->qualifiers & IEQUALIFIER_LALT)
      mods |= GLUT_ACTIVE_ALT;
    if (glutstuff.curwin->qualifiers & IEQUALIFIER_RALT)
      mods |= GLUT_ACTIVE_ALT;
  }
  else
    DEBUGOUT(1, "no window to get modifiers from\n");

  return mods;
}
