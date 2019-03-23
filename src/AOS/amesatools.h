#ifndef	AMIGAMESAtool_H
#define	AMIGAMESAtool_H

/*
 * $Id: $
 */

/*
 * Mesa 3-D graphics library
 * Version:  3.1
 * Copyright (C) 1995  Brian Paul  (brianp@ssec.wisc.edu)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

GLboolean allocTempRPort(amigaMesaContext amesa, int flags);
void freeTempRPort(amigaMesaContext amesa);
#define	ReallocTempRPort(str, am, flgs)	{					\
  freeTempRPort(am);								\
  if (!allocTempRPort(am, flgs))						\
    Error(str ": Not enough memory to allocate temp-Rastport\n"); }

struct RastPort *makeRPort(amigaMesaContext amesa, int width, int height, int depth, int flags, struct BitMap *friendbm);
void destroyRPort(amigaMesaContext amesa, struct RastPort *rp);

GLboolean makeTempRaster(amigaMesaContext amesa, struct RastPort *rp);
void destroyTempRaster(amigaMesaContext amesa, struct RastPort *rp);

GLubyte *allocPenBackArray(amigaMesaContext amesa, int width, int height, int bytes);
void destroyPenBackArray(amigaMesaContext amesa, GLubyte *);
#define	ReallocPenBackArray(str, am) {						\
  destroyPenBackArray((am), (am)->BackArray);					\
  if (!((am)->BackArray = allocPenBackArray(am, (am)->FixedWidth,		\
				(am)->FixedHeight, (am)->depth <= 8 ? 1 : 4)))	\
    Error(str ": Not enough memory to allocate pen-Backarray\n"); }

void AllocOneLine(amigaMesaContext amesa);
void FreeOneLine(amigaMesaContext amesa);
#define	ReallocOneLine(str, am)	{	\
  FreeOneLine(am);			\
  AllocOneLine(am); }

GLboolean allocArea(amigaMesaContext amesa, struct RastPort *rp);
void freeArea(amigaMesaContext amesa, struct RastPort *rp);

GLboolean makeFixedXY(amigaMesaContext amesa);

#endif
