/*
 * triangles.c
 *
 * Modified  27 Jun 1998
 * by Jarno van der Linden
 * jarno@kcbbs.gen.nz
 *
 * Based on triangles.c from CyberGL
 * Changes to work with AmigaMesaRTL
 *
 * Original copyright notice follows:
 */

/*
 *   $VER: triangles.c 1.0 (20.03.1997)
 *
 *   This is an example program for CyberGL
 *
 *      Written by Frank Gerberding
 *
 *   Copyright � 1996-1997 by phase5 digital products
 *      All Rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <sys/time.h>
#ifdef WARPOS
#pragma pack(push,2)
#endif
#include <intuition/intuition.h>

#ifndef WARPOS
#include <inline/exec.h>
#include <inline/graphics.h>
#include <inline/intuition.h>
#else
#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#endif

extern struct ExecBase *SysBase;
extern struct DOSBase *DOSBase;
#ifndef WARPOS
struct Library *IntuitionBase;
#else
struct IntuitionBase *IntuitionBase;
#endif
struct Screen *screen;
amigaMesaContext context;

#pragma pack(pop)

#include <GL/amigamesa.h>
#include <GL/gl.h>
#include <GL/glu.h>
#define WIDTH    300
#define HEIGHT   200

void drawTriangles(amigaMesaContext context, int num)
{
  amigaMesaBuffer buffer = NULL;
  int count;
  struct timeval startTime, stopTime;
  double secs;

  /* we have changed the context (maybe the buffer too, so
   * make it the current again
   */
  amigaMesaGetContextTags(context, AMA_Buffer, &buffer, TAG_DONE);
  if (buffer)
    amigaMesaMakeCurrent(context, buffer);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_DITHER);
  glShadeModel(GL_SMOOTH);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(-400.0, 400.0, -300.0, 300.0, 500.0, -500.0);

  srand(42);

  gettimeofday(&startTime, NULL);
  for (count = 0; count < num; count++) {
    glBegin(GL_TRIANGLES);
    glColor3ub(rand() % 256, rand() % 256, rand() % 256);
    glVertex3i(rand() % 800 - 400, rand() % 600 - 300, rand() % 1000 - 500);
    glColor3ub(rand() % 256, rand() % 256, rand() % 256);
    glVertex3i(rand() % 800 - 400, rand() % 600 - 300, rand() % 1000 - 500);
    glColor3ub(rand() % 256, rand() % 256, rand() % 256);
    glVertex3i(rand() % 800 - 400, rand() % 600 - 300, rand() % 1000 - 500);
    glEnd();
  }
  glFlush();
  gettimeofday(&stopTime, NULL);
  
  secs  = (double)stopTime.tv_micro  / 1000000 + stopTime.tv_secs ;
  secs -= (double)startTime.tv_micro / 1000000 + startTime.tv_secs;
  if (secs == 0.0)
    secs = 0.1;

  printf("%g triangles/s (%g secs)\n", (double)num / secs, secs);
}

void drawTrianglesIndexModes2(amigaMesaContext context, int num) {
  amigaMesaChangeContextTags(context, AMA_PaletteMode, AMESA_GREY_MATCH, TAG_DONE);
  printf("   PaletteMode : grey\n");
  drawTriangles(context, num);

  amigaMesaChangeContextTags(context, AMA_PaletteMode, AMESA_TRUECOLOR_MATCH, TAG_DONE);
  printf("   PaletteMode : color match\n");
  drawTriangles(context, num);

  amigaMesaChangeContextTags(context, AMA_PaletteMode, AMESA_WEIGHTED_MATCH, TAG_DONE);
  printf("   PaletteMode : weighted color\n");
  drawTriangles(context, num);

  amigaMesaChangeContextTags(context, AMA_PaletteMode, AMESA_HPCR_MATCH, TAG_DONE);
  printf("   PaletteMode : hpcr color\n");
  drawTriangles(context, num);
}

void drawTrianglesIndexModes1(amigaMesaContext context, int num) {
  amigaMesaChangeContextTags(context, AMA_PaletteCache, GL_FALSE, TAG_DONE);
  printf("  PaletteCache : off\n");
  drawTrianglesIndexModes2(context, num);

  amigaMesaChangeContextTags(context, AMA_PaletteCache, GL_TRUE, TAG_DONE);
  printf("  PaletteCache : on\n");
  drawTrianglesIndexModes2(context, num);
}

void drawTrianglesIndexModes0(amigaMesaContext context, int num) {
  amigaMesaChangeContextTags(context, AMA_PaletteMode, AMESA_TRUECOLOR_SHIFT, TAG_DONE);
  printf(" PaletteDither : off\n");
  printf("  PaletteCache : off\n");
  printf("   PaletteMode : color shift\n");
  drawTriangles(context, num);

  amigaMesaChangeContextTags(context, AMA_PaletteDither, GL_FALSE, TAG_DONE);
  printf(" PaletteDither : off\n");
  drawTrianglesIndexModes1(context, num);

  amigaMesaChangeContextTags(context, AMA_PaletteDither, GL_TRUE, TAG_DONE);
  printf(" PaletteDither : on\n");
  drawTrianglesIndexModes1(context, num);
}

void drawTrianglesRGBAModes(amigaMesaContext context, int num) {
  printf("   PaletteMode : off\n");
  drawTriangles(context, num);
}

void exitT(void) {
  if (context)
    amigaMesaDestroyContext(context);
  if (screen)
    CloseScreen(screen);
  if (IntuitionBase)
    CloseLibrary(IntuitionBase);
}

int main(int argc, char **argv)
{
  atexit(exitT);

  if ((IntuitionBase = OpenLibrary("intuition.library", 1))) {
    if ((screen = OpenScreenTags(NULL,
				 SA_Width, WIDTH,
				 SA_Height, HEIGHT,
				 SA_Depth, 8,
				 SA_SysFont, 1,
				 SA_Type, PUBLICSCREEN,
				 SA_ShowTitle, TRUE,
				 SA_PubName, "Triangles",
				 SA_Title, "Triangles",
				 TAG_END))) {
      if ((context = amigaMesaCreateContextTags(AMA_RastPort, (ULONG)&screen->RastPort,
						AMA_Screen, (ULONG)screen,
						AMA_Top, screen->BarHeight + 1,
						AMA_Width, WIDTH,
						AMA_Height, HEIGHT,
						AMA_RGBMode, GL_TRUE,
						TAG_END))) {
	if (screen->RastPort.BitMap->Depth <= 8)
	  drawTrianglesIndexModes0(context, argc == 2 ? atoi(argv[1]) : 500);
	else
	  drawTrianglesRGBAModes(context, argc == 2 ? atoi(argv[1]) : 500);
      }
    }
  }
  
  return 0;
}
