/*
 * help from Thabang Ngazimbi opengl-flight-simulator
 */

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
 *  F3         Toggle light distance (1/5)
 *  m          Toggles light movement
 *  []         Lower/rise light
 *  +/-        Change field of view of perspective
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 *	c		   cockpit view
 */
#include "CSCIx229.h"
#include <math.h>
#include "Common.h"

GLuint falconId;
#define PI 3.14159265358979323846
/*  D degrees of rotation */
#define DEF_D 5
/***************************************************************/
/******************** GLOBAL DEFINITIONS  **********************/
/***************************************************************/
#define MY_MAX(A,B) ((A) >= (B) ? (A) : (B))

/* my constants for GLUT window defaults */
#define MY_WIND_X_SIZE     600
#define MY_WIND_Y_SIZE     600
#define MY_WIND_X_POS      100
#define MY_WIND_Y_POS      100
#define Radius 10

/* my menu IDs for GLUT menu-handling functions */
#define MY_MENU_EXIT     -9999
#define MY_MENU_LIGHT       30
#define MY_MENU_OBJECT      40
#define MY_ROT_LIGHT        50
#define MY_MENU_PROJECT    100

const double plane_angle = 0.5;
double movex = 0;

/**
 * randBetween
 * Returns a random number between the given min and max.
 */
float randBetween(int min, int max) {
	if (max <= min) {
		return min;
	}
	return ((rand() % (max - min)) + min);
}
/*****************************************************************/
/******************** FUNCTION DEFINITIONS ***********************/
/*****************************************************************/
/*
 * */

static void Sky(double D);
static void ball(double x,double y,double z,double r);
void display();	
void idle();
void key(unsigned char ch,int x,int y);

void initNew();
void reshape(int width,int height);
void timer(int toggle);
void special(int key,int x,int y);
void vec_mat_mult( GLfloat M[], GLfloat vec[], GLfloat out_vec[]);
GLfloat renderingOptions[] = { GL_FILL, GL_LINE };

//int fp=1;         //  change 
int axes=1;       //  Display axes
int mode=1;       //  Projection mode
int ship=1;       //  toggle between cockpit and 3rd person
int move=1;       //  Move light
int th=0;         //  Azimuth of view angle
int ph=5;         //  Elevation of view angle
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=12.0;   //  Size of world

//first person aspects
int spin=0;    //  Rotation for first person

// Light values
int one       =   1;  // Unit value
int distance  =   9;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
int zh        =  200;  // Light azimuth
float ylight  =   0;  // Elevation of light
unsigned int texture[12];
unsigned int sky[5];

/* 3D Projection */
static GLdouble XCamera, YCamera, ZCamera;
static GLdouble FOVangle, FOVratio, ClipNear, ClipFar;

// ship movements
int    fly=0;    //  Animated flight
double roll=0;   //  Roll angle
double pitch=0;  //  Pitch angle
double yaw=0;    //  Yaw angle
int    pwr=100;  //  Power setting (%)


static GLfloat LightPos0[4];

static GLfloat LightPos1[4];

// new vars
static GLfloat rot_angle = 0.0;

static GLfloat sunMatrix[16] = {1,0,0,0,  0,1,0,0, 0,0,1,0, 0,0,0,1};

static GLfloat planeMatrix[16] = {1,0,0,0,  0,1,0,0, 0,0,1,0, 0,0,0,1};

static GLfloat azumMatrix[16] = {1,0,0,0,  0,1,0,0, 0,0,1,0, 0,0,0,1};

GLfloat gRotationMatrix[16] = {1, 0, 0, 0,
			       0, 1, 0, 0,
			       0, 0, 1, 0,
			       0, 0, 0, 1};
			       
double  position[3] = {0,0,0};			       	       
double  forward[3] = {0, 0, -1};
double  up[3] = {0, 1, 0};
double  right[3] = {1, 0, 0};
double  speed = 0.01;
			      
static float plane_pos[3] = { 0.0, 0.0, 0.0 };

static GLint pov = 1;

// transformations
static GLdouble  TX=0, TY=0, TZ=0;

/* user interaction */
static GLint InteractionMode;
static GLint XmouseDown, YmouseDown;
static GLint XmouseCurrent, YmouseCurrent;
static GLint XmousePrev, YmousePrev, DX, DY;
static GLint WindHeight, WindWidth;
static GLint KeybModifiers;
static GLint mouseButtons = 0;

float normal[3] = {1.0,0.0,0.0};

//GLMmodel* pmodel = NULL;
//GLMmodel* smodel = NULL;


/* calculate how the change in pitch effects flight*/
void static Pitch(double angle)
{
   forward[0] = forward[0] * cos(angle) + up[0] * sin(angle);
   forward[1] = forward[1] * cos(angle) + up[1] * sin(angle);
   forward[2] = forward[2] * cos(angle) + up[2] * sin(angle);
   double D = sqrt(forward[0]*forward[0]+forward[1]*forward[1]+forward[2]*forward[2]);
   forward[0] = forward[0] / D;
   forward[1] = forward[1] / D;
   forward[2] = forward[2] / D;
   up[0] =  up[1] *forward[2] - up[2]* forward[1]; 
   up[1] = up[2] *forward[0] - up[0]* forward[2]; 
   up[2] = up[0] *forward[1] - up[1]* forward[0]; 
}

void static Roll(double angle)
{
   right[0] = right[0] * cos(angle) + up[0] * sin(angle);
   right[1] = right[1] * cos(angle) + up[1] * sin(angle);
   right[2] = right[2] * cos(angle) + up[2] * sin(angle);
   double D = sqrt(right[0]*right[0]+right[1]*right[1]+right[2]*right[2]);
   right[0] = right[0] / D;
   right[1] = right[1] / D;
   right[2] = right[2] / D;
   up[0] =  up[1] *right[2] - up[2]* right[1];
   up[1] = up[2] *right[0] - up[0]* right[2];
   up[2] = up[0] *right[1] - up[1]* right[0];
}

void static Yaw(double angle)
{
   right[0] = right[0] * cos(angle) + forward[0] * sin(angle);
   right[1] = right[1] * cos(angle) + forward[1] * sin(angle);
   right[2] = right[2] * cos(angle) + forward[2] * sin(angle);
   double D = sqrt(right[0]*right[0]+right[1]*right[1]+right[2]*right[2]);
   right[0] = right[0] / D;
   right[1] = right[1] / D;
   right[2] = right[2] / D;
   up[0] =  up[1] *right[2] - up[2]* right[1];
   up[1] = up[2] *right[0] - up[0]* right[2]; 
   up[2] = up[0] *right[1] - up[1]* right[0]; 
}

void vec_mat_mult( GLfloat M[], GLfloat vec[], GLfloat out_vec[]){

  out_vec[0] = M[0]*vec[0] + M[4]*vec[1] + M[8]*vec[2] + M[12]*vec[3] ;
  out_vec[1] = M[1]*vec[0] + M[5]*vec[1] + M[9]*vec[2] + M[13]*vec[3] ;
  out_vec[2] = M[2]*vec[0] + M[6]*vec[1] + M[10]*vec[2] + M[14]*vec[3] ;
  out_vec[3] = M[3]*vec[0] + M[7]*vec[1] + M[11]*vec[2] + M[15]*vec[3] ;

  return;

}

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

void body_cone(double x, double y, double z, double dx, double dy, double dz, 
							 double deg_cir, double x_rot, double z_rot, unsigned int texture)
{
/* cone */

    /* sides */
    glPushMatrix();
    glTranslated(x,y,z);
    glScaled(dx, dy, dz);
    glRotated(x_rot, 1, 0, 0);
    glRotated(z_rot, 0, 0, 1);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_TRIANGLES);

    for (int k=0;k<=deg_cir;k+=DEF_D)
    {
	  x=Cos(k);
	  y= Sin(k);
	  double m = sqrt(Cos(k)*Cos(k)+Sin(k)*Sin(k));
	  double nx = x/m;
	  double ny = y/m;
	  double hr = 1/1;
	  glNormal3d(nx*hr, ny*hr, hr); 
      // height
      glTexCoord2f(.634,.5); glVertex3d(0,0,1);

      glTexCoord2f(.25*Cos(k-65)+0.634,.45*Sin(k-65)+.5); glVertex3d(Cos(k),Sin(k),0);

      glTexCoord2f(.25*Cos(k-65+DEF_D)+0.634,.45*Sin(k-65+DEF_D)+.5); glVertex3d(Cos(k+DEF_D),Sin(k+DEF_D),0);
    }
    glEnd();
    glPopMatrix();
}

void body_mid_sect(double x, double y, double z,
						  double dx, double dy, double dz,
						  double deg_cir, double x_rot, double z_rot)
{
   /* cylinder */
   /*  sides */
   double mag_x = .5;
   double mag_y = .5;
   glPushMatrix();
   glTranslated(x,y,z);
   glScaled(dx, dy, dz);
   glRotated(90, 1, 0, 0);
   glRotated(z_rot, 0, 1, 0);
   glBindTexture(GL_TEXTURE_2D, texture[2]);
   glBegin(GL_QUAD_STRIP);
   //DEF_D
   for (int j=0;j<=deg_cir;j+= DEF_D) {
	 glNormal3f(-Cos(j),0,-Sin(j));
     glTexCoord2f(0,Sin(j)); glVertex3d(Cos(j),+1,Sin(j));
     glNormal3f(-Cos(j),0,-Sin(j));
     glTexCoord2f(.085, Sin(j)); glVertex3d(Cos(j),-1,Sin(j));
   }
   glEnd();
   glPopMatrix();
}

/* This function will draw the cylinder
 *
 *   @x, y, z  :           positions coords for gltranslate
 *   @parameter1: radius = The radius of cylinder
 *   @parameter2: height = Height of the cylinder
 *   @parameter3: x_rot  = degrees to rotate around the x axis
 *
 *   @return: Nothing
*/
void draw_cylinder(GLfloat x, 
				   GLfloat y,
				   GLfloat z,
				   GLfloat radius,
				   GLfloat height,
				   double x_rot, unsigned int cap, unsigned int cyl)
{

    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.1;
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, cyl);
	glTranslatef(x, y, z);
	glRotated(x_rot, 1,0,0);
    /** Draw the tube */
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            double m = sqrt(x*+y*y);
	        double nx = x/m;
	        double ny = y/m;
            glNormal3f(nx,ny,0);
            glTexCoord2f(x,height); glVertex3f(x, y , height);
            glTexCoord2f(x,0); glNormal3f(x,y,0);
            glVertex3f(x, y , 0.0);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
        glVertex3f(radius, 0.0, 0.0);
    glEnd();

    /** Draw the circle on top of cylinder */
    glBindTexture(GL_TEXTURE_2D, cap);
    glBegin(GL_POLYGON);
    glNormal3f(0,0,1);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glTexCoord2f(.085*x+.619,.11*y+.52);
            glVertex3f(x, y , height);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
    glEnd();
    
    glPopMatrix();
}

/*
 * three rotations gives me the 45 degree angle I am 
 * looking for
 */
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
				   double stacks,
				   unsigned int texture)
{


    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture);
    glTranslatef(x, y, z);
    glRotated(th, dx, 0, 0);
    glRotated(sh, 0, dy, dz);

	glRotated(th, dx, 0, 0);
    GLUquadricObj *quadObj = gluNewQuadric();
    gluQuadricTexture(quadObj, GLU_TRUE); 
    gluCylinder(quadObj, base, top, height, slices, stacks);
    glPopMatrix();
}

/*only has 2 rotations parst outside of the ship and escape pods
 * 
 */
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
				   double stacks, 
				   unsigned int texture)
{


    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture);
    glTranslatef(x, y, z);
    glRotated(th, dx, 0, 0);
    glRotated(sh, 0, dy, 0);
    GLUquadricObj *quadObj = gluNewQuadric();
    gluQuadricTexture(quadObj, GLU_TRUE); 
    gluCylinder(quadObj, base, top, height, slices, stacks);
    glPopMatrix();
}



/* This function will draw the cylinder with out the top
 *
 *   @parameter1: radius = The radius of cylinder
 *   @parameter2: height = Height of the cylinder
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
void sphere1(double x,double y,double z,double r)
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
 * Function that handles the drawing of a circle using the line loop
 * method. This will create a hollow circle.
 *
 * Params:
 *	x (GLFloat) - the x position of the center point of the circle
 *	y (GLFloat) - the y position of the center point of the circle
 *	radius (GLFloat) - the radius that the painted circle will have
 */
void drawHollowCircle(GLfloat x, GLfloat y, double dx, double dy, double dz,
					GLfloat radius, double th, double sh, double mag_x,
					double mag_y, double center_x, double center_y, unsigned int texture)
{
	glPushMatrix();
	glTranslated(dx, dy, dz);
	glRotated(th,1,0,0);
	glRotated(sh,0,1,0);  
	glNormal3f(0,0,1);
	int i;
	int lineAmount = 100; //# of triangles used to draw circle

	
	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * PI;
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_POLYGON);
		for(i = 0; i <= lineAmount;i++) { 
			glTexCoord2f(
			    mag_x*(x + (radius * cos(i *  twicePi / lineAmount)))+center_x, 
			    mag_y*(y + (radius* sin(i * twicePi / lineAmount)))+center_y
			);
			glVertex2f(
			    x + (radius * cos(i *  twicePi / lineAmount)), 
			    y + (radius* sin(i * twicePi / lineAmount))
			);
		}
	glEnd();
	glPopMatrix();
}

void drawHollowpitCircle(GLfloat x, GLfloat y, double dx, double dy, double dz,
					GLfloat radius, double th, double sh)
{
	glPushMatrix();
	glNormal3f(0,0,1);
	int i;
	int lineAmount = 100; //# of triangles used to draw circle
	
    glLineWidth(8);


	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_LINE_STRIP);
		for(i = 0; i <= lineAmount;i++) { 
			glTexCoord2f(0.0,0.0);  glVertex2f(
			    x + (radius * cos(i *  PI / lineAmount)), 
			    y + (radius* sin(i * PI / lineAmount))
			);
		}
	glEnd();
    glPopMatrix();
}

void reactcore(double x, double y, double z,
				  double dx, double dy, double dz,
				  double th, unsigned int duralloy)
{
	glPushMatrix();
   glBindTexture(GL_TEXTURE_2D, duralloy);
   // reactor core
   glTranslated(x, y, z);
   glRotated(th, dx, 0,0);
   glBegin(GL_POLYGON);
   glNormal3f(0,0,-1);
   glColor3f(1,1,1);
   glTexCoord2f(0.0,0.0); glVertex3d(0.125,-3.55,0.0);
   glTexCoord2f(0.0,1.0); glVertex3d(0.125,-3.3,0.0);
   //outer most edge
   glTexCoord2f(1.0,1.0); glVertex3d(5.243,-0.695,0.0);
   glVertex3d(5.977,-0.865,0.0);
   glTexCoord2f(0.0,1.0); glVertex3d(6.365,-3.55,0.0);
   glEnd();
   glPopMatrix();
}

void corewalls(double x, double y, double z,
				  double dx, double dy, double dz,
				  double th)
{
		glPushMatrix();
   // reactor core
   glTranslated(x, y, z);
   glRotated(th, dx, 0,0);
   glBindTexture(GL_TEXTURE_2D, texture[0]);
   glBegin(GL_POLYGON);
   glNormal3f(0,1,0);

   glTexCoord2f(0.0,0.0); glVertex3d(6.365,-3.55,0.0);
   glTexCoord2f(0.0,1.0); glVertex3d(6.365,-3.55,-1.0);
   glTexCoord2f(1.0,1.0); glVertex3d(0.125,-3.55,-1.0);
   glTexCoord2f(1.0,0.0); glVertex3d(0.125,-3.55,0.0);
   glEnd();
   
   glBegin(GL_POLYGON);

   glNormal3f(1,0,0);
   glTexCoord2f(0.0,0.0); glVertex3d(0.125,-3.55,0.0);
   glTexCoord2f(0.0,1.0); glVertex3d(0.125,-3.55,-1.0);
   glTexCoord2f(1.0,1.0); glVertex3d(0.125,-3.3,-1.0);
   glTexCoord2f(1.0,0.0); glVertex3d(0.125,-3.3,0.0);
   glEnd();
   
   
   glBegin(GL_POLYGON);

   glNormal3f(0.45361012, -0.89120023,  0.);
   glTexCoord2f(0.0,0.0); glVertex3d(0.125,-3.3,0.0);
   glTexCoord2f(0.0,1.0); glVertex3d(0.125,-3.3,-1.0);
   glTexCoord2f(1.0,1.0); glVertex3d(5.243,-0.695,-1.0);
   glTexCoord2f(1.0,0.0); glVertex3d(5.243,-0.695,0.0);
   glEnd();
   
   glBegin(GL_POLYGON);

   glNormal3f(1,0,0);
   glTexCoord2f(0.0,0.0); glVertex3d(5.977,-0.865,0.0);
   glTexCoord2f(0.0,1.0); glVertex3d(5.977,-0.865,-1.0);
   glTexCoord2f(1.0,1.0); glVertex3d(6.365,-3.55,-1.0);
   glTexCoord2f(1.0,0.0); glVertex3d(6.365,-3.55,0.0);
   glEnd();
   glPopMatrix();
}

/*
 * connects the tunnel portion to the exterior hull of the ship
 * 
 */
void tunl_hull_conn(double x, double y, double z, 
				   double dx, double dy, double dz, unsigned int duralloy)
{
   glPushMatrix();
   // 
   glTranslated(x, y, z);
   glRotated(dx, 1, 0,0);
   glRotated(dz, 0, 0,1);

   glBindTexture(GL_TEXTURE_2D, duralloy);
   glColor3f(.9,.9,.9);
   glBegin(GL_POLYGON);
   glNormal3f(0.95589167,  0.29371944,  0.);
   glTexCoord2f(0.0,0.0); glVertex3d(6.269, -1,-1.1);
   glTexCoord2f(0.0,1.0); glVertex3d(6.269, -1,0.1);
   glTexCoord2f(1.0,1.0); glVertex3d(6.1, -.45,0.0);
   glTexCoord2f(1.0,0.0); glVertex3d(6.1, -.45,-1);
   glEnd();
   glBegin(GL_POLYGON);
   glNormal3f(0.89905734,  -0.4378309, 0.);
   glTexCoord2f(0.0,0.0); glVertex3d(8.069, -1,0.05);
   glTexCoord2f(0.0,1.0); glVertex3d(8.069, -1,-1.05);
   glTexCoord2f(1.0,1.0); glVertex3d(8.2, -.731,-1.0);
   glTexCoord2f(1.0,0.0); glVertex3d(8.2, -.731,0.0);
   glEnd();
   glBegin(GL_POLYGON);
   glNormal3f(-0.94915681, 0.31480367, 0.);
   glTexCoord2f(0.0,0.0); glVertex3d(6.269, -7.35,0.16);
   glTexCoord2f(0.0,1.0); glVertex3d(6.269, -7.35,-1.16);
   glTexCoord2f(1.0,1.0); glVertex3d(6.07, -7.95,-1);
   glTexCoord2f(1.0,0.0); glVertex3d(6.07, -7.95,0.0);
   glEnd();
   glBegin(GL_POLYGON);
   glNormal3f(-0.93132479, -0.36418969, -0.);
   glTexCoord2f(0.0,0.0); glVertex3d(8.069, -7.34,0.08);
   glTexCoord2f(0.0,1.0); glVertex3d(8.069, -7.34,-1.08);
   glTexCoord2f(1.0,1.0); glVertex3d(8.2, -7.675,-1.0);
   glTexCoord2f(1.0,0.0); glVertex3d(8.2, -7.675,0.0);
   glEnd();
   glPopMatrix();
}	
			   				   
void tunnel(double x, double y, double z, 
				   double dx, double dy, double dz,
				   double th, double sh)
{
   glPushMatrix();
   // tunnel top piece
   glTranslated(x, y, z);
   glRotated(th, dx, 0,0);
   glRotated(sh, 0, dy,dz);
   glBindTexture(GL_TEXTURE_2D, texture[3]);
   glBegin(GL_POLYGON);
   glNormal3f(0,0,1);
   glTexCoord2f(0.58,0.659); glVertex3d(6.669, -2.85,1.0); //1
   glTexCoord2f(0.643,0.661); glVertex3d(7.669, -2.85,1.0); //2
   glTexCoord2f(0.625,.915); glVertex3d(7.669, -1,1.0); //3
   glTexCoord2f(0.563,.91); glVertex3d(6.669, -1,1.0); //4
   glEnd();
   // tunnel side 1 piece
   glBegin(GL_POLYGON);
   glNormal3f(0.92796046,  0.01958756, -0.37216356);
   glTexCoord2f(0.58,0.659); glVertex3d(6.669, -2.85,1.0); //1
   glTexCoord2f(0.55,.659); glVertex3d(6.269, -2.9,0.0); //2
   glTexCoord2f(.535,.89); glVertex3d(6.269, -1,0.1);  //3
   glTexCoord2f(.565,.915); glVertex3d(6.669, -1,1.0); //4
   glEnd();
   // tunnel side 2 piece
   glBegin(GL_POLYGON);
   glNormal3f(0.92796046, -0.01958756,  0.37216356);
   glTexCoord2f(0.595,0.3); glVertex3d(7.669, -2.85,1.0);//1
   glTexCoord2f(0.56,0.3); glVertex3d(8.069, -2.9,0.0); //2
   glTexCoord2f(.57,0.1); glVertex3d(8.069, -1,0.1); //3
   glTexCoord2f(.602,0.1); glVertex3d(7.669, -1,1.0); //4
   glEnd();
   // tunnel interieor ship piece
   glBegin(GL_POLYGON);
   glNormal3f( 0.0, 0.99875234, -0.04993762);
   glTexCoord2f(0.58,0.659); glVertex3d(6.669, -2.85,1.0);//1
   glTexCoord2f(0.59,0.659); glVertex3d(7.669, -2.85,1.0);//2
   glTexCoord2f(0.61,0.60); glVertex3d(8.069, -2.9,0.0);//3
   glTexCoord2f(0.56,0.60); glVertex3d(6.269, -2.9,0.0);//4
   glEnd();
   glBindTexture(GL_TEXTURE_2D, texture[2]);
   // tunnel escape pod piece
   glBegin(GL_POLYGON);
   glNormal3f(0,-1,0);
   glTexCoord2f(0.402,1); glVertex3d(7.669, -1,1.0);//1
   glTexCoord2f(0.32,1); glVertex3d(6.669, -1,1.0);//2
   glTexCoord2f(.28,.97); glVertex3d(6.269, -1,0.1);//3
   glTexCoord2f(.442,.97); glVertex3d(8.069, -1,0.1);//4
   glEnd();
   glBegin(GL_POLYGON);
   glNormal3f(0,-1,0);
   glTexCoord2f(.28,.97); glVertex3d(6.269, -1,0.1);
   glTexCoord2f(.28,.9); glVertex3d(6.269, -1,-0.5);
   glTexCoord2f(.442,.9); glVertex3d(8.069, -1,-0.5);
   glTexCoord2f(.442,.97); glVertex3d(8.069, -1,0.1);
   glEnd();
   glPopMatrix();
}


/*
 *need different lengths for the tunels on diffent sides of the ship
 * 
 */
void tunnel2(double x, double y, double z, 
				   double dx, double dy, double dz,
				   double th, double sh, double sx,
				   double sy, double sz)
{
   glPushMatrix();
   // tunnel top piece
   glTranslated(x, y, z);
   glRotated(th, dx, 0,0);
   glRotated(sh, 0, dy,dz);
   glScaled(sx, sy, sz);
   glBindTexture(GL_TEXTURE_2D, texture[3]);
   glBegin(GL_POLYGON);
   glNormal3f(0,0,1);
   glTexCoord2f(0.58,0.659); glVertex3d(6.669, -2.85,1.0); //1
   glTexCoord2f(0.643,0.661); glVertex3d(7.669, -2.85,1.0); //2
   glTexCoord2f(0.625,.915); glVertex3d(7.669, -1,1.0); //3
   glTexCoord2f(0.563,.91); glVertex3d(6.669, -1,1.0); //4
   glEnd();
   // tunnel side 1 piece
   glBegin(GL_POLYGON);
   glNormal3f(0.92796046,  0.01958756, -0.37216356);
   glTexCoord2f(0.58,0.659); glVertex3d(6.669, -2.85,1.0); //1
   glTexCoord2f(0.55,.659); glVertex3d(6.269, -2.9,0.0); //2
   glTexCoord2f(.535,.89); glVertex3d(6.269, -1,0.1);  //3
   glTexCoord2f(.565,.915); glVertex3d(6.669, -1,1.0); //4
   glEnd();
   // tunnel side 2 piece
   glBegin(GL_POLYGON);
   glNormal3f(0.92796046, -0.01958756,  0.37216356);
   glTexCoord2f(0.595,0.3); glVertex3d(7.669, -2.85,1.0);//1
   glTexCoord2f(0.56,0.3); glVertex3d(8.069, -2.9,0.0); //2
   glTexCoord2f(.57,0.1); glVertex3d(8.069, -1,0.1); //3
   glTexCoord2f(.602,0.1); glVertex3d(7.669, -1,1.0); //4
   glEnd();
   // tunnel interieor ship piece
   glBegin(GL_POLYGON);
   glNormal3f( 0.0, 0.99875234, -0.04993762);
   glTexCoord2f(0.58,0.659); glVertex3d(6.669, -2.85,1.0);//1
   glTexCoord2f(0.59,0.659); glVertex3d(7.669, -2.85,1.0);//2
   glTexCoord2f(0.61,0.60); glVertex3d(8.069, -2.9,0.0);//3
   glTexCoord2f(0.56,0.60); glVertex3d(6.269, -2.9,0.0);//4
   glEnd();
   glBindTexture(GL_TEXTURE_2D, texture[2]);
   // tunnel escape pod piece
   glBegin(GL_POLYGON);
   glNormal3f(0,-1,0);
   glTexCoord2f(0.402,1); glVertex3d(7.669, -1,1.0);//1
   glTexCoord2f(0.32,1); glVertex3d(6.669, -1,1.0);//2
   glTexCoord2f(.28,.97); glVertex3d(6.269, -1,0.1);//3
   glTexCoord2f(.442,.97); glVertex3d(8.069, -1,0.1);//4
   glEnd();
   glBegin(GL_POLYGON);
   glNormal3f(0,-1,0);
   glTexCoord2f(.28,.97); glVertex3d(6.269, -1,0.1);
   glTexCoord2f(.28,.9); glVertex3d(6.269, -1,-0.5);
   glTexCoord2f(.442,.9); glVertex3d(8.069, -1,-0.5);
   glTexCoord2f(.442,.97); glVertex3d(8.069, -1,0.1);
   glEnd();
   glPopMatrix();
}
void fronts_piece(double x, double y, double z,
				  double dx, double dy, double dz,
				  double th)
{
   glPushMatrix();
   // fronts piece
   glTranslated(x, y, z);
   glRotated(th, dx, 0,0);
   glBindTexture(GL_TEXTURE_2D, texture[3]);
   glBegin(GL_POLYGON);
   glNormal3f( 0.23162053, 0, -0.97280621);
   //top pieace
   glTexCoord2f(0.175,.55); glVertex3d(2.0,-3.65,0.3);//1
   glTexCoord2f(0.525,.6); glVertex3d(5.9,-3.55,.79);//2
   glTexCoord2f(.53,.43); glVertex3d(5.9,-4.78,.75);//3
   glTexCoord2f(.175,.43); glVertex3d(2.0,-4.68,.3);//4
   glEnd();
   
   	

   //up slant pieces
   glBegin(GL_POLYGON);
   glNormal3f(-0.04661261, 0.95678517, 0.28703555);
   glTexCoord2f(0.275,.6); glVertex3d(2.0,-3.65,0.3);//1
   glTexCoord2f(0.525,.6); glVertex3d(5.9,-3.55,.79);//2
   glTexCoord2f(0.55,.65); glVertex3d(5.9,-3.25,0.1);//3
   glTexCoord2f(0.25,.65); glVertex3d(1.7,-3.35,0.1);//
   glEnd();	
   //down slant pieceside

   glBegin(GL_POLYGON);
   glNormal3f(0.07268645, 0.69821961, -0.712184);
   glTexCoord2f(.175,.43); glVertex3d(2.0,-4.68,0.3);
   glTexCoord2f(.53,.43); glVertex3d(5.9,-4.78,.79);
   glTexCoord2f(.53,.4); glVertex3d(5.9,-5.8,0.1);
   glTexCoord2f(.15, .425); glVertex3d(1.7,-4.98,0.1);
   glEnd();	
   // front slant pieces
   glBindTexture(GL_TEXTURE_2D, texture[5]);
   glBegin(GL_POLYGON);
   glNormal3f(0.91914503, 0, -0.3939193);\
   glTexCoord2f(0.485,.57); glVertex3d(2.0,-3.65,0.3);//1
   glTexCoord2f(.64,.58); glVertex3d(2.0,-4.68,0.3);//2
   glTexCoord2f(0.645,.525); glVertex3d(1.7,-4.98,0.1);//3
   glTexCoord2f(0.475,.52); glVertex3d(1.7,-3.35,0.1);//4
   glEnd();	
   // back
   glBegin(GL_POLYGON);
   glNormal3f(-1,0,0);

   glTexCoord2f(0.0,0.0); glVertex3d(5.9,-3.55,.79);
   glTexCoord2f(0.0,1.0); glVertex3d(5.9,-3.25,0.1);
   glTexCoord2f(1.0,1.0); glVertex3d(5.9,-5.8,0.1);
   glTexCoord2f(1.0,0.0); glVertex3d(5.9,-4.78,.79);
   glEnd();
   // bottom 
   glBegin(GL_POLYGON);
   glNormal3f(0,0,1);

   glTexCoord2f(0.0,0.0); glVertex3d(5.9,-3.25,0.1);
   glTexCoord2f(0.0,1.0); glVertex3d(1.7,-3.35,0.1);
   glTexCoord2f(1.0,1.0); glVertex3d(1.7,-4.98,0.1);
   glTexCoord2f(1.0,0.0); glVertex3d(5.9,-5.8,0.1);
   glEnd();
   glPopMatrix();
}
void cockpit()
{
   //  Screen edge
   float w = asp>2 ? asp : 2;
   //  Save transform attributes (Matrix Mode and Enabled Modes)
   glPushAttrib(GL_TRANSFORM_BIT|GL_ENABLE_BIT);
   //  Save projection matrix and set unit transform
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();
   glOrtho(-asp,+asp,-1,1,-1,1);
   //  Save model view matrix and set to indentity
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();
   //  Draw instrument panel with texture
   glColor3f(1,1,1);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, texture[1]);
   //left cont panel
   glBegin(GL_QUADS);
   glTexCoord2d(0.2,0.0);glVertex2f(-2,-1);
   glTexCoord2d(.39,0.0);glVertex2f(-.5,-1);
   glTexCoord2d(.39,.31);glVertex2f(-.5, -0.1);
   glTexCoord2d(0.2,.31);glVertex2f(-2, -0.1);
   glEnd();
   //middle control panel
   glBegin(GL_QUADS);
   glTexCoord2d(.39,0.0);glVertex2f(-.5,-1);
   glTexCoord2d(.57,0.0);glVertex2f(+.5,-1);
   glTexCoord2d(.57,.34);glVertex2f(+.5, 0);
   glTexCoord2d(0.39,.34);glVertex2f(-.5, 0);
   glEnd();
   //right control panel
   glBegin(GL_QUADS);
   glTexCoord2d(0.57,0.0);glVertex2f(.5,-1);
   glTexCoord2d(.8,0.0);glVertex2f(+2,-1);
   glTexCoord2d(.8,.31);glVertex2f(+2, -0.1);
   glTexCoord2d(0.57,.31);glVertex2f(.5, -0.1);
   glEnd();
   
   glDisable(GL_TEXTURE_2D);
   //  Draw the inside of the cockpit in grey
   glColor3f(0.6,0.6,0.6);
   glBegin(GL_QUADS);
   //  Port
   glVertex2f(-2,-1);
   glVertex2f(-2,+1);
   glVertex2f(-w,+1);
   glVertex2f(-w,-1);
   //  Starboard
   glVertex2f(+2,-1);
   glVertex2f(+2,+1);
   glVertex2f(+w,+1);
   glVertex2f(+w,-1);
   //  Port overhead
   glVertex2f(-2.00,+1);
   glVertex2f(-2.00,+1);
   glVertex2f(-0.03,+1);
   glVertex2f(-0.03,+1);
   //  Starboard overhead
   glVertex2f(+2.00,+1);
   glVertex2f(+2.00,+1);
   glVertex2f(+0.03,+1);
   glVertex2f(+0.03,+1);
   glEnd();
   glBegin(GL_QUADS);
   //  connector pieces
   glVertex2f(-1.12,.85);
   glVertex2f(-1.18,.76);
   glVertex2f(-.83,+.45);
   glVertex2f(-.8,.5);
   //  connector pieces
   glVertex2f(1.12,.85);
   glVertex2f(1.18,.76);
   glVertex2f(.83,+.45);
   glVertex2f(.8,.5);
   glEnd();
   //  Windshield divide
   drawHollowpitCircle(0.0,-.1,0,0,0,1,0,0);
   drawHollowpitCircle(0.0,-.1,0,0,0,1.01,0,0);
   drawHollowpitCircle(0.0,-.1,0,0,0,1.02,0,0);
   drawHollowpitCircle(0.0,-.1,0,0,0,.99,0,0);
   drawHollowpitCircle(0.0,-.1,0,0,0,1.45,0,0);
   drawHollowpitCircle(0,-.1,0,0,0,1.46,0,0);
   drawHollowpitCircle(0,-.1,0,0,0,1.47,0,0);

   //  Reset model view matrix
   glPopMatrix();
   //  Reset projection matrix
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   //  Pop transform attributes (Matrix Mode and Enabled Modes)
   glPopAttrib();
	
}



void compile_falcon()
{
	double x = -7;
	double y = 0;
	double z = -4;
	double dx = 1;
	double dy = 1;
    double dz = 1;

   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(270,1,0,0);
   glRotated(0,0,0,1);

   //glRotated(sh,0,1,0);


   glScaled(dx,dy,dz);
   //should make metal shine
   float white[] = {1,1,1,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,32.0f);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   
   //enable texture
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   
   //reactor core
   reactcore(0,0,0,0,0,0,0, texture[0]);
   reactcore(0,0,-1,0,0,0,0.0, texture[0]);
   corewalls(0,0,0,0,0,0,0);
   
   reactcore(0,-8.4,0,1,0,0,180, texture[0]);
   reactcore(0,-8.4,-1,1,0,0,180, texture[0]);
   corewalls(0,-8.4,-1,1,0,0,180);
    
   

   //glVertex3d(7.478,-3.234,0.0);
	
   glColor3f(1,1,1);
   //hull of the ship far back
   body_cone(6.669,-4.2,-1, 3.8, 3.8, 1,130,180,293, texture[3]);
   body_cone(6.669,-4.2,-1, 3.8, 3.8, 1,165.009,180,96.3, texture[3]);

   //glColor3f(.6,.6,1);
   glColor3f(1,1,1);
   //hull of the ship close back
   body_cone(6.669,-4.2,0, 3.8, 3.8, 1,130,0,293, texture[3]);
   //close front
   body_cone(6.669,-4.2,0, 3.8, 3.8, 1,165,0,94.5, texture[3]);
   // extra body closer
   //hull_close( 0, 0, 0, 0, 0, 0, texture[0]);
   


   // gun cap otherside body done we will have to send the functions the normals
   draw_cylinder(6.869,-3.975, -1.5, 1, .5,180, texture[3], texture[7]);
   draw_cylinder(6.869,-3.975, 0.5, 1, .48,0, texture[3], texture[7]);

   //cirular body of ship
   glColor3f(1,1,1);
   body_mid_sect(6.669, -4.2, -.5,3.8,3.8,.5,135,90,67);
   body_mid_sect(6.669, -4.2, -.5,3.8,3.8,.5,168,90,261);
   

   
   
   
   fronts_piece(0,0,0,0,0,0,0);
   fronts_piece(0,-8.5,-1,1,0,0,180);
   //one side
   //top
   tunnel2(0,0,0,0,0,0,0,0,1,1.05,1);
   //bot
   tunnel2(14.33,0,-1,1,0,1,180, 180,1,1.05,1);
   //other side
   tunnel2(0,-9.1,-1,1,0,0,180,0,1,1.45,1);
   tunnel2(14.33,-9.1,0,1,1,0,180,180,1,1.45,1);
   //tunnel

   // cock pit tunnel inside ship
   cock_pit_body(4.75, -0.2, 0.5, 1.0, 0.0, 1.0, 48.0, 25.0, 0.8, 0.8, 3.0, 50.0, 10.0, texture[7]);
   
   glColor3f(1,1,1);
   cock_pit_body(5.675, -3.01, 0.34, 1.0, 0.0, 1.0, 48.0, 25.0, 0.8, 0.3, .5, 50.0, 10.0, texture[7]);
   
   glColor3f(1.0,1.0,1.0);
   // cockpit tunnel out side shipt
   cock_pit_body2(3.299, -0.28, 0.5, 1, 1, 0, 90, 90, 0.8, 0.8, 1.5, 50, 10, texture[7]);
   glColor3f(1,1,1);
   //cock pit cone
   cock_pit_body2(1.789, -0.2799, 0.5, 1, 1, 0, 90, 90, 0.5, 0.8, 1.5, 50, 10, texture[3]);
   glColor3f(1,1,1);
   // join cock pit sections
   sphere1(4.779,-0.28,0.5,0.79);
   glColor3f(1,1,1);
   // escape pods
   cock_pit_body2(7.15,-.5,-.5,1,0,0,90,0, .7, .8, 1, 50, 10, texture[7]);
   cock_pit_body2(7.15,-7,-.5,1,0,0,90,0, .8, .7, 1, 50, 10, texture[7]);
   drawHollowCircle(0,0, 7.15, -.5, -.5, .7,90,0, .5, .28, .52, .455, texture[6]);
   drawHollowCircle(0,0, 7.15, -8, -.5, .7,90,0, .5, .28, .52, .455, texture[6]);
   
   //cockpit window
   drawHollowCircle(0,0,1.79,-.28,.5,.5,0,90, .2, .2, .1, .6, texture[5]);
   tunl_hull_conn(0,0,0,0,0,0, texture[0]);

   
   //gluCylinder(x, y, z, dx, dy, dz, th, sh, base, top, height, slices, stacks);
   glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}

void initFalcon()
{
    falconId = glGenLists(1);
    glNewList(falconId, GL_COMPILE);
        compile_falcon();
    glEndList();
}

void drawfalcon()
{
  glCallList(falconId);	
}


/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
void ball(double x,double y,double z,double r)
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
   glMaterialfv(GL_FRONT,GL_SHININESS,shiny);
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
 *  Draw sky box
 */
void Sky(double D)
{
   glColor3f(1,1,1);
   glEnable(GL_TEXTURE_2D);

   //  Sides
   glBindTexture(GL_TEXTURE_2D,sky[2]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.00,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);
   glTexCoord2f(0.00,1); glVertex3f(-D,+D,-D);

   glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);
   glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);

   glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);

   glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(1.00,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(1.00,1); glVertex3f(-D,+D,-D);
   glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
   glEnd();

   //  Top and bottom
   glBindTexture(GL_TEXTURE_2D,sky[1]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.,.5); glVertex3f(+D,+D,-D);
   glTexCoord2f(0.5,.5); glVertex3f(+D,+D,+D);
   glTexCoord2f(0.5,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.0,1); glVertex3f(-D,+D,-D);
   glEnd();
   glBindTexture(GL_TEXTURE_2D,sky[2]);
   glBegin(GL_QUADS);
   //glColor3f(0.2, 0.3, 0.7);
   glTexCoord2f(1.0,0.3); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.5,0.3); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.5,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(1.0,0); glVertex3f(-D,-D,-D);
   glEnd();

   glDisable(GL_TEXTURE_2D);
}


#define   DEGTORAD  (3.14/180.0)
#define   SPEED     0.05
static float  sun_angle = 0.0 ;

/*
 *  GLUT calls this routine every 50ms
 */
void timer(int toggle)
{
   //  Toggle movement
   if (toggle>0)
      move = !move;
   //  Increment light position
   else
      zh = (zh+5)%360;
   //  Animate flight using Lorenz transform
   if (fly)
   {
      //movex +=.05;
      //position[0] += speed *forward[0] + speed *up[0] + speed *right[0];
      //position[1] += speed *forward[1] + speed *up[1] + speed *right[1];
      //position[2] += speed *forward[2] + speed *up[2] + speed *right[2];
      
		float centerX = appWidth / 2.0f;
		float distanceX = (centerX - lastMouseX) / appWidth;
		planeRotation += distanceX / (10.5 * (planeSpeed + 1));

		planeTilt = distanceX * 40.0f;

		if (boolAccelerate == 1 && planeSpeed <= planeMaxSpeed - planeAcc) {
			planeSpeed += planeAcc;
		} else if (boolDeaccelerate == 1
				&& planeSpeed >= planeMinSpeed + planeDeacc) {
			planeSpeed -= planeDeacc;
		}
		//if alternate controls are enabled
		if (toggleAltControls == 0) {
			if (boolMoveUp == 1) {
				eyeY += planeRiseSpeed;
				atY += planeRiseSpeed;
			} else if (boolMoveDown == 1) {
				eyeY -= planeFallSpeed;
				atY -= planeFallSpeed;
			}
		} else {
			float centerY = appHeight / 2.0f;
			float distanceY = (centerY - lastMouseY) / appHeight;
			planeYawRotation += distanceY / (20 * (planeSpeed + 1));
			if (planeYawRotation > 1) {
				planeYawRotation = 1;
			} else if (planeYawRotation < -1) {
				planeYawRotation = -1;
			}

			eyeY += (sin(planeYawRotation) * (planeSpeed + 1)) / 10.0f;
			atY = eyeY + (sin(planeYawRotation) * 10.0f);

		}
		//move the camera based on circles using plane speed as the radius
		eyeX += (sin(planeRotation) * (planeSpeed + 1)) / 10.0f;
		eyeZ += (cos(planeRotation) * (planeSpeed + 1)) / 10.0f;
		//always look 10 units ahead
		atX = eyeX + (sin(planeRotation) * 10.0f);
		atZ = eyeZ + (cos(planeRotation) * 10.0f);

		if (eyeY < 2) {
			alive = 0;
			explosionScale = 0.0f;
			exploding = 1;
			eyeY = 2;
		}
	     gluLookAt(eyeX, eyeY, eyeZ, /* eye */
	     atX, atY, atZ, /* looking at*/
	     0.0, 1.0, 0.0);
		
   }
	   

   glutPostRedisplay();
   //  Set timer to go again
   if (move && toggle>=0) glutTimerFunc(10,timer,0);
   //  Tell GLUT it is necessary to redisplay the scene
   
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

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   
   
   double Ex = -2*dim*Sin(th)*Cos(ph);
   double Ey = +2*dim        *Sin(ph);
   double Ez = +2*dim*Cos(th)*Cos(ph);
   double Ox = 0;
   double Oy = 0;
   double Oz = 0;
   double Ux = 0; double Uy = 1; double Uz = 0;
   gluLookAt(Ex,Ey,Ez , Ox,Oy,Oz , Ux,Uy,Uz);
     

   Sky(3.5*dim);
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
   switch(ship)
   {
	  case 0:
	     //  Disable Z-buffering in OpenGL
         //glDisable(GL_DEPTH_TEST);
         //  Draw cockpit
         cockpit();
         break;

   
      case 1:
         
         //glTranslated(position[0], position[1], position[2]);
         //glTranslated(up[0], up[1], up[2]);
         //glTranslated(right[0], right[1], right[2]);
         //glRotated(yaw, 1,0,0);
         //glRotated(roll, 0, 1, 0);
         //glRotated(pitch, 0, 0, 1);

         //glScalef(0.75, 0.75, 0.75);
         glPushMatrix();
         //set up camera
	     //gluLookAt(eyeX, eyeY, eyeZ, // eye 
	     //atX, atY, atZ, // looking at
	     //0.0, 1.0, 0.0);
		 float rotationDeg=(planeRotation * 180.0f /M_PI);
		// glTranslatef(eyeX+((atX-eyeX)/2.0f), eyeY+((atY-eyeY)/2.0f)-2,eyeZ+((atZ-eyeZ)/2.0f));
	 	 //glScalef(0.8f, 0.8f, 0.8f);
		 //glRotatef(rotationDeg, 0, 1, 0);
		 //glRotatef(-planeTilt, 0, 0, 1);
		 //if (toggleAltControls == 1) 
		 //{
			// glRotatef(-planeYawRotation * 30.0f, 1, 0, 0);
	 	 //}
 
	  	 //glRotatef(90, 0, 1, 0);
           compile_falcon();
           //drawfalcon();

		glPopMatrix();
        break;
	}


   for (int i= 0; i<10; i++)
   {
	   double rx = randBetween(0,10);
	   double ry = randBetween(0,10);
	   double rz = randBetween(0,10);
	  sphere1(rx, ry, rz, 1);   
   }
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
   Print("Ambient=%d",ambient);
   ;
   if (light)
   {
      glWindowPos2i(5,45);
      Print("Model=%s Distance=%d Elevation=%.1f",smooth?"Smooth":"Flat",distance,ylight);
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",position[0],diffuse,specular,emission,shiny);
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
   // change light distance
   else if (key == GLUT_KEY_F3)
      distance = (distance==1) ? 8 : 1;
   //  Fly
   else if (key == GLUT_KEY_F1)
      dim = (fly = !fly) ? 30 : 12;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(fov,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   //  Update state
   timer(-1);
}


 /*  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;   
   //  control pitch
   else if (ch == 'i' || ch == 'I')
   {
	   yaw += plane_angle;
       Pitch(plane_angle);
   }
   //  control pitch down
   else if (ch == 'k' || ch == 'K')
   {
		 yaw -= plane_angle;
         Pitch(-plane_angle);
   }
   //  control roll left
   else if (ch == 'j' || ch == 'J')
   {
      Roll(-plane_angle); 
      roll += plane_angle;
   }
   //  control roll right
   else if (ch == 'l' || ch == 'L')
   {
      Roll(plane_angle); 
      roll -= plane_angle;
   }  
   //  control yaw left
   else if (ch == 'h' || ch == 'H')
   {
	 Yaw(-plane_angle);
     pitch -= plane_angle;
   }
   //  control yaw right
   else if (ch == ';' || ch == ':')
   {
	 Yaw(plane_angle);
     pitch += plane_angle;
   }  
   //speed up
   else if (ch == '8')
   {
	speed += .05;
   } 
   //slow down
   else if (ch == 'm' || ch == 'M')
   {
	 speed -= .05;
   } 
   //  Toggle axes
   else if (ch == 'q' || ch == 'Q')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == '/' || ch == '?')
      light = 1-light;
   //  Toggle light movement
   //else if (ch == 'm' || ch == 'M')
      //move = 1-move;
   //  change rotation angle
   else if (ch == 'w' || ch == 'W')
   {
       TY +=  0.01;
   }    
      //  change rotation angle
   else if (ch == 's' || ch == 'S')
   {
      if(TY<=0.5)
      {
	     TY=0.5;  
	  }
	  else
	  {
         TY -=  0.05;
      }
   }
   //  change rotation angle
   else if (ch == 'a' || ch == 'A')
   {
	  if(rot_angle>=360.0)
	  {
		  rot_angle = 0.0;
	  }
      rot_angle +=  1.0;

   }
   //  change rotation angle
   else if (ch == 'd' || ch == 'D')
   {
	  if(rot_angle<=0.0)
	  {
		  rot_angle = 360.0;
	  }
      rot_angle -=  1.0;

   }
   //  Move light
   else if (ch == '<')
      zh += 1;
   else if (ch == '>')
      zh -= 1;
 
   //  Toggle timer motion
   else if (ch == 'g' || ch == 'G')
      timer(1);
   //  Roll
   else if (ch == '4')
 	 roll -= 10;
   else if (ch == '5')
	 roll += 10;
   //  Pitch
   else if (ch == '`')
	 pitch -= 1;
   else if (ch == '2')
	 pitch += 1;
   //  Yaw
   else if (ch == '1')
	 yaw -= 1;
   else if (ch == '3')
	 yaw += 1;
   //  global perspecive fixed
   else if (ch == '6')
      pov = 1;
   //  view first person
   else if (ch == '7')
      pov = 2;
   //  view above plane
   else if (ch == '8')
      pov = 3;
   //  view global motion
   else if (ch == '9')
      pov = 4;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   //  toggle cock pit vs ship view
   else if (ch == 'c' || ch == 'C')
      ship = (ship+1)%2;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='v' && ambient>0)
      ambient -= 5;
   else if (ch=='V' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='b' && diffuse>0)
      diffuse -= 5;
   else if (ch=='B' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='n' && specular>0)
      specular -= 5;
   else if (ch=='N' && specular<100)
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
   //  Keep angles to +/-360 degrees
   spin %= 360;
   //  Translate shininess power to value (-1 => 0)
   shiny[0] = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(fov,asp,dim);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}


/**
 * mouseWheel
 * Event occurs when scroll wheel is spun.
 */
void mouseWheel(int button, int state, int x, int y) {
	if (toggleAltControls == 1) {
		boolAccelerate = 0;
		boolDeaccelerate = 0;
		if (state == GLUT_DOWN) {
			return;
		}
		if (button == 3) {
			if (planeSpeed <= planeMaxSpeed - planeAcc) {
				planeSpeed += planeAcc;
			}
		} else if (button == 4) {
			if (planeSpeed >= planeMinSpeed + planeDeacc) {
				planeSpeed -= planeDeacc;
			}
		}
	}
}
/**
 * mouseEvent
 * Gets the mouse position
 */
void mouseEvent(int x, int y) {
	lastMouseX = x;
	lastMouseY = y;
}

/**
 * keyUp
 * Is called when a normal key is released.
 * Used to toggle the various features on or off.
 */
void keyUp(unsigned char key, int mouseX, int mouseY) {
	if (key == 'w') {
		toggleWireframe = 1 - toggleWireframe;
		glPolygonMode( GL_BACK, renderingOptions[toggleWireframe]);
		glPolygonMode( GL_FRONT, renderingOptions[toggleWireframe]);
	}
	if (key == 'b') {
		toggleFog = 1 - toggleFog;
	}
	if (key == 'f') {
		toggleFullscreen = 1 - toggleFullscreen;
		//glutFullScreenToggle();
		//hackery because the windows freeglut doesn't have glutFullScreenToggle even though it's supposed to... ****
		if (toggleFullscreen == 1) {
			storedWidth = appWidth;
			storedHeight = appHeight;
			storedX = appX;
			storedY = appY;
			glutFullScreen();
		} else {
			glutReshapeWindow(storedWidth, storedHeight);
			glutPositionWindow(storedX, storedY);
			//have to do it twice because... ****ing freeglut/opengl
			glutReshapeWindow(storedWidth, storedHeight);
			glutPositionWindow(storedX, storedY);
		}
	}
	if (key == 's') {
		toggleGrid = 1 - toggleGrid;
	}
	if (key == 't') {
		toggleMountainTextures = 1 - toggleMountainTextures;
	}
	if (key == 'm') {
		toggleMountains = 1 - toggleMountains;
	}
	if (key == 'r') {
		//srand(time(NULL));

		//initNew();
		eyeX = 0;
		eyeY = 5;
		eyeZ = -15;
		atX = 0;
		atY = 5;
		atZ = -5;
		boolAccelerate = 0;
		boolDeaccelerate = 0;
		boolMoveUp = 0;
		boolMoveDown = 0;
		boolShoot = 0;
		planeSpeed = 1.0f;
		planeRotation = 0.0f;
		planeTilt = 0.0f;
		planeYawRotation = 0.0f;
		alive = 1;
	}
   if (key == 'g' || key == 'G')
      timer(1);
	if (key == 'q') {
		exit(0);
	}
}
/**
 * keySpecialDown
 * Is called when a special key is pressed.
 * Used to move the enterprise around (while not warping).
 */
void keySpecialDown(int key, int mouseX, int mouseY) {
	if (!toggleAltControls == 1) {
		if (key == GLUT_KEY_PAGE_UP) {
			boolAccelerate = 1;
		}
		if (key == GLUT_KEY_PAGE_DOWN) {
			boolDeaccelerate = 1;
		}
		if (key == GLUT_KEY_UP) {
			boolMoveUp = 1;
		}
		if (key == GLUT_KEY_DOWN) {
			boolMoveDown = 1;
		}
	}
	if (key == GLUT_KEY_F1) {
		toggleAltControls = 1 - toggleAltControls;
	}
	if (key == GLUT_KEY_F3)
      dim = (fly = !fly) ? 30 : 12;
	if (key == GLUT_KEY_F2) {
		toggleAltWeather = 1 - toggleAltWeather;
	}
}
/**
 * keySpecialUp
 * Is called when a special key is released.
 * Used to move the enterprise around (while not warping).
 */
void keySpecialUp(int key, int mouseX, int mouseY) {
	if (!toggleAltControls == 1) {
		if (key == GLUT_KEY_PAGE_UP) {
			boolAccelerate = 0;
		}
		if (key == GLUT_KEY_PAGE_DOWN) {
			boolDeaccelerate = 0;
		}
		if (key == GLUT_KEY_UP) {
			boolMoveUp = 0;
		}
		if (key == GLUT_KEY_DOWN) {
			boolMoveDown = 0;
		}

	}
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
   Project(fov,asp,dim);
}

void initGL(void) {

	glClearColor(0.0f, 0.0f, 0.2f, 1.0);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glMatrixMode(GL_PROJECTION);
	gluPerspective(fov, /* field of view in degree */
	(float) appWidth / (float) appHeight,/* aspect ratio */
	nearValue, farValue);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//(eyeX, eyeY, eyeZ, /* eye */
	//atX, atY, atZ, /* looking at*/
	//0.0, 1.0, 0.0);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
    float seed = time(NULL);
	//printf("Seed: %f\n", seed);

	srand(seed);
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(appWidth, appHeight);
   glutCreateWindow("Andrew Candelaresi Project Review");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   //glutPassiveMotionFunc(mouseEvent);

   //glutSpecialUpFunc(keySpecialUp);
   //glutSpecialFunc(keySpecialDown);

   //glutMouseFunc(mouseWheel);

   glutKeyboardUpFunc(keyUp);
   glutIdleFunc(idle);
   texture[0] = LoadTexBMP("duralloy1.bmp");
   sky[0] = LoadTexBMP("hoth1.bmp");
   sky[1] = LoadTexBMP("hothsky.bmp");
   sky[2] = LoadTexBMP("hoth3.bmp");
   sky[3] = LoadTexBMP("snow2.bmp");
   texture[1] = LoadTexBMP("cockpit.bmp");
   texture[2] = LoadTexBMP("falctex.bmp");
   texture[3] = LoadTexBMP("falctex2.bmp");
   texture[4] = LoadTexBMP("sideview.bmp");
   texture[5] = LoadTexBMP("frontview2.bmp");
   texture[6] = LoadTexBMP("pod2.bmp");
   texture[7] = LoadTexBMP("flighttunnel.bmp");
   //  Set timer
   timer(1);
   initGL();
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
