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
 * glutInitDisplayMode.c
 *
 * Version 1.0  27 Jun 1998
 * by Jarno van der Linden
 * jarno@kcbbs.gen.nz
 *
 */

#include "glutstuff.h"

void glutInitDisplayMode(unsigned int mode)
{
  if (mode & GLUT_RGBA)
    glutstuff.rgba = GL_TRUE;
  if (mode & GLUT_INDEX)
    glutstuff.rgba = GL_FALSE;

  if (mode & GLUT_SINGLE)
    glutstuff.db = GL_FALSE;
  if (mode & GLUT_DOUBLE)
    glutstuff.db = GL_TRUE;

  if (mode & GLUT_ACCUM)
    glutstuff.accum = GL_TRUE;

  if (mode & GLUT_ALPHA)
    glutstuff.alpha = GL_TRUE;

  if (mode & GLUT_DEPTH)
    glutstuff.depth = GL_TRUE;

  if (mode & GLUT_STENCIL)
    glutstuff.stencil = GL_TRUE;

  if (mode & GLUT_MULTISAMPLE)
    glutstuff.multisample = GL_TRUE;

  if (mode & GLUT_STEREO)
    glutstuff.stereo = GL_TRUE;

  if (mode & GLUT_LUMINANCE)
    glutstuff.luminance = GL_TRUE;

  glutstuff.initdisplaymode = mode;
}

void glutInitDisplayString(const char *string)
{
}
