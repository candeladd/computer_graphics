
#include <stdio.h>
#include <math.h>
#include <tgmath.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

double astroidcoords[50][3];

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
int main()
{
   srand(time(NULL));
   double x1 = 0;
   double y1 = 0;
   double z1 = 0;
   double* collision_zone;
	int numAstroidss = 50;
	//generate a certain number of Astroids
	for (int i = 0; i < numAstroidss; i++) {
		
		astroidcoords[i][0] = randBetween(0,100);
	    astroidcoords[i][1] = randBetween(0,100);
	    astroidcoords[i][2] = randBetween(0,100);
	    astroidcoords[i][3] = randBetween(0,10);
	 }
	 

    for (int i = 0; i<=50; i++)
   {
	   collision_zone = &astroidcoords[i][3];
	   double*  x = &astroidcoords[i][0];
	   double* y = &astroidcoords[i][1];
	   double* z = &astroidcoords[i][2];
	   	printf("{%f, %f, %f}", *x, *y, *z);
	   double distance = sqrt((pow((*x)-x1,2))+(pow((*y)-y1,2))+(pow((*z)-z1,2)));
	   if (distance<=*collision_zone)
	   {
		 return 1;  
	   }
	   printf("\ndistance  =  %f\n", distance);
   }

}
