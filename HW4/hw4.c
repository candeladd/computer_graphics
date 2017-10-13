/*
 *  Andrew Candelaresi
 * HW4
 * Computer Graphics Fall 2017
 *
 *  Key bindings:
 *  m          Toggle between perspective and orthogonal
 *  +/-        Changes field of view for perspective
 *  a          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  f toggle first person
 *  w,a,s,d    look around and move in first person mode
 * 
 *  ESC        Exit
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int axes=0;       //  Display axes
int mode=0;       //  Projection mode
int fp=0;         //  First Person mode
int th=0;         //  Azimuth of view angle
int ph=90;         //  Elevation of view angle
double zh=0;      //  Rotation of teapot
int spin=0;    //  Rotation for first person
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=7.0;   //  Size of world

double Ex = 0;
double Ey = .5;
double Ez = 5;
double Cx = 0; 
double Cz = 0;

//  Macro for sin & cos in degrees
#define Cos(th) cos(3.1415927/180*(th))
#define Sin(th) sin(3.1415927/180*(th))


/*
 *  Draw vertex in polar coordinates
 */
static void Vertex(double th,double ph)
{
   glColor3f(.25,.5,.25);
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
}

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}




/*
 *  Draw a sphere (version 1)
 *     at (x,y,z)
 *     radius (r)
 */
static void sphere1(double x,double y,double z,double r)
{
   const int d=5;
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);

   //  South pole cap
   glBegin(GL_TRIANGLE_FAN);
   Vertex(0,-90);
   for (th=0;th<=360;th+=d)
   {
      Vertex(th,d-90);
   }
   glEnd();

   //  Latitude bands
   for (ph=d-90;ph<=90-2*d;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
         Vertex(th,ph);
         Vertex(th,ph+d);
      }
      glEnd();
   }

   //  North pole cap
   glBegin(GL_TRIANGLE_FAN);
   Vertex(0,90);
   for (th=0;th<=360;th+=d)
   {
      Vertex(th,90-d);
   }
   glEnd();

   //  Undo transformations
   glPopMatrix();
}

static void tree(double x,double y,double z,
                 double dx,double dy,double dz,
                 double ux,double uy, double uz)
{
   // Dimensions used to size house
   const double wid= .2;
   //  Unit vector in direction of flght
   double D0 = sqrt(dx*dx+dy*dy+dz*dz);
   double X0 = dx/D0;
   double Y0 = dy/D0;
   double Z0 = dz/D0;
   //  Unit vector in "up" direction
   double D1 = sqrt(ux*ux+uy*uy+uz*uz);
   double X1 = ux/D1;
   double Y1 = uy/D1;
   double Z1 = uz/D1;
   //  Cross product gives the third vector
   double X2 = Y0*Z1-Y1*Z0;
   double Y2 = Z0*X1-Z1*X0;
   double Z2 = X0*Y1-X1*Y0;
   //  Rotation matrix
   double mat[16];
   mat[0] = X0;   mat[4] = X1;   mat[ 8] = X2;   mat[12] = 0;
   mat[1] = Y0;   mat[5] = Y1;   mat[ 9] = Y2;   mat[13] = 0;
   mat[2] = Z0;   mat[6] = Z1;   mat[10] = Z2;   mat[14] = 0;
   mat[3] =  0;   mat[7] =  0;   mat[11] =  0;   mat[15] = 1;

   //  Save current transforms
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glMultMatrixd(mat);
   // draw a square tree
   glBegin(GL_QUADS);
   glColor3f(0.5f, 0.35f, 0.05f);
   //main tree
   
   glVertex3d(0, wid, 0);
   glVertex3d(1.2,wid, 0);
   glVertex3d(1.2, -wid, 0);
   glVertex3d(0, -wid, 0);
  
   glVertex3d(0, .2, 0);
   glVertex3d(1.2,.2, 0);
   glVertex3d(1.2, .2, -.4);
   glVertex3d(0, .2, -.4);
   
   glVertex3d(0, -.2, 0);
   glVertex3d(1.2,-.2, 0);
   glVertex3d(1.2, -.2, -.4);
   glVertex3d(0, -.2, -.4);
   
  
   glVertex3d(0, wid, -.4);
   glVertex3d(1.2,wid, -.4);
   glVertex3d(1.2, -wid, -.4);
   glVertex3d(0, -wid, -.4);
   
   //branch
   
   glVertex3d(.7, .2, 0);
   glVertex3d(1,.4, 0);
   glVertex3d(1, .5, 0);
   glVertex3d(.6, .2, 0);
   
   
   glVertex3d(.6, .2, 0);
   glVertex3d(1,.5, 0);
   glVertex3d(1, .5, -.3);
   glVertex3d(.6, .2, -.3);
   
   glVertex3d(.7, .2, 0);
   glVertex3d(1,.4, 0);
   glVertex3d(1, .4, -.3);
   glVertex3d(.7, .2, -.3);
   
   glVertex3d(.7, .2, -.3);
   glVertex3d(1,.4, -.3);
   glVertex3d(1, .5, -.3);
   glVertex3d(.6, .2, -.3);
   
   glEnd();
   sphere1(1.5,0,-.2,.55);
   sphere1(1.1,.45,-.15,.25);
   // undo transformations
   glPopMatrix();
}

static void tree2(double x,double y,double z,
                 double dx,double dy,double dz,
                 double ux,double uy, double uz)
{
   // Dimensions used to size house
   const double wid= .2;
   //  Unit vector in direction of flght
   double D0 = sqrt(dx*dx+dy*dy+dz*dz);
   double X0 = dx/D0;
   double Y0 = dy/D0;
   double Z0 = dz/D0;
   //  Unit vector in "up" direction
   double D1 = sqrt(ux*ux+uy*uy+uz*uz);
   double X1 = ux/D1;
   double Y1 = uy/D1;
   double Z1 = uz/D1;
   //  Cross product gives the third vector
   double X2 = Y0*Z1-Y1*Z0;
   double Y2 = Z0*X1-Z1*X0;
   double Z2 = X0*Y1-X1*Y0;
   //  Rotation matrix
   double mat[16];
   mat[0] = X0;   mat[4] = X1;   mat[ 8] = X2;   mat[12] = 0;
   mat[1] = Y0;   mat[5] = Y1;   mat[ 9] = Y2;   mat[13] = 0;
   mat[2] = Z0;   mat[6] = Z1;   mat[10] = Z2;   mat[14] = 0;
   mat[3] =  0;   mat[7] =  0;   mat[11] =  0;   mat[15] = 1;

   //  Save current transforms
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glMultMatrixd(mat);
   // draw a square tree
   glBegin(GL_QUADS);
   glColor3f(0.5f, 0.35f, 0.05f);
   //main tree
   
   glVertex3d(0, wid, 0);
   glVertex3d(1.2,wid, 0);
   glVertex3d(1.2, -wid, 0);
   glVertex3d(0, -wid, 0);
  
   glVertex3d(0, .2, 0);
   glVertex3d(1.2,.2, 0);
   glVertex3d(1.2, .2, -.4);
   glVertex3d(0, .2, -.4);
   
   glVertex3d(0, -.2, 0);
   glVertex3d(1.2,-.2, 0);
   glVertex3d(1.2, -.2, -.4);
   glVertex3d(0, -.2, -.4);
   
   glVertex3d(0, wid, -.4);
   glVertex3d(1.2,wid, -.4);
   glVertex3d(1.2, -wid, -.4);
   glVertex3d(0, -wid, -.4);
   
   //branch
   
   glVertex3d(.7, .2, 0);
   glVertex3d(1,.4, 0);
   glVertex3d(1, .5, 0);
   glVertex3d(.6, .2, 0);
   
   glVertex3d(.6, .2, 0);
   glVertex3d(1,.5, 0);
   glVertex3d(1, .5, -.3);
   glVertex3d(.6, .2, -.3);
   
   glVertex3d(.7, .2, 0);
   glVertex3d(1,.4, 0);
   glVertex3d(1, .4, -.3);
   glVertex3d(.7, .2, -.3);
   
   glVertex3d(.7, .2, -.3);
   glVertex3d(1,.4, -.3);
   glVertex3d(1, .5, -.3);
   glVertex3d(.6, .2, -.3);
   
   //branch2
   glVertex3d(.6, -.2, 0);
   glVertex3d(.9,-.4, 0);
   glVertex3d(.9, -.5, 0);
   glVertex3d(.5, -.2, 0);
      
   glVertex3d(.5, -.2, 0);
   glVertex3d(.9,-.5, 0);
   glVertex3d(.9, -.5, -.3);
   glVertex3d(.5, -.2, -.3);
      
   glVertex3d(.6, -.2, 0);
   glVertex3d(.9,-.4, 0);
   glVertex3d(.9, -.4, -.3);
   glVertex3d(.6, -.2, -.3);
      
   glVertex3d(.6, -.2, -.3);
   glVertex3d(.9,-.4, -.3);
   glVertex3d(.9, -.5, -.3);
   glVertex3d(.5, -.2, -.3);
   
   glEnd();
   sphere1(1.5,0,-.2,.55);
   sphere1(1.1,.45,-.15,.25);
   sphere1(1,-.45,-.15,.3);
   // undo transformations
   glPopMatrix();
}

/*
 *  Draw solid house
 *    at (x,y,z)
 *    nose towards (dx,dy,dz)
 *    up towards (ux,uy,uz)
 */
static void Solidhouse(double x,double y,double z,
                       double dx,double dy,double dz,
                       double ux,double uy, double uz)
{
   // Dimensions used to size house
   const double fence =-.7;
   const double wid= 0.5;
   const double nose=+2.0;
   const double cone= .98;
   const double chimtop = 2;
   const double chimbot = .8;
   //  Unit vector in direction of flght
   double D0 = sqrt(dx*dx+dy*dy+dz*dz);
   double X0 = dx/D0;
   double Y0 = dy/D0;
   double Z0 = dz/D0;
   //  Unit vector in "up" direction
   double D1 = sqrt(ux*ux+uy*uy+uz*uz);
   double X1 = ux/D1;
   double Y1 = uy/D1;
   double Z1 = uz/D1;
   //  Cross product gives the third vector
   double X2 = Y0*Z1-Y1*Z0;
   double Y2 = Z0*X1-Z1*X0;
   double Z2 = X0*Y1-X1*Y0;
   //  Rotation matrix
   double mat[16];
   mat[0] = X0;   mat[4] = X1;   mat[ 8] = X2;   mat[12] = 0;
   mat[1] = Y0;   mat[5] = Y1;   mat[ 9] = Y2;   mat[13] = 0;
   mat[2] = Z0;   mat[6] = Z1;   mat[10] = Z2;   mat[14] = 0;
   mat[3] =  0;   mat[7] =  0;   mat[11] =  0;   mat[15] = 1;

   //  Save current transforms
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glMultMatrixd(mat);
   //  Nose (4 sided)
   glColor3f(0,1,1);
   glBegin(GL_TRIANGLES);
   glVertex3d(nose, 0.0, 0.0);
   glVertex3d(cone, wid, wid);
   glVertex3d(cone,-wid, wid);

   glVertex3d(nose, 0.0, 0.0);
   glVertex3d(cone, wid,-wid);
   glVertex3d(cone,-wid,-wid);

   glVertex3d(nose, 0.0, 0.0);
   glVertex3d(cone, wid, wid);
   glVertex3d(cone, wid,-wid);

   glVertex3d(nose, 0.0, 0.0);
   glVertex3d(cone,-wid, wid);
   glVertex3d(cone,-wid,-wid);
   glEnd();
   glBegin(GL_QUADS);
   //back exterior wall
   glColor3f(1, 0, 0);
   glVertex3d(cone, -wid, wid);
   glVertex3d(0,-wid, wid);
   glVertex3d(0, wid, wid);
   glVertex3d(cone, wid, wid);
   //right exterior wall
   glColor3f(0, 0, 1);
   glVertex3d(cone, wid, -wid);
   glVertex3d(0, wid, -wid);
   glVertex3d(0,wid, wid);
   glVertex3d(cone, wid, wid);
   //front exterior wall
   glColor3f(1, 0, 0);
   glVertex3d(cone, -wid, -wid);
   glVertex3d(0, -wid, -wid);
   glVertex3d(0,wid, -wid);
   glVertex3d(cone, wid, -wid);
   //left exterior wall
   glColor3f(0, 0, 1);
   glVertex3d(cone, -wid, -wid);
   glVertex3d(0, -wid, -wid);
   glVertex3d(0,-wid, wid);
   glVertex3d(cone, -wid, wid);
   //floor
   glColor3f(0,.5,.5);
   glVertex3d(0,.5,0.5);
   glVertex3d(0,.5,-0.5);
   glVertex3d(0,-.5,-0.5);
   glVertex3d(0,-.5,0.5);
   //door
   glColor3f(.5,.35,.5);
   glVertex3d(0,0.2,-.501);
   glVertex3d(0,-0.2,-.501);
   glVertex3d(.5,-0.2,-.501);
   glVertex3d(.5,0.2,-.501);
   // window left
   //window right
   //chimney
   glColor3f(1,1,1);
   glVertex3d(chimbot,-.48,0);
   glVertex3d(chimtop,-.48,0);
   glVertex3d(chimtop,-0.2,0);
   glVertex3d(chimbot,-0.2,0);
   //chimney2
   glColor3f(1,1,1);
   glVertex3d(chimbot,-.48,.2);
   glVertex3d(chimtop,-.48,.2);
   glVertex3d(chimtop,-0.2,.2);
   glVertex3d(chimbot,-0.2,.2);
   //chimney3
   glColor3f(1,1,1);
   glVertex3d(chimbot,-0.48,0);
   glVertex3d(chimbot,-0.48,0.2);
   glVertex3d(chimtop,-0.48,.2);
   glVertex3d(chimtop,-0.48,0);
   //chimney4
   glColor3f(1,1,1);
   glVertex3d(chimbot,-0.2,0);
   glVertex3d(chimbot,-0.2,0.2);
   glVertex3d(chimtop,-0.2,0.2);
   glVertex3d(chimtop,-0.2,0);
   //chimney topper
   glColor3f(1,1,0);
   glVertex3d(chimtop,-0.51,0.21);
   glVertex3d(2.1,-0.51,0.21);
   glVertex3d(2.1,-0.51,-0.01);
   glVertex3d(chimtop,-0.51,-0.01);
   //chimney topper2
   glColor3f(1,1,0);
   glVertex3d(chimtop,-0.51,0.21);
   glVertex3d(chimtop,-0.19,0.21);
   glVertex3d(2.1,-0.19,0.21);
   glVertex3d(2.1,-0.51,0.21);
   //chimney topper3
   glColor3f(1,1,0);
   glVertex3d(chimtop,-0.19,0.21); 
   glVertex3d(chimtop,-0.19,-0.01);
   glVertex3d(2.1,-0.19,-0.01);
   glVertex3d(2.1,-0.19,0.21);
   //chimney topper4
   glColor3f(1,1,0);
   glVertex3d(chimtop,-0.19,-.01);
   glVertex3d(2.1,-0.19,-0.01);
   glVertex3d(2.1,-0.51,-0.01);
   glVertex3d(chimtop,-0.51,-0.01);
   //chimney topper base
   glColor3f(1,1,0);
   glVertex3d(chimtop,-0.51,0.21);
   glVertex3d(chimtop,-0.51,-0.01);
   glVertex3d(chimtop,-0.19,-0.01);
   glVertex3d(chimtop,-0.19,0.21);
   //chimney topper top
   glColor3f(1,1,0);
   glVertex3d(2.1,-0.51,0.21);
   glVertex3d(2.1,-0.51,-0.01);
   glVertex3d(2.1,-0.19,-0.01);
   glVertex3d(2.1,-0.19,0.21);
   //picket
   glColor3f(1,1,1);
   glVertex3d(.3, -0.5, fence);
   glVertex3d(0,-0.5, fence);
   glVertex3d(0, -0.4, fence);
   glVertex3d(.3, -0.4, fence);
    
   glColor3f(1,1,1);
   glVertex3d(.3, -0.3, fence);
   glVertex3d(0,-0.3, fence);
   glVertex3d(0, -0.2, fence);
   glVertex3d(.3, -0.2, fence);
  
   glColor3f(1,1,1);
   glVertex3d(.3, -.1, fence);
   glVertex3d(0, -.1, fence);
   glVertex3d(0, 0, fence);
   glVertex3d(.3, 0, fence);
   
   glColor3f(1,1,1);
   glVertex3d(.3, .1, fence);
   glVertex3d(0,.1, fence);
   glVertex3d(0, .2, fence);
   glVertex3d(.3, .2, fence);
   
   glColor3f(1,1,1);
   glVertex3d(.3, .3, fence);
   glVertex3d(0,.3, fence);
   glVertex3d(0, .4, fence);
   glVertex3d(.3, .4, fence);
   
   glColor3f(1,1,1);
   glVertex3d(.3, .5, fence);
   glVertex3d(0,.5, fence);
   glVertex3d(0, .6, fence);
   glVertex3d(.3, .6, fence);
  
  
   glColor3f(1,1,1);
   glVertex3d(.24, -wid, fence);
   glVertex3d(.2,-wid, fence);
   glVertex3d(.2, wid, fence);
   glVertex3d(.24, wid, fence);
  
   glColor3f(1,1,1);
   glVertex3d(.11, -wid, fence);
   glVertex3d(0.07,-wid, fence);
   glVertex3d(0.07, wid, fence);
   glVertex3d(.11, wid, fence);
   glEnd();
   
   glBegin(GL_TRIANGLES);
   glColor3f(1,1,1);
   glVertex3d(.3, .5, fence);
   glVertex3d(.35, .55, fence);
   glVertex3d(.3, .6, fence);
   
   glVertex3d(.3, .3, fence);
   glVertex3d(.35, .35, fence);
   glVertex3d(.3, .4, fence);
   
   glVertex3d(.3, .1, fence);
   glVertex3d(.35, .15, fence);
   glVertex3d(.3, .2, fence);
   
   glVertex3d(.3, -.1, fence);
   glVertex3d(.35, -.05, fence);
   glVertex3d(.3, 0, fence);
   
   glVertex3d(.3, -.5, fence);
   glVertex3d(.35, -.45, fence);
   glVertex3d(.3, -.4, fence);
   
   glVertex3d(.3, -.3, fence);
   glVertex3d(.35, -.25, fence);
   glVertex3d(.3, -.2, fence);
   glEnd();
   // undo transformations
   glPopMatrix();
}

/*
 * Function that handles the drawing of a circle using the triangle fan
 * method. This will create a filled circle.
 *
 * Params:
 *	x (GLFloat) - the x position of the center point of the circle
 *	y (GLFloat) - the y position of the center point of the circle
 *	radius (GLFloat) - the radius that the painted circle will have
 */
void circle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius){
	int i;
	int triangleAmount = 20; //# of triangles used to draw circle
	
	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * M_PI;
	
	glRotated(90, 0, 0,1);
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(x, y,z); // center of circle
		for(i = 0; i <= triangleAmount;i++) { 
			glVertex3f(
		            x, 
			    y + (radius * sin(i * twicePi / triangleAmount)),
			    z + (radius * cos(i *  twicePi / triangleAmount))
			);
		}
	glEnd();
}




/*
 *  Draw solid airplane
 *    at (x,y,z)
 *    nose towards (dx,dy,dz)
 *    up towards (ux,uy,uz)
 */
static void Solidhouse2(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   const double wid= 0.5;
   const double nose=+2.0;
   const double cone= 1;
   const double chimtop = 2;
   const double chimbot = .8;
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,0,1);
   glRotated(-85,1,0,0);
   glScaled(dx,dy,dz);
   //  roof (4 sided)
   glScaled(dx,dy,dz);
   glColor3f(.5,.5,1);
   glBegin(GL_TRIANGLES);
   glVertex3d(nose, 0.0, 0.0);
   glVertex3d(cone, wid, wid);
   glVertex3d(cone,-wid, wid);

   glVertex3d(nose, 0.0, 0.0);
   glVertex3d(cone, wid,-wid);
   glVertex3d(cone,-wid,-wid);

   glVertex3d(nose, 0.0, 0.0);
   glVertex3d(cone, wid, wid);
   glVertex3d(cone, wid,-wid);

   glVertex3d(nose, 0.0, 0.0);
   glVertex3d(cone,-wid, wid);
   glVertex3d(cone,-wid,-wid);
   glEnd();
   glBegin(GL_QUADS);
   //back exterior wall
   glColor3f(1, .5, 0);
   glVertex3d(cone, -wid, wid);
   glVertex3d(0,-wid, wid);
   glVertex3d(0, wid, wid);
   glVertex3d(cone, wid, wid);
   //right exterior wall
   glColor3f(0, .5, 1);
   glVertex3d(cone, wid, -wid);
   glVertex3d(0, wid, -wid);
   glVertex3d(0,wid, wid);
   glVertex3d(cone, wid, wid);
   //front exterior wall
   glColor3f(1, .5, 0);
   glVertex3d(cone, -wid, -wid);
   glVertex3d(0, -wid, -wid);
   glVertex3d(0,wid, -wid);
   glVertex3d(cone, wid, -wid);
   //left exterior wall
   glColor3f(0, .5, 1);
   glVertex3d(cone, -wid, -wid);
   glVertex3d(0, -wid, -wid);
   glVertex3d(0,-wid, wid);
   glVertex3d(cone, -wid, wid);
   //floor
   glColor3f(.5,.5,.5);
   glVertex3d(0,.5,0.5);
   glVertex3d(0,.5,-0.5);
   glVertex3d(0,-.5,-0.5);
   glVertex3d(0,-.5,0.5);
   //door
   glColor3f(.5,.35,.5);
   glVertex3d(0,0.2,-.501);
   glVertex3d(0,-0.2,-.501);
   glVertex3d(.4,-0.2,-.501);
   glVertex3d(.4,0.2,-.501);
   // window left
   //window right
   //chimney
   glColor3f(1,.8,.5);
   glVertex3d(chimbot,-.48,0);
   glVertex3d(chimtop,-.48,0);
   glVertex3d(chimtop,-0.2,0);
   glVertex3d(chimbot,-0.2,0);
   //chimney2
   glColor3f(1,.8,.5);
   glVertex3d(chimbot,-.48,.2);
   glVertex3d(chimtop,-.48,.2);
   glVertex3d(chimtop,-0.2,.2);
   glVertex3d(chimbot,-0.2,.2);
   //chimney3
   glColor3f(1,.8,.5);
   glVertex3d(chimbot,-0.48,0);
   glVertex3d(chimbot,-0.48,0.2);
   glVertex3d(chimtop,-0.48,.2);
   glVertex3d(chimtop,-0.48,0);
   //chimney4
   glColor3f(1,.8,.5);
   glVertex3d(chimbot,-0.2,0);
   glVertex3d(chimbot,-0.2,0.2);
   glVertex3d(chimtop,-0.2,0.2);
   glVertex3d(chimtop,-0.2,0);
   //chimney topper
   glColor3f(1,1,0);
   glVertex3d(chimtop,-0.51,0.21);
   glVertex3d(2.1,-0.51,0.21);
   glVertex3d(2.1,-0.51,-0.01);
   glVertex3d(chimtop,-0.51,-0.01);
   //chimney topper2
   glColor3f(1,1,0);
   glVertex3d(chimtop,-0.51,0.21);
   glVertex3d(chimtop,-0.19,0.21);
   glVertex3d(2.1,-0.19,0.21);
   glVertex3d(2.1,-0.51,0.21);
   //chimney topper3
   glColor3f(1,1,0);
   glVertex3d(chimtop,-0.19,0.21); 
   glVertex3d(chimtop,-0.19,-0.01);
   glVertex3d(2.1,-0.19,-0.01);
   glVertex3d(2.1,-0.19,0.21);
   //chimney topper4
   glColor3f(1,1,0);
   glVertex3d(chimtop,-0.19,-.01);
   glVertex3d(2.1,-0.19,-0.01);
   glVertex3d(2.1,-0.51,-0.01);
   glVertex3d(chimtop,-0.51,-0.01);
   //chimney topper base
   glColor3f(1,1,0);
   glVertex3d(chimtop,-0.51,0.21);
   glVertex3d(chimtop,-0.51,-0.01);
   glVertex3d(chimtop,-0.19,-0.01);
   glVertex3d(chimtop,-0.19,0.21);
   //chimney topper top
   glColor3f(1,1,0);
   glVertex3d(2.1,-0.51,0.21);
   glVertex3d(2.1,-0.51,-0.01);
   glVertex3d(2.1,-0.19,-0.01);
   glVertex3d(2.1,-0.19,0.21);
   glEnd();
   // undo transformations
   glPopMatrix();
}
/*
 *  Set projection
 */
static void Project()
{
   //  Tell gl to start manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  kill previous transformations
   glLoadIdentity();
   //  first person
   if(fp) {
      gluPerspective(fov,asp,dim/4,4*dim);
   }
   else 
   {
	  if (mode)
         gluPerspective(fov,asp,dim/4,4*dim);
      //  Orthogonal projection
      else
         glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
    }
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
} 

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double grass =5;
   const double len=1.5;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (fp) 
   {
      Cx = +2*dim*Sin(spin); // Change camera dimensions 
      Cz = -2*dim*Cos(spin);

      gluLookAt(Ex,Ey,Ez, Cx+Ex,Ey,Cz+Ez, 0,1,0); //  Use gluLookAt
   }
   else
   { 
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
   }
   //  draw houses
   Solidhouse(Cos(zh),Sin(zh), 0 ,-Sin(zh),Cos(zh),0 , Cos(4*zh),0,Sin(4*zh));
   Solidhouse(3,Sin(zh), .7 ,-Sin(zh),Cos(zh),0 , Cos(4*zh),0,Sin(100));
   Solidhouse(-1,Sin(zh), .7 ,-Sin(zh),Cos(zh),0 , Cos(4*zh),0,Sin(300));
   tree(2,Sin(zh), -.2 ,-Sin(zh),Cos(zh),0 , Cos(4*zh),0,Sin(4*zh));
   tree(1,Sin(zh), -2 ,-Sin(zh),Cos(zh),0 , Cos(4*zh),0,Sin(4*zh));
   tree(0,Sin(zh), -1.5 ,-Sin(zh),Cos(zh),0 , Cos(4*zh),0,Sin(4*zh));
   tree(2.5,Sin(zh), -3 ,-Sin(zh),Cos(zh),0 , Cos(4*zh),0,Sin(4*zh));
   tree2(-1.5,Sin(zh), -3 ,-Sin(zh),Cos(zh),0 , Cos(4*zh),0,Sin(150));
   tree2(-2,Sin(zh), .1 ,-Sin(zh),Cos(zh),0 , Cos(4*zh),0,Sin(180));
   tree2(-1.8,Sin(zh), -1 ,-Sin(zh),Cos(zh),0 , Cos(4*zh),0,Sin(270));
   tree2(3,Sin(zh), 2 ,-Sin(zh),Cos(zh),0 , Cos(4*zh),0,Sin(270));
   Solidhouse2(-2.6, 0, 3, .9, 1.5, 1.5, 90);

   //grass
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonOffset(1,1);
   glBegin(GL_QUADS);
   glColor3f(0,1,.1);
   glVertex3d(grass,0,grass);
   glVertex3d(-grass,0,grass);
   glVertex3d(-grass,0,-grass);
   glVertex3d(grass,0,-grass);
   glEnd();
   glDisable(GL_POLYGON_OFFSET_FILL);
   glColor3f(.5,.34,.5);
   circle(0.001,-0.8,2.6,2);
   glColor3f(1,1,1);
   if (axes)
   {
	  glLoadIdentity();
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
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",th,ph,dim,fov,mode?"Perpective":"Orthogonal");
   //  Render the scene and make it visible
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th -= 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th += 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_UP)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project();
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
   {   
      th = 0;
      ph = 90;
   }
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
   //  Toggle first person
   else if (ch == 'f' || ch == 'F')
   {
      fp = 1-fp;
   }
   if (fp)
   {
      double dt = 0.05;
      if (ch == 'w' || ch == 'W'){
         Ex += Cx*dt;
         Ez += Cz*dt;
      }
      else if (ch == 'a' || ch == 'A'){
         spin -= 3;
      }
      else if (ch == 's' || ch == 'S'){
         Ex -= Cx*dt;
         Ez -= Cz*dt;
      }
      else if (ch == 'd' || ch == 'D'){
         spin += 3;
      }
      //  Keep angles to +/-360 degrees
      spin %= 360;
   }
   //  Switch display mode
   else if (ch == 'm' || ch == 'M')
      mode = 1-mode;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   //  Reproject
   Project();
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
   Project();
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
   glutInitWindowSize(600,600);
   glutCreateWindow("Andrew Candelaresi: HW4 Views");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
