#include "CSCIx229.h"


/*
 * draw the millenium falcon
*/
static void falcon(double x, double y, double z, 
				   doubld dx, double dy, double dz,
				    double th)
{
  //  Save transformation
  glPushMatrix();
  // offset, scale and rotate
  glTranslated(x, y, z);
  glrotated(th, 0,1,0);
  glScaled(dx, dy, dz);
  glBegin(GL_TRIANGLES);
  glVertex3d();
  
  
}
