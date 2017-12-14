#include "OGLFalconSim.h"
#include "Common.h"
#include <math.h>

#define PI 3.14159265358979323846
/*  D degrees of rotation */
#define DEF_D 5


/*
 *  Draw vertex in polar coordinates with normal
 */
void Vertex(double th,double ph)
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
	  x = Cos(k)*Cos(k);
	  y = Sin(k)*Sin(k);
	  double m = sqrt(x+y);
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
						  double deg_cir, double x_rot, double z_rot,
						  unsigned int texture)
{
   /* cylinder */
   /*  sides */
   glPushMatrix();
   glTranslated(x,y,z);
   glScaled(dx, dy, dz);
   glRotated(90, 1, 0, 0);
   glRotated(z_rot, 0, 1, 0);
   glBindTexture(GL_TEXTURE_2D, texture);
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
				   double x_rot,
				   unsigned int texture)
{

    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.1;
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture);
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
            glTexCoord2f(.085*x+.619,.11*y+.52);
            glVertex3f(x, y , height);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
    glEnd();
    
    glPopMatrix();
}

/*
 * draws the tunnel that connects the cockpit to the ship
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

/*
 * draws the tunnel that connects the cockpit to the ship
 * only has 2 rotations parst outside of the ship and escape pods
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
				   GLfloat height, unsigned int texture)
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
   //float yellow[] = {0.0,1.0,0.0,1.0};
   //float black[] = {0,0,0,1};

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //glMaterialf(GL_FRONT,GL_SHININESS,.5);
   //glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   //glMaterialfv(GL_FRONT,GL_EMISSION,black);
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
					GLfloat radius, double th, double sh, unsigned int texture)
{
	glPushMatrix();
	glNormal3f(0,0,1);
	int i;
	int lineAmount = 100; //# of triangles used to draw circle
	
    glLineWidth(8);


	glBindTexture(GL_TEXTURE_2D, texture);
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
   glBindTexture(GL_TEXTURE_2D, texture[2]);
   // reactor core
   glTranslated(x, y, z);
   glRotated(th, dx, 0,0);
   glBegin(GL_POLYGON);
   glNormal3f(0,0,-1);
   glColor3f(1,1,1);
   glTexCoord2f(.4,0.85); glVertex3d(0.125,-3.55,0.0);
   glTexCoord2f(0.5,.75); glVertex3d(0.125,-3.3,0.0);
   //outer most edge
   glTexCoord2f(.65,0.0); glVertex3d(5.243,-0.695,0.0);
   //glTexCoord2f(.6,.3); glVertex3d(5.977,-0.865,0.0);
   glTexCoord2f(0.1,.1); glVertex3d(6.365,-3.55,0.0);
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
