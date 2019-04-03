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
 * glutGetSetColor.c
 *
 * Version 1.0  28 Jun 1998
 * by Jarno van der Linden
 * jarno@kcbbs.gen.nz
 *
 */

#ifndef WARPOS
#include <inline/graphics.h>
#include <inline/intuition.h>
#else
#pragma pack(push,2)
#include <proto/graphics.h>
#include <proto/intuition.h>
#pragma pack(pop)
#endif
#include "glutstuff.h"

GLfloat glutGetColor(int ndx, int component)
{
  /* no window */
  if(!glutstuff.curwin || !glutstuff.curwin->context) {
    DEBUGOUT(1, "no window to get colors from\n");
    return (-1.0);
  }
  /* we are in rgb-mode not in index-mode */
  else if(glutGet(GLUT_WINDOW_RGBA)) {
    DEBUGOUT(1, "no index mode\n");
    return (-1.0);
  }
  /* index out of range */
  else if ((ndx < 0) || (ndx > 255)) {
    DEBUGOUT(1, "index out of range\n");
    return (-1.0);
  }

  switch (component) {
    case GLUT_RED:
      DEBUGOUT(4, "%g = glutGetColor(%d, %d)\n", ((GLfloat) ((amigaMesaGetOneColor(glutstuff.curwin->context, ndx) >> 16) & 0x000000FF)) / 255, ndx , component);

      return ((GLfloat) ((amigaMesaGetOneColor(glutstuff.curwin->context, ndx) >> 16) & 0x000000FF)) / 255;
      break;
    case GLUT_GREEN:
      DEBUGOUT(4, "%g = glutGetColor(%d, %d)\n", ((GLfloat) ((amigaMesaGetOneColor(glutstuff.curwin->context, ndx) >>  8) & 0x000000FF)) / 255, ndx , component);

      return ((GLfloat) ((amigaMesaGetOneColor(glutstuff.curwin->context, ndx) >>  8) & 0x000000FF)) / 255;
      break;
    case GLUT_BLUE:
      DEBUGOUT(4, "%g = glutGetColor(%d, %d)\n", ((GLfloat) ((amigaMesaGetOneColor(glutstuff.curwin->context, ndx) >>  0) & 0x000000FF)) / 255, ndx , component);

      return ((GLfloat) ((amigaMesaGetOneColor(glutstuff.curwin->context, ndx) >>  0) & 0x000000FF)) / 255;
      break;
    default:    
      return (-1.0);
      break;
  }

  return (-1.0);
}

#undef	CLAMP
#define CLAMP(i) ((i) > 1.0 ? 1.0 : ((i) < 0.0 ? 0.0 : (i)))

void glutSetColor(int cell, GLfloat red, GLfloat green, GLfloat blue)
{
  DEBUGOUT(4, "glutSetColor(%d, %g, %g, %g)\n", cell, red, green, blue);

  if(glutstuff.curwin && glutstuff.curwin->context)
    amigaMesaSetOneColor(glutstuff.curwin->context, cell, CLAMP(red), CLAMP(green), CLAMP(blue));
  else
    DEBUGOUT(1, "no window to set colors to\n");
}

void glutCopyColormap(int winnum)
{
  struct GlutWindow *win;

  if(glutstuff.curwin && (win = stuffGetWin(winnum))) {
    struct amigamesa_context *this_context, *that_context;
    int cols;
  
    if((this_context = glutstuff.curwin->context) && (that_context = win->context)) {
      for (cols = 256 - 1; cols >= 0; cols--) {
        ULONG color;

        color = amigaMesaGetOneColor(that_context, cols);
        amigaMesaSetOneColor(this_context, cols, ((GLfloat) ((color >> 16) & 0x000000FF)) / 255,
						 ((GLfloat) ((color >>  8) & 0x000000FF)) / 255,
						 ((GLfloat) ((color >>  0) & 0x000000FF)) / 255);
      }

      return;
    }

    DEBUGOUT(1, "incomplete contexts\n");
    return;
  }

  DEBUGOUT(1, "cannot find second colormap to copy\n");
  return;
}
