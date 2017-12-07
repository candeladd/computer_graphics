/*
 *  falcon code
 */
#include "CSCIx229.h"


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

void body_mid_sect(double x, double y, double z,
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

/* This function will draw the cylinder
 *
 *   @parameter1: radius = The radius of cylinder
 *   @parameter2: height = Height of the cylinder
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
					GLfloat radius, double th, double sh)
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
void fronts_piece(double x, double y, double z,
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
