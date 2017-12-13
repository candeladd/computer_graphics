/**
 * @author	: Andrew Candelaresi
 * Flight code, bullets, and random generator from Matthew Thomas
 * Code borrowed from course examples in CSCI 5229
 * CSCI 5229 Project: Millenium Falcon Sim
 * Creates an area to fly around with in a Millenium falcon sim.
 * Generates 3 randomly generated islands.
 * Astroids used midpoints and some "jitter" to make the edges not always straight up
 * Generates normals for the Astroids per vertex.
 * w - toggles between frame and solid rendering
 * f - toggles between fullscreen and windowed mode
 * b - toggle fog on or off
 * s - toggle between grid and sea+sky
 * m - toggles Astroids on and off
 * t - toggles textures for Astroids on and off
 * q - quit
 *
 * Standard controls:
 * 	Page Up - Increase Speed
 * 	Page Down - Decrease Speed
 * 	Up - Raise elevation of plane
 * 	Down - Lower elevation of plane
 * 	mouse on left side of screen - Turn plane to the left
 * 	mouse on right side of screen - Turn plane to the right
 *
 * Bonuses:
 * 	f1 - toggle between standard and alternate controls
 * 	f2 - toggle between standard fog and cloud-like fog
 * 	z - shoot bullets
 * 	Spedometer (Shows speed roughly, green bar bottom left)
 * 	Altometer (Shows altitude roughly, blue bar bottom right)
 * 	Crash into the sea and there is an explosion + plane dies
 * 	r - respawn and generate new random world
 *	crosshair
 *
 * Alternate controls:
 *  mouse on top of screen - Tilt plane and camera up
 *  mouse on bottom of screen - Title plane and camera down
 *  scroll wheel - increase or decrease speed
 */

#include "OGLFalconSim.h"
#include "Common.h"

#define PI 3.14159265358979323846q
/*  D degrees of rotation */
#define DEF_D 5
GLfloat renderingOptions[] = { GL_FILL, GL_LINE };
int inc2       =  10;  // Ball increment
double col_ex[1][3];


int check_colision(double x1,double y1,double z1)
{
   double* collision_zone;
   
   for (int i = 0; i<=50; i++)
   {
	   collision_zone = &astroidcoords[i][3];
	   double* x = &astroidcoords[i][0];
	   double* y = &astroidcoords[i][1];
	   double* z = &astroidcoords[i][2];

	   double distance = sqrt((pow((*x)-x1,2))+(pow((*y)-y1,2))+(pow((*z)-z1,2)));
	   if (distance<=*collision_zone-1)
	   {
		 return 1;  
	   }
   }
   
   return 0;

}

/*
 * draws the cockpit view for the millenium falcon
 * 
 */

void cockpit(unsigned int texture, unsigned int duralloy)
{
   glEnable(GL_COLOR_MATERIAL);
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
   glBindTexture(GL_TEXTURE_2D, texture);
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
   drawHollowpitCircle(0.0,-.1,0,0,0,1,0,0, duralloy);
   drawHollowpitCircle(0.0,-.1,0,0,0,1.01,0,0, duralloy);
   drawHollowpitCircle(0.0,-.1,0,0,0,1.02,0,0, duralloy);
   drawHollowpitCircle(0.0,-.1,0,0,0,.99,0,0, duralloy);
   drawHollowpitCircle(0.0,-.1,0,0,0,1.45,0,0, duralloy);
   drawHollowpitCircle(0,-.1,0,0,0,1.46,0,0, duralloy);
   drawHollowpitCircle(0,-.1,0,0,0,1.47,0,0, duralloy);

   //  Reset model view matrix
   glPopMatrix();
   //  Reset projection matrix
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   //  Pop transform attributes (Matrix Mode and Enabled Modes)
   glPopAttrib();
   glDisable(GL_COLOR_MATERIAL);
	
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

void compile_falcon()
{
	float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
	float Specular2[]  = {0.001*specular,0.001*specular,0.01*specular,1.0};
	//glCallList(falconId);
	double x = -12;
	double y = 0;
	double z = -4;
	double dx = 1;
	double dy = 1;
    double dz = 1;
	glEnable(GL_COLOR_MATERIAL);
    glPushMatrix();
   
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(270,1,0,0);
   glRotated(0,0,0,1);

   //glRotated(sh,0,1,0);


   glScaled(dx,dy,dz);
   //should make metal shine
   float white[] = {1,1,1,1};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT,GL_EMISSION,black);
   
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
   body_cone(6.669,-4.2,-1, 3.8, 3.8, 1,130,180,293, texture[3],0);
   body_cone(6.669,-4.2,-1, 3.8, 3.8, 1,165.009,180,96.3, texture[3],0);

   //glColor3f(.6,.6,1);
   glColor3f(1,1,1);
   //hull of the ship close back
   body_cone(6.669,-4.2,0, 3.8, 3.8, 1,130,0,293, texture[3], 65);
   //close front
   body_cone(6.669,-4.2,0, 3.8, 3.8, 1,165,0,94.5, texture[3],0);
   // extra body cap
   glColor3f(1,1,1);
   //body_cone(6.669,-4.2,0.495, 2, 2, .5,360,0,0);
   //body_cone(6.669,-4.2,-1.495, 2, 2, .5,360,180,0);

   // gun cap otherside body done we will have to send the functions the normals
   draw_cylinder(6.869,-3.975, -1.5, 1, .5,180, texture[3], texture[7]);
   draw_cylinder(6.869,-3.975, 0.5, 1, .48,0, texture[3], texture[7]);

   //cirular body of ship
   glColor3f(1,1,1);
   glEnable(GL_POLYGON_OFFSET_FILL);

   body_mid_sect(6.669, -4.2, -.5,3.8,3.8,.5,135,90,67, texture[2],2,2);
   body_mid_sect(6.669, -4.2, -.5,3.8,3.8,.5,168,90,261, texture[2], 0,0);

   //blue thingy
   glLightfv(GL_LIGHT0,GL_SPECULAR,Specular2);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   thruster(6.669, -4.2, -.5,3.8,3.8,.4,90,90,47, texture[0],1,1, emission);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
   glColor3d(1,1,1);

   // bar across it
   body_mid_sect(6.669, -4.2, -.5,3.8,3.8,.1,90,90,47, texture[0],0,0);
   glDisable(GL_POLYGON_OFFSET_FILL);
   
   
   fronts_piece(0,0,0,0,0,0,0);
   fronts_piece(0,-8.5,-1,1,0,0,180);
   //one side
   //top
   tunnel(0,0,0,0,0,0,0,0,1,1.05,1);
   //bot
   tunnel(14.33,0,-1,1,0,1,180, 180,1,1.05,1);
   //other side
   tunnel(0,-9.1,-1,1,0,0,180,0,1,1.45,1);
   tunnel(14.33,-9.1,0,1,1,0,180,180,1,1.45,1);
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
   cock_pit_body2(1.789, -0.2799, 0.5, 1, 1, 0, 90, 90, 0.5, 0.8, 1.5, 50, 10, texture[7]);
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
   	glDisable(GL_COLOR_MATERIAL);
}


/**
 * main
 * The main method initially run. Creates the context of the application and begins the display loop.
 */
int main(int argc, char** argv) {
	init(argc, argv);
	glutMainLoop();
	return 0;
}
/**
 * init
 * Initializes all of the components for the application.
 * 
 */
void init(int argc, char** argv) {
	 //current calendar time (seconds since Jan 1, 1970)
	 //seed for random number generator in raise astroid
	float seed = time(NULL);
	//printf("Seed: %f\n", seed);

	srand(seed);
	initGLUT(argc, argv);
	initNew();
	initGL();
	ErrCheck("init");
}
/**
 * initNew
 * Initializes a new world
 */
void initNew() {
	numBullets = 0;
	maxNumBullets = 100;
	currentBullet = 0;
	targetScale = 1;
	firstBullet = NULL;
	currBullet = NULL;
	initTextures();
	initSea();
	initSky();
	initTarget();
	initGrid();
	initAstroids();
	initLight();
    ErrCheck("initnew");

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
   for (ph=-90;ph<90;ph+=inc2)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc2)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc2);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}


/**
 * initLight
 * Initializes the world's light.
 */
void initLight() {
	
	//  Translate intensity to color vectors
	float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
	float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
	float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
	//  Light position
	float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),20.0};
	//  Draw light position as ball (still no lighting here)
	glColor3f(1,1,1);
	ball(Position[0],Position[1],20.0 , 0.1);
	//  OpenGL should normalize normal vectors
	glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
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
	/*
	float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
    //ambientLight0   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
	// Set lighting values
	glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition0);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);

	// Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat diffuseMaterial[4] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat ambientMaterial[4] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat specularMaterial[4] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat emissiveMaterial[4] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mShininess[] = { 1 };
	//  function specifies material parameters for the lighting model.
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mShininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emissiveMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMaterial);
    */	

}

/**
 * colorAstroidByHeight
 * Colors a Astroid vertex based on its height.
 */
void colorAstroidByHeight(float x, float y, float z,
		float AstroidDetailAccuracy) {

	GLfloat diffuseMaterial[] = { 0.0, 0.0, 0.0, 1.0 };
	if (y < 7) {
		diffuseMaterial[0] = darkgreen[0];
		diffuseMaterial[1] = darkgreen[1];
		diffuseMaterial[2] = darkgreen[2];
		diffuseMaterial[3] = darkgreen[3];
	} else if (y < 15) {
		diffuseMaterial[0] = lightgreen[0];
		diffuseMaterial[1] = lightgreen[1];
		diffuseMaterial[2] = lightgreen[2];
		diffuseMaterial[3] = lightgreen[3];
	} else {
		diffuseMaterial[0] = white[0];
		diffuseMaterial[1] = white[1];
		diffuseMaterial[2] = white[2];
		diffuseMaterial[3] = white[3];
	}
	glColor4f(diffuseMaterial[0], diffuseMaterial[1], diffuseMaterial[2],
			diffuseMaterial[3]);
}

/**
 * drawAstroid
 * Draws a randomly generated Astroid
 */
void drawAstroid(double x, double y, double z, double rr) {
   	troid(x, y, z, rr, AstroidTexture);  

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

/**
 * initAstroids
 * Initializes the Astroids including position and scale
 */
void initAstroids() {
	int mapSize = (int) AstroidDetailAccuracy;
	AstroidId = glGenLists(1);
	glNewList(AstroidId, GL_COMPILE);

	glBindTexture(GL_TEXTURE_2D, AstroidTexture);

	glColor4f(1, 1, 1, 1);
	int i;
	astroidcoords[0][0] = 20.0;
	astroidcoords[0][1] = 20.0;
	astroidcoords[0][2] = 20.0;
	astroidcoords[0][3] = 1.0;
	//drawAstroid(astroidcoords[0][0],astroidcoords[0][1],astroidcoords[0][2], astroidcoords[0][3]);
	int numAstroidss = 35;
	//generate a certain number of Astroids
	for (i = 0; i < numAstroidss; i++) {
		
		//astroidcoords[i][0] = randBetween(0,mapSize);
	    //astroidcoords[i][1] = randBetween(0,mapSize);
	    //astroidcoords[i][2] = randBetween(0,mapSize);
	    //astroidcoords[i][3] = randBetween(0,10);
	     astroidcoords[i][0] = i+randBetween(0,30);
	     astroidcoords[i][1] = i +randBetween(0,30);;
	     astroidcoords[i][2] = i+randBetween(0,30);
	     astroidcoords[i][3] = randBetween(0,10);;
		
		glPushMatrix();
		float angle = (90 * i);
		float distance = 80;

		//move it up so they float
		//glTranslatef(0, 0.0f, 0);
		glScalef(0.5f, 0.5f, 0.5f);
		//glScalef(randBetween(20, 150) / 100.0f, randBetween(20, 150) / 100.0f,
				//randBetween(20, 150) / 100.0f);

		//glTranslatef(sin(angle) * distance, 0, cos(angle) * distance);
		//glTranslatef(AstroidDetailAccuracy / -2.0f, AstroidDetailAccuracy / 2.0f,
				//AstroidDetailAccuracy / -2.0f);
		//int accuracy = AstroidDetailAccuracy;
		drawAstroid( astroidcoords[i][0], astroidcoords[i][1], astroidcoords[i][2],astroidcoords[i][3]);

		glPopMatrix();
	}
	glEndList();
}

/**
 * drawAstroidss
 * Draws all of the astroids with or without textures based on texture.
 */
void drawAstroids() {
	glPushMatrix();
	glDisable( GL_TEXTURE_2D);
	if (toggleAstroidTextures == 1) {
		glEnable( GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, AstroidTexture);
	}
	glColor4f(1, 1, 1, 1);
	glDisable(GL_CULL_FACE);
	glCallList(AstroidId);
	glEnable(GL_CULL_FACE);
	glPopMatrix();
}


/**
 * drawCircle
 * Draws a circle.
 */
void drawCircle() {
	float numPoints = 120;
	double angleStep = (2 * 3.14) / numPoints;
	glBegin(GL_LINE_STRIP);
	double angle = 0.0;
	int i;
	for (i = 0; i <= numPoints; i++) {
		glVertex2f(sin(angle), cos(angle));
		angle += angleStep;
	}
	glEnd();
}
/**
 * initTarget
 * Initializes the target/crosshair
 */
void initTarget() {
	float lineLength = 1.0f;
	targetId = glGenLists(1);
	glNewList(targetId, GL_COMPILE);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	//y
	glVertex3f(0, -lineLength / 2.0f, 0);
	glVertex3f(0, lineLength / 2.0f, 0);
	//z
	glVertex3f(0, 0, -lineLength / 2.0f);
	glVertex3f(0, 0, lineLength / 2.0f);
	glEnd();
	glRotatef(90, 0, 1, 0);
	glScalef(0.3, 0.3, 1);
	drawCircle();
	glEndList();
}
/*
 * drawTarget
 * Draws the target/crosshair.
 */
void drawTarget() {
	glPushMatrix();
	glTranslatef(-10, 0, 0);

	if (boolShoot == 1) {
		targetScale *= 1.3f;
		if (targetScale > 1.5) {
			targetScale = 1.5;
		}
	} else if (targetScale > 1.0f) {
		targetScale *= 0.5f;
	}

	glScalef(targetScale, targetScale, targetScale);
	glCallList(targetId);
	glPopMatrix();
}
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
/**
 * initSky
 * Initializes the sky.
 */
void initSky() {

	skyObj = gluNewQuadric();
	skyId = glGenLists(4);
	gluQuadricNormals(skyObj, GLU_SMOOTH);
	gluQuadricTexture(skyObj, GL_TRUE);
	//texture the sky
	glBindTexture(GL_TEXTURE_2D, skyTexture);
	gluQuadricTexture(skyObj, skyTexture);

	glNewList(skyId, GL_COMPILE);
	glTranslatef(0, (seaDetailAccuracy * 2) - 2.0f, 0);
	glRotatef(90, 1, 0, 0);

	GLfloat diffuseMaterial[4] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat ambientMaterial[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mShininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mShininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMaterial);
	gluCylinder(skyObj, seaDetailAccuracy, seaDetailAccuracy,
			seaDetailAccuracy * 2, 100, 100);

	glRotatef(-180, 0, 1, 0);

	glScalef(1.01, 1, 1.01);
	gluCylinder(skyObj, seaDetailAccuracy, seaDetailAccuracy,
			(seaDetailAccuracy * 1.9f), 100, 100);
	glEndList();

}
/**
 * initSea
 * Initializes the sea
 */
void initSea() {
	seaObj = gluNewQuadric();
	seaId = glGenLists(1);
	gluQuadricNormals(seaObj, GLU_SMOOTH);
	gluQuadricTexture(seaObj, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, seaTexture);
	gluQuadricTexture(seaObj, seaTexture);

	glNewList(seaId, GL_COMPILE);
	glRotatef(270, 1, 0, 0);
	glColor4f(0.0, 0.0, 0.6, 1.0);
	GLfloat ambientMaterial[4] = { 0.2, 0.2, 0.4, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMaterial);
	gluDisk(seaObj, 0, seaDetailAccuracy + 2, 64, 64);
	glEndList();
}
/**
 * initGrid
 * Initializes the grid.
 */
void initGrid() {
	gridId = glGenLists(1);
	glNewList(gridId, GL_COMPILE);
	float x, y, z;
	y = -0.5f;
	GLfloat diffuseMaterial[4] = { 0.3, 0.3, 0.3, 1.0 };
	glBegin(GL_QUADS);
	for (x = 0; x < gridSize; x++) {
		for (z = 0; z < gridSize; z++) {
			glColor4f(diffuseMaterial[0], diffuseMaterial[1],
					diffuseMaterial[2], diffuseMaterial[3]);
			glNormal3f(0, 1, 0);

			glTexCoord2f((x / gridSize), ((z + 1) / gridSize));
			glVertex3f(x, y, z + 1);

			glTexCoord2f(((x + 1) / gridSize), ((z + 1) / gridSize));
			glVertex3f(x + 1, y, z + 1);

			glTexCoord2f(((x + 1) / gridSize), (z / gridSize));
			glVertex3f(x + 1, y, z);

			glTexCoord2f((x / gridSize), (z / gridSize));
			glVertex3f(x, y, z);

		}
	}
	glEnd();
	glEndList();

}
/**
 * drawGrid
 * Draws the grid.
 */
void drawGrid() {
	glTranslatef(0, -0.5f, 0);
	glEnable(GL_COLOR_MATERIAL);
	// set material properties which will be assigned by glColor
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glCallList(gridId);
	glDisable(GL_COLOR_MATERIAL);
}
/**
 * drawSea
 */
void drawSea() {
	glCallList(seaId);
}
/**
 * drawSky
 */
void drawSky() {
	glCallList(skyId);
}


/**
 * loadGLTexture
 * Loads a texture and returns the textureid
 */
GLuint loadGLTexture(const char * filename, int wrap, int width, int height) {
	GLuint texture;
	BYTE * data;
	FILE * file;

	// open .raw texture file, reading bytes
	// .raw has no header
	file = fopen(filename, "rb");
	if (file == NULL) {
		printf("Could not load texture.\n");
		return 0;
	}
	// create data buffer
	data = malloc(width * height * 3);

	// read texture data
	fread(data, width * height * 3, 1, file);
	fclose(file);

	// generate texture id
	glGenTextures(1, &texture);
	// bind the texture
	glBindTexture( GL_TEXTURE_2D, texture);
	//configuration
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
	GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//wrap or cut off based on param
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP);

	// build 2d mipmaps
	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,
	GL_RGB, GL_UNSIGNED_BYTE, data);

	// free data buffer, no longer needed
	free(data);

	//return the texture
	return texture;
}

/**
 * initTextures
 * Initializes the textures used.
 * Astroids, ship, sky, sea
 */
void initTextures() {
   AstroidTexture = LoadTexBMP("troid.bmp");
   seaTexture = LoadTexBMP("milk.bmp");
   skyTexture = LoadTexBMP("milk.bmp");
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
}

/**
 * display
 * Displays the scene, called by opengl every so many milliseconds.
 */
void display(int ms) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	update();
	draw();
	glutSwapBuffers();
	glutTimerFunc(ms, display, ms);
	ErrCheck("display");
}
/**
 * LITERALLY DOES NOTHING OTHER THAN CALL THE RESHAPE FUNCTION "handleResize" WHEN NEEDED.
 * TIMERFUNCTION "display" IS USED FOR EVERYTHING ELSE.
 * **** OPENGL SOMETIMES...
 */
void myDisplay(void) {

}
/**
 * handleResize
 * Handles the resizing of the window at a very basic level.
 */
void handleResize(int w, int h) {
	appWidth = w;
	appHeight = h;

	// Set the viewport to be the entire window
	glViewport(0, 0, appWidth, appHeight);

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);
	// Reset Matrix
	glLoadIdentity();

	// Set the correct perspective.
	gluPerspective(fov, (float) appWidth / (float) appHeight, nearValue,
			farValue);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);

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
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   gluPerspective(fov,asp,dim/16,16*dim);
   
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
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
				emission += 7;
				
			}
		} else if (button == 4) {
			if (planeSpeed >= planeMinSpeed + planeDeacc) {
				planeSpeed -= planeDeacc;
				emission -= 7;
				if (emission <10)
				{
					emission = 10;
				}
			}
		}
	}
}
/**
 * initGLUT
 * Initializes some GLUT values for the Application Window.
 * Configures the timer function for the display, and mouse function for GUI.
 */
void initGLUT(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(appWidth, appHeight);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(appX, appY);
	glutCreateWindow("Flight Sim CSCI 5229 Andrew Candelaresi");

	glutTimerFunc(15, display, 15);
	glutDisplayFunc(myDisplay);
	glutPassiveMotionFunc(mouseEvent);

	glutSpecialUpFunc(keySpecialUp);
	glutSpecialFunc(keySpecialDown);

	glutMouseFunc(mouseWheel);

	glutKeyboardUpFunc(keyUp);
	glutKeyboardFunc(keyDown);
	glutReshapeFunc(handleResize);
}
/**
 * initializeGL
 * Initializes GL settings.
 */
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
	gluLookAt(eyeX, eyeY, eyeZ, /* eye */
	atX, atY, atZ, /* looking at*/
	.0, 1.0, 0.0); /* up is in positive Y direction */

	glEnable(GL_BLEND);
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glDepthMask(GL_TRUE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode( GL_BACK, renderingOptions[toggleWireframe]);
	glPolygonMode( GL_FRONT, renderingOptions[toggleWireframe]);

	// define the color of the fog, light pink because assignment screenshots showed it
	GLfloat fogColor[4] = { 1, 0.7, 0.8, 0.3 };
	// set the color of the fog
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_MODE, GL_EXP);
	glHint(GL_FOG_HINT, GL_NICEST);
	glFogf(GL_FOG_DENSITY, originalFogDensity);
	glDisable(GL_FOG);

}


/**
 * drawAltometer
 * Shows the altitude (y-coord) of camera
 */
void drawAltometer() {
	float scaleY = (eyeY / 20.0f);
	glDisable(GL_LIGHTING);

	glPushMatrix();
	glColor3f(0, 0, 1);	//blue
	glTranslatef(4.8f, -2.8f, -5.0f);
	glScalef(1, scaleY, 0);
	//glRotatef(-90,0,1,0);

	glBegin(GL_POLYGON);
	float sizeX = 0.2f;
	float sizeY = 1.0f;
	glVertex3f(0, 0, 0);
	glVertex3f(sizeX, 0, 0);
	glVertex3f(sizeX, sizeY, 0);
	glVertex3f(0, sizeY, 0);

	glEnd();
	glPopMatrix();
	glEnable(GL_LIGHTING);
}
/**
 * drawSpedometer
 * Shows the speed of the plane
 */
void drawSpedometer() {
	float scaleX = (2 * planeSpeed / planeMaxSpeed);
	glDisable(GL_LIGHTING);

	glPushMatrix();
	glColor3f(0, 1, 0);	//green
	glTranslatef(-5.05f, -2.8f, -5.0f);
	glScalef(scaleX, 1, 1);
	//glRotatef(-90,0,1,0);

	glBegin(GL_POLYGON);
	float sizeX = 1.0f;
	float sizeY = 0.2f;
	glVertex3f(0, 0, 0);
	glVertex3f(sizeX, 0, 0);
	glVertex3f(sizeX, sizeY, 0);
	glVertex3f(0, sizeY, 0);

	glEnd();
	glPopMatrix();
	glEnable(GL_LIGHTING);

}
/**
 * drawExplosion
 * Draws the explosion when you crash into the sea
 */
void drawExplosion(int slices, int stacks) {
	int i, j;
	for (i = 0; i <= slices; i++) {
		float lat0 = M_PI * (-0.5 + (float) (i - 1) / slices);
		float z0 = sin(lat0);
		float zr0 = cos(lat0);

		float lat1 = M_PI * (-0.5 + (float) i / slices);
		float z1 = sin(lat1);
		float zr1 = cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for (j = 0; j <= stacks; j++) {
			float lng = 2 * M_PI * (float) (j - 1) / stacks;
			//randomize the position to create somewhat of a jagged shape .
			float x = cos(lng)
					+ (((randBetween(0, 1000) / 1000.0f) - 0.5f) / 10.0f);
			float y = sin(lng)
					+ (((randBetween(0, 1000) / 1000.0f) - 0.5f) / 10.0f);
			glNormal3f(x * zr0, y * zr0, z0);
			glVertex3f(x * zr0, y * zr0, z0);
			glNormal3f(x * zr1, y * zr1, z1);
			glVertex3f(x * zr1, y * zr1, z1);
		}
		glEnd();
	}
}
/**
 * explode
 * Causes the plane to explode when it hits the sea creating an explosion
 */
void explode() {
	glEnable(GL_COLOR_MATERIAL);
	// set material properties which will be assigned by glColor
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	int i;
	for (i = 0; i < 10; i++) {
		explosionScale += delta / 50.0f;
		glScalef(explosionScale, explosionScale, explosionScale);
		glColor4f(1, 0, 0, 0.2f);
		glRotatef(randBetween(0, 360), 1, 0, 0);

		glDisable(GL_CULL_FACE);
		drawExplosion(32, 32);
		glEnable(GL_CULL_FACE);
	}
	if (explosionScale > 1) {
		exploding = 0;
		explosionScale = 1.0;
	}
	glDisable(GL_COLOR_MATERIAL);
}
/**
 * update
 * Updates the appplication's logic.
 * Controls actual movement of camera and bullets
 */
void update() {
	universeTime += delta;
	appX = glutGet((GLenum) GLUT_WINDOW_X);
	appY = glutGet((GLenum) GLUT_WINDOW_Y);

    ErrCheck("update1");
	//if shooting
	if (boolShoot == 1) {
		//create a new bullet
		Bullet* bullet = malloc(sizeof(Bullet));
		bullet->x = eyeX + (sin(planeRotation));
		bullet->y = eyeY + (sin(planeYawRotation)) - 1;
		bullet->z = eyeZ + (cos(planeRotation));
		bullet->rotation = planeRotation
				+ ((randBetween(0, targetScale * 2) - (targetScale)) / 80.0f);
		bullet->yaw = planeYawRotation
				+ ((randBetween(0, targetScale * 2) - (targetScale)) / 80.0f);
		bullet->nextBullet = NULL;
		//if its the first bullet
		if (numBullets == 0) {
			firstBullet = bullet;
			currBullet = bullet;
		} else {
			//otherwise add to end of linked list
			currBullet->nextBullet = bullet;
			currBullet = bullet;
		}
		//count number of bullets
		numBullets++;
		//if more than allowed number of bullets
		if (numBullets > maxNumBullets) {
			//remove the first bullet and set the next in line as the new first
			Bullet* tempBullet = firstBullet;
			free(firstBullet);
			firstBullet = tempBullet->nextBullet;
		}
	}
	//if there are bullets
	if (numBullets > 0) {
		//move them a bit based on bullet direction and speed
		Bullet* bullet = firstBullet;
		while (bullet != NULL) {
			bullet->x += (sin(bullet->rotation) * (bulletSpeed));
			bullet->z += (cos(bullet->rotation) * (bulletSpeed));
			bullet->y += (sin(bullet->yaw) * (bulletSpeed));
			bullet = bullet->nextBullet;
		}
	}
	//if alive update the state of camera/plane
	if (alive == 1) {

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
        //calculate collisions based on the eye level
		if (eyeY < 2) {
			alive = 0;
			explosionScale = 0.0f;
			exploding = 1;
			eyeY = 2;
		}
		else if (check_colision(atX, atY, atZ) == 1)
		{
			alive = 0;
			explosionScale = 0.0f;
			exploding = 1;
			eyeY = 2;
		}
	}
	ErrCheck("update2");

}
/**
 * drawBullet
 * Draws a bullet
 */
void drawBullet(float x, float y, float z) {
	glColor3f(0.7f, 0.7f, 0.7f);
	glTranslatef(x, y, z);
	glutSolidSphere(0.03, 16, 16);
}
/**
 * draw
 *
 * Draws the entire scene based on toggles and current state
 */
void draw() {
	glLoadIdentity();

	glPushMatrix();
	glColor4f(1.0, 1.0, 1.0, 1.0f);
	//set up camera
	/*
   double Ex = -2*dim*Sin(th)*Cos(ph);
   double Ey = +2*dim        *Sin(ph);
   double Ez = +2*dim*Cos(th)*Cos(ph);
   double Ox = 0;
   double Oy = 0;
   double Oz = 0;
   double Ux = 0; double Uy = 1; double Uz = 0;
   gluLookAt(Ex,Ey,Ez , Ox,Oy,Oz , Ux,Uy,Uz);
   */
	
	gluLookAt(eyeX, eyeY, eyeZ, /* eye */
	atX, atY, atZ, /* looking at*/
	0.0, 1.0, 0.0); /* up is in positive Y direction */

	//reset light including position
	initLight();


	if (toggleGrid == 1) {
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glEnable(GL_LIGHTING);
		glPopMatrix();
	}
	glColor4f(1, 1, 1, 1);
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, dull);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
	if (toggleAstroids == 1) {
		glPushMatrix();
		glDisable(GL_COLOR_MATERIAL);

		if (toggleAstroidTextures == 0) {
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, none);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, none);

			glEnable(GL_COLOR_MATERIAL);
			// set material properties which will be assigned by glColor
			glColorMaterial(GL_FRONT, GL_DIFFUSE);
		}

		drawAstroids();

		glDisable(GL_COLOR_MATERIAL);
		glPopMatrix();
	}

	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, dull);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, none);
	glEnable(GL_CULL_FACE);
	glDisable( GL_TEXTURE_2D);
	if (toggleGrid == 1) {
		glTranslatef(-100, 0, -100);

		drawGrid();

		glDisable(GL_FOG);

	} else {
		glEnable( GL_TEXTURE_2D);
		glPushMatrix();
		glDisable(GL_CULL_FACE);
		glTranslatef(0, -1, 0);
		gluQuadricNormals(seaObj, GLU_SMOOTH);
		gluQuadricTexture(seaObj, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D, seaTexture);
		gluQuadricTexture(seaObj, seaTexture);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, none);

		drawSea();

		glPopMatrix();

		if (toggleAltWeather == 0) {
			glDisable(GL_FOG);
		}

		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, dull);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
		glPushMatrix();
		gluQuadricNormals(skyObj, GLU_SMOOTH);
		gluQuadricTexture(skyObj, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D, skyTexture);
		gluQuadricTexture(skyObj, skyTexture);
		glDisable(GL_CULL_FACE);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, white);
		drawSky();
		glEnable(GL_CULL_FACE);
		glPopMatrix();
		glDisable( GL_TEXTURE_2D);
	}

	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, dull);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);

	glDisable(GL_FOG);
	glPopMatrix();

	initLight();
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, littleshiny);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, littlespecular);

	if(alive==1){
		glPushMatrix();
		float rotationDeg=(planeRotation * 180.0f /M_PI);
		glTranslatef(eyeX+((atX-eyeX)/2.0f), eyeY+((atY-eyeY)/2.0f)-2,eyeZ+((atZ-eyeZ)/2.0f));
		glScalef(0.8f, 0.8f, 0.8f);
		glRotatef(rotationDeg, 0, 1, 0);
		glRotatef(-planeTilt, 0, 0, 1);
		if (toggleAltControls == 1) {
			glRotatef(-planeYawRotation * 30.0f, 1, 0, 0);
		}
		
		glRotatef(90, 0, 1, 0);
        switch(ship)
        {
	       case 0:
	           glDisable(GL_CULL_FACE);
	           //  Disable Z-buffering in OpenGL
               //glDisable(GL_DEPTH_TEST);
               //  Draw cockpit
               cockpit(texture[1], texture[0]);
               glEnable(GL_CULL_FACE);
               glPopMatrix();
               break;

   
            case 1:
                glDisable(GL_CULL_FACE);
                glScaled(.5, .5, .5);
                compile_falcon();
                ErrCheck("display");
		        //drawfalcon();
		        glDisable(GL_LIGHTING);
		        drawTarget();
		        glEnable(GL_CULL_FACE);
		        glPopMatrix();
		        break;
		}
		if (toggleshower ==1)
		{
			for (int i= 0; i<10; i++)
			{
				int mapSize = (int) AstroidDetailAccuracy;
				double rx = randBetween(0,mapSize);
				double ry = randBetween(0,mapSize);
				double rz = randBetween(0,mapSize);
				sphere1(rx, ry, rz, 1);   
            }
	    }
		
	}
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, dull);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, white);

	//draw bullets if there are any
	if (numBullets > 0) {
		Bullet* bullet = firstBullet;
		while (bullet != NULL) {
			glPushMatrix();
			drawBullet(bullet->x+((atX-bullet->x)/2.0f), bullet->y+((atY-bullet->y)/2.0f)-2, bullet->z+((atZ-bullet->z)/2.0f));
			glPopMatrix();
			bullet = bullet->nextBullet;
		}
	}
	glPopMatrix();

	//everything after is no longer affected by camera
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	glPushMatrix();
	drawSpedometer();
	glPopMatrix();
	glPushMatrix();
	drawAltometer();
	glPopMatrix();

	if (alive == 0 && exploding == 1) {
		explode();
	}
	glEnable(GL_LIGHTING);

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
 * keyDown
 * Is called when a normal key is pressed down.
 */
void keyDown(unsigned char key, int mouseX, int mouseY) {
	//z for shooting
	if (key == 'z') {
		boolShoot = 1;
	}
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
	//  toggle cock pit vs ship view
    if (key == 'c' || key == 'C')
      ship = (ship+1)%2;
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
		toggleAstroidTextures = 1 - toggleAstroidTextures;
	}
	if (key == 'm') {
		toggleAstroids = 1 - toggleAstroids;
	}
	if (key == 'r') {
		srand(time(NULL));

		initNew();
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
	//z for shooting
	if (key == 'z') {
		boolShoot = 0;
	}
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
	if ((!toggleAltControls) == 1) {
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
	if (key == GLUT_KEY_F2) {
		toggleAltWeather = 1 - toggleAltWeather;
	}
}
/**
 * keySpecialUp
 * Is called when a special key is released.
 * 
 */
void keySpecialUp(int key, int mouseX, int mouseY) {
	if ((!toggleAltControls) == 1) {
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
	if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
    if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
    if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
    if (key == GLUT_KEY_DOWN)
      ph -= 5;
}
