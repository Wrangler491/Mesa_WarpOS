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
 * glutXFunc.c
 *
 * Version 1.0  27 Jun 1998
 * by Jarno van der Linden
 * jarno@kcbbs.gen.nz
 *
 */

#include <stdlib.h>
#ifndef WARPOS
#include <inline/intuition.h>
#else
#pragma pack(push,2)
#include <proto/intuition.h>
#pragma pack(pop)
#endif
#include "glutstuff.h"

void glutTimerFunc(unsigned int msecs, void (*func) (int value), int value)
{
  if (glutstuff.curwin) {
    struct GlutTimer *gt;

    if ((gt = AllocVecPooled(glutPool, sizeof(struct GlutTimer)))) {
      gt->timerfunc = func;
      gt->TimerValue = value;

      CurrentTime(&gt->secs, &gt->micros);
      gt->micros += msecs * 1000;
      while(gt->micros >= 1000000) {
        gt->micros -= 1000000;
        gt->secs += 1;
      }
      
      DEBUGOUT(4, "add timer for (%ld:%ld) (+%d)\n", gt->secs, gt->micros, msecs);
      nAddTail(&glutstuff.curwin->WindowTimers, &gt->TimerNode);
    }
  }
  else
    DEBUGOUT(1, "no window to time to\n");
}

void glutDisplayFunc(void (*func) (void))
{
  glutstuff.curwin->displayfunc = func;
}

void glutIdleFunc(void (*func) (void))
{
  glutstuff.idlefunc = func;
}

void glutEntryFunc(void (*func) (int state))
{
  if (glutstuff.curwin)
    glutstuff.curwin->entryfunc = func;
}

void glutKeyboardFunc(void (*func) (unsigned char key, int x, int y))
{
  if (glutstuff.curwin)
    glutstuff.curwin->keyboardfunc = func;
}

void glutKeyboardUpFunc(void (*func) (unsigned char key, int x, int y))
{
  if (glutstuff.curwin)
    glutstuff.curwin->keyboardupfunc = func;
}

void glutReshapeFunc(void (*func) (int width, int height))
{
  if (glutstuff.curwin)
    glutstuff.curwin->reshapefunc = func;
}

void glutVisibilityFunc(void (*func) (int state))
{
  if (glutstuff.curwin)
    glutstuff.curwin->visibilityfunc = func;
}

void glutSpecialFunc(void (*func) (int key, int x, int y))
{
  if (glutstuff.curwin)
    glutstuff.curwin->specialfunc = func;
}

void glutSpecialUpFunc(void (*func) (int key, int x, int y))
{
  if (glutstuff.curwin)
    glutstuff.curwin->specialupfunc = func;
}

void glutWindowStatusFunc(void (*func) (int status))
{
  if (glutstuff.curwin)
    glutstuff.curwin->windowstatusfunc = func;
}

void glutMenuStatusFunc(void (*func) (int status, int x, int y))
{
  glutstuff.menustatusfunc = func;
}

void glutMenuStateFunc(void (*func) (int state))
{
  glutstuff.menustatefunc = func;
}

void glutMouseFunc(void (*func) (int buton, int state, int x, int y))
{
  if (glutstuff.curwin)
    glutstuff.curwin->mousefunc = func;
}

void glutJoystickFunc(void (*func) (unsigned int buttonMask, int x, int y, int z), int pollInterval)
{
  if (glutstuff.curwin) {
    glutstuff.curwin->joystickfunc = func;
    glutstuff.curwin->joystickpoll = pollInterval;
  }
}

void glutForceJoystickFunc(void)
{
}

void glutMotionFunc(void (*func) (int x, int y))
{
  if (glutstuff.curwin)
    glutstuff.curwin->motionfunc = func;
}

void glutPassiveMotionFunc(void (*func) (int x, int y))
{
  if (glutstuff.curwin)
    glutstuff.curwin->passivemotionfunc = func;
}

void glutSpaceballMotionFunc(void (* func)(int x, int y, int z)) {
}

void glutSpaceballRotateFunc(void (* func)(int x, int y, int z)) {
}

void glutSpaceballButtonFunc(void (* func)(int button, int state)) {
}

void glutButtonBoxFunc(void (* func)(int button, int state)) {
}

void glutDialsFunc(void (* func)(int dial, int value)) {
}

void glutTabletMotionFunc(void (* func)(int x, int y)) {
}

void glutTabletButtonFunc(void (* func)(int button, int state, int x, int y)) {
} 
