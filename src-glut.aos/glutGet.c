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
 * glutGet.c
 *
 * Version 1.0  27 Jun 1998
 * by Jarno van der Linden
 * jarno@kcbbs.gen.nz
 *
 */

#ifndef WARPOS
#include <inline/intuition.h>
#else
#pragma pack(push,2)
#include <proto/intuition.h>
#pragma pack(pop)
#endif
#include "glutstuff.h"

int glutGet(GLenum state) {

  /* independent */
  switch (state) {
    case GLUT_DISPLAY_MODE_POSSIBLE:
      return TRUE;
    case GLUT_ELAPSED_TIME: {
	ULONG secs, micros;

	CurrentTime(&secs, &micros);
	if (!glutstuff.havebasetime) {
	  glutstuff.basetime_secs = secs;
	  glutstuff.basetime_micros = micros;
	  glutstuff.havebasetime = TRUE;
	}

	return (((int)(secs - glutstuff.basetime_secs)) * 1000 + ((int)(micros - glutstuff.basetime_micros)) / 1000);
      }
    case GLUT_INIT_DISPLAY_MODE:
      return glutstuff.initdisplaymode;
    case GLUT_INIT_WINDOW_X:
      return glutstuff.initposx;
    case GLUT_INIT_WINDOW_Y:
      return glutstuff.initposy;
    case GLUT_INIT_WINDOW_WIDTH:
      return glutstuff.initwidth;
    case GLUT_INIT_WINDOW_HEIGHT:
      return glutstuff.initheight;
    case GLUT_SCREEN_WIDTH:
      return glutstuff.scrwidth;
    case GLUT_SCREEN_HEIGHT:
      return glutstuff.scrheight;
    case GLUT_SCREEN_WIDTH_MM:
    case GLUT_SCREEN_HEIGHT_MM:
      return 0;
    default:
      break;
  }

  if (!glutstuff.curwin) {
    DEBUGOUT(1, "something get wrong in glutGet(%d): no window\n", state);
    return -1;
  }
  /* window dependent */
  switch (state) {
    case GLUT_MENU_NUM_ITEMS:
      return glutstuff.curmenu ? glutstuff.curmenu->NumEntries : 0;
    case GLUT_WINDOW_CURSOR:
      return glutstuff.curwin->cursor;
    case GLUT_WINDOW_X:
      return glutstuff.curwin->wincurx;
    case GLUT_WINDOW_Y:
      return glutstuff.curwin->wincury;
    case GLUT_WINDOW_WIDTH:
      return glutstuff.curwin->wincurwidth;
    case GLUT_WINDOW_HEIGHT:
      return glutstuff.curwin->wincurheight;
    case GLUT_WINDOW_NUM_CHILDREN:
      return glutstuff.curwin->NumChildren;
    case GLUT_WINDOW_PARENT:
      return glutstuff.curwin->SubWindowParent ? glutstuff.curwin->SubWindowParent->WinID : 0;
    default:
      break;
  }

  if (!glutstuff.curwin->context) {
    DEBUGOUT(1, "something get wrong in glutGet(%d): no context\n", state);
    return -1;
  }
  /* context dependent */
  switch (state) {
    case GLUT_WINDOW_NUM_SAMPLES:
      return 0;
    case GLUT_WINDOW_DOUBLEBUFFER: {
	GLboolean db;

	glGetBooleanv(GL_DOUBLEBUFFER, &db);
	return (db ? TRUE : FALSE);
      }
    case GLUT_WINDOW_RGBA: {
	GLboolean rgb;

	glGetBooleanv(GL_RGBA_MODE, &rgb);
	return (rgb ? TRUE : FALSE);
      }
    case GLUT_WINDOW_COLORMAP_SIZE: {
	GLint idx;

	glGetIntegerv(GL_INDEX_BITS, &idx);
	return idx ? 1 << idx : idx;
      }
    case GLUT_WINDOW_BUFFER_SIZE: {
	if(glutGet(GLUT_WINDOW_RGBA))
	  return glutGet(GLUT_WINDOW_RED_SIZE) +
		 glutGet(GLUT_WINDOW_GREEN_SIZE) +
		 glutGet(GLUT_WINDOW_BLUE_SIZE) +
		 glutGet(GLUT_WINDOW_ALPHA_SIZE);
	else {
	  GLint size;

	  glGetIntegerv(GL_INDEX_BITS, &size);
	  return size;
	}
      }
    case GLUT_WINDOW_STEREO: {
	GLboolean stereo;

	glGetBooleanv(GL_STEREO, &stereo);
	return (stereo ? TRUE : FALSE);
      }
    case GLUT_WINDOW_STENCIL_SIZE: {
	GLint size;

	glGetIntegerv(GL_STENCIL_BITS, &size);
	return size;
      }
    case GLUT_WINDOW_DEPTH_SIZE: {
	GLint size;

	glGetIntegerv(GL_DEPTH_BITS, &size);
	return size;
      }
    case GLUT_WINDOW_RED_SIZE: {
	GLint size;

	glGetIntegerv(GL_RED_BITS, &size);
	return size;
      }
    case GLUT_WINDOW_GREEN_SIZE: {
	GLint size;

	glGetIntegerv(GL_GREEN_BITS, &size);
	return size;
      }
    case GLUT_WINDOW_BLUE_SIZE: {
	GLint size;

	glGetIntegerv(GL_BLUE_BITS, &size);
	return size;
      }
    case GLUT_WINDOW_ALPHA_SIZE: {
	GLint size;

	glGetIntegerv(GL_ALPHA_BITS, &size);
	return size;
      }
    case GLUT_WINDOW_ACCUM_RED_SIZE: {
	GLint size;

	glGetIntegerv(GL_ACCUM_RED_BITS, &size);
	return size;
      }
    case GLUT_WINDOW_ACCUM_GREEN_SIZE: {
	GLint size;

	glGetIntegerv(GL_ACCUM_GREEN_BITS, &size);
	return size;
      }
    case GLUT_WINDOW_ACCUM_BLUE_SIZE: {
	GLint size;

	glGetIntegerv(GL_ACCUM_BLUE_BITS, &size);
	return size;
      }
    case GLUT_WINDOW_ACCUM_ALPHA_SIZE: {
	GLint size;

	glGetIntegerv(GL_ACCUM_ALPHA_BITS, &size);
	return size;
      }
    default:
      break;
  }

  return -1;
}
