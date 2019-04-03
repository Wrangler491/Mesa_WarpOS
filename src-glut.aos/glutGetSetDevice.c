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
 * glutGetDevice.c
 *
 * Version 1.0  27 Jun 1998
 * by Jarno van der Linden
 * jarno@kcbbs.gen.nz
 *
 */

#include "glutstuff.h"

int glutDeviceGet(GLenum param)
{
 /*
  * TODO: joystick-stuff
  */
  switch (param) {
    case GLUT_HAS_KEYBOARD:
    case GLUT_HAS_MOUSE:
      return TRUE;
    case GLUT_HAS_JOYSTICK:
    case GLUT_HAS_SPACEBALL:
    case GLUT_HAS_DIAL_AND_BUTTON_BOX:
    case GLUT_HAS_TABLET:		/* not really true, intuition supports tablets */
      return FALSE;
    case GLUT_NUM_MOUSE_BUTTONS:
      return 2;				/* not really true, could be 3 button mouse */
    case GLUT_DEVICE_IGNORE_KEY_REPEAT:
      return glutstuff.curwin ? glutstuff.curwin->ignorekeyrepeat : 0;
    case GLUT_DEVICE_KEY_REPEAT:
      return glutstuff.curwin ? glutstuff.curwin->repeatmode : 0;
    case GLUT_JOYSTICK_POLL_RATE:
/*  case GLUT_NUM_JOYSTICK_AXES: */	/* FUTURE: glut 3.8 */
/*  case GLUT_NUM_JOYSTICK_BUTTONS: */	/* FUTURE: glut 3.8 */
    case GLUT_NUM_SPACEBALL_BUTTONS:
    case GLUT_NUM_BUTTON_BOX_BUTTONS:
    case GLUT_NUM_DIALS:
    case GLUT_NUM_TABLET_BUTTONS:
      return -1;
    default:
      break;
  }
  return -1;
}

void glutIgnoreKeyRepeat(int ignore)
{
  if (glutstuff.curwin)
    glutstuff.curwin->ignorekeyrepeat = ignore;
}

void glutSetKeyRepeat(int repeatmode)
{
  if (glutstuff.curwin)
    glutstuff.curwin->repeatmode = repeatmode;
}
