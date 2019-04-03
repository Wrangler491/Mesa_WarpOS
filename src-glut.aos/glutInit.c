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
 * glutInit.c
 *
 * Version 1.0  27 Jun 1998
 * by Jarno van der Linden
 * jarno@kcbbs.gen.nz
 *
 * Version 1.1  02 Aug 1998
 * by Jarno van der Linden
 * jarno@kcbbs.gen.nz
 *
 * - Added quantizer plugin arguments
 *
 */

#include <string.h>
#ifndef WARPOS
#include <inline/intuition.h>
#else
#pragma pack(push,2)
#include <proto/intuition.h>
#endif
#include <intuition/screens.h>
#pragma pack(pop)
#include "glutstuff.h"

#define stricmp strcasecmp

int Limit(int v, int minv, int maxv)
{
  if (v < minv)
    return (minv);
  if (v > maxv)
    return (maxv);

  return (v);
}

void RemoveArg(int n, int *argc, char **argv)
{
  int t;
  char *p;

  p = argv[n];

  for (t = n; t < ((*argc) - 1); t++) {
    argv[t] = argv[t + 1];
  }
  argv[(*argc) - 1] = p;
  (*argc)--;
}

void glutInit(int *argcp, char **argv)
{
  int t;
  char *geometrystring = 0;
  struct Screen *screen;

  CurrentTime(&(glutstuff.basetime_secs), &(glutstuff.basetime_micros));
  glutstuff.havebasetime = TRUE;

  for (t = (*argcp) - 1; t >= 1; t--) {
    if (!stricmp(argv[t], "-pubscreen")) {
      if (t < ((*argcp) - 1)) {
	glutstuff.pubscreenname = argv[t + 1];
	RemoveArg(t, argcp, argv);
	RemoveArg(t, argcp, argv);
      }
    }
    else if (!strcmp(argv[t], "-display")) {
      DEBUGOUT(1, "%s option invalid for amigaos glut.\n", argv[t]);
      if (t < ((*argcp) - 1))
	RemoveArg(t, argcp, argv);
      RemoveArg(t, argcp, argv);
    }
    else if (!strcmp(argv[t], "-direct") ||
	     !strcmp(argv[t], "-indirect") ||
	     !strcmp(argv[t], "-sync")) {
      DEBUGOUT(1, "%s option invalid for amigaos glut.\n", argv[t]);
      RemoveArg(t, argcp, argv);
    }
    else if (!strcmp(argv[t], "-iconic") ||
	     !strcmp(argv[t], "-gldebug")) {
      RemoveArg(t, argcp, argv);
    }
    else if (!strcmp(argv[t], "-geometry")) {
      if (t < ((*argcp) - 1) && argv[t + 1][0]) {
	geometrystring = argv[t + 1];
	RemoveArg(t, argcp, argv);
      }
      RemoveArg(t, argcp, argv);
    }
    else if (!stricmp(argv[t], "-double")) {
      if (t < ((*argcp) - 1)) {
	if (!stricmp(argv[t + 1], "on"))
	  glutstuff.doublemode = 1;
	else if (!stricmp(argv[t + 1], "off"))
	  glutstuff.doublemode = -1;
	RemoveArg(t, argcp, argv);
      }
      RemoveArg(t, argcp, argv);
    }
    else if (!stricmp(argv[t], "-rgba")) {
      if (t < ((*argcp) - 1)) {
	if (!stricmp(argv[t + 1], "on"))
	  glutstuff.rgbamode = 1;
	else if (!stricmp(argv[t + 1], "off"))
	  glutstuff.rgbamode = -1;
	RemoveArg(t, argcp, argv);
      }
      RemoveArg(t, argcp, argv);
    }
    else if (!stricmp(argv[t], "-stereo")) {
      if (t < ((*argcp) - 1)) {
	if (!stricmp(argv[t + 1], "on"))
	  glutstuff.stereomode = 1;
	else if (!stricmp(argv[t + 1], "off"))
	  glutstuff.stereomode = -1;
	RemoveArg(t, argcp, argv);
      }
      RemoveArg(t, argcp, argv);
    }
    else if (!stricmp(argv[t], "-palmode")) {
      if (t < ((*argcp) - 1)) {
	if (!stricmp(argv[t + 1], "weighted"))
	  glutstuff.palmode = AMESA_WEIGHTED_MATCH;
	else if (!stricmp(argv[t + 1], "colorshift"))
	  glutstuff.palmode = AMESA_TRUECOLOR_SHIFT;
	else if (!stricmp(argv[t + 1], "colormatch"))
	  glutstuff.palmode = AMESA_TRUECOLOR_MATCH;
	else if (!stricmp(argv[t + 1], "grey"))
	  glutstuff.palmode = AMESA_GREY_MATCH;
	else if (!stricmp(argv[t + 1], "hpcr"))
	  glutstuff.palmode = AMESA_HPCR_MATCH;
	RemoveArg(t, argcp, argv);
      }
      RemoveArg(t, argcp, argv);
    }
  }

  if (!(screen = LockPubScreen(glutstuff.pubscreenname)))
    screen = LockPubScreen(NULL);
  if (screen) {
    glutstuff.scrwidth = screen->Width;
    glutstuff.scrheight = screen->Height;
    DEBUGOUT(2, "screen dimensions are %dx%d\n", glutstuff.scrwidth, glutstuff.scrheight);
    UnlockPubScreen(NULL, screen);
  }

  if (geometrystring) {
    unsigned char *string = geometrystring, *nextstring;

    if ((nextstring = strchr(string, 'x')) || (nextstring = strchr(string, 'X'))) {
      *nextstring++ = 0;
      glutstuff.initwidth = atoi(string);
      DEBUGOUT(2, "geometry initwidth is %s (%d)\n", string, glutstuff.initwidth);
      string = nextstring;
    }

    if (!((*string == '-') || (*string == '+'))) {
      char removed;

      nextstring = string;
      while (!((*nextstring == '-') || (*nextstring == '+') || (*nextstring == '\0')))
	nextstring++;
      removed = *nextstring;
      *nextstring = 0;
      glutstuff.initheight = atoi(string);
      DEBUGOUT(2, "geometry initheight is %s (%d)\n", string, glutstuff.initheight);
      *nextstring = removed;
      string = nextstring;
    }

    if (((*string == '-') || (*string == '+'))) {
      char removed;

      nextstring = string + 1;
      while (!((*nextstring == '-') || (*nextstring == '+') || (*nextstring == '\0')))
	nextstring++;
      removed = *nextstring;
      *nextstring = 0;
      glutstuff.initposx = glutstuff.scrwidth - glutstuff.initwidth + atoi(string);
      DEBUGOUT(2, "geometry initposx is %s (%d)\n", string, glutstuff.initposx);
      *nextstring = removed;
      string = nextstring;
    }

    if (((*string == '-') || (*string == '+'))) {
      char removed;

      nextstring = string + 1;
      while (!((*nextstring == '-') || (*nextstring == '+') || (*nextstring == '\0')))
	nextstring++;
      removed = *nextstring;
      *nextstring = 0;
      glutstuff.initposy = glutstuff.scrheight - glutstuff.initheight + atoi(string);
      DEBUGOUT(2, "geometry initposy is %s (%d)\n", string, glutstuff.initposy);
      *nextstring = removed;
      string = nextstring;
    }
  }
  glutstuff.initdisplaymode = (GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
}
