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
 * glutAddMenuEntry.c
 *
 * Version 1.0  27 Jun 1998
 * by Jarno van der Linden
 * jarno@kcbbs.gen.nz
 *
 */

#include <stdlib.h>
#ifndef WARPOS
#include <inline/graphics.h>
#include <inline/intuition.h>
#else
#pragma pack(push,2)
#include <proto/graphics.h>
#include <proto/intuition.h>
#endif
#include <graphics/modeid.h>
#pragma pack(pop)
#include "glutstuff.h"

void CloseWindowSafely(struct Window *win);

int glutEnterGameMode(void) {
  struct GlutGameMode *GameMode;

 /*
  * TODO:
  *  open screen, open borderless (or maybe 1x1) window, bind new context
  *  change the whole context and open on Screen.RastPort
  */
  if ((glutstuff.GameMode = GameMode = AllocVecPooled(glutPool, sizeof(struct GlutGameMode)))) {
    ULONG ScreenModeID;

    /* Search best ModeID */
    if ((ScreenModeID = BestModeID(BIDTAG_Depth, 8,					/* TODO: user-configureable */
			           BIDTAG_NominalWidth, glutstuff.curwin ? glutstuff.curwin->winwidth : glutstuff.initwidth,
			           BIDTAG_NominalHeight, glutstuff.curwin ? glutstuff.curwin->winheight : glutstuff.initheight,
				   BIDTAG_DesiredWidth, glutstuff.curwin ? glutstuff.curwin->winwidth : glutstuff.initwidth,
				   BIDTAG_DesiredHeight, glutstuff.curwin ? glutstuff.curwin->winheight : glutstuff.initheight,
				   TAG_DONE)) == INVALID_ID) {
      /* this does work! */
      FreeVecPooled(glutPool, (ULONG *)GameMode);
      glutstuff.GameMode = NULL;

      return glutstuff.curwin ? glutstuff.curwin->WinID : 0;
    }

    if ((GameMode->GameScreen = OpenScreenTags(0,
					       SA_Width, glutstuff.curwin->winwidth,
					       SA_Height, glutstuff.curwin->winheight,
					       SA_DisplayID, ScreenModeID,
					       SA_Depth, 8,			/* TODO: user-configureable */
					       SA_SharePens, TRUE))) {
      if ((GameMode->FakeyWindow.window = OpenWindowTags(0,
							 WA_Left, 0,
							 WA_Top, 0,
							 WA_Width, glutstuff.curwin->winwidth,
							 WA_Height, glutstuff.curwin->winheight,
							 WA_IDCMP, glutstuff.curwin->window->IDCMPFlags,
							 WA_Flags, (glutstuff.curwin->window->Flags | WFLG_BORDERLESS | WFLG_ACTIVATE | WFLG_BACKDROP) & ~(WFLG_RMBTRAP),
							 WA_CustomScreen, (ULONG)GameMode->GameScreen,
							 WA_MinWidth, glutstuff.curwin->winwidth,
							 WA_MinHeight, glutstuff.curwin->winheight,
							 WA_MaxWidth, glutstuff.curwin->winwidth,
							 WA_MaxHeight, glutstuff.curwin->winheight))) {
        if((GameMode->FakeyWindow.context = amigaMesaCreateContextTags(AMA_Window, GameMode->FakeyWindow.window,
								       AMA_RGBMode, (glutstuff.rgbamode ? (glutstuff.rgbamode < 0 ? GL_FALSE : GL_TRUE) : glutstuff.rgba),
								       AMA_AlphaChannel, glutstuff.alpha,
								       AMA_Stereo, (glutstuff.stereomode ? (glutstuff.stereomode < 0 ? GL_FALSE : GL_TRUE) : glutstuff.stereo),
								       AMA_DoubleBuffer, (glutstuff.doublemode ? (glutstuff.doublemode < 0 ? GL_FALSE : GL_TRUE) : glutstuff.db),
								       AMA_PaletteMode, glutstuff.palmode,
								       AMA_RastPort, GameMode->FakeyWindow.window->RPort,
								       AMA_Screen, GameMode->FakeyWindow.window->WScreen,
								       AMA_Width, glutstuff.curwin->winwidth,
								       AMA_Height, glutstuff.curwin->winheight,
								       TAG_END))) {
          GameMode->backuped_port = glutstuff.msgport;
          GameMode->backuped_window = glutstuff.curwin;

	  nNewList(&GameMode->FakeyWindow.SubWindows);		/* QUEST: is it possible to open subwindows in gamemode? ANSWER: NO!!!!!!!!!!!! */
	  nNewList(&GameMode->FakeyWindow.WindowTimers);

	  GameMode->FakeyWindow.window->UserData = (APTR)&GameMode->FakeyWindow;

	  GameMode->FakeyWindow.mousex = -1;
	  GameMode->FakeyWindow.mousey = -1;

	  GameMode->FakeyWindow.winwidth = GameMode->FakeyWindow.wincurwidth = GameMode->FakeyWindow.window->Width;
	  GameMode->FakeyWindow.winheight = GameMode->FakeyWindow.wincurheight = GameMode->FakeyWindow.window->Height;

	  GameMode->FakeyWindow.needreshapegui = TRUE;
	  GameMode->FakeyWindow.needpositiongui = TRUE;
	  GameMode->FakeyWindow.needredisplay = TRUE;
	  GameMode->FakeyWindow.needvisibility = TRUE;
	  GameMode->FakeyWindow.visible = TRUE;

          glutstuff.curwin = &GameMode->FakeyWindow;
          glutstuff.msgport = GameMode->FakeyWindow.window->UserPort;

	  stuffLinkInWin(&GameMode->FakeyWindow);

	  if (glutstuff.depth)
	    glEnable(GL_DEPTH_TEST);

	//ActivateWindow(win);
	//glutstuff.activeWindow = gw;
	  DEBUGOUT(2, "%d = glutGameMode()\n", GameMode->FakeyWindow.WinID);

          return GameMode->FakeyWindow.WinID;
        }

	DEBUGOUT(1, "failed to create gamecontext\n");
	CloseWindow(GameMode->FakeyWindow.window);
      }
      else
	DEBUGOUT(1, "failed to open gamewindow\n");
      
      CloseScreen(GameMode->GameScreen);
    }
    else
      DEBUGOUT(1, "failed to open gamescreen\n");
    
    /* this does work! */
    FreeVecPooled(glutPool, (ULONG *)GameMode);
    glutstuff.GameMode = NULL;
  }
  else
    DEBUGOUT(1, "failed to allocate gamemode\n");

  return glutstuff.curwin ? glutstuff.curwin->WinID : 0;
}

void glutLeaveGameMode(void) {
  struct GlutGameMode *GameMode;

  if ((GameMode = glutstuff.GameMode)) {
    struct nnode *act;

    stuffLinkOutWin(&GameMode->FakeyWindow);

    glutstuff.curwin = GameMode->backuped_window;
    glutstuff.msgport = GameMode->backuped_port;

    while ((act = nGetTail(&GameMode->FakeyWindow.WindowTimers))) {
      nRemove(&GameMode->FakeyWindow.WindowTimers, act);
      FreeVecPooled(glutPool, (ULONG *)act);
    }

    while ((act = nGetTail(&GameMode->FakeyWindow.SubWindows)))
      glutDestroyWindow(((struct GlutWindow *)act)->WinID);		/* WARNING: THIS MUST NOT HAPPEN!!! */

    if (GameMode->FakeyWindow.context)
      amigaMesaDestroyContext(GameMode->FakeyWindow.context);
    if (GameMode->FakeyWindow.window)
      CloseWindowSafely(GameMode->FakeyWindow.window);
    if (GameMode->GameScreen)
      CloseScreen(GameMode->GameScreen);

    FreeVecPooled(glutPool, (ULONG *)GameMode);
    glutstuff.GameMode = 0;
    
    stuffMakeCurrent(glutstuff.curwin);
  }
  else
    DEBUGOUT(1, "not in gamemode before\n");
}

int glutGameModeGet(GLenum mode) {

  switch (mode) {
    case GLUT_GAME_MODE_ACTIVE:
      return glutstuff.GameMode ? TRUE : FALSE;
    case GLUT_GAME_MODE_POSSIBLE:
      return TRUE;
    case GLUT_GAME_MODE_WIDTH:
      return glutstuff.GameMode ? glutstuff.GameMode->FakeyWindow.winwidth : -1;
    case GLUT_GAME_MODE_HEIGHT:
      return glutstuff.GameMode ? glutstuff.GameMode->FakeyWindow.winheight : -1;
    case GLUT_GAME_MODE_PIXEL_DEPTH:
      return 8;								/* TODO: user-configureable */
    case GLUT_GAME_MODE_DISPLAY_CHANGED:
      return glutstuff.GameMode ? TRUE : -1;				/* this is always true, 'cause we open a new screen (aka XDisplay) */
    case GLUT_GAME_MODE_REFRESH_RATE:
    									/* TODO: parse ScreenModeID for hertz */
    default:
      break;
  }
  return -1;
}

void glutGameModeString(const char *string) {
}
