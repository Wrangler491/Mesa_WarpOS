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

#undef	NO_CONTEXT_AVAILABLE
#include <AOS/amigamesa.h>

/**********************************************************************/
/*****                  amiga/Mesa Private Functions (RastPort)   *****/
/**********************************************************************/

GLboolean allocTempRPort(amigaMesaContext amesa, int flags)
{
  struct RastPort *temprp;

  assert(amesa);

  DEBUGOUT(1, "allocTempRPort(0x%08x, 0x%02x)\n", amesa, flags);

  if ((temprp = AllocVecPooled(amesa->mempool, sizeof(struct RastPort)))) {
    CopyMem(amesa->rp, temprp, sizeof(struct RastPort));

    temprp->Layer = NULL;
    if ((temprp->BitMap = AllocBitMap(amesa->FixedWidth, 1, amesa->rp->BitMap->Depth, flags | BMF_MINPLANES, amesa->rp->BitMap))) {
#if 0
      temprp->BytesPerRow = amesa->FixedWidth * 8;
#endif
      amesa->temprp = temprp;
      return GL_TRUE;
    }
    else
      DEBUGOUT(0, "allocTempRPort: failed AllocBitMap()\n");
    FreeVecPooled(amesa->mempool, (ULONG *) temprp);
  }
  else
    DEBUGOUT(0, "allocTempRPort: failed AllocVecPooled()\n");

  return GL_FALSE;
}

void freeTempRPort(amigaMesaContext amesa)
{
  assert(amesa);

  DEBUGOUT(1, "freeTempRPort(0x%08x, 0x%08x)\n", amesa, amesa->temprp);

  if (amesa->temprp) {
    if (amesa->temprp->BitMap)
      FreeBitMap(amesa->temprp->BitMap);
    FreeVecPooled(amesa->mempool, (ULONG *) amesa->temprp);
    amesa->temprp = NULL;
  }
}

/*
 * Create a new rastport to use as a back buffer.
 * Input:  width, height - size in pixels
 *        depth - number of bitplanes
 */

struct RastPort *makeRPort(amigaMesaContext amesa, int width, int height, int depth, int flags, struct BitMap *friendbm)
{
  struct RastPort *rp;
  struct BitMap *bm;

  DEBUGOUT(1, "makeRPort(0x%08x, %d, %d, %d, 0x%02x, 0x%08x)\n", amesa, width, height, depth, flags, friendbm);

  if ((bm = AllocBitMap(width, height, depth, flags | BMF_CLEAR | BMF_MINPLANES, friendbm))) {
    if ((rp = (struct RastPort *)AllocVecPooled(amesa->mempool, sizeof(struct RastPort)))) {
      bzero(rp, sizeof(struct RastPort));

      InitRastPort(rp);
      rp->BitMap = bm;

      return rp;
    }
    else
      DEBUGOUT(0, "makeRPort: failed AllocVecPooled()\n");
    FreeBitMap(bm);
  }
  else
    DEBUGOUT(0, "makeRPort: failed AllocBitMap()\n");

  return NULL;
}

/*
 * Deallocate a rastport.
 */

void destroyRPort(amigaMesaContext amesa, struct RastPort *rp)
{
  DEBUGOUT(1, "destroyRPort(0x%08x, 0x%08x)\n", amesa, rp);

  if (rp) {
    WaitBlit();
    
    if (rp->BitMap)
      FreeBitMap(rp->BitMap);
    FreeVecPooled(amesa->mempool, (ULONG *)rp);
  }
}

/**********************************************************************/
/*****                  amiga/Mesa Private Functions (Raster)     *****/
/**********************************************************************/

/*
 * Construct a temporary raster for use by the given rasterport.
 * Temp rasters are used for polygon drawing.
 */

GLboolean makeTempRaster(amigaMesaContext amesa, struct RastPort *rp)
{
  GLuint width, height;
  PLANEPTR p;
  struct TmpRas *tmpras;

  assert(rp);
  assert(rp->BitMap);

  DEBUGOUT(1, "makeTempRaster(0x%08x, 0x%08x)\n", amesa, rp);

  width = rp->BitMap->BytesPerRow * 8;
  height = rp->BitMap->Rows;

  /* allocate structures */
  if ((p = AllocRaster(width, height))) {
    if ((rp->TmpRas = tmpras = (struct TmpRas *)AllocVecPooled(amesa->mempool, sizeof(struct TmpRas)))) {
      bzero(tmpras, sizeof(struct TmpRas));

      InitTmpRas(tmpras, p, ((width + 15) >> 4) * height);

      return GL_TRUE;
    }
    else
      DEBUGOUT(0, "makeTempRaster: failed AllocVecPooled()\n");
    FreeRaster(p, width, height);
  }
  else
    DEBUGOUT(0, "makeTempRaster: failed AllocRaster()\n");

  return GL_FALSE;
}

/*
 * Destroy a temp raster.
 */

void destroyTempRaster(amigaMesaContext amesa, struct RastPort *rp)
{
  DEBUGOUT(1, "destroyTempRaster(0x%08x, 0x%08x)\n", amesa, rp);

  if (rp) {
    GLuint width = 0, height = 0;

    if (rp->BitMap) {
      width = rp->BitMap->BytesPerRow * 8;
      height = rp->BitMap->Rows;
    }

    if (rp->TmpRas) {
      if (rp->TmpRas->RasPtr)
        FreeRaster(rp->TmpRas->RasPtr, width, height);
      FreeVecPooled(amesa->mempool, (ULONG *) rp->TmpRas);
      rp->TmpRas = NULL;
    }
  }
}

/**********************************************************************/
/*****                  amiga/Mesa Private Functions (PenBack)    *****/
/**********************************************************************/
/*
 * Color_buf is a array of pens equals the drawing area
 * it's for faster dubbelbuffer rendering
 * Whent it's time for bufferswitch just use c2p and copy.
 */

GLubyte *allocPenBackArray(amigaMesaContext amesa, int width, int height, int bytes)
{
  GLubyte *PenBackArray;

  DEBUGOUT(1, "allocPenBackArray(0x%08x, %d, %d, %d)\n", amesa, width, height, bytes);

  if ((PenBackArray = AllocVecPooled(amesa->mempool, width * height * bytes)))
#if 0
    BltClear(PenBackArray, width * height * bytes, 1);
#else
    bzero(PenBackArray, width * height * bytes);
#endif

  return PenBackArray;
}

void destroyPenBackArray(amigaMesaContext amesa, GLubyte * buf)
{
  assert(amesa);

  DEBUGOUT(1, "destroyPenBackArray(0x%08x, 0x%08x)\n", amesa, buf);

  if (buf)
    FreeVecPooled(amesa->mempool, (ULONG *) buf);
}

/**********************************************************************/
/*****                  amiga/Mesa Private Functions (OneLine)    *****/
/**********************************************************************/

void AllocOneLine(amigaMesaContext amesa)
{
  assert(amesa);

  DEBUGOUT(1, "AllocOneLine(0x%08x)\n", amesa);

  if (!(amesa->imageline = AllocVecPooled(amesa->mempool, amesa->FixedWidth * (amesa->depth <= 8 ? 1 : 4))))
    Error("AllocOneLine: Not enough memory to allocate imageline\n");
}

void FreeOneLine(amigaMesaContext amesa)
{
  assert(amesa);

  DEBUGOUT(1, "FreeOneLine(0x%08x, 0x%08x)\n", amesa, amesa->imageline);

  if (amesa->imageline) {
    FreeVecPooled(amesa->mempool, (ULONG *) amesa->imageline);
    amesa->imageline = 0;
  }
}

/**********************************************************************/
/*****                  amiga/Mesa Private Functions (Area)       *****/
/**********************************************************************/

GLboolean allocArea(amigaMesaContext amesa, struct RastPort *rp)
{
  struct AreaInfo *areainfo;
  UWORD *pattern;
  APTR vbuffer;

  DEBUGOUT(1, "allocArea(0x%08x, 0x%08x)\n", amesa, rp);

  if ((areainfo = (struct AreaInfo *)AllocVecPooled(amesa->mempool, sizeof(struct AreaInfo)))) {
    bzero(areainfo, sizeof(struct AreaInfo));

    if ((pattern = (UWORD *) AllocVecPooled(amesa->mempool, sizeof(UWORD)))) {
      *pattern = 0xffff;							/*  @@@ org: 0xffffffff */

      if ((vbuffer = (APTR) AllocVecPooled(amesa->mempool, MAX_POLYGON * 5 * sizeof(WORD)))) {
	bzero(vbuffer, MAX_POLYGON * 5 * sizeof(WORD));

	/* initialize */
	InitArea(areainfo, vbuffer, MAX_POLYGON);

	/* bind to rastport */
	rp->AreaPtrn = pattern;
	rp->AreaInfo = areainfo;
	rp->AreaPtSz = 0;

	return GL_TRUE;
      }
      else
        DEBUGOUT(0, "allocArea: failed AllocVecPooled()\n");
      FreeVecPooled(amesa->mempool, (ULONG *) pattern);
    }
    else
      DEBUGOUT(0, "allocArea: failed AllocVecPooled()\n");
    FreeVecPooled(amesa->mempool, (ULONG *) areainfo);
  }
  else
    DEBUGOUT(0, "allocArea: failed AllocVecPooled()\n");

  return GL_FALSE;
}

void freeArea(amigaMesaContext amesa, struct RastPort *rp)
{
  assert(rp);

  DEBUGOUT(1, "freeArea(0x%08x, 0x%08x)\n", amesa, rp);

  if (rp->AreaPtrn) {
    FreeVecPooled(amesa->mempool, (ULONG *) rp->AreaPtrn);
    rp->AreaPtrn = NULL;
  }
  if (rp->AreaInfo) {
    if (rp->AreaInfo->VctrTbl)
      FreeVecPooled(amesa->mempool, (ULONG *) rp->AreaInfo->VctrTbl);
    FreeVecPooled(amesa->mempool, (ULONG *) rp->AreaInfo);
    rp->AreaInfo = NULL;
  }
}

/**********************************************************************/
/*****                  amiga/Mesa Private Functions (FixedXY)    *****/
/**********************************************************************/

GLboolean makeFixedXY(amigaMesaContext amesa)
{
  GLint *FixedXY, **FixedXYdb;

  assert(amesa);

  amesa->FixedX = amesa->left;
  amesa->FixedY = amesa->FixedHeight - amesa->bottom;
  DEBUGOUT(1, "makeFixedXY(0x%08x, %d, %d, 0x%08x, 0x%08x)\n", amesa, amesa->FixedX, amesa->FixedY, amesa->FixedXY, amesa->FixedXYdb);

#if 1
  if ((FixedXY   = amesa->FixedXY  ))
    FreeVecPooled(amesa->mempool, (ULONG *) FixedXY  );
  if ((FixedXYdb = amesa->FixedXYdb))
    FreeVecPooled(amesa->mempool, (ULONG *) FixedXYdb);

  if ((FixedXY   = amesa->FixedXY   = AllocVecPooled(amesa->mempool, amesa->FixedHeight * sizeof(GLuint  ))) &&
      (FixedXYdb = amesa->FixedXYdb = AllocVecPooled(amesa->mempool, amesa->FixedHeight * sizeof(GLuint *)))) {
    register GLshort byteshift = amesa->depth <= 8 ? 0 : 2;
    register GLubyte *Array = amesa->BackArray;
#if 1
    register GLint row;

    for (row = 0; row < amesa->FixedHeight; row++) {
      FixedXY  [row] =              (amesa->FixedWidth * FIXy(row)) + FIXx(0);
//    DEBUGOUT(5, "makeFixedXY:   [%d] = 0x%08x\n", row,   (amesa->FixedWidth * FIXy(row)) + FIXx(0));
      FixedXYdb[row] = (GLuint *)((((amesa->FixedWidth * FIXy(row)) + FIXx(0)) << byteshift) + Array);
//    DEBUGOUT(5, "makeFixedXY: db[%d] = 0x%08x/%d/0x%08x\n", row, (((amesa->FixedWidth * FIXy(row)) + FIXx(0)) << byteshift) + Array, byteshift, Array);
    }
#else
#if 1
    register GLint row, offset;

    for (row = 0, offset = FIXx(0); row < amesa->FixedHeight; row++, offset += (amesa->FixedWidth * FIXy(1))) {
      FixedXY  [row] =  offset;
      FixedXYdb[row] = (offset << byteshift) + Array;
    }
#else
    register GLshort row = amesa->FixedHeight - 1;
    register GLint offset = (amesa->FixedWidth * FIXy(row)) + FIXx(0);
    register GLint offseb = (amesa->FixedWidth * FIXy(row)) + FIXx(0) << byteshift;
    register GLint sut = (amesa->FixedWidth * FIXy(1));
    register GLint sub = (amesa->FixedWidth * FIXy(1)) << byteshift;
    register GLint * FixedXYflow   = FixedXY   + (amesa->FixedHeight * sizeof(GLint));
    register GLint **FixedXYdbflow = FixedXYdb + (amesa->FixedHeight * sizeof(GLint *));

    while (--row >= 0) {
      *--FixedXYflow   = offset;
      *--FixedXYdbflow = offseb + Array;
      offset -= sut;
      offseb -= sub;
    }
#endif
#endif

#if 0
    srand(time(NULL));
    for (row = 0; row <= 100; row++) {
      GLshort x, y;
      GLubyte *db = (GLubyte *)Array;
      
      x = rand() % amesa->FixedWidth;
      y = rand() % amesa->FixedHeight;

      DEBUGOUT(5, " (%d) = FIXx(%d)/FixedX(%d)\n", x, amesa->left + x, amesa->FixedX + x);
      DEBUGOUT(5, " (%d) = FIXy(%d)/FixedY(%d)\n", y, amesa->FixedHeight - amesa->bottom - y, amesa->FixedY - y);
      DEBUGOUT(5, " (%d/%d) = FIXxy(%d)/FixedXY(%d)\n", x, y, (amesa->FixedWidth * FIXy(y)) + FIXx(x), FixedXY[y] + x);
      if (amesa->depth <= 8)
        DEBUGOUT(5, " (%d/%d) = FIXxy(0x%08x)/FixedXY(0x%08x)\n", x, y, (GLubyte *)db + FIXxy(x, y), (GLubyte *)FixedXYdb[y] + x);
      else
        DEBUGOUT(5, " (%d/%d) = FIXxy(0x%08x)/FixedXY(0x%08x)\n", x, y, (GLuint *)db + FIXxy(x, y), (GLuint *)FixedXYdb[y] + x);
    }
#endif
				 
    return GL_TRUE;
  }
#else
  return GL_TRUE;
#endif

  return GL_FALSE;
}
