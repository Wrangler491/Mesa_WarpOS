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
 * glutAttachDetachMenu.c
 *
 * Version 1.0  27 Jun 1998
 * by Jarno van der Linden
 * jarno@kcbbs.gen.nz
 *
 */

#include <stdlib.h>
#ifndef WARPOS
#include <inline/gadtools.h>
#include <inline/intuition.h>
#else
#pragma pack(push,2)
#include <proto/gadtools.h>
#include <proto/intuition.h>
#endif
#include <libraries/gadtools.h>
#pragma pack(pop)
#include "glutstuff.h"

static struct Menu *GetMenuPointer(int button)
{
  struct GlutWindow *actWindow;
  struct Menu *actMenu = NULL;

  if ((actWindow = glutstuff.curwin)) {
    if ((actMenu = actWindow->menu)) {
      switch (button) {
	case GLUT_RIGHT_BUTTON:
	  actMenu = actMenu->NextMenu;
	case GLUT_MIDDLE_BUTTON:
	  actMenu = actMenu->NextMenu;
	case GLUT_LEFT_BUTTON:
	  break;
      }
    }
  }

  return actMenu;
}

/* TODO: use dList->nodes instead */
static int CountMenuEntries(struct GlutMenu *gm)
{
  int n = 1;									/* This menu item */

  if (gm) {
    struct GlutMenuEntry *actEntry = (struct GlutMenuEntry *)&gm->MenuEntries;

    while ((actEntry = (struct GlutMenuEntry *)nGetNext(&actEntry->EntryNode))) {
      DEBUGOUT(5, "\tcount 0x%08x (`%s')\n", actEntry, actEntry->name);

      if (actEntry->issubmenu) {
	if (actEntry->EntryMenu)
	  n += CountMenuEntries(actEntry->EntryMenu) + 2;			/* Will include (sub)menu item */
	else
	  DEBUGOUT(1, "missing submenue for `%s'\n", actEntry->name);
      }
      else
	n++;									/* Just this menu entry */
    }
  }

  return n;
}

static void FillEntry(struct NewMenu **nm, UBYTE nm_type, const STRPTR nm_Label, APTR nm_UserData)
{
  (*nm)->nm_Type = nm_type;
  (*nm)->nm_Label = (char *)nm_Label;
  (*nm)->nm_CommKey = 0;
  (*nm)->nm_Flags = 0;
  (*nm)->nm_MutualExclude = 0;
  (*nm)->nm_UserData = nm_UserData;
  (*nm)++;
}

static void FillMenu(struct GlutMenu *gm, struct NewMenu **nm, UBYTE type)
{
  if (gm) {
    struct GlutMenuEntry *actEntry = (struct GlutMenuEntry *)&gm->MenuEntries;

    while ((actEntry = (struct GlutMenuEntry *)nGetNext(&actEntry->EntryNode))) {
      DEBUGOUT(5, "\t  FillEntry(0x%08x, 0x%02x, `%s', 0x%08x);\n", *nm, type, actEntry->name, actEntry);
      FillEntry(nm, type, actEntry->name, actEntry);

      if (actEntry->issubmenu) {
	if (actEntry->EntryMenu) {
	  if (type == NM_SUB) {
	    ((*nm) - 1)->nm_Flags = NM_ITEMDISABLED;
	    DEBUGOUT(5, "\t  FillEntry(0x%08x, 0x%02x, \"--\");\n", *nm, type);
	    FillEntry(nm, type, NM_BARLABEL, 0);
	  }
	  DEBUGOUT(5, "\t  FillMenu(0x%08x, 0x%02x);\n", actEntry->EntryMenu, *nm);
	  FillMenu(actEntry->EntryMenu, nm, NM_SUB);
	  if (type == NM_SUB) {
	    DEBUGOUT(5, "\t  FillEntry(0x%08x, 0x%02x, \"--\");\n", *nm, type);
	    FillEntry(nm, type, NM_BARLABEL, 0);
	  }
	}
	else
	  DEBUGOUT(1, "missing submenue for `%s'\n", actEntry->name);
      }
    }
  }
}

static struct MenuItem *MakeMenu(struct GlutMenu *gm)
{
  struct NewMenu *nm, *nmp;
  struct MenuItem *menu = NULL;

  if (gm) {
    if ((nm = AllocVecPooled(glutPool, (CountMenuEntries(gm) + 1) * sizeof(struct NewMenu)))) {
      nmp = nm;

      DEBUGOUT(5, "\t FillMenu(0x%08x, 0x%08x);\n", gm, nmp);
      FillMenu(gm, &nmp, NM_ITEM);
      DEBUGOUT(5, "\t FillEntry(0x%08x);\n", nmp);
      FillEntry(&nmp, NM_END, NULL, 0);

      DEBUGOUT(5, "\t CreateMenus(0x%08x);\n", nm);
      menu = (struct MenuItem *)CreateMenus(nm, TAG_END);
      FreeVecPooled(glutPool, (ULONG *)nm);
    }
    else
      DEBUGOUT(1, "failed to allocate new menu\n");
  }

  return menu;
}

void RedoMenu(int button, struct GlutMenu *glutmenu)
{
  struct GlutWindow *actWindow;
  struct Menu *menu;
  struct MenuItem *menuitems;

  if ((actWindow = glutstuff.curwin) && (actWindow->window) && (actWindow->menu)) {
    if ((menu = GetMenuPointer(button))) {
      DEBUGOUT(5, "\tClearMenuStrip(0x%08x);\n", actWindow->window);
      ClearMenuStrip(actWindow->window);
      DEBUGOUT(5, "\tFreeMenus(0x%08x);\n", menu->FirstItem);
      FreeMenus(menu->FirstItem);						/* TODO: cast */

      if (glutmenu && (menuitems = MakeMenu(glutmenu))) {
	menu->FirstItem = menuitems;
	DEBUGOUT(5, "\tLayoutMenuItems(0x%08x, 0x%08x, 0x%08x, 0x%08x);\n", menuitems, actWindow->vi, GTMN_Menu, (ULONG) menu);
	LayoutMenuItems(menuitems, actWindow->vi,
			GTMN_Menu, (ULONG) menu,
			TAG_END);
      }
      else
	menu->FirstItem = NULL;

      DEBUGOUT(5, "\tSetMenuStrip(0x%08x, 0x%08x);\n", actWindow->window, actWindow->menu);
      SetMenuStrip(actWindow->window, actWindow->menu);
    }
  }
}

void glutAttachMenu(int button)
{
  struct GlutWindow *actWindow;

  if ((actWindow = glutstuff.curwin)) {
    switch (button) {
      case GLUT_LEFT_BUTTON:
	actWindow->leftmenu = glutstuff.curmenu;
	actWindow->needleftmenu = TRUE;
	break;
      case GLUT_MIDDLE_BUTTON:
	actWindow->middlemenu = glutstuff.curmenu;
	actWindow->needmiddlemenu = TRUE;
	break;
      case GLUT_RIGHT_BUTTON:
	actWindow->rightmenu = glutstuff.curmenu;
	actWindow->needrightmenu = TRUE;
	break;
    }
  }
}

void glutDetachMenu(int button)
{
  struct GlutWindow *actWindow;

  if ((actWindow = glutstuff.curwin)) {
    switch (button) {
      case GLUT_LEFT_BUTTON:
	actWindow->leftmenu = NULL;
	actWindow->needleftmenu = TRUE;
	break;
      case GLUT_MIDDLE_BUTTON:
	actWindow->middlemenu = NULL;
	actWindow->needmiddlemenu = TRUE;
	break;
      case GLUT_RIGHT_BUTTON:
	actWindow->rightmenu = NULL;
	actWindow->needrightmenu = TRUE;
	break;
    }
  }
}
