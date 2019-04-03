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
 * glutString.c
 *
 * Version 1.0  27 Jun 1998
 * by Jarno van der Linden
 * jarno@kcbbs.gen.nz
 *
 */

#include "glutstuff.h"

static char *compstr[] =
{
  "none", "=", "!=", "<=", ">=", ">", "<", "~"
};
static char *capstr[] =
{
  "rgba", "bufsize", "double", "stereo", "auxbufs", "red", "green", "blue", "alpha",
  "depth", "stencil", "acred", "acgreen", "acblue", "acalpha", "level", "xvisual",
  "transparent", "samples", "xstaticgray", "xgrayscale", "xstaticcolor", "xpseudocolor",
  "xtruecolor", "xdirectcolor", "slow", "conformant", "num"
};

/* Frame buffer capability macros and types. */
#define RGBA                    0
#define BUFFER_SIZE             1
#define DOUBLEBUFFER            2
#define STEREO                  3
#define AUX_BUFFERS             4
#define RED_SIZE                5				/* Used as mask bit for
								 * "color selected". */
#define GREEN_SIZE              6
#define BLUE_SIZE               7
#define ALPHA_SIZE              8
#define DEPTH_SIZE              9
#define STENCIL_SIZE            10
#define ACCUM_RED_SIZE          11				/* Used as mask bit for
								 * "acc selected". */
#define ACCUM_GREEN_SIZE        12
#define ACCUM_BLUE_SIZE         13
#define ACCUM_ALPHA_SIZE        14
#define LEVEL                   15

#define NUM_GLXCAPS             (LEVEL + 1)

#define XVISUAL                 (NUM_GLXCAPS + 0)
#define TRANSPARENT             (NUM_GLXCAPS + 1)
#define SAMPLES                 (NUM_GLXCAPS + 2)
#define XSTATICGRAY             (NUM_GLXCAPS + 3)		/* Used as mask bit for
								 * "any visual type selected". */
#define XGRAYSCALE              (NUM_GLXCAPS + 4)
#define XSTATICCOLOR            (NUM_GLXCAPS + 5)
#define XPSEUDOCOLOR            (NUM_GLXCAPS + 6)
#define XTRUECOLOR              (NUM_GLXCAPS + 7)
#define XDIRECTCOLOR            (NUM_GLXCAPS + 8)
#define SLOW                    (NUM_GLXCAPS + 9)
#define CONFORMANT              (NUM_GLXCAPS + 10)

#define NUM_CAPS                (NUM_GLXCAPS + 11)

/* Frame buffer capablities that don't have a corresponding
 * FrameBufferMode entry.  These get used as mask bits.
 */
#define NUM                     (NUM_CAPS + 0)
#define RGBA_MODE               (NUM_CAPS + 1)
#define CI_MODE                 (NUM_CAPS + 2)
#define LUMINANCE_MODE		(NUM_CAPS + 3)

#define NONE			0
#define EQ			1
#define NEQ			2
#define LTE			3
#define GTE			4
#define GT			5
#define LT			6
#define MIN			7

typedef struct _Criterion {
  int capability;
  int comparison;
  int value;
} Criterion;

/* converts string-part to criterion */
int parseString(char *word, int *mode)
{
  char *cstr, *vstr, *response;
  int comparator, value;
  int rgb, rgba, acc, acca, count, i;

  cstr = strpbrk(word, "=><!~");
  if (cstr) {
    switch (cstr[0]) {
      case '=':
	comparator = EQ;
	vstr = &cstr[1];
	break;
      case '~':
	comparator = MIN;
	vstr = &cstr[1];
	break;
      case '>':
	if (cstr[1] == '=') {
	  comparator = GTE;
	  vstr = &cstr[2];
	}
	else {
	  comparator = GT;
	  vstr = &cstr[1];
	}
	break;
      case '<':
	if (cstr[1] == '=') {
	  comparator = LTE;
	  vstr = &cstr[2];
	}
	else {
	  comparator = LT;
	  vstr = &cstr[1];
	}
	break;
      case '!':
	if (cstr[1] == '=') {
	  comparator = NEQ;
	  vstr = &cstr[2];
	}
	else {
	  return -1;
	}
	break;
      default:
	return -1;
    }
    value = (int)strtol(vstr, &response, 0);
    if (response == vstr) {
      /* Not a valid number. */
      return -1;
    }
    *cstr = '\0';
  }
  else {
    comparator = NONE;
  }
  switch (word[0]) {
    case 'a':
      if (!strcmp(word, "alpha")) {
	criterion[0].capability = ALPHA_SIZE;
	if (comparator == NONE) {
	  criterion[0].comparison = GTE;
	  criterion[0].value = 1;
	}
	else {
	  criterion[0].comparison = comparator;
	  criterion[0].value = value;
	}
	*mode |= (1 << RGBA);
	*mode |= (1 << ALPHA_SIZE);
	*mode |= (1 << RGBA_MODE);
	return 1;
      }
      acca = !strcmp(word, "acca");
      acc = !strcmp(word, "acc");
      if (acc || acca) {
	criterion[0].capability = ACCUM_RED_SIZE;
	criterion[1].capability = ACCUM_GREEN_SIZE;
	criterion[2].capability = ACCUM_BLUE_SIZE;
	criterion[3].capability = ACCUM_ALPHA_SIZE;
	if (acca) {
	  count = 4;
	}
	else {
	  count = 3;
	  criterion[3].comparison = MIN;
	  criterion[3].value = 0;
	}
	if (comparator == NONE) {
	  comparator = GTE;
	  value = 8;
	}
	for (i = 0; i < count; i++) {
	  criterion[i].comparison = comparator;
	  criterion[i].value = value;
	}
	*mode |= (1 << ACCUM_RED_SIZE);
	return 4;
      }
      if (!strcmp(word, "auxbufs")) {
	criterion[0].capability = AUX_BUFFERS;
	if (comparator == NONE) {
	  criterion[0].comparison = MIN;
	  criterion[0].value = 1;
	}
	else {
	  criterion[0].comparison = comparator;
	  criterion[0].value = value;
	}
	*mode |= (1 << AUX_BUFFERS);
	return 1;
      }
      return -1;
    case 'b':
      if (!strcmp(word, "blue")) {
	criterion[0].capability = BLUE_SIZE;
	if (comparator == NONE) {
	  criterion[0].comparison = GTE;
	  criterion[0].value = 1;
	}
	else {
	  criterion[0].comparison = comparator;
	  criterion[0].value = value;
	}
	*mode |= (1 << RGBA);
	*mode |= (1 << RGBA_MODE);
	return 1;
      }
      if (!strcmp(word, "buffer")) {
	criterion[0].capability = BUFFER_SIZE;
	if (comparator == NONE) {
	  criterion[0].comparison = GTE;
	  criterion[0].value = 1;
	}
	else {
	  criterion[0].comparison = comparator;
	  criterion[0].value = value;
	}
	return 1;
      }
      return -1;
    case 'c':
      if (!strcmp(word, "conformant")) {
	criterion[0].capability = CONFORMANT;
	if (comparator == NONE) {
	  criterion[0].comparison = EQ;
	  criterion[0].value = 1;
	}
	else {
	  criterion[0].comparison = comparator;
	  criterion[0].value = value;
	}
	*mode |= (1 << CONFORMANT);
	return 1;
      }
      return -1;
    case 'd':
      if (!strcmp(word, "depth")) {
	criterion[0].capability = DEPTH_SIZE;
	if (comparator == NONE) {
	  criterion[0].comparison = GTE;
	  criterion[0].value = 12;
	}
	else {
	  criterion[0].comparison = comparator;
	  criterion[0].value = value;
	}
	*mode |= (1 << DEPTH_SIZE);
	return 1;
      }
      if (!strcmp(word, "double")) {
	criterion[0].capability = DOUBLEBUFFER;
	if (comparator == NONE) {
	  criterion[0].comparison = EQ;
	  criterion[0].value = 1;
	}
	else {
	  criterion[0].comparison = comparator;
	  criterion[0].value = value;
	}
	*mode |= (1 << DOUBLEBUFFER);
	return 1;
      }
      return -1;
    case 'g':
      if (!strcmp(word, "green")) {
	criterion[0].capability = GREEN_SIZE;
	if (comparator == NONE) {
	  criterion[0].comparison = GTE;
	  criterion[0].value = 1;
	}
	else {
	  criterion[0].comparison = comparator;
	  criterion[0].value = value;
	}
	*mode |= (1 << RGBA);
	*mode |= (1 << RGBA_MODE);
	return 1;
      }
      return -1;
    case 'i':
      if (!strcmp(word, "index")) {
	criterion[0].capability = RGBA;
	criterion[0].comparison = EQ;
	criterion[0].value = 0;
	criterion[1].capability = BUFFER_SIZE;
	if (comparator == NONE) {
	  criterion[1].comparison = GTE;
	  criterion[1].value = 1;
	}
	else {
	  criterion[1].comparison = comparator;
	  criterion[1].value = value;
	}
	*mode |= (1 << RGBA);
	*mode |= (1 << CI_MODE);
	return 2;
      }
      return -1;
    case 'l':
      if (!strcmp(word, "luminance")) {
	criterion[0].capability = RGBA;
	criterion[0].comparison = EQ;
	criterion[0].value = 1;

	criterion[1].capability = RED_SIZE;
	if (comparator == NONE) {
	  criterion[1].comparison = GTE;
	  criterion[1].value = 1;
	}
	else {
	  criterion[1].comparison = comparator;
	  criterion[1].value = value;
	}

	criterion[2].capability = GREEN_SIZE;
	criterion[2].comparison = EQ;
	criterion[2].value = 0;

	criterion[3].capability = BLUE_SIZE;
	criterion[3].comparison = EQ;
	criterion[3].value = 0;
	*mode |= (1 << RGBA);
	*mode |= (1 << RGBA_MODE);
	*mode |= (1 << LUMINANCE_MODE);
	return 4;
      }
      return -1;
    case 'n':
      if (!strcmp(word, "num")) {
	criterion[0].capability = NUM;
	if (comparator == NONE) {
	  return -1;
	}
	else {
	  criterion[0].comparison = comparator;
	  criterion[0].value = value;
	  return 1;
	}
      }
      return -1;
    case 'r':
      if (!strcmp(word, "red")) {
	criterion[0].capability = RED_SIZE;
	if (comparator == NONE) {
	  criterion[0].comparison = GTE;
	  criterion[0].value = 1;
	}
	else {
	  criterion[0].comparison = comparator;
	  criterion[0].value = value;
	}
	*mode |= (1 << RGBA);
	*mode |= (1 << RGBA_MODE);
	return 1;
      }
      rgba = !strcmp(word, "rgba");
      rgb = !strcmp(word, "rgb");
      if (rgb || rgba) {
	criterion[0].capability = RGBA;
	criterion[0].comparison = EQ;
	criterion[0].value = 1;

	criterion[1].capability = RED_SIZE;
	criterion[2].capability = GREEN_SIZE;
	criterion[3].capability = BLUE_SIZE;
	criterion[4].capability = ALPHA_SIZE;
	if (rgba) {
	  count = 5;
	}
	else {
	  count = 4;
	  criterion[4].comparison = MIN;
	  criterion[4].value = 0;
	}
	if (comparator == NONE) {
	  comparator = GTE;
	  value = 1;
	}
	for (i = 1; i < count; i++) {
	  criterion[i].comparison = comparator;
	  criterion[i].value = value;
	}
	*mode |= (1 << RGBA);
	*mode |= (1 << RGBA_MODE);
	return 5;
      }
      return -1;
    case 's':
      if (!strcmp(word, "stencil")) {
	criterion[0].capability = STENCIL_SIZE;
	if (comparator == NONE) {
	  criterion[0].comparison = MIN;
	  criterion[0].value = 1;
	}
	else {
	  criterion[0].comparison = comparator;
	  criterion[0].value = value;
	}
	*mode |= (1 << STENCIL_SIZE);
	return 1;
      }
      if (!strcmp(word, "single")) {
	criterion[0].capability = DOUBLEBUFFER;
	if (comparator == NONE) {
	  criterion[0].comparison = EQ;
	  criterion[0].value = 0;
	  *allowDoubleAsSingle = True;
	  *mode |= (1 << DOUBLEBUFFER);
	  return 1;
	}
	else {
	  return -1;
	}
      }
      if (!strcmp(word, "stereo")) {
	criterion[0].capability = STEREO;
	if (comparator == NONE) {
	  criterion[0].comparison = EQ;
	  criterion[0].value = 1;
	}
	else {
	  criterion[0].comparison = comparator;
	  criterion[0].value = value;
	}
	*mode |= (1 << STEREO);
	return 1;
      }
      if (!strcmp(word, "samples")) {
	criterion[0].capability = SAMPLES;
	if (comparator == NONE) {
	  criterion[0].comparison = LTE;
	  criterion[0].value = 4;
	}
	else {
	  criterion[0].comparison = comparator;
	  criterion[0].value = value;
	}
	*mode |= (1 << SAMPLES);
	return 1;
      }
      if (!strcmp(word, "slow")) {
	criterion[0].capability = SLOW;
	if (comparator == NONE) {
	  /* Just "slow" means permit fast visuals, but accept
	   * slow ones in preference. Presumably the slow ones
	   * must be higher quality or something else desirable. */
	  criterion[0].comparison = GTE;
	  criterion[0].value = 0;
	}
	else {
	  criterion[0].comparison = comparator;
	  criterion[0].value = value;
	}
	*mode |= (1 << SLOW);
	return 1;
      }
      return -1;
    case 'x':
      /* Be a little over-eager to fill in the comparison and
       * value so we won't have to replicate the code after each
       * string match. */
      if (comparator == NONE) {
	criterion[0].comparison = EQ;
	criterion[0].value = 1;
      }
      else {
	criterion[0].comparison = comparator;
	criterion[0].value = value;
      }

      if (!strcmp(word, "xstaticgray")) {
	criterion[0].capability = XSTATICGRAY;
	*mode |= (1 << XSTATICGRAY);				/* Indicates _any_ visual
								 * class selected. */
	return 1;
      }
      if (!strcmp(word, "xgrayscale")) {
	criterion[0].capability = XGRAYSCALE;
	*mode |= (1 << XSTATICGRAY);				/* Indicates _any_ visual
								 * class selected. */
	return 1;
      }
      if (!strcmp(word, "xstaticcolor")) {
	criterion[0].capability = XSTATICCOLOR;
	*mode |= (1 << XSTATICGRAY);				/* Indicates _any_ visual
								 * class selected. */
	return 1;
      }
      if (!strcmp(word, "xpseudocolor")) {
	criterion[0].capability = XPSEUDOCOLOR;
	*mode |= (1 << XSTATICGRAY);				/* Indicates _any_ visual
								 * class selected. */
	return 1;
      }
      if (!strcmp(word, "xtruecolor")) {
	criterion[0].capability = XTRUECOLOR;
	*mode |= (1 << XSTATICGRAY);				/* Indicates _any_ visual
								 * class selected. */
	return 1;
      }
      if (!strcmp(word, "xdirectcolor")) {
	criterion[0].capability = XDIRECTCOLOR;
	*mode |= (1 << XSTATICGRAY);				/* Indicates _any_ visual
								 * class selected. */
	return 1;
      }
      return -1;
    default:
      return -1;
  }
}

Criterion *parseModeString(char *mode, int *ncriteria, BOOL *allowDoubleAsSingle,
			   Criterion *requiredCriteria, int nRequired, int requiredMask)
{
  Criterion *criteria = NULL;
  int n, mask, parsed, i;
  char *copy, *word;

  *allowDoubleAsSingle = FALSE;
  copy = strdup(mode);
  /* Attempt to estimate how many criteria entries should be
   * needed.
   */
  n = 0;
  word = strtok(copy, " \t");
  while (word) {
    n++;
    word = strtok(NULL, " \t");
  }
  /* Overestimate by 4 times ("rgba" might add four criteria
   * entries) plus add in possible defaults plus space for
   * required criteria.
   */
  if ((criteria = (Criterion *)malloc((4 * n + 30 + nRequired) * sizeof(Criterion)))) {
  /* Re-copy the copy of the mode string. */
  strcpy(copy, mode);

  /* First add the required criteria (these match at the
   * highest priority). Typically these will be used to force a
   * specific level (layer), transparency, and/or visual type.
   */
  mask = requiredMask;
  for (i = 0; i < nRequired; i++)
    criteria[i] = requiredCriteria[i];
  n = nRequired;

  word = strtok(copy, " \t");
  while (word) {
    parsed = parseCriteria(word, &criteria[n], &mask, allowDoubleAsSingle);
    if (parsed >= 0)
      n += parsed;
    else
      Printf("Unrecognized display string word: %s (ignoring)\n", word);
    word = strtok(NULL, " \t");
  }

  if (!(mask & (1 << ACCUM_RED_SIZE))) {
    criteria[n].capability = ACCUM_RED_SIZE;
    criteria[n].comparison = MIN;
    criteria[n].value = 0;
    criteria[n + 1].capability = ACCUM_GREEN_SIZE;
    criteria[n + 1].comparison = MIN;
    criteria[n + 1].value = 0;
    criteria[n + 2].capability = ACCUM_BLUE_SIZE;
    criteria[n + 2].comparison = MIN;
    criteria[n + 2].value = 0;
    criteria[n + 3].capability = ACCUM_ALPHA_SIZE;
    criteria[n + 3].comparison = MIN;
    criteria[n + 3].value = 0;
    n += 4;
  }
  if (!(mask & (1 << AUX_BUFFERS))) {
    criteria[n].capability = AUX_BUFFERS;
    criteria[n].comparison = MIN;
    criteria[n].value = 0;
    n++;
  }
  if (!(mask & (1 << RGBA))) {
    criteria[n].capability = RGBA;
    criteria[n].comparison = EQ;
    criteria[n].value = 1;
    criteria[n + 1].capability = RED_SIZE;
    criteria[n + 1].comparison = GTE;
    criteria[n + 1].value = 1;
    criteria[n + 2].capability = GREEN_SIZE;
    criteria[n + 2].comparison = GTE;
    criteria[n + 2].value = 1;
    criteria[n + 3].capability = BLUE_SIZE;
    criteria[n + 3].comparison = GTE;
    criteria[n + 3].value = 1;
    criteria[n + 4].capability = ALPHA_SIZE;
    criteria[n + 4].comparison = MIN;
    criteria[n + 4].value = 0;
    n += 5;
    mask |= (1 << RGBA_MODE);
  }

  if (!(mask & (1 << XSTATICGRAY))) {
    if ((mask & (1 << RGBA_MODE))) {
      /* Normally, request an RGBA mode visual be TrueColor,
       * except in the case of Mesa where we trust Mesa (and
       * other code in GLUT) to handle any type of RGBA visual
       * reasonably.
       */
      if (mask & (1 << LUMINANCE_MODE))
	/* If RGBA luminance was requested, actually go for
	 * a StaticGray visual.
         */
        criteria[n].capability = XSTATICGRAY;
      else
        criteria[n].capability = XTRUECOLOR;
      criteria[n].value = 1;
      criteria[n].comparison = EQ;

      n++;
    }
    if (mask & (1 << CI_MODE)) {
      criteria[n].capability = XPSEUDOCOLOR;
      criteria[n].value = 1;
      criteria[n].comparison = EQ;
      n++;
    }
  }

  if (!(mask & (1 << STEREO))) {
    criteria[n].capability = STEREO;
    criteria[n].comparison = EQ;
    criteria[n].value = 0;
    n++;
  }
  if (!(mask & (1 << DOUBLEBUFFER))) {
    criteria[n].capability = DOUBLEBUFFER;
    criteria[n].comparison = EQ;
    criteria[n].value = 0;
    *allowDoubleAsSingle = True;
    n++;
  }
  if (!(mask & (1 << DEPTH_SIZE))) {
    criteria[n].capability = DEPTH_SIZE;
    criteria[n].comparison = MIN;
    criteria[n].value = 0;
    n++;
  }
  if (!(mask & (1 << STENCIL_SIZE))) {
    criteria[n].capability = STENCIL_SIZE;
    criteria[n].comparison = MIN;
    criteria[n].value = 0;
    n++;
  }
  if (!(mask & (1 << LEVEL))) {
    criteria[n].capability = LEVEL;
    criteria[n].comparison = EQ;
    criteria[n].value = 0;
    n++;
  }

  if (n) {
    /* Since over-estimated the size needed; squeeze it down to
     * reality.
     */
    if (!(criteria = (Criterion *) realloc(criteria, n * sizeof(Criterion))))
      return NULL;
  }
  else {
    /* For portability, avoid "realloc(ptr,0)" call. */
    free(criteria);
    criteria = NULL;
  }

  free(copy);
  *ncriteria = n;
  }

  return criteria;
}

/* converts criterion to mode */
int critToMode(int mode)
{
  return 0;
}

void glutInitDisplayString(const char *string)
{
  char *this, *next, *old;
  int mode;

  old = this = strdup(string);
  while ((next = strstr(this, ' '))) {
    *next++ = 0;
    while (*next == ' ')
      next++;

    if (*this) {
      if (parseString(this, &mode) > 0)
	glutInitDisplayMode(critToMode(mode));
    }
    this = next;
  }

  if (*this) {
    if (parseString(this, &mode) > 0)
      glutInitDisplayMode(critToMode(mode));
  }

  free(old);
}
