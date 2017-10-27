/*
 *  Lighting
 *
 *  Demonstrates basic lighting using a sphere and a cube.
 *
 *  Key bindings:
 *  l          Toggles lighting
 *  a/A        Decrease/increase ambient light
 *  d/D        Decrease/increase diffuse light
 *  s/S        Decrease/increase specular light
 *  e/E        Decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  F1         Toggle smooth/flat shading
 *  F3         Toggle light distance (1/5)
 *  m          Toggles light movement
 *  []         Lower/rise light
 *  p          Toggles ortogonal/perspective projection
 *  +/-        Change field of view of perspective
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"

int axes=1;       //  Display axes
int mode=1;       //  Projection mode
int move=1;       //  Move light
int th=0;         //  Azimuth of view angle
int ph=5;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=7.0;   //  Size of world
// Light values
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light
unsigned int texture[8];  //  Texture names

/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}

/*
 * draw some gras for my city to be placed on
 * 
 */
static void grass(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, unsigned int grasstex)
{
   //  Set specular color to white
   float subtlegreen[] = {0,1,0,1};
   float green[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,.3);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,subtlegreen);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,green);
   //  Save transformation
   glPushMatrix();
   
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D, grasstex);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3d(.4,.6,.1);
   glNormal3f( 0, -1, 0);
   glTexCoord2f(0.0, 0.0); glVertex3f(-5,0, -5);
   glTexCoord2f(0.0, 1.0);glVertex3f(-3,0, -5);
   glTexCoord2f(1.0, 1.0); glVertex3f(-3,0, -3);
   glTexCoord2f(1.0, 0.0); glVertex3f(-5,0, -3);
   
   glTexCoord2f(0.0, 0.0); glVertex3f(-5,0, -3);
   glTexCoord2f(0.0, 1.0); glVertex3f(-3,0, -3);
   glTexCoord2f(1.0, 1.0); glVertex3f(-3,0, -1);
   glTexCoord2f(1.0, 0.0); glVertex3f(-5,0, -1);
   
   glTexCoord2f(0.0, 0.0); glVertex3f(-5,0, -1);
   glTexCoord2f(0.0, 1.0); glVertex3f(-3,0, -1);
   glTexCoord2f(1.0, 1.0); glVertex3f(-3,0, 1);
   glTexCoord2f(1.0, 0.0); glVertex3f(-5,0, 1);
   
   glTexCoord2f(0.0, 0.0); glVertex3f(-5,0, 1);
   glTexCoord2f(0.0, 1.0); glVertex3f(-3,0, 1);
   glTexCoord2f(1.0, 1.0); glVertex3f(-3,0, 3);
   glTexCoord2f(1.0, 0.0); glVertex3f(-5,0, 3);
   
   glTexCoord2f(0.0, 0.0); glVertex3f(-5,0, 3);
   glTexCoord2f(0.0, 1.0); glVertex3f(-3,0, 3);
   glTexCoord2f(1.0, 1.0); glVertex3f(-3,0, 5);
   glTexCoord2f(1.0, 0.0); glVertex3f(-5,0, 5);
   
   glTexCoord2f(0.0, 0.0); glVertex3f(-3,0, -3);
   glTexCoord2f(0.0, 1.0);glVertex3f(-1,0, -3);
   glTexCoord2f(1.0, 1.0);glVertex3f(-1,0, -1);
   glTexCoord2f(1.0, 0.0);glVertex3f(-3,0, -1);
   
   glTexCoord2f(0.0, 0.0); glVertex3f(-3,0, -5);
   glTexCoord2f(0.0, 1.0);glVertex3f(-1,0, -5);
   glTexCoord2f(1.0, 1.0);glVertex3f(-1,0, -3);
   glTexCoord2f(1.0, 0.0);glVertex3f(-3,0, -3);
   
   glTexCoord2f(0.0, 0.0); glVertex3f(-3,0, -1);
   glTexCoord2f(0.0, 1.0);glVertex3f(-1,0, -1);
   glTexCoord2f(1.0, 1.0);glVertex3f(-1,0, 1);
   glTexCoord2f(1.0, 0.0);glVertex3f(-3,0, 1);
   
   glTexCoord2f(0.0, 0.0); glVertex3f(-3,0, 1);
   glTexCoord2f(0.0, 1.0);glVertex3f(-1,0, 1);
   glTexCoord2f(1.0, 1.0);glVertex3f(-1,0, 3);
   glTexCoord2f(1.0, 0.0);glVertex3f(-3,0, 3);
   
   glTexCoord2f(0.0, 0.0); glVertex3f(-3,0, 3);
   glTexCoord2f(0.0, 1.0);glVertex3f(-1,0, 3);
   glTexCoord2f(1.0, 1.0);glVertex3f(-1,0, 5);
   glTexCoord2f(1.0, 0.0);glVertex3f(-3,0, 5);
   
   glTexCoord2f(0.0, 0.0); glVertex3f(-1,0, -5);
   glTexCoord2f(0.0, 1.0);glVertex3f(1,0, -5);
   glTexCoord2f(1.0, 1.0);glVertex3f(1,0, -3);
   glTexCoord2f(1.0, 0.0);glVertex3f(-1,0, -3);
   
   glTexCoord2f(0.0, 0.0); glVertex3f(-1,0, -3);
   glTexCoord2f(0.0, 1.0);glVertex3f(1,0, -3);
   glTexCoord2f(1.0, 1.0);glVertex3f(1,0, -1);
   glTexCoord2f(1.0, 0.0);glVertex3f(-1,0, -1);
   
   glTexCoord2f(0.0, 0.0); glVertex3f(-1,0, -1);
   glTexCoord2f(0.0, 1.0);glVertex3f(1,0, -1);
   glTexCoord2f(1.0, 1.0);glVertex3f(1,0, 1);
   glTexCoord2f(1.0, 0.0);glVertex3f(-1,0, 1);
   
   glTexCoord2f(0.0, 0.0); glVertex3f(-1,0, 1);
   glTexCoord2f(0.0, 1.0);glVertex3f(1,0, 1);
   glTexCoord2f(1.0, 1.0);glVertex3f(1,0, 3);
   glTexCoord2f(1.0, 0.0);glVertex3f(-1,0, 3);
   
   glTexCoord2f(0.0, 0.0); glVertex3f(-1,0, 3);
   glTexCoord2f(0.0, 1.0);glVertex3f(1,0, 3);
   glTexCoord2f(1.0, 1.0);glVertex3f(1,0, 5);
   glTexCoord2f(1.0, 0.0);glVertex3f(-1,0, 5);
   
   glTexCoord2f(0.0, 0.0); glVertex3f(1,0, -5);
   glTexCoord2f(0.0, 1.0);glVertex3f(3,0, -5);
   glTexCoord2f(1.0, 1.0);glVertex3f(3,0, -3);
   glTexCoord2f(1.0, 0.0);glVertex3f(1,0, -3);
   

   glTexCoord2f(0.0, 0.0); glVertex3f(1,0, -3);
   glTexCoord2f(0.0, 1.0);glVertex3f(3,0, -3);
   glTexCoord2f(1.0, 1.0);glVertex3f(3,0, -1);
   glTexCoord2f(1.0, 0.0);glVertex3f(1,0, -1);
   
   glTexCoord2f(0.0, 0.0); glVertex3f(1,0, -1);
   glTexCoord2f(0.0, 1.0);glVertex3f(3,0, -1);
   glTexCoord2f(1.0, 1.0);glVertex3f(3,0, 1);
   glTexCoord2f(1.0, 0.0);glVertex3f(1,0, 1);
   
   glTexCoord2f(0.0, 0.0); glVertex3f(1,0, 3);
   glTexCoord2f(0.0, 1.0);glVertex3f(3,0, 3);
   glTexCoord2f(1.0, 1.0);glVertex3f(3,0, 5);
   glTexCoord2f(1.0, 0.0);glVertex3f(1,0, 5);
   
   glTexCoord2f(0.0, 0.0); glVertex3f(1,0, 1);
   glTexCoord2f(0.0, 1.0);glVertex3f(3,0, 1);
   glTexCoord2f(1.0, 1.0);glVertex3f(3,0, 3);
   glTexCoord2f(1.0, 0.0);glVertex3f(1,0, 3);
   
   glTexCoord2f(0.0, 0.0); glVertex3f(1,0, 1);
   glTexCoord2f(0.0, 1.0);glVertex3f(1,0, 3);
   glTexCoord2f(1.0, 1.0);glVertex3f(3,0, 3);
   glTexCoord2f(1.0, 0.0);glVertex3f(3,0, 1);
   
   glTexCoord2f(0.0, 0.0); glVertex3f(3,0,-5);
   glTexCoord2f(0.0, 1.0);glVertex3f(5,0,-5);
   glTexCoord2f(1.0, 1.0);glVertex3f(5,0,-3);
   glTexCoord2f(1.0, 0.0);glVertex3f(3,0,-3);
   
   glTexCoord2f(0.0, 0.0); glVertex3f(3,0,-3);
   glTexCoord2f(0.0, 1.0);glVertex3f(5,0,-3);
   glTexCoord2f(1.0, 1.0);glVertex3f(5,0,-1);
   glTexCoord2f(1.0, 0.0);glVertex3f(3,0,-1);
   
   glTexCoord2f(0.0, 0.0); glVertex3f(3,0,-1);
   glTexCoord2f(0.0, 1.0);glVertex3f(5,0,-1);
   glTexCoord2f(1.0, 1.0);glVertex3f(5,0,1);
   glTexCoord2f(1.0, 0.0);glVertex3f(3,0,1);
   
   glTexCoord2f(0.0, 0.0); glVertex3f(3,0,1);
   glTexCoord2f(0.0, 1.0);glVertex3f(5,0,1);
   glTexCoord2f(1.0, 1.0);glVertex3f(5,0,3);
   glTexCoord2f(1.0, 0.0);glVertex3f(3,0,3);
   
   glTexCoord2f(0.0, 0.0); glVertex3f(3,0,3);
   glTexCoord2f(0.0, 1.0);glVertex3f(5,0,3);
   glTexCoord2f(1.0, 1.0);glVertex3f(5,0,5);
   glTexCoord2f(1.0, 0.0);glVertex3f(3,0,5);

   //  End
   glEnd();
   glDisable(GL_TEXTURE_2D);
   //  Undo transofrmations
   glPopMatrix();
   
  }

/*
 *  Draw a sphere (version 1)
 *     at (x,y,z)
 *     radius (r)
 */
static void sphere1(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {0.0,1.0,0.0,1.0};
   float black[] = {0,0,0,1};

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);
   glMaterialf(GL_FRONT,GL_SHININESS,.5);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,black);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}

static void swing_set(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, unsigned int swingtex)
{
   // Dimensions used to size house
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D, swingtex);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   // front board
   glBegin(GL_QUADS);
   glColor3d(1,1,1);
   glNormal3d(0, 0.28734789,  0.95782629);
   glTexCoord2f(0.0, 0.0); glVertex3d(0,0,.4);
   glTexCoord2f(1.0, 0.0); glVertex3d(.2,0,.4);
   glTexCoord2f(1.0, 1.0); glVertex3d(.2,1,.1);
   glTexCoord2f(0.0, 1.0); glVertex3d(0,1,.1);
   //back board
   glColor3d(1,1,1);
   glNormal3d(0, -0.28734789, 0.95782629);
   glTexCoord2f(0.0, 0.0); glVertex3d(0,0,.3);
   glTexCoord2f(1.0, 0.0); glVertex3d(.2,0,.3);
   glTexCoord2f(1.0, 1.0); glVertex3d(.2,1,0);
   glTexCoord2f(0.0, 1.0); glVertex3d(0,1,0);
   //left side
   glColor3d(1,1,1);
   glNormal3d(1,0,0);
   glTexCoord2f(0.0, 0.0); glVertex3d(0,0,.3);
   glTexCoord2f(1.0, 0.0); glVertex3d(0,1,0);
   glTexCoord2f(1.0, 1.0); glVertex3d(0,1,.1);
   glTexCoord2f(0.0, 1.0); glVertex3d(0,0,.4);
   //right side
   glColor3d(1,1,1);
   glNormal3d(1,0,0);
   glTexCoord2f(0.0, 0.0); glVertex3d(.2,0,.3);
   glTexCoord2f(1.0, 0.0); glVertex3d(.2,1,0);
   glTexCoord2f(1.0, 1.0); glVertex3d(.2,1,.1);
   glTexCoord2f(0.0, 1.0); glVertex3d(.2,0,.4);
   // front board
   glColor3d(1,1,1);
   glNormal3d(0, 0.28734789, 0.95782629);
   glTexCoord2f(0.0, 0.0); glVertex3d(0,0,-.4);
   glTexCoord2f(0.0, 1.0); glVertex3d(.2,0,-.4);
   glTexCoord2f(1.0, 1.0); glVertex3d(.2,1,-.1);
   glTexCoord2f(1.0, 0.0); glVertex3d(0,1,-.1);
   //back board
   glColor3d(1,1,1);
   glNormal3d(0, -0.28734789, 0.95782629);
   glTexCoord2f(0.0, 0.0); glVertex3d(0,0,-.3);
   glTexCoord2f(1.0, 0.0); glVertex3d(.2,0,-.3);
   glTexCoord2f(1.0, 1.0); glVertex3d(.2,1,0);
   glTexCoord2f(0.0, 1.0); glVertex3d(0,1,0);
   //left side
   glColor3d(1,1,1);
   glNormal3d(-1,0,0);
   glTexCoord2f(0.0, 0.0); glVertex3d(0,0,-.3);
   glTexCoord2f(1.0, 0.0); glVertex3d(0,1,0);
   glTexCoord2f(1.0, 1.0); glVertex3d(0,1,-.1);
   glTexCoord2f(0.0, 1.0); glVertex3d(0,0,-.4);
   //right side
   glColor3d(1,1,1);
   glNormal3d(-1,0,0);
   glTexCoord2f(0.0, 0.0); glVertex3d(.2,0,-.3);
   glTexCoord2f(1.0, 0.0); glVertex3d(.2,1,0);
   glTexCoord2f(1.0, 1.0); glVertex3d(.2,1,-.1);
   glTexCoord2f(0.0, 1.0); glVertex3d(.2,0,-.4);
   //far out**************   
   glColor3d(1,1,1);
   glNormal3d(0, 0.28734789, 0.95782629);
   glTexCoord2f(0.0, 0.0); glVertex3d(1,0,.4);
   glTexCoord2f(0.0, 1.0); glVertex3d(1.2,0,.4);
   glTexCoord2f(1.0, 1.0); glVertex3d(1.2,1,.1);
   glTexCoord2f(1.0, 0.0); glVertex3d(1,1,.1);
   //back board
   glColor3d(1,1,1);
   glNormal3d(0, -0.28734789, 0.95782629);
   glTexCoord2f(0.0, 0.0); glVertex3d(1,0,.3);
   glTexCoord2f(1.0, 0.0); glVertex3d(1.2,0,.3);
   glTexCoord2f(1.0, 1.0); glVertex3d(1.2,1,0);
   glTexCoord2f(0.0, 1.0); glVertex3d(1,1,0);
   //left side
   glColor3f(1,1,1);
   glNormal3d(1,0,0);
   glTexCoord2f(0.0, 0.0); glVertex3d(1,0,.3);
   glTexCoord2f(1.0, 0.0); glVertex3d(1,1,0);
   glTexCoord2f(1.0, 1.0); glVertex3d(1,1,.1);
   glTexCoord2f(0.0, 1.0); glVertex3d(1,0,.4);
   //right side
   glColor3f(1,1,1);
   glNormal3d(1,0,0);
   glTexCoord2f(0.0, 0.0); glVertex3d(1.2,0,.3);
   glTexCoord2f(1.0, 0.0); glVertex3d(1.2,1,0);
   glTexCoord2f(1.0, 1.0); glVertex3d(1.2,1,.1);
   glTexCoord2f(0.0, 1.0); glVertex3d(1.2,0,.4);
   // front board
   glColor3d(1,1,1);
   glNormal3d(0, 0.28734789, 0.95782629);
   glTexCoord2f(0.0, 0.0); glVertex3d(1,0,-.4);
   glTexCoord2f(1.0, 0.0); glVertex3d(1.2,0,-.4);
   glTexCoord2f(1.0, 1.0); glVertex3d(1.2,1,-.1);
   glTexCoord2f(0.0, 1.0); glVertex3d(1,1,-.1);
   //back board
   glColor3d(1,1,1);
   glNormal3d(0, -0.28734789, 0.95782629);
   glTexCoord2f(0.0, 0.0); glVertex3d(1,0,-.3);
   glTexCoord2f(1.0, 0.0); glVertex3d(1.2,0,-.3);
   glTexCoord2f(1.0, 1.0); glVertex3d(1.2,1,0);
   glTexCoord2f(0.0, 1.0); glVertex3d(1,1,0);
   //left side
   glColor3d(1,1,1);
   glNormal3d(-1,0,0);
   glTexCoord2f(0.0, 0.0); glVertex3d(1,0,-.3);
   glTexCoord2f(1.0, 0.0); glVertex3d(1,1,0);
   glTexCoord2f(1.0, 1.0); glVertex3d(1,1,-.1);
   glTexCoord2f(0.0, 1.0); glVertex3d(1,0,-.4);
   //right side
   glColor3d(1,1,1);
   glNormal3d(-1,0,0);
   glTexCoord2f(0.0, 0.0); glVertex3d(1.2,0,-.3);
   glTexCoord2f(1.0, 0.0); glVertex3d(1.2,1,0);
   glTexCoord2f(1.0, 1.0); glVertex3d(1.2,1,-.1);
   glTexCoord2f(0.0, 1.0); glVertex3d(1.2,0,-.4);
   //top beam
   glColor3d(1,1,1);
   glNormal3d(0,0,-1);
   glTexCoord2f(0.0, 0.0); glVertex3d(0,.8,.1);
   glTexCoord2f(1.0, 0.0); glVertex3d(0,1,.1);
   glTexCoord2f(1.0, 1.0); glVertex3d(1.2,1,.1);
   glTexCoord2f(0.0, 1.0); glVertex3d(1.2,.8,.1);
   //back top beam
   glColor3d(1,1,1);
   glNormal3d(0,0,-1);
   glTexCoord2f(0.0, 0.0); glVertex3d(0,.8,-.1);
   glTexCoord2f(1.0, 0.0); glVertex3d(0,1,-.1);
   glTexCoord2f(1.0, 1.0); glVertex3d(1.2,1,-.1);
   glTexCoord2f(0.0, 1.0); glVertex3d(1.2,.8,-.1);
   //top top beam
   glColor3d(1,1,1);
   glNormal3d(0,-1,0);
   glTexCoord2f(0.0, 0.0); glVertex3d(0,1,-.1);
   glTexCoord2f(1.0, 0.0); glVertex3d(1.2,1,-.1);
   glTexCoord2f(1.0, 1.0); glVertex3d(1.2,1,.1);
   glTexCoord2f(0.0, 1.0); glVertex3d(0,1,.1);
   //bottom top beam
   glColor3d(1,1,1);
   glNormal3d(0,-1,0);
   glTexCoord2f(0.0, 0.0); glVertex3d(0,.8,-.1);
   glTexCoord2f(1.0, 0.0); glVertex3d(1.2,.8,-.1);
   glTexCoord2f(1.0, 1.0); glVertex3d(1.2,.8,.1);
   glTexCoord2f(0.0, 1.0); glVertex3d(0,.8,.1);
   //side caps beam
   glColor3d(1,1,1);
   glNormal3d(1,0,0);
   glTexCoord2f(0.0, 0.0); glVertex3d(0.001,.8,-.1);
   glTexCoord2f(1.0, 0.0); glVertex3d(0.001,1,-.1);
   glTexCoord2f(1.0, 1.0); glVertex3d(0.001,1,.1);
   glTexCoord2f(0.0, 1.0); glVertex3d(0.001,.8,.1);
   //side caps beam
   glColor3d(1,1,1);
   glNormal3d(1,0,0);
   glTexCoord2f(0.0, 0.0); glVertex3d(1.201,.8,-.1);
   glTexCoord2f(1.0, 0.0); glVertex3d(1.201,1,-.1);
   glTexCoord2f(1.0, 1.0); glVertex3d(1.201,1,.1);
   glTexCoord2f(0.0, 1.0); glVertex3d(1.201,.8,.1);
   //swing. top
   glColor3d(1,1,1);
   glNormal3d(0,-1,0);
   glTexCoord2f(0.0, 0.0); glVertex3d(.5,.2,-.05);
   glTexCoord2f(1.0, 0.0); glVertex3d(.7,.2,-.05);
   glTexCoord2f(1.0, 1.0); glVertex3d(.7,.2,.05);
   glTexCoord2f(0.0, 1.0); glVertex3d(.5,.2,.05);
   //swing bot
   glNormal3d(0,-1,0);
   glTexCoord2f(0.0, 0.0); glVertex3d(.5,.15,-.05);
   glTexCoord2f(1.0, 0.0); glVertex3d(.7,.15,-.05);
   glTexCoord2f(1.0, 1.0); glVertex3d(.7,.15,.05);
   glTexCoord2f(0.0, 1.0); glVertex3d(.5,.15,.05);
   //swing front
   glNormal3d(0,0,-1);
   glTexCoord2f(0.0, 0.0); glVertex3d(.5,.15,.05);
   glTexCoord2f(1.0, 0.0); glVertex3d(.5,.2,.05);
   glTexCoord2f(1.0, 1.0); glVertex3d(.7,.2,.05);
   glTexCoord2f(0.0, 1.0); glVertex3d(.7,.15,.05);
   //swing back
   glNormal3d(0,0,-1);
   glTexCoord2f(0.0, 0.0); glVertex3d(.5,.15,-.05);
   glTexCoord2f(1.0, 0.0); glVertex3d(.5,.2,-.05);
   glTexCoord2f(1.0, 1.0); glVertex3d(.7,.2,-.05);
   glTexCoord2f(0.0, 1.0); glVertex3d(.7,.15,-.05);
   //swing left
   glNormal3d(1,0,0);
   glTexCoord2f(0.0, 0.0); glVertex3d(.5,.15,-.05);
   glTexCoord2f(1.0, 0.0); glVertex3d(.5,.2,-.05);
   glTexCoord2f(1.0, 1.0); glVertex3d(.5,.2,.05);
   glTexCoord2f(0.0, 1.0); glVertex3d(.5,.15,.05);
   //swing right
   glNormal3d(1,0,0);
   glTexCoord2f(0.0, 0.0); glVertex3d(.7,.15,-.05);
   glTexCoord2f(1.0, 0.0); glVertex3d(.7,.2,-.05);
   glTexCoord2f(1.0, 1.0); glVertex3d(.7,.2,.05);
   glTexCoord2f(0.0, 1.0); glVertex3d(.7,.15,.05);
   glEnd();
   
   glBegin(GL_LINES);
   glNormal3d(0,0,-1);
   glVertex3d(.5,1,0);
   glVertex3d(.5,.17,0);
   glVertex3d(.7,1,0);
   glVertex3d(.7,.17,0);
   glEnd();
   glDisable(GL_TEXTURE_2D);
   // undo transformations
   glPopMatrix();
   
}

static void shrub(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, unsigned int treetex)
{
	
   // Dimensions used to size house
   //  Set specular color to white
   float white[] = {.2,.7,.2,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,.2);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D, treetex);
   // draw a square tree
   glBegin(GL_QUADS);
   glColor3f(0.5f, 0.35f, 0.05f);
   //main tree
   glNormal3f(0.0, -0.70710678, 0.70710678);
   glTexCoord2f(0.0, 0.0); glVertex3d(0,0, 0);
   glTexCoord2f(1.0, 0.0); glVertex3d(.2, 0, 0);
   glTexCoord2f(1.0, 1.0); glVertex3d(.4, .2, -.2);
   glTexCoord2f(0.0, 1.0); glVertex3d(-.2, .2, -.2);
   
   glNormal3f(-0.70710678,  -0.70710678, 0.0);
   glTexCoord2f(0.0, 0.0); glVertex3d(.2,0, 0);
   glTexCoord2f(1.0, 0.0); glVertex3d(.2, 0, .2);
   glTexCoord2f(1.0, 1.0); glVertex3d(.4, .2, .4);
   glTexCoord2f(0.0, 1.0); glVertex3d(.4, .2, -.2);
   
   
   glNormal3f(0.0, -0.70710678, -0.70710678);
   glTexCoord2f(0.0, 0.0); glVertex3d(.2,0, .2);
   glTexCoord2f(1.0, 0.0); glVertex3d(0, 0, .2);
   glTexCoord2f(1.0, 1.0); glVertex3d(-.2, .2, .4);
   glTexCoord2f(0.0, 1.0); glVertex3d(.4, .2, .4);
   
   glNormal3f(0.70710678, -0.70710678, 0.0);
   glTexCoord2f(0.0, 0.0); glVertex3d(0,0, .2);
   glTexCoord2f(1.0, 0.0); glVertex3d(0, 0, 0);
   glTexCoord2f(1.0, 1.0); glVertex3d(-.2, .2, -.2);
   glTexCoord2f(0.0, 1.0); glVertex3d(-.2, .2, .4);
   
   glEnd();
   glDisable(GL_TEXTURE_2D);
   glColor3d(.2,.8,0);
   glScaled(1.5*dx, dy,2*dz);
   sphere1(.05, .63,.05,.55);
   // undo transformations
   glPopMatrix();
}


static void tree(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, unsigned int treetex)
{
	
   // Dimensions used to size house
   const double wid= .2;
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,.2);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //enable textures and set texture envi
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D, treetex);
   // draw a square tree

   glBegin(GL_QUADS);
   glColor3f(0.5f, 0.35f, 0.05f);
   //main tree
   glNormal3f(0,0,1);
   glTexCoord2f(0.0, 0.0); glVertex3d(wid,0, 0);
   glTexCoord2f(0.0, 1.0); glVertex3d(wid,1.2, 0);
   glTexCoord2f(1.0, 1.0); glVertex3d(-wid, 1.2, 0);
   glTexCoord2f(1.0, 0.0); glVertex3d(-wid, 0, 0);
   
   glNormal3f(-1,0,0);
   glTexCoord2f(0.0, 0.0); glVertex3d(.2, 0, 0);
   glTexCoord2f(0.0, 1.0); glVertex3d(.2, 1.2, 0);
   glTexCoord2f(1.0, 1.0); glVertex3d(.2, 1.2, -.4);
   glTexCoord2f(1.0, 0.0); glVertex3d(.2, 0, -.4);
   
   glNormal3f(-1,0,0);
   glTexCoord2f(0.0, 0.0); glVertex3d(-.2, 0, 0);
   glTexCoord2f(0.0, 1.0); glVertex3d(-.2, 1.2, 0);
   glTexCoord2f(1.0, 1.0); glVertex3d(-.2, 1.2, -.4);
   glTexCoord2f(1.0, 0.0); glVertex3d(-.2, 0, -.4);
   
   glNormal3f(0,0,1);
   glTexCoord2f(0.0, 0.0); glVertex3d(wid, 0, -.4);
   glTexCoord2f(0.0, 1.0); glVertex3d(wid, 1.2, -.4);
   glTexCoord2f(1.0, 1.0); glVertex3d(-wid, 1.2, -.4);
   glTexCoord2f(1.0, 0.0); glVertex3d(-wid, 0, -.4);
   
   //branch
   glNormal3d(0,0,-1);
   glTexCoord2f(0.0, 0.0); glVertex3d(.2, .7, 0);
   glTexCoord2f(0.0, 1.0); glVertex3d(.4, 1, 0);
   glTexCoord2f(1.0, 1.0); glVertex3d(.5, 1, 0);
   glTexCoord2f(1.0, 0.0); glVertex3d(.2,.6, 0);
   
   glNormal3d(-0.8,  -0.6,  0.0);
   glTexCoord2f(0.0, 0.0); glVertex3d(.2, .6, 0);
   glTexCoord2f(0.0, 1.0); glVertex3d(.5, 1,0);
   glTexCoord2f(1.0, 1.0); glVertex3d(.5, 1,-.3);
   glTexCoord2f(1.0, 0.0); glVertex3d(.2, .6,-.3);
   
   glNormal3d(-0.83205029, 0.5547002, 0.0);        
   glTexCoord2f(0.0, 0.0); glVertex3d(.2,.7, 0);
   glTexCoord2f(0.0, 0.0); glVertex3d(.4, 1, 0);
   glTexCoord2f(0.0, 0.0); glVertex3d(.4, 1,-.3);
   glTexCoord2f(0.0, 0.0); glVertex3d(.2, .7,-.3);
   
   glNormal3d(0,0,-1);
   glTexCoord2f(0.0, 0.0); glVertex3d(.2, .7, -.3);
   glTexCoord2f(0.0, 1.0); glVertex3d(.4, 1, -.3);
   glTexCoord2f(1.0, 1.0); glVertex3d(.5, 1, -.3);
   glTexCoord2f(1.0, 0.0); glVertex3d(.2, .6, -.3);
   
   glEnd();
   glDisable(GL_TEXTURE_2D);
   glColor3d(.2,.8,0);
   sphere1(0, 1.5,-.2,.55);
   sphere1(.45, 1.1,-.15,.25);
   // undo transformations
   glPopMatrix();
}

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
static void ball(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}


/*
 *  Draw solid house
 *    at (x,y,z)
 *    scale by (dx, dy, dz)
 */
static void Solidhouse(double x,double y,double z,
                       double dx,double dy,double dz,
                       double th, double sh, unsigned int rooftex,
                       unsigned int bricktex, unsigned int doortex)
{
	
   // Dimensions used to size house
   const double fence =-.7;
   const double wid= 0.5;
   const double nose=+2.0;
   const double cone= .98;
   const double chimtop = 2;
   const double chimbot = .8;
      //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(sh,0,1,0);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Nose (4 sided)
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D, rooftex);
   glColor3f(1,1,1);
   glBegin(GL_TRIANGLES);
   glNormal3d(0.0, 0.4472136, -0.89442719);
   glTexCoord2f(0.0,nose); glVertex3d(0.0, nose, 0.0);
   glTexCoord2f(wid,cone); glVertex3d( wid, cone, wid);
   glTexCoord2f(-wid, cone); glVertex3d(-wid, cone, wid);
   
   //correct order
   glNormal3d(0.0, 0.4472136, -0.89442719);
   glTexCoord2f(0.0, nose);  glVertex3d( 0.0, nose, 0.0);
   glTexCoord2f(wid,cone);   glVertex3d( wid,cone, -wid);
   glTexCoord2f(-wid, cone); glVertex3d(-wid, cone, -wid);

   glNormal3d( 0.89442719, 0.4472136, 0.0);
   glTexCoord2f(0.0, 0.0); glVertex3d( 0.0, nose, 0.0);
   glTexCoord2f(wid, cone); glVertex3d( wid, cone, wid);
   glTexCoord2f(-wid, cone); glVertex3d( wid, cone, -wid);

   glNormal3d(0.89442719, 0.4472136, 0.0);
   glTexCoord2f(0.0, nose); glVertex3d( 0.0, nose, 0.0);
   glTexCoord2f(wid, cone); glVertex3d(-wid, cone, wid);
   glTexCoord2f(-wid,cone); glVertex3d(-wid,cone, -wid);
   glEnd();
   
   glBindTexture(GL_TEXTURE_2D, bricktex);
   glBegin(GL_QUADS);
   glColor3f( 1, 1, 1);
   
   //back exterior wall
   glNormal3f(0,0,1);
   glTexCoord2f(0.0, 0.0); glVertex3d(-wid, 1, wid);
   glTexCoord2f(0.0, 1.0); glVertex3d(-wid, 0, wid);
   glTexCoord2f(1.0, 1.0); glVertex3d(wid,  0, wid);
   glTexCoord2f(1.0, 0.0); glVertex3d(wid, 1, wid);
   
   //right exterior wall
   glNormal3f(-1,0,0);
   glTexCoord2f(0.0, 0.0); glVertex3d(wid, 1, -wid);
   glTexCoord2f(0.0, 1.0); glVertex3d(wid, 0, -wid);
   glTexCoord2f(1.0, 1.0); glVertex3d(wid, 0, wid);
   glTexCoord2f(1.0, 0.0); glVertex3d(wid, 1, wid);
   
   //front exterior wall
   glNormal3f(0,0,1);
   glTexCoord2f(0.0, 0.0); glVertex3d( -wid,1, -wid);
   glTexCoord2f(0.0, 1.0); glVertex3d(-wid,0, -wid);
   glTexCoord2f(1.0, 1.0); glVertex3d(wid,0, -wid);
   glTexCoord2f(1.0, 0.0); glVertex3d(wid,1, -wid);
  
  //left exterior wall
   glNormal3f(-1,0,0);
   glTexCoord2f(0.0, 0.0); glVertex3d(-wid,1, -wid);
   glTexCoord2f(0.0, 1.0); glVertex3d(-wid,0, -wid);
   glTexCoord2f(1.0, 1.0); glVertex3d(-wid,0, wid);
   glTexCoord2f(1.0, 0.0); glVertex3d(-wid,1, wid);
   
   //window right
   //chimney
   glColor3f(1,1,1);
   glNormal3d(0,0,-1);
   glTexCoord2f(0.0, 0.0); glVertex3d(-.48,chimbot,0);
   glTexCoord2f(0.0, 1.0); glVertex3d(-.48,chimtop,0);
   glTexCoord2f(1.0, 1.0); glVertex3d(-0.2,chimtop,0);
   glTexCoord2f(1.0, 0.0); glVertex3d(-0.2,chimbot,0);
   //chimney2
   glColor3f(1,1,1);
   glNormal3d(0,0,-1);
   glTexCoord2f(0.0, 0.0); glVertex3d(-.48,chimbot,.2);
   glTexCoord2f(0.0, 1.0); glVertex3d(-.48,chimtop,.2);
   glTexCoord2f(1.0, 1.0); glVertex3d(-0.2,chimtop,.2);
   glTexCoord2f(1.0, 0.0); glVertex3d(-0.2,chimbot,.2);
   //chimney3
   glColor3f(1,1,1);
   glNormal3d(-1,0,0);
   glTexCoord2f(0.0, 0.0); glVertex3d(-0.48,chimbot,0);
   glTexCoord2f(0.0, 1.0); glVertex3d(-0.48,chimbot,0.2);
   glTexCoord2f(1.0, 1.0); glVertex3d(-0.48,chimtop,.2);
   glTexCoord2f(1.0, 0.0); glVertex3d(-0.48,chimtop,0);
   //chimney4 left
   glColor3f(1,1,1);
   glNormal3d(-1,0,0);
   glTexCoord2f(0.0, 0.0); glVertex3d(-0.2,chimbot,0);
   glTexCoord2f(0.0, 1.0); glVertex3d(-0.2,chimbot,0.2);
   glTexCoord2f(1.0, 1.0); glVertex3d(-0.2,chimtop,0.2);
   glTexCoord2f(1.0, 0.0); glVertex3d(-0.2,chimtop,0);
   //chimney topper
   glColor3f(1,1,0);
   glNormal3d(-1,0,0);
   glVertex3d(-0.51,chimtop,0.21);
   glVertex3d(-0.51,2.1,0.21);
   glVertex3d(-0.51,2.1,-0.01);
   glVertex3d(-0.51,chimtop,-0.01);
   //chimney topper2
   glColor3f(1,1,0);
   glNormal3d(0,0,1);
   glVertex3d(-0.51,chimtop,0.21);
   glVertex3d(-0.19,chimtop,0.21);
   glVertex3d(-0.19,2.1,0.21);
   glVertex3d(-0.51,2.1,0.21);
   //chimney topper3
   glColor3f(1,1,0);
   glNormal3d(1,0,0);
   glVertex3d(-0.19,chimtop,0.21); 
   glVertex3d(-0.19,chimtop,-0.01);
   glVertex3d(-0.19,2.1,-0.01);
   glVertex3d(-0.19,2.1,0.21);
   //chimney topper4
   glColor3f(1,1,0);
   glNormal3d(0,0,1);
   glVertex3d(-0.19,chimtop,-.01);
   glVertex3d(-0.19,2.1,-0.01);
   glVertex3d(-0.51,2.1,-0.01);
   glVertex3d(-0.51,chimtop,-0.01);
   //chimney topper base
   glColor3f(1,1,0);
   glNormal3d(0,-1,0);
   glVertex3d(-0.51,chimtop,0.21);
   glVertex3d(-0.51,chimtop,-0.01);
   glVertex3d(-0.19,chimtop,-0.01);
   glVertex3d(-0.19,chimtop,0.21);
   //chimney topper top
   glColor3f(1,1,0);
   glNormal3d(0,-1,0);
   glVertex3d(-0.51,2.1,0.21);
   glVertex3d(-0.51,2.1,-0.01);
   glVertex3d(-0.19,2.1,-0.01);
   glVertex3d(-0.19,2.1,0.21);
   glEnd();
   
   glBegin(GL_QUADS);
   //picket
   glNormal3f(0,0,1);
   glColor3f(1,1,1);
   glVertex3d( -0.5, .3, fence);
   glVertex3d(-0.5, 0, fence);
   glVertex3d( -0.4, 0, fence);
   glVertex3d( -0.4, .3, fence);
    
   glColor3f(1,1,1);
   glVertex3d( -0.3, .3, fence);
   glVertex3d(-0.3, 0, fence);
   glVertex3d( -0.2, 0, fence);
   glVertex3d( -0.2, .3, fence);
  
   glColor3f(1,1,1);
   glVertex3d(-.1, .3, fence);
   glVertex3d(-.1, 0, fence);
   glVertex3d(0, 0, fence);
   glVertex3d(0, .3, fence);
   
   glColor3f(1,1,1);
   glVertex3d( .1, .3, fence);
   glVertex3d(.1, 0, fence);
   glVertex3d(.2, 0, fence);
   glVertex3d(.2, .3, fence);
   
   glColor3f(1,1,1);
   glVertex3d(.3, .3, fence);
   glVertex3d(.3, 0, fence);
   glVertex3d(.4, 0, fence);
   glVertex3d(.4, .3, fence);
   
   glColor3f(1,1,1);
   glVertex3d(.5, .3, fence);
   glVertex3d(.5, 0, fence);
   glVertex3d(.6, 0, fence);
   glVertex3d(.6, .3, fence);
  
  
   glColor3f(1,1,1);
   glVertex3d(-wid, .24, fence);
   glVertex3d(-wid, .2, fence);
   glVertex3d(wid, .2, fence);
   glVertex3d(wid, .24, fence);
  
   glColor3f(1,1,1);
   glVertex3d(-wid, .11,fence);
   glVertex3d(-wid, .07,fence);
   glVertex3d(wid, .07,fence);
   glVertex3d(wid, .11,fence);
   glEnd();
   
   glBegin(GL_TRIANGLES);
   glNormal3d(0,0,-1);
   glColor3f(1,1,1);
   glVertex3d( .5,.3, fence);
   glVertex3d( .55,.35, fence);
   glVertex3d( .6,.3, fence);
   
   glVertex3d(.3,.3, fence);
   glVertex3d(.35,.35, fence);
   glVertex3d(.4,.3, fence);
   
   glVertex3d(.1,.3, fence);
   glVertex3d(.15,.35, fence);
   glVertex3d(.2,.3, fence);
   
   glVertex3d(-.1,.3, fence);
   glVertex3d(-.05,.35, fence);
   glVertex3d(0,.3, fence);
   
   glVertex3d(-.5,.3, fence);
   glVertex3d(-.45,.35, fence);
   glVertex3d(-.4,.3, fence);
   
   glVertex3d(-.3,.3, fence);
   glVertex3d(-.25,.35, fence);
   glVertex3d(-.2,.3, fence);
   glEnd();
   
   
   //door
   glBindTexture(GL_TEXTURE_2D, doortex);
   glBegin(GL_QUADS);
   glColor3f(1.0,1.0,1.0);
   glNormal3f(0,0,-1);
   glTexCoord2f(0.0, 0); glVertex3d(0.2,0,-.501);
   glTexCoord2f(1.0, 0.0); glVertex3d(-0.2,0,-.501);
   glTexCoord2f(1.0, 1.0); glVertex3d(-0.2,.5,-.501);
   glTexCoord2f(0.0, 1.0); glVertex3d(0.2,.5,-.501);
   // window left
   glEnd();
   glDisable(GL_TEXTURE_2D);

   // undo transformations
   glPopMatrix();
}



/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=2.0;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);

   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (mode)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }

   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  Light switch
   if (light)
   {
        //  Translate intensity to color vectors
        float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
        float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
        float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
        //  Light position
        float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
        //  Draw light position as ball (still no lighting here)
        glColor3f(1,1,1);
        ball(Position[0],Position[1],Position[2] , 0.1);
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Location of viewer for specular calculations
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0
        glEnable(GL_LIGHT0);
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
     glDisable(GL_LIGHTING);

   //  Draw scene
   //cube(+1,0,0 , 0.5,0.5,0.5 , 0);
   swing_set(.5,0,-.6,1,1,1,0, texture[5]);
   tree(+1,0,-2,1,1,1, 0, texture[3]);
   tree(-3,0,-1,1.5,1.5,1.5,0, texture[3]);
   tree(-3,0,-1,1.5,1.5,1.5,0, texture[3]);
   shrub(2.5,0,2.5,1,1,1,0, texture[3]);
   Solidhouse(1,0,1,1.5,1.5,1.5,0,180,texture[0], texture[2], texture[4]);
   Solidhouse(-2,0,1,2.5,1,1,0, 90, texture[0], texture[2], texture[4]);
   //texture at 1 is grass 
   grass(0,0,0,1,1,1,0, texture[1]);
   

   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }

   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s Light=%s",
     th,ph,dim,fov,mode?"Perpective":"Orthogonal",light?"On":"Off");
   if (light)
   {
      glWindowPos2i(5,45);
      Print("Model=%s Distance=%d Elevation=%.1f",smooth?"Smooth":"Flat",distance,ylight);
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
   }

   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   //  Smooth color model
   else if (key == GLUT_KEY_F1)
      smooth = 1-smooth;
   else if (key == GLUT_KEY_F3)
      distance = (distance==1) ? 5 : 1;

   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(mode?fov:0,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Switch projection mode
   else if (ch == 'p' || ch == 'P')
      mode = 1-mode;
   //  Toggle light movement
   else if (ch == 'm' || ch == 'M')
      move = 1-move;
   //  Move light
   else if (ch == '<')
      zh += 1;
   else if (ch == '>')
      zh -= 1;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
   //  Translate shininess power to value (-1 => 0)
   shiny = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(mode?fov:0,asp,dim);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(mode?fov:0,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(700,700);
   glutCreateWindow("Andrew Candelaresi HW5");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   texture[0] = LoadTexBMP("tile_roof.bmp");
   texture[1] = LoadTexBMP("grass1.bmp");
   texture[2] = LoadTexBMP("RedBrick.bmp");
   texture[3] = LoadTexBMP("bark1.bmp");
   texture[4] = LoadTexBMP("door1.bmp");
   texture[5] = LoadTexBMP("wood1.bmp");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
