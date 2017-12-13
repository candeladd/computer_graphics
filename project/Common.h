/*
 * Common.h
 * @author	: Andrew Candelaresi
 * CSCI 5229
 */

#ifndef COMMON_H_
#define COMMON_H_

const GLfloat seaDetailAccuracy = 100.0f;
GLuint AstroidDetailAccuracy = 200;
const GLfloat gridSize = 200.0f;
GLint appWidth = 1200;  //dimensions of window on init
GLint appHeight = 900; //dimensions of window on init
//used to return to previous size when exiting fullscreen
GLint storedWidth = 1200;
GLint storedHeight = 900;
GLint storedX = 100;
GLint storedY = 100;
int appX = 100;
int appY = 100;
GLfloat nearValue = 0.1f;
GLfloat farValue = 100000.0f;
GLfloat universeTime = 0.0f;
GLfloat delta = 1.0f;
GLfloat explosionScale=1.0f;
GLuint exploding=0;
GLint fov = 60;
GLfloat originalFogDensity = 0.005f;

// Light values
int one       =   1;  // Unit value
int distance  =   12;  // Light distance

int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   30;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   35;  // Specular intensity (%)
int shininess =   16;  // Shininess (power of two)
int zh        =  200;  // Light azimuth
float ylight  =   0;  // Elevation of light

int ship=1;       //  toggle between cockpit and 3rd person
int ph=5;         //  Elevation of view angle
int th=0;         //  Azimuth of view angle
double asp=1;     //  Aspect ratio
double dim=12.0;   //  Size of world


GLint lastMouseX;
GLint lastMouseY;
GLuint alive = 1;
GLdouble eyeX = 0;
GLdouble eyeY = 5;
GLdouble eyeZ = -15;
GLdouble atX = 0;
GLdouble atY = 5;
GLdouble atZ = -5;

GLint toggleWireframe = 0;
GLint toggleFullscreen = 0;
GLint toggleFog = 1;
GLint toggleGrid = 0;
GLint toggleAstroids = 1;
GLint toggleAltControls = 0;
GLint toggleAltWeather = 0;
GLint toggleAstroidTextures = 1;
GLint toggleshower = 0;

GLint boolAccelerate = 0;
GLint boolDeaccelerate = 0;
GLint boolMoveUp = 0;
GLint boolMoveDown = 0;
GLint boolShoot=0;
GLfloat planeSpeed = 1.0f;
GLfloat planeMaxSpeed = 4.0f;
GLfloat planeMinSpeed = 1.0f;
GLfloat planeAcc = 0.2f;

GLfloat planeDeacc = 0.1f;
GLfloat planeRotation = 0.0f;
GLfloat planeYawRotation = 0.0f;
GLfloat planeTilt = 0.0f;


GLfloat planeRiseSpeed = 0.2f;
GLfloat planeFallSpeed = 0.2f;
GLuint targetId;
GLuint falconId;

GLuint AstroidId;
GLuint gridId;
GLuint seaId;
GLuint skyId;

GLuint AstroidTexture;
GLuint seaTexture;
GLuint skyTexture;


//GLfloat ambientLight0[] = { 0.5*ambient, 0.5*ambient, 0.5*ambient};
//GLfloat diffuseLight0[] = { 1, 1, 1};
//GLfloat specularLight0[] = { 1, 1, 1};
//GLfloat lightPosition0[] = { 0, 0.3, -1, 0 };
GLfloat none[] ={0,0,0,0};
GLfloat dull[] = {1};
GLfloat littleshiny[] = {30};
GLfloat shiny[] = {50};
GLfloat littlespecular[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat darkgreen[] = { 0,1,0, 1.0 };
GLfloat lightgreen[] = { 0.6, 0.8, 0.5, 1.0 };
GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat grey[] = { 0.3, 0.3, 0.3, 1.0 };
GLfloat yellow[] = { 0.8, 0.8, 0.0, 1.0 };
GLfloat red[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat blue[] = { 0.0, 0.0, 0.8, 1.0 };
GLfloat purple[] = { 0.7, 0.5, 0.9, 1.0 };
//stores randoly generated astroid coords for collision detection
double astroidcoords[50][4];
//double astroidcoords2[2][4];

typedef struct Point {
	float x;
	float y;
	float z;
	float nx;
	float ny;
	float nz;
} Point;

Point calcNormal(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
void calcVertexNormal(float map[AstroidDetailAccuracy][AstroidDetailAccuracy], float jitterX[AstroidDetailAccuracy][AstroidDetailAccuracy], float jitterZ[AstroidDetailAccuracy][AstroidDetailAccuracy],int x, int z);
void raiseAstroid(float map[AstroidDetailAccuracy][AstroidDetailAccuracy], int left, int right, int top, int bottom, int iteration);


typedef struct Bullet Bullet;
struct Bullet {
	float x;
	float y;
	float z;
	float rotation;
	float yaw;
	Bullet* nextBullet;
};
Bullet* firstBullet;
Bullet* currBullet;
GLuint numBullets;
GLuint currentBullet;
GLuint maxNumBullets;
GLfloat targetScale;
GLfloat bulletSpeed=4.0f;
#endif /* COMMON_H_ */
