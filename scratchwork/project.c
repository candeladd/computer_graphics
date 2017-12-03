/*
 *  Set projection
 */
#include "CSCIx229.h"

void Project(int fp, double fov,double asp,double dim)
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if(fp) {
      gluPerspective(fov,asp,dim/4,4*dim);
   }
   else
   {
      if (fov)
         gluPerspective(fov,asp,dim/16,16*dim);
      //  Orthogonal transformation
      else
         glOrtho(-asp*dim,asp*dim,-dim,+dim,-dim,+dim);
   }
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

