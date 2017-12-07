/*
 *  Thunderbirds
 *
 *  Draw complex objects from vertex list.
 *  Flight simulation demonstration.
 *
 *  Key bindings:
 *  l/L        Toggle lighting on/off
 *  f/F        Toggle flight/static mode
 *  KP 4/6     Roll (static mode)
 *  KP 2/8     Pitch (static mode)
 *  KP 1/3     Yaw (static mode)
 *  <>         Power (static mode)
 *  a          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"

/*  D degrees of rotation */
#define DEF_D 5
#define PI 3.14159265358979323846
int    fly=0;    //  Animated flight
int    axes=0;   //  Display axes
int    move=0;   //  Light movement
int    light=1;  //  Lighting
int    F16[3];   //  F16 display list
int inc = 10;    // Ball increment
int    th=0;     //  Azimuth of view angle
int    ph=0;     //  Elevation of view angle
int    zh=0;     //  Azimuth of light
double Yl=2;     //  Elevation of light
double roll=0;   //  Roll angle
double pitch=0;  //  Pitch angle
double yaw=0;    //  Yaw angle
int    pwr=100;  //  Power setting (%)
int    fov=55;   //  Field of view (for perspective)
double asp=1;    //  Aspect ratio
double dim=3.0;  //  Size of world
int    box=1;    //  Draw sky
int    sky[5];   //  Sky textures
double X  = 0;   //  Location
double Y  = 0;   //  Location
double Z  = 0;   //  Location
double Dx = 1;   //  Direction
double Dy = 0;   //  Direction
double Dz = 0;   //  Direction
double Sx = 1;   //  Sideways
double Sy = 0;   //  Sideways
double Sz = 0;   //  Sideways
double Ux = 1;   //  Up
double Uy = 0;   //  Up
double Uz = 0;   //  Up
double Ox = 0;   //  LookAt
double Oy = 0;   //  LookAt
double Oz = 0;   //  LookAt
double Ex = 1;   //  Eye
double Ey = 1;   //  Eye
double Ez = 1;   //  Eye
unsigned int texture[12];  // textures for ship

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
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_TRIANGLES);

    for (int k=0;k<=deg_cir;k+=DEF_D)
    {
	  x = Cos(k)*Cos(k);
	  y = Sin(k)*Sin(k);
	  double m = sqrt(x+y);
	  double nx = x/m;
	  double ny = y/m;
	  double hr = 1/1;
	  glNormal3d(nx*hr, ny*hr, hr); 
      // height
      glTexCoord2f(.5,1.0); glVertex3d(0,0,1);

      glTexCoord2f(0.0,0.0); glVertex3d(Cos(k),Sin(k),0);

      glTexCoord2f(1.0,0.0); glVertex3d(Cos(k+DEF_D),Sin(k+DEF_D),0);
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
				   GLfloat height,
				   double x_rot)
{

    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.1;
    glPushMatrix();
	glTranslatef(x, y, z);
	glRotated(x_rot, 1,0,0);
    /** Draw the tube */
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glNormal3f(cos(angle),sin(angle),0);
            glVertex3f(x, y , height);
            glVertex3f(x, y , 0.0);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
        glVertex3f(radius, 0.0, 0.0);
    glEnd();

    /** Draw the circle on top of cylinder */
    glBegin(GL_POLYGON);
    glNormal3f(0,0,1);
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


void static body_mid_sect(double x, double y, double z,
						  double dx, double dy, double dz,
						  double deg_cir, double x_rot, double z_rot)
{
   /* cylinder */
   /*  sides */
   glPushMatrix();
   glTranslated(x,y,z);
   glScaled(dx, dy, dz);
   glRotated(90, 1, 0, 0);
   glRotated(z_rot, 0, 1, 0);

   glBegin(GL_QUAD_STRIP);
   for (int j=0;j<=deg_cir;j+=DEF_D) {
	 glNormal3f(-Cos(j),0,-Sin(j));
     glVertex3d(Cos(j),+1,Sin(j));
     glNormal3f(-Cos(j),0,-Sin(j));
     glVertex3d(Cos(j),-1,Sin(j));
   }
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
				   double stacks)
{


    glPushMatrix();
    glTranslatef(x, y, z);
    glRotated(th, dx, 0, 0);
    glRotated(sh, 0, dy, 0);
    GLUquadricObj *quadObj = gluNewQuadric();
    gluQuadricTexture(quadObj, GLU_TRUE); 
    gluCylinder(quadObj, base, top, height, slices, stacks);
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
					GLfloat radius, double th, double sh){
	glPushMatrix();
	glTranslated(dx, dy, dz);
	glRotated(th,1,0,0);
	glRotated(sh,0,1,0);  
	glNormal3f(0,0,1);
	int i;
	int lineAmount = 100; //# of triangles used to draw circle
	
	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * PI;
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_POLYGON);
		for(i = 0; i <= lineAmount;i++) { 
			glTexCoord2f(0.0,0.0);  glVertex2f(
			    x + (radius * cos(i *  twicePi / lineAmount)), 
			    y + (radius* sin(i * twicePi / lineAmount))
			);
		}
	glEnd();
	glPopMatrix();
}

void drawHollowpitCircle(GLfloat x, GLfloat y, double dx, double dy, double dz,
					GLfloat radius, double th, double sh){
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

void static reactcore(double x, double y, double z,
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

void static corewalls(double x, double y, double z,
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
void static tunl_hull_conn(double x, double y, double z, 
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
			   				   
void static tunnel(double x, double y, double z, 
				   double dx, double dy, double dz,
				   double th, double sh)
{
   glPushMatrix();
   // tunnel top piece
   glTranslated(x, y, z);
   glRotated(th, dx, 0,0);
   glRotated(sh, 0, dy,dz);
   glBindTexture(GL_TEXTURE_2D, texture[0]);
   glBegin(GL_POLYGON);
   glNormal3f(0,0,1);
   glTexCoord2f(0.0,0.0); glVertex3d(6.669, -2.85,1.0);
   glTexCoord2f(0.0,1.0); glVertex3d(7.669, -2.85,1.0);
   glTexCoord2f(1.0,1.0); glVertex3d(7.669, -1,1.0);
   glTexCoord2f(1.0,1.0); glVertex3d(6.669, -1,1.0);
   glEnd();
   // tunnel side 1 piece
   glBegin(GL_POLYGON);
   glNormal3f(0.92796046,  0.01958756, -0.37216356);
   glTexCoord2f(0.0,0.0); glVertex3d(6.669, -2.85,1.0);
   glTexCoord2f(0.0,1.0); glVertex3d(6.269, -2.9,0.0);
   glTexCoord2f(1.0,1.0); glVertex3d(6.269, -1,0.1);
   glTexCoord2f(1.0,0.0); glVertex3d(6.669, -1,1.0);
   glEnd();
   // tunnel side 2 piece
   glBegin(GL_POLYGON);
   glNormal3f(0.92796046, -0.01958756,  0.37216356);
   glTexCoord2f(0.0,0.0); glVertex3d(7.669, -2.85,1.0);
   glTexCoord2f(0.0,1.0); glVertex3d(8.069, -2.9,0.0);
   glTexCoord2f(1.0,1.0); glVertex3d(8.069, -1,0.1);
   glTexCoord2f(1.0,0.0); glVertex3d(7.669, -1,1.0);
   glEnd();
   // tunnel interieor ship piece
   glBegin(GL_POLYGON);
   glNormal3f( 0.0, 0.99875234, -0.04993762);
   glTexCoord2f(0.0,0.0); glVertex3d(6.669, -2.85,1.0);
   glTexCoord2f(0.0,1.0); glVertex3d(7.669, -2.85,1.0);
   glTexCoord2f(1.0,1.0); glVertex3d(8.069, -2.9,0.0);
   glTexCoord2f(1.0,0.0); glVertex3d(6.269, -2.9,0.0);
   glEnd();
   // tunnel escape pod piece
   glBegin(GL_POLYGON);
   glNormal3f(0,-1,0);
   glTexCoord2f(0.0,0.0); glVertex3d(7.669, -1,1.0);
   glTexCoord2f(0.0,1.0); glVertex3d(6.669, -1,1.0);
   glTexCoord2f(1.0,1.0); glVertex3d(6.269, -1,0.1);
   glTexCoord2f(1.0,0.0); glVertex3d(8.069, -1,0.1);
   glEnd();
   glBegin(GL_POLYGON);
   glNormal3f(0,-1,0);
   glTexCoord2f(0.0,0.0); glVertex3d(6.269, -1,0.1);
   glTexCoord2f(0.0,1.0); glVertex3d(6.269, -1,-0.5);
   glTexCoord2f(1.0,1.0); glVertex3d(8.069, -1,-0.5);
   glTexCoord2f(1.0,0.0); glVertex3d(8.069, -1,0.1);
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
   glBindTexture(GL_TEXTURE_2D, texture[0]);
   glBegin(GL_POLYGON);
   glNormal3f( 0.23162053, 0, -0.97280621);

   glTexCoord2f(0.0,0.0); glVertex3d(2.0,-3.65,0.8);
   glTexCoord2f(0.0,1.0); glVertex3d(5.9,-3.55,1.1);
   glTexCoord2f(1.0,1.0); glVertex3d(5.9,-4.78,1.1);
   glTexCoord2f(1.0,0.0); glVertex3d(2.0,-4.68,0.8);
   glEnd();	
   //up slant pieces
   glBegin(GL_POLYGON);
   glNormal3f(-0.04661261, 0.95678517, 0.28703555);

   glTexCoord2f(0.0,0.0); glVertex3d(2.0,-3.65,0.8);
   glTexCoord2f(0.0,1.0); glVertex3d(5.9,-3.55,1.1);
   glTexCoord2f(1.0,1.0); glVertex3d(5.9,-3.25,0.1);
   glTexCoord2f(1.0,0.0); glVertex3d(1.7,-3.35,0.1);
   glEnd();	
   //down slant pieceside
   glBegin(GL_POLYGON);
   glNormal3f(0.07268645, 0.69821961, -0.712184);

   glTexCoord2f(0.0,0.0); glVertex3d(2.0,-4.68,0.8);
   glTexCoord2f(0.0,1.0); glVertex3d(5.9,-4.78,1.1);
   glTexCoord2f(1.0,1.0); glVertex3d(5.9,-5.8,0.1);
   glTexCoord2f(1.0,0.0); glVertex3d(1.7,-4.98,0.1);
   glEnd();	
   // front slant pieces
   glBegin(GL_POLYGON);
   glNormal3f(0.91914503, 0, -0.3939193);

   glTexCoord2f(0.0,0.0); glVertex3d(2.0,-3.75,0.8);
   glTexCoord2f(0.0,1.0); glVertex3d(2.0,-4.68,0.8);
   glTexCoord2f(1.0,1.0); glVertex3d(1.7,-4.98,0.1);
   glTexCoord2f(1.0,0.0); glVertex3d(1.7,-3.35,0.1);
   glEnd();	
   // back
   glBegin(GL_POLYGON);
   glNormal3f(-1,0,0);

   glTexCoord2f(0.0,0.0); glVertex3d(5.9,-3.55,1.1);
   glTexCoord2f(0.0,1.0); glVertex3d(5.9,-3.25,0.1);
   glTexCoord2f(1.0,1.0); glVertex3d(5.9,-5.8,0.1);
   glTexCoord2f(1.0,0.0); glVertex3d(5.9,-4.78,1.1);
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
void static cockpit()
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
void static compile_falcon()
{
	double x = 10;
	double y = 0;
	double z = 2;
	double dx = 1;
	double dy = 1;
    double dz = 1;
	double th = 270;
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   //glRotated(sh,0,1,0);

   glRotated(th,1,0,0);
   glRotated(180,0,0,1);
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
   body_cone(6.669,-4.2,-1, 3.8, 3.8, 1,130,180,293);
   body_cone(6.669,-4.2,-1, 3.8, 3.8, 1,165.009,180,96.3);

   //glColor3f(.6,.6,1);
   glColor3f(1,1,1);
   //hull of the ship close back
   body_cone(6.669,-4.2,0, 3.8, 3.8, 1,130,0,293);
   //close front
   body_cone(6.669,-4.2,0, 3.8, 3.8, 1,165,0,94.5);
   // extra body cap
   glColor3f(1,1,1);
   body_cone(6.669,-4.2,0.495, 2, 2, .5,360,0,0);
   body_cone(6.669,-4.2,-1.495, 2, 2, .5,360,180,0);

   // cap done we will have to send the functions the normals
   draw_cylinder(6.869,-3.975, -1.5, 1, .5,180);
   draw_cylinder(6.869,-3.975, 0.5, 1, .5,0);
   //cirular body of ship
   glColor3f(1,1,1);
   body_mid_sect(6.669, -4.2, -.5,3.8,3.8,.5,135,90,67);
   body_mid_sect(6.669, -4.2, -.5,3.8,3.8,.5,168,90,261);

   
   
   
   fronts_piece(0,0,0,0,0,0,0);
   fronts_piece(0,-8.5,-1,1,0,0,180);
   //one side
   //top
   tunnel(0,0,0,0,0,0,0,0);
   //bot
   tunnel(14.33,0,-1,1,0,1,180, 180);
   //other side
   tunnel(0,-8.35,-1,1,0,0,180,0);
   tunnel(14.33,-8.35,0,1,1,0,180,180);
   //tunnel

   // cock pit tunnel inside ship
   cock_pit_body(4.75, -0.2, 0.5, 1.0, 0.0, 1.0, 48.0, 25.0, 0.8, 0.8, 3.0, 50.0, 10.0);
   
   glColor3f(1,1,1);
   cock_pit_body(5.675, -3.01, 0.34, 1.0, 0.0, 1.0, 48.0, 25.0, 0.8, 0.3, .5, 50.0, 10.0);
   
   glColor3f(1.0,1.0,1.0);
   // cockpit tunnel out side shipt
   cock_pit_body2(3.299, -0.28, 0.5, 1, 1, 0, 90, 90, 0.8, 0.8, 1.5, 50, 10);
   glColor3f(1,1,1);
   //cock pit cone
   cock_pit_body2(1.789, -0.2799, 0.5, 1, 1, 0, 90, 90, 0.5, 0.8, 1.5, 50, 10);
   glColor3f(1,1,1);
   // join cock pit sections
   sphere1(4.779,-0.28,0.5,0.8);
   glColor3f(1,1,1);
   // escape pods
   cock_pit_body2(7.15,-.5,-.5,1,0,0,90,0, .7, .8, 1, 50, 10);
   cock_pit_body2(7.15,-7,-.5,1,0,0,90,0, .8, .7, 1, 50, 10);
   drawHollowCircle(0,0, 7.15, -.5, -.5, .7,90,0);
   drawHollowCircle(0,0, 7.15, -8, -.5, .7,90,0);
   drawHollowCircle(0,0,1.79,-.28,.5,.5,0,90);
   tunl_hull_conn(0,0,0,0,0,0, texture[0]);

   
   //gluCylinder(x, y, z, dx, dy, dz, th, sh, base, top, height, slices, stacks);
   glDisable(GL_TEXTURE_2D);
   glPopMatrix();
}


/* 
 *  Draw sky box
 */
static void Sky(double D)
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

/*
 *  Draw Flight
 */
static void DrawFlight(double x0,double y0,double z0,
                       double dx,double dy,double dz,
                       double ux,double uy,double uz)
{
   int i =1;
   //  Position of members
   double X[] = {-1.0,+1.0,+0.0,+0.0};
   double Y[] = {-0.3,+0.3,+0.0,+0.0};
   double Z[] = {+0.0,+0.0,-1.5,+1.5};
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
   double M[16] = {X0,Y0,Z0,0 , X1,Y1,Z1,0 , X2,Y2,Z2,0 , 0,0,0,1};

   //  Save current transforms
   glPushMatrix();
   //  Offset and rotate
   glTranslated(x0,y0,z0);
   glMultMatrixd(M);


   //  Draw
   glPushMatrix();
   glTranslated(X[0],Y[0],Z[0]);
   glRotated(yaw,0,1,0);
   glRotated(pitch,0,0,1);
   glRotated(roll,1,0,0);
   //float power[] = {0.01*pwr,0.01*pwr,0.01*pwr,1};
   //glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,power);
   compile_falcon();
   glPopMatrix();

   //  Undo transformations
   glPopMatrix();
}



/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius r
 */
static void ball(double x,double y,double z,double r)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,0);
   glutSolidSphere(1.0,16,16);
   //  Undo transofrmations
   glPopMatrix();
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=2.5;  //  Length of axes
   //  Erase the window and the depth buffer
   glClearColor(0,0.3,0.7,0);
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //glEnable(GL_CULL_FACE);

   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   gluLookAt(Ex,Ey,Ez , Ox,Oy,Oz , Ux,Uy,Uz);

   //  Draw sky
   if (box) Sky(3.5*dim);

   //  Light switch
   if (light)
   {
      //  Translate intensity to color vectors
      float F = (light==2) ? 1 : 0.3;
      float Ambient[]   = {0.3*F,0.3*F,0.3*F,1};
      float Diffuse[]   = {0.5*F,0.5*F,0.5*F,1};
      float Specular[]  = {1.0*F,1.0*F,1.0*F,1};
      float white[]     = {1,1,1,1};
      //  Light direction
      float Position[]  = {5*Cos(zh),Yl,5*Sin(zh),1};
      //  Draw light position as ball (still no lighting here)
      ball(Position[0],Position[1],Position[2] , 0.1);
      //  Enable lighting with normalization
      glEnable(GL_LIGHTING);
      glEnable(GL_NORMALIZE);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
      glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,32.0f);
      glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   }
   else
      glDisable(GL_LIGHTING);

   //  Draw flight of F16s
   compile_falcon();
   
   //DrawFlight(X,Y,Z , Dx,Dy,Dz , Ux,Uy,Uz);

   //  Draw axes
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
   Print("Angle=%d,%d  Dim=%.1f  Roll %.1f Pitch %.1f Yaw %.1f",th,ph,dim,roll,pitch,yaw);
   if (light) Print(" Light=%d,%.1f",zh,Yl);
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

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
      //  Lorenz integration parameters
      double dt = 0.003;
      double s = -1.7;
      double b = 2.66;
      double r = 50;
      //  Old vectors
      double D,Nx,Ny,Nz;
      double Dx0 = Dx;
      double Dy0 = Dy;
      double Dz0 = Dz;
      double Ux0 = Ux;
      double Uy0 = Uy;
      double Uz0 = Uz;
      //  Fix degenerate case
      if (X==0 && Y==0 && Z==0) Y = Z = 40;
      //  Update position
      Dx = s*(Y-X);
      Dy = X*(r-Z)-Y;
      Dz = X*Y - b*Z;
      X += dt*Dx;
      Y += dt*Dy;
      Z += dt*Dz;
      //  Normalize DX
      D = sqrt(Dx*Dx+Dy*Dy+Dz*Dz);
      Dx /= D;
      Dy /= D;
      Dz /= D;
      //  Calculate sideways
      Sx  = Dy0*Dz-Dz0*Dy;
      Sy  = Dz0*Dx-Dx0*Dz;
      Sz  = Dx0*Dy-Dy0*Dx;
      //  Calculate Up
      Ux  = Dz*Sy - Dy*Sz;
      Uy  = Dx*Sz - Dz*Sx;
      Uz  = Dy*Sx - Dx*Sy;
      //  Normalize Up
      D = sqrt(Ux*Ux+Uy*Uy+Uz*Uz);
      Ux /= D;
      Uy /= D;
      Uz /= D;
      //  Eye and lookat position
      Ex = X-7*Dx;
      Ey = Y-7*Dy;
      Ez = Z-7*Dz;
      Ox = X;
      Oy = Y;
      Oz = Z;
      //  Next DX
      Nx = s*(Y-X);
      Ny = X*(r-Z)-Y;
      Nz = X*Y - b*Z;
      //  Pitch angle
      //pitch = 180*acos(Dx*Dx0+Dy*Dy0+Dz*Dz0);
      //  Roll angle
      D = (Ux*Ux0+Uy*Uy0+Uz*Uz0) / (Dx*Dx0+Dy*Dy0+Dz*Dz0);
      if (D>1) D = 1;
      roll = (Nx*Sx+Ny*Sy+Nz*Sz>0?+1:-1)*960*acos(D);
      //  Yaw angle
      yaw = 0;
      //  Power setting (0-1)
      if (Dy>0.8)
         pwr = 100;
      else if (Dy>-0.2)
	 pwr = 20+100*Dy;
      else
	 pwr = 0;
   }
   //  Static Roll/Pitch/Yaw
   else
   {
      Ex = -2*dim*Sin(th)*Cos(ph);
      Ey = +2*dim        *Sin(ph);
      Ez = +2*dim*Cos(th)*Cos(ph);
      Ox = Oy = Oz = 0;
      X = Y = Z = 0;
      Dx = 1; Dy = 0; Dz = 0;
      Ux = 0; Uy = 1; Uz = 0;
   }
   //  Set timer to go again
   if (move && toggle>=0) glutTimerFunc(50,timer,0);
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
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(fov,asp,dim);
   //  Update state
   timer(-1);
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
      roll = pitch = yaw = th = ph = 0;
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
   //  Fly
   else if (ch == 'f' || ch == 'F')
      dim = (fly = !fly) ? 30 : 3;
   //  Toggle skybox
   else if (ch == 'b' || ch == 'B')
      box = 1-box;
   //  Cycle light
   else if (ch == 'l')
      light = (light+1)%3;
   else if (ch == 'L')
      light = (light+2)%3;
   //  Toggle light movement
   else if (ch == 's' || ch == 'S')
      timer(1);
   //  Roll
   else if (ch == '4')
      roll -= 10;
   else if (ch == '6')
      roll += 10;
   //  Pitch
   else if (ch == '8')
      pitch -= 1;
   else if (ch == '2')
      pitch += 1;
   //  Yaw
   else if (ch == '1')
      yaw -= 1;
   else if (ch == '3')
      yaw += 1;
   //  Power
   else if (ch=='<' && pwr>0)
      pwr -= 10;
   else if (ch=='>' && pwr<100)
      pwr += 10;
   //  Light azimuth
   else if (ch=='[')
      zh -= 1;
   else if (ch==']')
      zh += 1;
   //  Light elevation
   else if (ch=='-')
      Yl -= 0.1;
   else if (ch=='+')
      Yl += 0.1;
   //  Reproject
   Project(fov,asp,dim);
   //  Update state
   timer(-1);
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

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("F16 Thunderbirds");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   //  Load skybox texture
   texture[0] = LoadTexBMP("duralloy1.bmp");
   sky[0] = LoadTexBMP("hoth1.bmp");
   sky[1] = LoadTexBMP("hothsky.bmp");
   sky[2] = LoadTexBMP("hoth3.bmp");
   sky[3] = LoadTexBMP("snow2.bmp");
   texture[1] = LoadTexBMP("cockpit.bmp");
   //  Set timer
   timer(1);
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
