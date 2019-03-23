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

#define	NO_CONTEXT_AVAILABLE
#include <AOS/amigamesa.h>

/***************************************************************************************************************************/
/* Subfunctions *********************************************************************************************************** */
/***************************************************************************************************************************/

#if 0
APTR Reallocate(struct MemHeader *freeList, APTR memoryBlock, ULONG byteSize, ULONG oldSize) {
}

APTR ReallocMem(APTR memoryBlock, ULONG byteSize, ULONG oldSize, ULONG requirements) {
}

APTR ReallocAbs(APTR location, ULONG byteSize, ULONG oldSize) {
}

struct MemList *ReallocEntry(struct MemList *entry) {
}

APTR ReallocVec(APTR memoryBlock, ULONG byteSize, ULONG requirements) {
}

APTR ReallocPooled(APTR poolHeader, APTR memoryBlock, ULONG byteSize, ULONG oldSize) {
}

APTR ReallocVecPooled(APTR poolHeader, APTR memoryBlock, ULONG byteSize) {
}
#endif

//APTR AllocVecPooled(register APTR poolHeader __asm__("a0"), register ULONG byteSize __asm__("d0"))
APTR AllocVecPooled(APTR poolHeader, ULONG byteSize)
{
  register ULONG *address;

  byteSize += 4;
  if ((address = AllocPooled(poolHeader, byteSize)))
    *address++ = byteSize;

  return (APTR)address;
}

//void FreeVecPooled(register APTR poolHeader __asm__("a0"), register ULONG *memory __asm__("a1"))
void FreeVecPooled(APTR poolHeader, ULONG *memory)
{
  register ULONG byteSize;

  if (poolHeader && memory) {
    byteSize = *--memory;
    FreePooled(poolHeader, memory, byteSize);
  }
}

/***************************************************************************************************************************/

#if 0
#ifdef	REPLACE_ALLOC
void *realloc(void *mem, size_t newlen) {
  ULONG oldlen = ((ULONG *)mem)[-1];
  ULONG *newmem = mem;

  if (newlen != oldlen) {
#if 0
    /* heavy style, but should work */
    if (newlen < oldlen)
      FreePooled(amesaPool, (unsigned char *)mem + newlen, oldlen - newlen);
    else
#endif
    if ((newmem = AllocVecPooled(amesaPool, newlen))) {
      CopyMem(newmem, mem, oldlen);
      FreeVecPooled(amesaPool, (ULONG *)mem);
    }
  }

  return newmem;
}

void *calloc(size_t len, size_t cnt) {
  void *ret;

  if ((ret = AllocVecPooled(amesaPool, len * cnt)))
    bzero(ret, len * cnt);
  
  return ret;
}

void *malloc(size_t len) {
  return AllocVecPooled(amesaPool, len);
}

void free(void *mem) {
  FreeVecPooled(amesaPool, mem);
}
#endif
#endif
