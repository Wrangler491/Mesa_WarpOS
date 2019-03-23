#ifndef	AMIGAMESAmem_H
#define	AMIGAMESAmem_H

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

#if 0
APTR AllocVecPooled(register APTR poolHeader __asm__("a0"), register ULONG byteSize __asm__("d0"));
void FreeVecPooled(register APTR poolHeader __asm__("a0"), register ULONG *memory __asm__("a1"));
#endif

APTR	PPCAllocVecPooled( APTR poolHeader, unsigned long memSize );
void	PPCFreeVecPooled( APTR poolHeader, APTR memory);

#endif
