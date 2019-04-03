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
 * glutVideo.c
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

#include "glutstuff.h"

int glutVideoResizeGet(GLenum param) {

  switch (param) {
    case GLUT_VIDEO_RESIZE_POSSIBLE:
    case GLUT_VIDEO_RESIZE_IN_USE:
      return 0;
    case GLUT_VIDEO_RESIZE_X_DELTA:
    case GLUT_VIDEO_RESIZE_Y_DELTA:
    case GLUT_VIDEO_RESIZE_WIDTH_DELTA:
    case GLUT_VIDEO_RESIZE_HEIGHT_DELTA:
    case GLUT_VIDEO_RESIZE_X:
    case GLUT_VIDEO_RESIZE_Y:
    case GLUT_VIDEO_RESIZE_WIDTH:
    case GLUT_VIDEO_RESIZE_HEIGHT:
      return -1;
    default:
      Printf("invalid glutVideoResizeGet parameter: %ld", param);
      return -1;
  }
}

void glutSetupVideoResizing(void) {
}

void glutStopVideoResizing(void) {
}

void glutVideoResize(int x, int y, int width, int height) {
}

void glutVideoPan(int x, int y, int width, int height) {
}
