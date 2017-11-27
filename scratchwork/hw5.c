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


#define PI 3.14159265358979323846
/*  D degrees of rotation */
#define DEF_D 5
int axes=1;       //  Display axes
int mode=1;       //  Projection mode
int move=1;       //  Move light
int th=0;         //  Azimuth of view angle
int ph=5;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=12.0;   //  Size of world
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

void static body_cone(double x, double y, double z, double dx, double dy, double dz, 
							 double deg_cir, double x_rot, double z_rot)
{
  /* cone */

    /* sides */
    glPushMatrix();
    glTranslated(x,y,z);
    glScaled(dx, dy, dz);
    glRotated(x_rot, 1, 0, 0);
    glRotated(z_rot, 0, 0, 1);
    glBegin(GL_TRIANGLES);

    for (int k=0;k<=deg_cir;k+=DEF_D)
    {
      // height
      glVertex3d(0,0,1);

      glVertex3d(Cos(k),Sin(k),0);
      //glColor3d(1.0,0.0,0.0);
      glVertex3d(Cos(k+DEF_D),Sin(k+DEF_D),0);
    }
    glEnd();
        /* bottom circle */ 
    /* rotate back *
    glRotated(90,1,0,0);
    glBegin(GL_TRIANGLES);
    for (int k=0;k<=deg_cir;k+=DEF_D) {
      //glColor3f(1.0,0.0,0.0);
      glVertex3f(0,0,0);
      //glColor3f(1.0,0.0,1.0);
      glVertex3f(Cos(k),0,Sin(k));
      //glColor3f(1.0,1.0,0.0);
      glVertex3f(Cos(k+DEF_D),0,Sin(k+DEF_D));
    }
    glEnd();
    * */
    glPopMatrix();
}

void static body_mid_sect(double x, double y, double z,
						  double dx, double dy, double dz,
						  double deg_cir, double x_rot, double z_rot)
{
   /* cylinder */
   /*  sides */
   glPushMatrix();
   glTranslated(x,y,z);
   glScaled(dx, dy, dz);
   glRotated(x_rot, 1, 0, 0);
   glRotated(z_rot, 0, 0, 1);
   glBegin(GL_QUAD_STRIP);
   for (int j=0;j<=deg_cir;j+=DEF_D) {
     glVertex3d(Cos(j),+1,Sin(j));
     glVertex3d(Cos(j),-1,Sin(j));
   }
   glEnd();
   glPopMatrix();
}

/* This function will draw the cylinder
 *
 *   @parameter1: radius = The radius of cylinder
 *   @parameter2: height = Height of the cylinder
 *   @parameter3: R = Red value of the cylinder's color
 *   @parameter4: G = Green value of the cylinder's color
 *   @parameter5: B = Blue value of the cylinder's color
 *
 *   @return: Nothing
*/
void draw_cylinder(GLfloat x, 
				   GLfloat y,
				   GLfloat z,
				   GLfloat radius,
				   GLfloat height)
{

    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.1;
    glPushMatrix();
	glTranslatef(x, y, z);
    /** Draw the tube */
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            glVertex3f(x, y , 0.0);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
        glVertex3f(radius, 0.0, 0.0);
    glEnd();

    /** Draw the circle on top of cylinder */
    glColor3f(.5,.7,.3);
    glBegin(GL_POLYGON);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
    glEnd();
    
    glPopMatrix();
}

void cock_pit_body(GLfloat x, 
				   GLfloat y,
				   GLfloat z,
				   double dx,
				   double dy, 
				   double dz,
				   double th,
				   double sh,
				   double base,
				   double top,
				   double height,
				   double slices,
				   double stacks)
{


    glPushMatrix();
    glTranslatef(x, y, z);
    glRotated(th, dx, 0, 0);
    glRotated(sh, 0, dy, dz);

	glRotated(th, dx, 0, 0);
    GLUquadricObj *quadObj = gluNewQuadric();
    gluCylinder(quadObj, base, top, height, slices, stacks);
    glPopMatrix();
}

void cock_pit_body2(GLfloat x, 
				   GLfloat y,
				   GLfloat z,
				   double dx,
				   double dy, 
				   double dz,
				   double th,
				   double sh,
				   double base,
				   double top,
				   double height,
				   double slices,
				   double stacks)
{


    glPushMatrix();
    glTranslatef(x, y, z);
    glRotated(th, dx, 0, 0);
    glRotated(sh, 0, dy, 0);
    GLUquadricObj *quadObj = gluNewQuadric();
    gluCylinder(quadObj, base, top, height, slices, stacks);
    glPopMatrix();
}



/* This function will draw the cylinder with out the top
 *
 *   @parameter1: radius = The radius of cylinder
 *   @parameter2: height = Height of the cylinder
 *   @parameter3: R = Red value of the cylinder's color
 *   @parameter4: G = Green value of the cylinder's color
 *   @parameter5: B = Blue value of the cylinder's color
 *
 *   @return: Nothing
*/
void draw_cylinder_no_top(GLfloat x, 
				   GLfloat y,
				   GLfloat z,
				   GLfloat radius,
				   GLfloat height)
{

    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.1;
    glPushMatrix();
	glTranslatef(x, y, z);
    /** Draw the tube */
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            glVertex3f(x, y , 0.0);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
        glVertex3f(radius, 0.0, 0.0);
    glEnd();

    /** Draw the circle on top of cylinder 
    glColor3f(.5,.7,.3);
    glBegin(GL_POLYGON);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
    glEnd();
    */
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
 * Function that handles the drawing of a circle using the line loop
 * method. This will create a hollow circle.
 *
 * Params:
 *	x (GLFloat) - the x position of the center point of the circle
 *	y (GLFloat) - the y position of the center point of the circle
 *	radius (GLFloat) - the radius that the painted circle will have
 */
void drawHollowCircle(GLfloat x, GLfloat y, GLfloat radius){
	int i;
	int lineAmount = 100; //# of triangles used to draw circle
	
	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * PI;
	
	glBegin(GL_POLYGON);
		for(i = 0; i <= lineAmount;i++) { 
			glVertex2f(
			    x + (radius * cos(i *  twicePi / lineAmount)), 
			    y + (radius* sin(i * twicePi / lineAmount))
			);
		}
	glEnd();
}

void static reactcore(double x, double y, double z,
				  double dx, double dy, double dz,
				  double th)
{
	glPushMatrix();
   // reactor core
   glTranslated(x, y, z);
   glRotated(th, dx, 0,0);
   glBegin(GL_POLYGON);
   glColor3f(1,1,1);
   glVertex3d(0.125,-3.55,0.0);
   glVertex3d(0.125,-3.3,0.0);
   glVertex3d(5.243,-0.575,0.0);
   glVertex3d(5.977,-0.865,0.0);
   glVertex3d(6.365,-3.55,0.0);
   glEnd();
   glPopMatrix();
}

void static corewalls(double x, double y, double z,
				  double dx, double dy, double dz,
				  double th)
{
		glPushMatrix();
   // reactor core
   glTranslated(x, y, z);
   glRotated(th, dx, 0,0);
   glBegin(GL_POLYGON);
   glColor3f(1,0,0);
   glVertex3d(6.365,-3.55,0.0);
   glVertex3d(6.365,-3.55,-1.0);
   glVertex3d(0.125,-3.55,-1.0);
   glVertex3d(0.125,-3.55,0.0);
   glEnd();
   
   glBegin(GL_POLYGON);
   glColor3f(0,1,1);
   glVertex3d(0.125,-3.55,0.0);
   glVertex3d(0.125,-3.55,-1.0);
   glVertex3d(0.125,-3.3,-1.0);
   glVertex3d(0.125,-3.3,0.0);

   //glVertex3d(5.243,-0.675,-1.0);
   //glVertex3d(5.243,-0.675,0.0);
   glEnd();
   
   
   glBegin(GL_POLYGON);
   glColor3f(0,1,0);
   glVertex3d(0.125,-3.3,0.0);
   glVertex3d(0.125,-3.3,-1.0);
   glVertex3d(5.243,-0.675,-1.0);
   glVertex3d(5.243,-0.675,0.0);
   glEnd();
   
   glBegin(GL_POLYGON);
   glColor3f(0,0,1);
   glVertex3d(5.977,-0.865,0.0);
   glVertex3d(5.977,-0.865,-1.0);
   glVertex3d(6.365,-3.55,-1.0);
   glVertex3d(6.365,-3.55,0.0);
   glEnd();
   /*
   glBegin(GL_POLYGON);
   glColor3f(1,.5,.5);
   glVertex3d(6.365,-3.677,0.0);
   glVertex3d(6.365,-3.677,-1.0);
   glVertex3d(1.912,-3.677,-1.0);
   glVertex3d(1.912,-3.677,0.0);
   glEnd();
   */ 
   glPopMatrix();
}

void static tunnel(double x, double y, double z, 
				   double dx, double dy, double dz,
				   double th, double sh)
{
   glPushMatrix();
   // tunnel top piece
   glTranslated(x, y, z);
   glRotated(th, dx, 0,0);
   glRotated(sh, 0, dy,dz);
   glBegin(GL_POLYGON);
   glColor3f(1,0,0);
   glVertex3d(6.669, -2.85,1.0);
   glVertex3d(7.669, -2.85,1.0);
   glVertex3d(7.669, -1,1.0);
   glVertex3d(6.669, -1,1.0);
   glEnd();
   // tunnel side 1 piece
   glBegin(GL_POLYGON);
   glColor3f(1,1,0);
   glVertex3d(6.669, -2.85,1.0);
   glVertex3d(6.269, -2.9,0.0);
   glVertex3d(6.269, -1,0.0);
   glVertex3d(6.669, -1,1.0);
   glEnd();
   // tunnel side 2 piece
   glBegin(GL_POLYGON);
   glColor3f(1,1,0);
   glVertex3d(7.669, -2.85,1.0);
   glVertex3d(8.069, -2.9,0.0);
   glVertex3d(8.069, -1,0.0);
   glVertex3d(7.669, -1,1.0);
   glEnd();
   // tunnel interieor ship piece
   glBegin(GL_POLYGON);
   glColor3f(1,1,0);
   glVertex3d(6.669, -2.85,1.0);
   glVertex3d(7.669, -2.85,1.0);
   glVertex3d(8.069, -2.9,0.0);
   glVertex3d(6.269, -2.9,0.0);
   glEnd();
   // tunnel escape pod piece
   glBegin(GL_POLYGON);
   glColor3f(1,0,1);
   glVertex3d(7.669, -1,1.0);
   glVertex3d(6.669, -1,1.0);
   glVertex3d(6.269, -1,0.0);
   glVertex3d(8.069, -1,0.0);
   glEnd();
      glBegin(GL_POLYGON);
   glColor3f(1,0,1);
   glVertex3d(6.269, -1,0.0);
   glVertex3d(6.269, -1,-0.5);
   glVertex3d(8.069, -1,-0.5);
   glVertex3d(8.069, -1,0.0);
   glEnd();
   glPopMatrix();
}
void static fronts_piece(double x, double y, double z,
				  double dx, double dy, double dz,
				  double th)
{
   glPushMatrix();
   // fronts piece
   glTranslated(x, y, z);
   glRotated(th, dx, 0,0);
   glBegin(GL_POLYGON);
   glColor3f(1,0,0);
   glVertex3d(2.0,-3.65,0.8);
   glVertex3d(5.9,-3.55,1.1);
   glVertex3d(5.9,-4.78,1.1);
   glVertex3d(2.0,-4.68,0.8);
   glEnd();	
   //up slant pieces
   glBegin(GL_POLYGON);
   glColor3f(0.1,0.5,0.6);
   glVertex3d(2.0,-3.65,0.8);
   glVertex3d(5.9,-3.55,1.1);
   glVertex3d(5.9,-3.25,0.1);
   glVertex3d(1.7,-3.35,0.1);
   glEnd();	
   //down slant pieceside
   glBegin(GL_POLYGON);
   glColor3f(0.1,0.5,0.6);
   glVertex3d(2.0,-4.68,0.8);
   glVertex3d(5.9,-4.78,1.1);
   glVertex3d(5.9,-5.8,0.1);
   glVertex3d(1.7,-4.98,0.1);
   glEnd();	
   // front slant pieces
   glBegin(GL_POLYGON);
   glColor3f(0.1,1.0,0.6);
   glVertex3d(2.0,-3.75,0.8);
   glVertex3d(2.0,-4.68,0.8);
   glVertex3d(1.7,-4.98,0.1);
   glVertex3d(1.7,-3.35,0.1);
   glEnd();	
   // back
   glBegin(GL_POLYGON);
   glColor3f(0.1,1.0,0.6);
   glVertex3d(5.9,-3.55,1.1);
   glVertex3d(5.9,-3.25,0.1);
   glVertex3d(5.9,-5.8,0.1);
   glVertex3d(5.9,-4.78,1.1);
   glEnd();
   // bottom 
   glBegin(GL_POLYGON);
   glColor3f(0.1,1.0,0.6);
   glVertex3d(5.9,-3.25,0.1);
   glVertex3d(1.7,-3.35,0.1);
   glVertex3d(1.7,-4.98,0.1);
   glVertex3d(5.9,-5.8,0.1);
   glEnd();
   glPopMatrix();
}

void static falcon1(double x, double y, double z,
				  double dx, double dy, double dz,
				  double th)
{
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   //glRotated(sh,0,1,0);
   glRotated(th,1,0,0);
   glScaled(dx,dy,dz);
   
   reactcore(0,0,0,0,0,0,0);
   reactcore(0,0,-1,0,0,0,0.0);
   corewalls(0,0,0,0,0,0,0);
   
   reactcore(0,-8.4,0,1,0,0,180);
   reactcore(0,-8.4,-1,1,0,0,180);
   corewalls(0,-8.4,-1,1,0,0,180);
   // reactor core
   /*
   
   
   //tail
   glBegin(GL_POLYGON);
   glColor3f(1,1,1);
   glVertex3d(9.022,-1.283,0.0);
   glVertex3d(9.482,-1.323,0.0);
   glVertex3d(9.514,-1.376,0.0);
   glVertex3d(9.831,-1.672,0.0);
   glVertex3d(10.065,-2.016,0.0);
   glVertex3d(10.322,-2.377,0.0);
   glVertex3d(10.522,-2.777,0.0);
   glVertex3d(10.635,-3.237,0.0);
   glVertex3d(10.673,-3.274,0.0);
   glVertex3d(10.749,-3.694,0.0);
   glVertex3d(10.798,-4.137,0.0);
   glVertex3d(10.789,-4.586,0.0);
   glVertex3d(10.736,-5.053,0.0);
   glVertex3d(10.607,-5.543,0.0);
   glVertex3d(10.245,-6.385,0.0);
   glVertex3d(10.205,-6.454,0.0);
   glVertex3d(9.987,-6.735,0.0);
   glVertex3d(9.542,-6.854,0.0);
   glVertex3d(7.454,-3.226,0.0);
   glVertex3d(7.683,-3.488,0.0);
   glEnd();
   //tail 2 
   glBegin(GL_POLYGON);
   glVertex3d(9.542,-6.854,0.0);
   glVertex3d(7.597,-4.726,0.0);
   glVertex3d(7.732,-4.472,0.0);
   glVertex3d(7.801,-4.289,0.0);
   glVertex3d(7.801,-3.761,0.0);
   glVertex3d(7.732,-3.601,0.0);
   glVertex3d(7.801,-4.289,0.0);
   glEnd();
   
   
   // tail flap 1
   glBegin(GL_QUADS);
   glColor3f(1.0,1.0,1.0);
   glVertex3d(9.508,-1.372,0.0);
   glVertex3d(9.635,-1.257,0.0);
   glVertex3d(9.905,-1.570,0.0);
   glVertex3d(9.796,-1.654,0.0);
   glEnd();
   
   // tail flap 2
   glBegin(GL_QUADS);
   glColor3f(1.0,1.0,1.0);
   glVertex3d(9.811,-1.687,0.0);
   glVertex3d(9.934,-1.597,0.0);
   glVertex3d(10.194,-1.936,0.0);
   glVertex3d(10.057,-2.006,0.0);
   glEnd();
   
   // tail flap 3
   glBegin(GL_QUADS);
   glColor3f(1.0,1.0,1.0);
   glVertex3d(10.076,-2.045,0.0);
   glVertex3d(10.210,-1.981,0.0);
   glVertex3d(10.443,-2.337,0.0);
   glVertex3d(10.320,-2.389,0.0);
   glEnd();
   
   // tail flap 4
   glBegin(GL_QUADS);
   glColor3f(1.0,1.0,1.0);
   glVertex3d(10.321,-2.434,0.0);
   glVertex3d(10.454,-2.377,0.0);
   glVertex3d(10.640,-2.741,0.0);
   glVertex3d(10.491,-2.802,0.0);
   glEnd();   
   
   // tail flap 5
   glBegin(GL_QUADS);
   glColor3f(1.0,1.0,1.0);
   glVertex3d(10.508,-2.849,0.0);
   glVertex3d(10.642,-2.811,0.0);
   glVertex3d(10.781,-3.207,0.0);
   glVertex3d(10.632,-3.241,0.0);
   glEnd();
   
      // tail flap 6
   glBegin(GL_QUADS);
   glColor3f(1.0,1.0,1.0);
   glVertex3d(10.647,-3.289,0.0);
   glVertex3d(10.782,-3.245,0.0);
   glVertex3d(10.892,-3.661,0.0);
   glVertex3d(10.736,-3.688,0.0);
   glEnd();
   
      // tail flap 7
   glBegin(GL_QUADS);
   glColor3f(1.0,1.0,1.0);
   glVertex3d(10.738,-3.739,0.0);
   glVertex3d(10.885,-3.702,0.0);
   glVertex3d(10.947,-4.135,0.0);
   glVertex3d(10.785,-4.117,0.0);
   glEnd();
   
      // tail flap 8
   glBegin(GL_QUADS);
   glColor3f(1.0,1.0,1.0);
   glVertex3d(10.782,-4.181,0.0);
   glVertex3d(10.938,-4.181,0.0);
   glVertex3d(10.936,-4.585,0.0);
   glVertex3d(10.789,-4.581,0.0);
   glEnd();
   
      // tail flap 9
   glBegin(GL_QUADS);
   glColor3f(1.0,1.0,1.0);
   glVertex3d(10.785,-4.661,0.0);
   glVertex3d(10.949,-4.669,0.0);
   glVertex3d(10.892,-5.076,0.0);
   glVertex3d(10.737,-5.040,0.0);
   glEnd();
   
      // tail flap 10
   glBegin(GL_QUADS);
   glColor3f(1.0,1.0,1.0);
   glVertex3d(10.721,-5.090,0.0);
   glVertex3d(10.882,-5.143,0.0);
   glVertex3d(10.772,-5.546,0.0);
   glVertex3d(10.618,-5.497,0.0);
   glEnd();
   
      // tail flap 11
   glBegin(GL_QUADS);
   glColor3f(1.0,1.0,1.0);
   glVertex3d(10.380,-5.555,0.0);
   glVertex3d(10.745,-5.632,0.0);
   glVertex3d(10.590,-6.010,0.0);
   glVertex3d(10.451,-5.941,0.0);
   glEnd();
   
      // tail flap 12
   glBegin(GL_QUADS);
   glColor3f(1.0,1.0,1.0);
   glVertex3d(10.428,-5.982,0.0);
   glVertex3d(10.551,-6.046,0.0);
   glVertex3d(10.364,-6.408,0.0);
   glVertex3d(10.233,-6.337,0.0);
   glEnd();
   
      // tail flap 13
   glBegin(GL_QUADS);
   glColor3f(1.0,1.0,1.0);
   glVertex3d(10.202,-6.391,0.0);
   glVertex3d(10.326,-6.486,0.0);
   glVertex3d(10.112,-6.827,0.0);
   glVertex3d(9.988,-6.732,0.0);
   glEnd();
   
   
      //left top bay
   glBegin(GL_POLYGON);
   glColor3f(1,1,1);
   glVertex3d(7.389,-7.526,-0.2);
   glVertex3d(6.687,-7.563,-0.2);
   glVertex3d(6.589,-5.156,-0.2);
   glVertex3d(7.324,-5.147,-0.2);
   glEnd();
	*/
   //
   //glVertex3d(7.478,-3.234,0.0);
	
   glColor3f(.6,.6,1);
   //hull of the ship
   body_cone(6.669,-4.2,-1, 3.8, 3.8, 1,125,180,291);
   body_cone(6.669,-4.2,0, 3.8, 3.8, 1,164.99,180,96);
   //cock_pit_body(6.669, -4.2, -1, 1, 0, 0, 90, 0, 3.8, 0, 1, 50, 10);
   //glColor3f(.6,.6,1);
   glColor3f(0,1,1);
   //hull of the ship close back
   body_cone(6.669,-4.2,0, 3.8, 3.8, 1,130,0,293);
   //close front
   body_cone(6.669,-4.2,0, 3.8, 3.8, 1,164.99,0,96);
   // extra body cap
   body_cone(6.669,-4.2,0, 1.8, 1.8, 1,360,0,0);
   //cock_pit_body(6.669, -4.2, 0, 0, 0, 0, 0, 0, 3.8, 0, 1, 50, 10);

   draw_cylinder(6.869,-3.975, 0.5, 1, .5);
   //cirular body of ship
   glColor3f(1,1,1);
   body_mid_sect(0, 0, 0,0,0,0,360,0,0);
   // draw_cylinder_no_top(6.669, -4.2, -1, 3.7, 1);
   fronts_piece(0,0,0,0,0,0,0);
   fronts_piece(0,-8.5,-1,1,0,0,180);
   tunnel(0,0,0,0,0,0,0,0);
   tunnel(14.25,0,-1,1,0,1,180, 180);
   tunnel(0,-8.35,-1,1,0,0,180,0);
   tunnel(14.25,-8.35,0,1,1,0,180,180);
   //tunnel
   glColor3f(0.0,1.0,0.0);
   // cock pit inside ship
   cock_pit_body(4.75, -0.2, 0.5, 1.0, 0.0, 1.0, 48.0, 25.0, 0.8, 0.8, 3.0, 50.0, 10.0);
   glColor3f(1.0,0.0,0.0);
   // cockpit out side shipt
   cock_pit_body2(3.299, -0.28, 0.5, 1, 1, 0, 90, 90, 0.8, 0.8, 1.5, 50, 10);
   glColor3f(0,0,1);
   //cock pit cone
   cock_pit_body2(1.789, -0.28, 0.5, 1, 1, 0, 90, 90, 0.5, 0.8, 1.5, 50, 10);
   glColor3f(1,1,1);
   // join cock pit sections
   sphere1(4.779,-0.28,0.5,0.8);
   glColor3f(1,0,0);
   // escape pods
   cock_pit_body2(7.1,0,0,1,0,0,90,0, .5, .5, .5, 50, 10);



   
   //gluCylinder(x, y, z, dx, dy, dz, th, sh, base, top, height, slices, stacks);
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




   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   
   //  Draw scene !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   falcon1(-5.0, 0,0,1,1,1,0);
   //body_cone(0,0,0, 2, .5, 2,160,270,0);
   // body_cone(double x, double y, double z, double dx, double dy, double dz,double deg_cir, double x_rot, double z_rot)
   
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
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
