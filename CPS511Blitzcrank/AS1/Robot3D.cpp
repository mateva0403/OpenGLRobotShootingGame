/*******************************************************************
		   Hierarchical Multi-Part Model Example
		   Edited By: Matthew Evangelista 501158179
********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <gl/glut.h>
#include <utility>
#include <vector>
#include "VECTOR3D.h"
#include "QuadMesh.h"

const int vWidth  = 650;    // Viewport width in pixels
const int vHeight = 500;    // Viewport height in pixels

// We are following the usage of scripting

// We use radius so just one value
float robotBodyRadius = 6.0;

float robotBodyDepth = 6.0;
float headRadius = 0.33*robotBodyRadius;
float headDepth = headRadius;
float limbLength = 0.60*robotBodyRadius;
float limbWidth = 0.125*robotBodyRadius;
float gunLength = limbLength / 4.0;
float gunWidth = limbWidth;
float gunDepth = limbWidth;
float stanchionLength = robotBodyRadius;
float stanchionRadius = 0.1*robotBodyDepth;
float baseWidth = 2 * robotBodyRadius;
float baseLength = 0.25*stanchionLength;

// My Variables
float gunAngle = 270.0; // DO NOT ROTATE THIS!

// Control Robot body rotation on base
float robotAngle = 0.0;

// Joint Selection, 1 = knee, 2 = fot, 3 = leg
int currentJointControl = 0;
// Joint Rotation
float shoulderAngle = 0.0;
float gunRotateAngle = 0.0;
float kneeAngle = 0.0;
float footAngle = 0.0;
float legAngle = 0.0;

// Lighting/shading and material properties for robot - upcoming lecture - just copy for now
// Robot RGBA material properties (NOTE: we will learn about this later in the semester)
GLfloat robotBody_mat_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat robotBody_mat_specular[] = { 0.45f,0.55f,0.45f,1.0f };
GLfloat robotBody_mat_diffuse[] = { 1.0f,0.75f,0.1f,1.0f };
GLfloat robotBody_mat_shininess[] = { 32.0F };

// Torso 

GLfloat robotTorso_mat_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat robotTorso_mat_specular[] = { 0.45f,0.55f,0.45f,1.0f };
GLfloat robotTorso_mat_diffuse[] = { 1.0f,0.55f,0.1f,1.0f };
GLfloat robotTorso_mat_shininess[] = { 32.0F };

// Eyes

GLfloat robotEye_mat_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat robotEye_mat_specular[] = { 0.45f,0.55f,0.45f,1.0f };
GLfloat robotEye_mat_diffuse[] = { 0.209f,0.661f,0.641f,1.0f };
GLfloat robotEye_mat_shininess[] = { 32.0F };

GLfloat robotArm_mat_ambient[] = { 0.0215f, 0.1745f, 0.0215f, 0.55f };
GLfloat robotArm_mat_diffuse[] = { 0.0f,0.0f,0.0f,0.0f };
GLfloat robotArm_mat_specular[] = { 0.7f, 0.6f, 0.6f, 1.0f };
GLfloat robotArm_mat_shininess[] = { 32.0F };

GLfloat gun_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat gun_mat_diffuse[] = { 0.15f,0.15f,0.15f,1.0f };
GLfloat gun_mat_specular[] = { 0.05f, 0.05f, 0.05f, 1.0f };
GLfloat gun_mat_shininess[] = { 100.0F };

GLfloat robotLowerBody_mat_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat robotLowerBody_mat_diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat robotLowerBody_mat_specular[] = { 0.774597f, 0.774597f, 0.774597f, 1.0f };
GLfloat robotLowerBody_mat_shininess[] = { 76.8F };


// Light properties
GLfloat light_position0[] = { -4.0F, 8.0F, 8.0F, 1.0F };
GLfloat light_position1[] = { 4.0F, 8.0F, 8.0F, 1.0F };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_ambient[] = { 0.2F, 0.2F, 0.2F, 1.0F };


// Mouse button
int currentButton;

// A flat open mesh
QuadMesh *groundMesh = NULL;

// Structure defining a bounding box, currently unused
typedef struct BoundingBox {
    VECTOR3D min;
	VECTOR3D max;
} BBox;

// Default Mesh Size
int meshSize = 16;

// Prototypes for functions in this module
void initOpenGL(int w, int h);
void display(void);
void reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void mouseMotionHandler(int xMouse, int yMouse);
void keyboard(unsigned char key, int x, int y);
void functionKeys(int key, int x, int y);
void animationHandler(int param);
void drawRobot();
void drawBody();
void drawHead();
void drawLowerBody();
void drawLeftArm();
void drawRightArm();

// My Functions for Blitzcrank
void drawBodyShoulder();

void drawRightLowerArm();
void drawLeftLowerArm();
void drawFist();
void drawEye();

void drawLeftUpperLeg();
void drawRightUpperLeg();
void drawFoot();

int main(int argc, char **argv)
{
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(vWidth, vHeight);
	glutInitWindowPosition(200, 30);
	glutCreateWindow("BLITZCRANK");

	// Initialize GL
	initOpenGL(vWidth, vHeight);

	// Register callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotionHandler);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(functionKeys);

	// Start event loop, never returns
	glutMainLoop();

	return 0;
}


// Set up OpenGL. For viewport and projection setup see reshape(). 
void initOpenGL(int w, int h)
{
	// Set up and enable lighting
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);   // This second light is currently off

	// Other OpenGL setup
	glEnable(GL_DEPTH_TEST);   // Remove hidded surfaces
	glShadeModel(GL_SMOOTH);   // Use smooth shading, makes boundaries between polygons harder to see 
	glClearColor(0.7F, 0.7F, 0.9F, 0.0F);  // Color and depth for glClear
	glClearDepth(1.0f);
	glEnable(GL_NORMALIZE);    // Renormalize normal vectors 
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   // Nicer perspective

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	// Other initializatuion
	// Set up ground quad mesh
	VECTOR3D origin = VECTOR3D(-16.0f, 0.0f, 16.0f);
	VECTOR3D dir1v = VECTOR3D(1.0f, 0.0f, 0.0f);
	VECTOR3D dir2v = VECTOR3D(0.0f, 0.0f, -1.0f);
	groundMesh = new QuadMesh(meshSize, 32.0);
	groundMesh->InitMesh(meshSize, origin, 32.0, 32.0, dir1v, dir2v);

	VECTOR3D ambient = VECTOR3D(0.0f, 0.05f, 0.0f);
	VECTOR3D diffuse = VECTOR3D(0.4f, 0.8f, 0.4f);
	VECTOR3D specular = VECTOR3D(0.04f, 0.04f, 0.04f);
	float shininess = 0.2;
	groundMesh->SetMaterial(ambient, diffuse, specular, shininess);

}


// Callback, called whenever GLUT determines that the window should be redisplayed
// or glutPostRedisplay() has been called.
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	// Create Viewing Matrix V
	// Set up the camera at position (0, 6, 22 [CHANGED to 28 CUZ BLITZCRANK BIG] looking at the origin, up along positive y axis
	gluLookAt(0.0, 6.0, 28.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// Draw Robot

	// Apply modelling transformations M to move robot
	// Current transformation matrix is set to IV, where I is identity matrix
	// CTM = IV
	drawRobot();

	// Draw ground
	glPushMatrix();
	glTranslatef(0.0, -20.0, 0.0);
	groundMesh->DrawMesh(meshSize);
	glPopMatrix();

	glutSwapBuffers();   // Double buffering, swap buffers
}

void drawRobot()
{
	glPushMatrix();
	 // spin robot on base. 
	 glRotatef(robotAngle, 0.0, 1.0, 0.0);

	 drawBody();
	 drawHead();
	 drawLeftArm();
	 drawRightArm();
	 drawLowerBody();
	 drawLeftUpperLeg();
	 drawRightUpperLeg();
	glPopMatrix();
	
	glPushMatrix();
		
	glPopMatrix();
	
	glPopMatrix();
}




void drawBody()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);

	glPushMatrix();
	glutSolidSphere(robotBodyRadius, 20.0, 20.0);

}

void drawBodyShoulder()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);

	glScalef(0.75 * robotBodyRadius, -0.35 * robotBodyRadius, 0.70 * robotBodyDepth);
	glutSolidCube(1.0);

}


void drawHead()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);

	glPushMatrix();
	// Position head with respect to parent (body)
	glTranslatef(0.0, 1.15*robotBodyRadius+0.5*headRadius, headRadius);
	
	// Build Head
	glPushMatrix();
	//glScalef(0.4*robotBodyRadius, 0.4*robotBodyRadius, 0.4*robotBodyRadius);
	glutSolidSphere(headRadius, 20.0, 20.0);

	// Left Eye (Our Right)
	glPushMatrix();
	glTranslatef(0.4 * headRadius, 0.0, 0.8 * headRadius);
	drawEye();
	glPopMatrix();

	// Right Eye (Our Left)
	glPushMatrix();
	glTranslatef(-0.4 * headRadius, 0.0, 0.8 * headRadius);
	drawEye();
	glPopMatrix();
	
	glPopMatrix();

	glPopMatrix();
}

void drawEye()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotEye_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotEye_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotEye_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotEye_mat_shininess);
	glutSolidSphere(0.25 * headRadius, 20.0, 20.0);
}

void drawLeftArm()
{
	glPushMatrix();
	// Rotation AT shoulder pad, to be implemented! 
	// glRotatef(shoulderAngle, 1.0, 0.0, 0.0);
	glTranslatef((0.5 * robotBodyRadius + 0.5 * limbWidth), 0.5 * robotBodyRadius, 0.0);
	
	// Shoulder Pad LEFT (Our Right)
	glPushMatrix();
	glTranslatef((0.75 * robotBodyRadius), limbWidth, 0.0);
	drawBodyShoulder();
	glPopMatrix();

	// Left Upper Arm
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotArm_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotArm_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotArm_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotArm_mat_shininess);

	glPushMatrix();
	glTranslatef(4.0 * limbWidth, -0.5 * limbLength, 0.0);
	glScalef(limbWidth, limbLength, limbWidth);
	glutSolidCube(1.0);
	glPopMatrix();

	// Left Lower Arm
	glPushMatrix();
	glTranslatef(4.0 * limbWidth, -1.4 * limbLength, 0.0);
	drawLeftLowerArm();
	glPopMatrix();

	glPopMatrix();
}

void drawLeftLowerArm()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);

	glScalef(2.1 * limbWidth, 0.77 * limbLength, 3.0 * limbWidth);
	glutSolidCube(1.0);

	// Wrist (similar to Gun)
	
	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, gun_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, gun_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, gun_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, gun_mat_shininess);

	glTranslatef(0.0, -(0.18 * limbLength + 0.5 * gunLength), 0.0);
	glRotatef(270.0, 1.0, 0.0, 0.0);
	
	GLUquadricObj* blitzGun;
	blitzGun = gluNewQuadric();
	gluCylinder(blitzGun, limbWidth, limbWidth, limbWidth, 20, 20);

	// FIST

	glPushMatrix();
	glTranslatef(0.0, 0.0, -0.2 * limbLength);
	drawFist();
	glPopMatrix();

	glPopMatrix();

}

void drawFist()
{
	// No Push and Pop Here because we want to use multiple instances of the fist in the future
	// TO IMPLEMENT UNDER HIERARCHY - FINGERS

	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);

	glScalef(0.35 * robotBodyRadius, -0.575 * robotBodyRadius, 0.25 * robotBodyDepth);
	glutSolidCube(1.0);
}

void drawRightArm()
{
	glPushMatrix();

	// Rotation AT shoulder pad
	glTranslatef(-(0.5 * robotBodyRadius + 0.5 * limbWidth), 0.5 * robotBodyRadius, 0.0);
	glRotatef(shoulderAngle, 1.0, 0.0, 0.0); 

	// Shoulder Pad RIGHT (Our Left)
	glPushMatrix();
	glTranslatef(-(0.75 * robotBodyRadius), limbWidth, 0.0);
	drawBodyShoulder();
	glPopMatrix();

	// Right Upper Arm
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotArm_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotArm_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotArm_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotArm_mat_shininess);

	glPushMatrix();
	glTranslatef(-4.0*limbWidth, -0.5 * limbLength, 0.0);
	glScalef(limbWidth, limbLength, limbWidth);
	glutSolidCube(1.0);
	glPopMatrix();

	// Right Lower Arm
	glPushMatrix();
	glTranslatef(-4.0*limbWidth, -1.4 * limbLength, 0.0);
	drawRightLowerArm();
	glPopMatrix();

	glPopMatrix();

}


void drawRightLowerArm()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);

	glScalef(2.1 * limbWidth, 0.77 * limbLength, 3.0 * limbWidth);
	glutSolidCube(1.0);

	//  Gun
	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, gun_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, gun_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, gun_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, gun_mat_shininess);

	glTranslatef(0.0, -(0.23 * limbLength + 0.5 * gunLength), 0.0);
	
	// rotate gun
	glRotatef(gunAngle, 1.0, 0.0, 0.0);
	glRotatef(gunRotateAngle, 0.0, 0.0, 1.0);

	// Build Gun 

	// CYLINDER VERSION
	GLUquadricObj* blitzGun;
	blitzGun = gluNewQuadric();
	gluCylinder(blitzGun, limbWidth, limbWidth, limbWidth, 20, 20);

	glPushMatrix();
	glTranslatef(0.0, 0.0, -0.2 * limbLength);
	drawFist();
	glPopMatrix();

	glPopMatrix();
}

void drawLowerBody()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotLowerBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotLowerBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotLowerBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotLowerBody_mat_shininess);

	// General Lower Body
	glPushMatrix();
	
	glTranslatef(0, -1.5 * robotBodyRadius, 0.0);

	// Position stanchion and base with respect to body
	
	/* Stanchion
	glPushMatrix();
	glScalef(stanchionRadius, stanchionLength, stanchionRadius);
	glTranslatef(0.0, -0.1 * stanchionLength, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(gluNewQuadric(), 1.0, 1.0, 1.0, 20, 10);
	glPopMatrix();
	*/

	// base
	glPushMatrix();
	// Position base with respect to parent stanchion
	glTranslatef(0.0, -1.30 * stanchionLength, 0.0);
	// Build base
	glScalef(baseWidth, baseLength, baseWidth);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	// Actual start of PELVIS
	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);

	glTranslatef(0, 0.45 * robotBodyRadius, 0.0);
	
	glScalef(0.13 * robotBodyRadius, 0.025 * robotBodyRadius, 0.13 * robotBodyDepth);
	glutSolidCube(robotBodyRadius);

	// LEGS
	
	glPopMatrix();

	glPopMatrix();
}

void drawLeftUpperLeg()
{
	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);
	glTranslatef(2.7 * limbWidth, -1.0 * robotBodyRadius, 0.0);
	glRotatef(legAngle, 1.0, 0.0, 0.0);
	
	glPushMatrix(); // UPPER LEG
	
	// Drawing of Leg
	
	glTranslatef(0.0, -limbLength, 0.0);
	glScalef(limbWidth, 1.35 * limbLength, limbWidth);
	glutSolidCube(1.0);
	glPopMatrix(); // Upper Leg END

	// Lower Leg

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotTorso_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotTorso_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotTorso_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotTorso_mat_shininess);

	glTranslatef(0.0, -1.7 * limbLength, 0.0);
	glRotatef(kneeAngle, 1.0, 0.0, 0.0);

	glPushMatrix();
	glTranslatef(0.0, -0.46 * limbLength, 0.0);
	glScalef(limbWidth, limbLength, limbWidth);
	glutSolidCube(1.0);
	glPopMatrix(); // Lower Leg END

	// Draw Foot under Lower Leg!

	glPushMatrix();
	glTranslatef(0.0, -1.0 * limbLength, 0.50 * limbWidth);
	glRotatef(footAngle, 1.0, 0.0, 0.0);
	drawFoot();
	glPopMatrix(); // Foot End
	

	glPopMatrix();

	glPopMatrix(); // Whole Left Leg END
}


void drawRightUpperLeg() // NOTE: rotations are commented out here to put emphasis on only one leg rotation.
{
	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);
	glTranslatef(-2.7 * limbWidth, -1.0 * robotBodyRadius, 0.0);
	//glRotatef(legAngle, 1.0, 0.0, 0.0);

	glPushMatrix(); // UPPER LEG

	// Drawing of Leg

	glTranslatef(0.0, -limbLength, 0.0);
	glScalef(limbWidth, 1.35 * limbLength, limbWidth);
	glutSolidCube(1.0);
	glPopMatrix(); // Upper Leg END

	// Lower Leg

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotTorso_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotTorso_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotTorso_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotTorso_mat_shininess);

	glTranslatef(0.0, -1.7 * limbLength, 0.0);
	//glRotatef(kneeAngle, 1.0, 0.0, 0.0);

	glPushMatrix();
	glTranslatef(0.0, -0.46 * limbLength, 0.0);
	glScalef(limbWidth, limbLength, limbWidth);
	glutSolidCube(1.0);
	glPopMatrix(); // Lower Leg END

	// Draw Foot

	glPushMatrix();
	glTranslatef(0.0, -1.0 * limbLength, 0.50 * limbWidth);
	//glRotatef(footAngle, 1.0, 0.0, 0.0);
	drawFoot();
	glPopMatrix(); // Foot End


	glPopMatrix();

	glPopMatrix(); // Whole Right Leg END
}

void drawFoot()
{
	// No push and pop again to account for indiv. translations
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotTorso_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotTorso_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotTorso_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotTorso_mat_shininess);
	glScalef(1.0 * limbLength, 0.75 * limbWidth, 1.0 * limbLength);
	glutSolidCube(1.0);
}

// Callback, called at initialization and whenever user resizes the window.
void reshape(int w, int h)
{
	// Set up viewport, projection, then change to modelview matrix mode - 
	// display function will then set up camera and do modeling transforms.
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)w / h, 0.2, 40.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Set up the camera at position (0, 6, 22) looking at the origin, up along positive y axis
	gluLookAt(0.0, 6.0, 22.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

// Anim Variables
bool stop = false;
bool currWalking = false;
bool walkReturn = false;

// Callback, handles input from the keyboard, non-arrow keys
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r':
		robotAngle += 2.0;
		break;
	case 'R':
		robotAngle -= 2.0;
		break;
	case 'a':
		shoulderAngle += 2.0;
		break;
	case 'A':
		shoulderAngle -= 2.0;
		break;
	// Joint Control
	case 'k':
		currentJointControl = 1;
		break;
	case 'f':
		currentJointControl = 2;
		break;
	case 'l':
		currentJointControl = 3;
		break;
	// Anim - Rotations
	case 's':
		glutTimerFunc(10, animationHandler, 1);
		break;
	case 'S':
		stop = true;
		break;
	case 'c':
		glutTimerFunc(10, animationHandler, 2);
		break;
	// Anim - Complex Animations
	case 'w':
		if (!currWalking)
		{
			currWalking = true;
			walkReturn = false;
			glutTimerFunc(10, animationHandler, 3);
		}
		break;
	case 'W':
		legAngle = 0;
		kneeAngle = 0;
		footAngle = 0;
		currWalking = false;
		walkReturn = false;
		break;
	}
	glutPostRedisplay();   // Trigger a window redisplay
}


void animationHandler(int param)
{
	if (!stop)
	{
		switch (param) // Each parameter is an animation. In the future, simple rotations will be ordered to be the first few cases, followed by more complex ones.
		{
		case 1: // Shoulder Rotate
			shoulderAngle += 1.0;
			glutPostRedisplay();
			glutTimerFunc(10, animationHandler, 1);
			break;
		case 2: // Cannon Rotate
			gunRotateAngle += 1.0;
			glutPostRedisplay();
			glutTimerFunc(10, animationHandler, 2);
			break;
		case 3: // Walk Animation
			if (currWalking)
			{
				if (!walkReturn)
				{
					if (legAngle > -70.0)
					{
						legAngle -= 2.0;
						kneeAngle += 2.0;
					}
					else if (legAngle == -70.0)
					{
						walkReturn = true;
					}
				}
				else if (walkReturn)
				{
					if (kneeAngle > 0.0)
					{
						kneeAngle -= 2.0;
						legAngle += 2.0;
					}
					
					else if (kneeAngle == 0.0 && legAngle == 0.0)
					{
						walkReturn = false;
						currWalking = false;
					}
				}
				glutTimerFunc(10, animationHandler, 3);
			}
			glutPostRedisplay();
			break;
		}
		
	}
}



// Callback, handles input from the keyboard, function and arrow keys
void functionKeys(int key, int x, int y)
{
	switch (key) // Swapped to switch just in case we would need more function keys in the future. Also because i'm using if statements lol.
	{
	case GLUT_KEY_F1: 	// Help key
		break;

	case GLUT_KEY_RIGHT:
		if (currentJointControl == 1) { kneeAngle += 2.5; }
		else if (currentJointControl == 2) { footAngle += 2.5; }
		else if (currentJointControl == 3) { legAngle += 2.5; }
		break;

	case GLUT_KEY_LEFT:
		if (currentJointControl == 1) { kneeAngle -= 2.5; }
		else if (currentJointControl == 2) { footAngle -= 2.5; }
		else if (currentJointControl == 3) { legAngle -= 2.5; }
	}
	glutPostRedisplay();   // Trigger a window redisplay
}


// Mouse button callback - use only if you want to 
void mouse(int button, int state, int x, int y)
{
	currentButton = button;

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			;

		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
			;
		}
		break;
	default:
		break;
	}

	glutPostRedisplay();   // Trigger a window redisplay
}


// Mouse motion callback - use only if you want to 
void mouseMotionHandler(int xMouse, int yMouse)
{
	if (currentButton == GLUT_LEFT_BUTTON)
	{
		;
	}

	glutPostRedisplay();   // Trigger a window redisplay
}

