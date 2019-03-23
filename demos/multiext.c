/* $Id: multiext.c,v 1.1 1998/06/10 02:51:27 brianp Exp $ */

/*
 * Multitexture demo  USING NEW GL_EXT_multitexture EXTENSION
 * Brian Paul  June 1998  This program is in the public domain.
 */

/*
 * $Log: multiext.c,v $
 * Revision 1.1  1998/06/10 02:51:27  brianp
 * Initial revision
 *
 */


/*
 * Some information about the multitexture extension:

There are two distinct elements to texture mapping:  coordinate specification
and sampling/application.  This distinction must be understood in order to
effectively use multitexturing.

The multitexture extension supports multiple sets of texture coordinates
and multiple texture environments (an environment is a texture image and
its sampling and application parameters.)

Typically, texture coordinate set 0 is used to sample texture image 0
and texture coordinate set 1 is used to sample texture image 1.  But, it's
also possible to use coordinate set 0 to sample both images (as seen later).

The number of texture coordinate sets available is queried with
glGetIntegerv(GL_MAX_TEXTURE_COORD_SETS_EXT, &n).

The number of texture environments available is queried with
glGetIntegerv(GL_MAX_TEXTURES_EXT, &n).


Coordinate specification:

   The new glMultiTexCoord*EXT() functions take a target parameter which
   explictly controls which set of texture coordinates are being set.

   The normal glTexCoord() functions modify the GL_TEXTURE0_EXT (0th) set
   of texture coordinates, by default.

   The new glSelectTextureCoordSetEXT() function is used to change the
   texture coordinate set targeted by the glTexCoord() functions.  For
   example, after calling glSelectTextureCoordSetEXT(GL_TEXTURE1_EXT)
   the glTexCoord*() functions will modify texture coordinate set 1 instead
   of 0.


Texture sampling/application:

   The texture environment controls how textures are sampled (nearest,
   linear, etc) and applied (replace, modulate, blend, etc).

   glSelectTextureEXT() selects the current texture environment.
   Subsequent calls to glTexEnv*(), glTexParameter*(), glTexImage*D(),
   etc will address the texture environment which was specified by
   glSelectTextureEXT().

   Of particular interest is the call glTexEnv*(GL_TEXTURE_ENV,
   GL_TEXTURE_ENV_COORD_SET_EXT, t).  This controls the texture coordinate
   source for the current texture environment.

   For example, if you have two texture environments and want both to use
   texture coordinate set 0 then you would do the following:

      glSelectTextureEXT(GL_TEXTURE0_EXT);
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COORD_SET_EXT, GL_TEXTURE0_EXT);
      glSelectTextureEXT(GL_TEXTURE1_EXT);
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COORD_SET_EXT, GL_TEXTURE0_EXT);

   Another example- if you have two texture environments and wanted to use
   separate texture coordinates for each environment then you would do the
   following:

      glSelectTextureEXT(GL_TEXTURE0_EXT);
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COORD_SET_EXT, GL_TEXTURE0_EXT);
      glSelectTextureEXT(GL_TEXTURE1_EXT);
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COORD_SET_EXT, GL_TEXTURE1_EXT);


Coordinate transformation and automatic generation:

   Multitexturing also replicates texture coordinate transformation state.
   That is, there will be multiple texture coordinate matrices and texture
   matrix stacks.

   Texture matrix 0 will transform texture coordiante set 0,
   texture matrix 1 will transform texture coordiante set 1, etc.

   Similarly, automatic texture coordinate generation can be independantly
   specified for each texture coordinate set.

   The new glSelectTextureTransformEXT() function controls which texture
   matrix or texgen state is the current one.

   For example, after calling glSelectTextureTransformEXT(GL_TEXTUREn_EXT)
   and glMatrixMode(GL_TEXTURE) the nth texture matrix will be effected by
   subsequent glLoadMatrix(), glMultMatrix(), glTranslate(), etc calls.
   The demonstration program below does exactly this.

 */



#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>

#include "../util/readtex.c"   /* I know, this is a hack. */


#define TEX0 1
#define TEX1 2
#define TEXBOTH 3
#define ANIMATE 10
#define QUIT 100

static GLboolean Animate = GL_TRUE;

static GLfloat Drift = 0.0;
static GLfloat Xrot = 20.0, Yrot = 30.0, Zrot = 0.0;
static GLfloat DXrot = 1.0, DYrot = 2.5;


static void Idle( void )
{
   if (Animate) {
      Drift += 0.05;

      /* Transformation of texture coordinate set 0 */
#ifdef GL_EXT_multitexture
      glSelectTextureTransformEXT(GL_TEXTURE0_EXT);
#endif
      glMatrixMode(GL_TEXTURE);
      glLoadIdentity();
      glTranslatef(Drift, 0.0, 0.0);

      /* Transformation of texture coordinate set 1 */
#ifdef GL_EXT_multitexture
      glSelectTextureTransformEXT(GL_TEXTURE1_EXT);
#endif
      glMatrixMode(GL_TEXTURE);
      glLoadIdentity();
      glTranslatef(0.0, Drift, 0.0);


      glMatrixMode(GL_MODELVIEW);

      glutPostRedisplay();
   }
}


static void DrawObject(void)
{
   glBegin(GL_QUADS);

#ifdef GL_EXT_multitexture
   glMultiTexCoord2fEXT(GL_TEXTURE0_EXT, 0.0, 0.0);
   glMultiTexCoord2fEXT(GL_TEXTURE1_EXT, 0.0, 0.0);
   glVertex2f(-1.0, -1.0);

   glMultiTexCoord2fEXT(GL_TEXTURE0_EXT, 2.0, 0.0);
   glMultiTexCoord2fEXT(GL_TEXTURE1_EXT, 1.0, 0.0);
   glVertex2f(1.0, -1.0);

   glMultiTexCoord2fEXT(GL_TEXTURE0_EXT, 2.0, 2.0);
   glMultiTexCoord2fEXT(GL_TEXTURE1_EXT, 1.0, 1.0);
   glVertex2f(1.0, 1.0);

   glMultiTexCoord2fEXT(GL_TEXTURE0_EXT, 0.0, 2.0);
   glMultiTexCoord2fEXT(GL_TEXTURE1_EXT, 0.0, 1.0);
   glVertex2f(-1.0, 1.0);
#else
   glTexCoord2f(0.0, 0.0);
   glVertex2f(-1.0, -1.0);

   glTexCoord2f(1.0, 0.0);
   glVertex2f(1.0, -1.0);

   glTexCoord2f(1.0, 1.0);
   glVertex2f(1.0, 1.0);

   glTexCoord2f(0.0, 1.0);
   glVertex2f(-1.0, 1.0);
#endif

   glEnd();
}



static void Display( void )
{
   glClear( GL_COLOR_BUFFER_BIT );

   glPushMatrix();
      glRotatef(Xrot, 1.0, 0.0, 0.0);
      glRotatef(Yrot, 0.0, 1.0, 0.0);
      glRotatef(Zrot, 0.0, 0.0, 1.0);
      glScalef(5.0, 5.0, 5.0);
      DrawObject();
   glPopMatrix();

   glutSwapBuffers();
}


static void Reshape( int width, int height )
{
   glViewport( 0, 0, width, height );
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   glFrustum( -1.0, 1.0, -1.0, 1.0, 10.0, 100.0 );
   /*glOrtho( -6.0, 6.0, -6.0, 6.0, 10.0, 100.0 );*/
   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity();
   glTranslatef( 0.0, 0.0, -70.0 );
}


static void ModeMenu(int entry)
{
   GLboolean enable0 = GL_FALSE, enable1 = GL_FALSE;
   if (entry==TEX0) {
      enable0 = GL_TRUE;
   }
   else if (entry==TEX1) {
      enable1 = GL_TRUE;
   }
   else if (entry==TEXBOTH) {
      enable0 = GL_TRUE;
      enable1 = GL_TRUE;
   }
   else if (entry==ANIMATE) {
      Animate = !Animate;
   }
   else if (entry==QUIT) {
      exit(0);
   }

   if (entry != ANIMATE) {
#ifdef GL_EXT_multitexture
      glSelectTextureEXT(GL_TEXTURE0_EXT);
#endif
      if (enable0) {
         glEnable(GL_TEXTURE_2D);
      }
      else
         glDisable(GL_TEXTURE_2D);

#ifdef GL_EXT_multitexture
      glSelectTextureEXT(GL_TEXTURE1_EXT);
#endif
      if (enable1) {
         glEnable(GL_TEXTURE_2D);
      }
      else
         glDisable(GL_TEXTURE_2D);
   }

   glutPostRedisplay();
}


static void Key( unsigned char key, int x, int y )
{
   switch (key) {
      case 27:
         exit(0);
         break;
   }
   glutPostRedisplay();
}


static void SpecialKey( int key, int x, int y )
{
   float step = 3.0;

   switch (key) {
      case GLUT_KEY_UP:
         Xrot += step;
         break;
      case GLUT_KEY_DOWN:
         Xrot -= step;
         break;
      case GLUT_KEY_LEFT:
         Yrot += step;
         break;
      case GLUT_KEY_RIGHT:
         Yrot -= step;
         break;
   }
   glutPostRedisplay();
}


static void Init( void )
{
   const char *exten = (const char *) glGetString(GL_EXTENSIONS);
   if (!strstr(exten, "GL_EXT_multitexture")) {
      printf("Sorry, GL_EXT_multitexture not supported by this renderer.\n");
      exit(1);
   }

   /* setup textur env 0 */
#ifdef GL_EXT_multitexture
   glSelectTextureEXT(GL_TEXTURE0_EXT);
   glSelectTextureCoordSetEXT(GL_TEXTURE0_EXT);
#endif
#ifdef LINEAR_FILTER
   /* linear filtering looks much nicer but is much slower for Mesa */
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#else
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#endif

   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   if (!LoadRGBMipmaps("girl.rgb", GL_RGB)) {
      printf("Error: couldn't load texture image\n");
      exit(1);
   }


   /* setup textur env 1 */
#ifdef GL_EXT_multitexture
   glSelectTextureEXT(GL_TEXTURE1_EXT);
   glSelectTextureCoordSetEXT(GL_TEXTURE1_EXT);
#endif
#ifdef LINEAR_FILTER
   /* linear filtering looks much nicer but is much slower for Mesa */
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#else
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#endif

   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

   if (!LoadRGBMipmaps("reflect.rgb", GL_RGB)) {
      printf("Error: couldn't load texture image\n");
      exit(1);
   }

   glShadeModel(GL_FLAT);
   glClearColor(0.3, 0.3, 0.4, 1.0);

   ModeMenu(TEXBOTH);
}


int main( int argc, char *argv[] )
{
   glutInit( &argc, argv );
   glutInitWindowSize( 300, 300 );
   glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
   glutCreateWindow(argv[0] );

   Init();

   glutReshapeFunc( Reshape );
   glutKeyboardFunc( Key );
   glutSpecialFunc( SpecialKey );
   glutDisplayFunc( Display );
   glutIdleFunc( Idle );

   glutCreateMenu(ModeMenu);
   glutAddMenuEntry("Texture 0", TEX0);
   glutAddMenuEntry("Texture 1", TEX1);
   glutAddMenuEntry("Multi-texture", TEXBOTH);
   glutAddMenuEntry("Toggle Animation", ANIMATE);
   glutAddMenuEntry("Quit", QUIT);
   glutAttachMenu(GLUT_RIGHT_BUTTON);

   glutMainLoop();
   return 0;
}
