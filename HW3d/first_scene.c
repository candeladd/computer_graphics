/*
 *  Projections
 *
 *  Draw 27 cubes to demonstrate orthogonal & prespective projections
 *
 *  Key bindings:
 *  m          Toggle between perspective and orthogonal
 *  +/-        Changes field of view for perspective
 *  a          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
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
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
double zh=0;      //  Rotation of teapot
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=5.0;   //  Size of world

//  Macro for sin & cos in degrees
#define Cos(th) cos(3.1415927/180*(th))
#define Sin(th) sin(3.1415927/180*(th))

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
 *  Draw solid airplane
 *    at (x,y,z)
 *    nose towards (dx,dy,dz)
 *    up towards (ux,uy,uz)
 */
static void Solidhouse(double x,double y,double z,
                       double dx,double dy,double dz,
                       double ux,double uy, double uz)
{
   // Dimensions used to size house
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
   glVertex3d(.4,-0.2,-.501);
   glVertex3d(.4,0.2,-.501);
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
   glEnd();
   // undo transformations
   glPopMatrix();
}

/*
 *  Set projection
 */
static void Project()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if (mode)
      gluPerspective(fov,asp,dim/4,4*dim);
   //  Orthogonal projection
   else
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(1,0,0);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glColor3f(0,0,1);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glColor3f(1,1,0);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glColor3f(0,1,0);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glColor3f(0,1,1);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glColor3f(1,0,1);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transofrmations
   glPopMatrix();
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=1.5;  //  Length of axes
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
   //  draw houses
   Solidhouse(Cos(zh),Sin(zh), 0 ,-Sin(zh),Cos(zh),0 , Cos(4*zh),0,Sin(4*zh));
   Solidhouse(-1,Sin(zh), 0 ,-Sin(zh),Cos(zh),0 , Cos(180),0,Sin(4*zh));
   Solidhouse(-1,Sin(zh), 2 ,-Sin(zh),Cos(zh),0 , Cos(zh),0,Sin(90));
   //grass
   glBegin(GL_QUADS);
   glColor3f(0,1,0);
   glVertex3d(3,0,3);
   glVertex3d(-3,0,3);
   glVertex3d(-3,0,-3);
   glVertex3d(3,0,-3);
   glEnd();
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
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
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
   glutCreateWindow("Projections");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
