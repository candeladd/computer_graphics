/*
 * OGLFalconSim.h
 * @author	: Andrew Candelaresi andrew.candelaresi@colorado.edu
 * CSCI 5229 Project: Falcon Sim
 */

#ifndef OGLFlightSim_H_
#define OGLFlightSim_H_
#include <stdio.h>
#include <math.h>
#include <tgmath.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <gl/Gl.h>
#include <gl/freeglut.h>
#define sleep(x) Sleep(1000 * x)
#elif linux
#include <string.h>
#include </usr/include/GL/gl.h>
#include </usr/include/GL/freeglut.h>
typedef unsigned char BYTE;
#endif



#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))


//Update logic methods
void update();

//Initialization methods
void init();
void initNew();
void initLight();
void initTextures();
void initSea();
void initSky();
void initGrid();
void initTarget();
void initAstroids();
void initGL(void);
void initializeGL(void);
void initGLUT(int argc, char** argv);
void handleResize(int w, int h);
void Print(const char* format , ...);
void Fatal(const char* format , ...);
unsigned int LoadTexBMP(const char* file);
void Project(double fov,double asp,double dim);
void ErrCheck(const char* where);
int  LoadOBJ(const char* file);




//free memory
void freeApp();

//Drawing methods
void draw();
void drawGrid();
void drawSea();

void drawProps();
void drawPlane();
void drawSky();
void drawTarget();
void drawAstroids();
void drawTarget();
void drawExplosion(int slices, int stacks);
void drawBullet(float x, float y, float z);
void drawAstroid(double x, double y, double z, double rr);


void colorAstroidByHeight(float x, float y, float z, float AstroidDetailAccuracy);

void explode();

//controls
void keyDown(unsigned char key, int mouseX, int mouseY);
void keyUp(unsigned char key, int mouseX, int mouseY);
void keySpecialDown(int key, int mouseX, int mouseY);
void keySpecialUp(int key, int mouseX, int mouseY);
void mouseEvent(int x, int y);
void mouseWheel(int button, int state, int x, int y);
void display(int ms);
void handleResize(int w, int h);

float randBetween(int min, int max);

//textue arrays
unsigned int texture[12];
unsigned int sky[5];

//falcon methods
void initFalcon();
void body_cone(double x, double y, double z, double dx, double dy, double dz, 
							 double deg_cir, double x_rot, 
							 double z_rot, unsigned int texture,
							 double spin_x);
							 void troid(double x,double y,double z,double r, unsigned int texture);
void body_mid_sect(double x, double y, double z,
						  double dx, double dy, double dz,
						  double deg_cir, double x_rot, double z_rot,
						  unsigned int texture, int o1, int o2);
void thruster(double x, double y, double z,
						  double dx, double dy, double dz,
						  double deg_cir, double x_rot, double z_rot,
						  unsigned int texture, int o1, int o2, int emission);
void draw_cylinder(GLfloat x, 
				   GLfloat y,
				   GLfloat z,
				   GLfloat radius,
				   GLfloat height,
				   double x_rot, 
				   unsigned int cap, unsigned int cyl);
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
				   unsigned int texture);
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
				   unsigned int texture);
				   				   				   
void sphere1(double x,double y,double z,double r);
void drawHollowCircle(GLfloat x, GLfloat y, 
                      double dx, double dy, double dz,
					  GLfloat radius, 
					  double th, double sh, double mag_x,
					double mag_y, double center_x, double center_y, unsigned int texture);
void drawHollowpitCircle(GLfloat x, GLfloat y, 
                         double dx, double dy, double dz,
					     GLfloat radius, 
					     double th, double sh, 
					     unsigned int texture);
void reactcore(double x, double y, double z,
				  double dx, double dy, double dz,
				  double th, unsigned int duralloy);
void corewalls(double x, double y, double z,
				  double dx, double dy, double dz,
				  double th);
void tunl_hull_conn(double x, double y, double z, 
				   double dx, double dy, double dz, 
				   unsigned int duralloy);
void tunnel(double x, double y, double z, 
		    double dx, double dy, double dz,
			double th, 
			double sh, double sx, double sy, double sz);
void fronts_piece(double x, double y, double z,
				  double dx, double dy, double dz,
				  double th);
void cockpit(unsigned int texture, unsigned int duralloy);
void  compile_falcon();
void Vertex(double th,double ph);
void ball(double x,double y,double z,double r);


GLUquadricObj *seaObj;
GLUquadricObj *skyObj;
GLUquadricObj *AstroidObj;
#endif /* OGLFalconSim */


