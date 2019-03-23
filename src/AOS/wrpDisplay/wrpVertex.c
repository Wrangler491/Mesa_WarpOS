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

void wrpCopyFlat(W3D_Vertex * dest, W3D_Vertex * source)
{
  /* vertex copy function
   * flat shading, no texture mapping, no fogging, no ZBuffering
   */

  dest->x = source->x;
  dest->y = source->y;
}

void wrpCopyFlatZ(W3D_Vertex * dest, W3D_Vertex * source)
{
  /* vertex copy function
   * flat shading, no texture mapping, no fogging, ZBuffering
   */

  dest->x = source->x;
  dest->y = source->y;
  dest->z = source->z;
}

void wrpCopyGouraud(W3D_Vertex * dest, W3D_Vertex * source)
{
  /* vertex copy function
   * gouraud shading, no texture mapping, no fogging, no ZBuffering
   */

  dest->x = source->x;
  dest->y = source->y;

  dest->color = source->color;
}

void wrpCopyGouraudZ(W3D_Vertex * dest, W3D_Vertex * source)
{
  /* vertex copy function
   * gouraud shading, no texture mapping, no fogging, ZBuffering
   */

  dest->x = source->x;
  dest->y = source->y;
  dest->z = source->z;

  dest->color = source->color;
}

void wrpCopyFlatFog(W3D_Vertex * dest, W3D_Vertex * source)
{
  /* vertex copy function
   * flat shading, no texture mapping, fogging, ZBuffering
   */

  dest->x = source->x;
  dest->y = source->y;

  dest->w = source->w;
}

void wrpCopyFlatFogZ(W3D_Vertex * dest, W3D_Vertex * source)
{
  /* vertex copy function
   * flat shading, no texture mapping, fogging, ZBuffering
   */

  dest->x = source->x;
  dest->y = source->y;
  dest->z = source->z;

  dest->w = source->w;
}

void wrpCopyGouraudFog(W3D_Vertex * dest, W3D_Vertex * source)
{
  /* vertex copy function
   * gouraud shading, no texture mapping, fogging, no ZBuffering
   */

  dest->x = source->x;
  dest->y = source->y;

  dest->w = source->w;

  dest->color = source->color;
}

void wrpCopyGouraudFogZ(W3D_Vertex * dest, W3D_Vertex * source)
{
  /* vertex copy function
   * gouraud shading, no texture mapping, fogging, ZBuffering
   */

  dest->x = source->x;
  dest->y = source->y;
  dest->z = source->z;

  dest->w = source->w;

  dest->color = source->color;
}

void wrpCopyFlatTex(W3D_Vertex * dest, W3D_Vertex * source)
{
  /* vertex copy function
   * flat shading, texture mapping, no ZBuffering
   */

  dest->x = source->x;
  dest->y = source->y;

  dest->u = source->u;
  dest->v = source->v;
  dest->w = source->w;
}

void wrpCopyFlatTexZ(W3D_Vertex * dest, W3D_Vertex * source)
{
  /* vertex copy function
   * flat shading, texture mapping, ZBuffering
   */

  dest->x = source->x;
  dest->y = source->y;
  dest->z = source->z;

  dest->u = source->u;
  dest->v = source->v;
  dest->w = source->w;
}

void wrpCopyGouraudTex(W3D_Vertex * dest, W3D_Vertex * source)
{
  /* vertex copy function
   * gouraud shading, texture mapping, no ZBuffering
   */

  dest->x = source->x;
  dest->y = source->y;

  dest->u = source->u;
  dest->v = source->v;
  dest->w = source->w;

  dest->color = source->color;
}

void wrpCopyGouraudTexZ(W3D_Vertex * dest, W3D_Vertex * source)
{
  /* vertex copy function
   * gouraud shading, texture mapping, ZBuffering
   */

  dest->x = source->x;
  dest->y = source->y;
  dest->z = source->z;

  dest->u = source->u;
  dest->v = source->v;
  dest->w = source->w;

  dest->color = source->color;
}
