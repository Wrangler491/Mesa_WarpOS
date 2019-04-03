/*
 * glutGetSetMenu.c
 *
 * Version 1.0  27 Jun 1998
 * by Jarno van der Linden
 * jarno@kcbbs.gen.nz
 *
 */

#include "glutstuff.h"

void glutSetMenu(int menu)
{
  struct GlutMenu *gm;

  if((gm = stuffGetMenu(menu)))
    stuffMakeCurrentMenu(gm);
  else
    DEBUGOUT(1, "somethings wrong in glutSetMenu(%d)\n", menu);
}

int glutGetMenu(void)
{
  return glutstuff.curmenu ? glutstuff.curmenu->MenuID : 0;
}
