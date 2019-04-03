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
 * glutLayers.c
 *
 * Version 1.0  27 Jun 1998
 * by Jarno van der Linden
 * jarno@kcbbs.gen.nz
 *
 */

#ifndef WARPOS
#include <inline/dos.h>
#else
#pragma pack(push,2)
#include <proto/dos.h>
#pragma pack(pop)
#endif

#include <stdlib.h>

#include "glutstuff.h"

BOOL overlayed = FALSE;

int glutLayerGet(GLenum param) {

  switch (param) {
    case GLUT_OVERLAY_POSSIBLE:
      return 0;
    case GLUT_LAYER_IN_USE:
      return -1;
    case GLUT_HAS_OVERLAY:
      return overlayed;
    case GLUT_TRANSPARENT_INDEX:
      if (overlayed)
        return -1;
      else
        return -1;
    case GLUT_NORMAL_DAMAGED:
      return -1;
    case GLUT_OVERLAY_DAMAGED:
      if (overlayed)
        return -1;
      else
        return -1;
    default:
      Printf("invalid glutLayerGet param: %ld\n", param);
      break;
  }
  return -1;
}

void glutUseLayer(GLenum layer) {

  switch (layer) {
    case GLUT_NORMAL:
      break;
    case GLUT_OVERLAY:
    default:
      Printf("glutUseLayer: unknown layer, %ld\n", layer);
      break;
  }
}

void glutRemoveOverlay(void) {
}

void glutEstablishOverlay(void) {
}

void glutShowOverlay(void) {
  if(!overlayed)
    Printf("glutOverlayDisplayFunc: window has no overlay established\n");
}

void glutHideOverlay(void) {
  if(!overlayed)
    Printf("glutOverlayDisplayFunc: window has no overlay established\n");
}

void glutPostOverlayRedisplay(void) {
  glutPostRedisplay();
}

void glutPostWindowOverlayRedisplay(int win) {
  glutPostWindowRedisplay(win);
}

void glutOverlayDisplayFunc(void (*func)(void)) {
  if(!overlayed)
    Printf("glutOverlayDisplayFunc: window has no overlay established\n");
  else if (glutstuff.curwin);
    glutstuff.curwin->overlaydisplayfunc = func;
}

