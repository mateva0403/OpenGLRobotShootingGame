/*******************************************************************
		   Hierarchical Multi-Part Model Example
		   Edited By: Matthew Evangelista 501158179
		   CONVERTED TO BLITZCRANK.H type stuff!
********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <gl/glut.h>
#include <GL/glew.h>
#include <utility>
#include <vector>
#include "VECTOR3D.h"
#include "QuadMesh.h"
// #include "cannonMesh.h" // THIS IS NOT WORKING.
// FOR RANDOM
#include <cstdlib>
#include <ctime>

float robotBodyRadius = 6.0;
float robotBodyDepth = 6.0;
float headRadius = 0.33 * robotBodyRadius;
float headDepth = headRadius;
float limbLength = 0.60 * robotBodyRadius;
float limbWidth = 0.125 * robotBodyRadius;
float gunLength = limbLength / 4.0;
float gunWidth = limbWidth;
float gunDepth = limbWidth;
float stanchionLength = robotBodyRadius;
float stanchionRadius = 0.1 * robotBodyDepth;
float baseWidth = 2 * robotBodyRadius;
float baseLength = 0.25 * stanchionLength;

// My Variables

// Round/Game/State/HP
bool inRound = false;
int curRound = 1;
bool roundClear = true;
int playerHP = 10;

// Cam/Gun
float yaw = 0.0;
float pitch = 0.0;
float canAngle = 0.0;
float gunAngle = 270.0; // DO NOT ROTATE THIS!

float eyeX = 0.0;
float eyeY = 0.0;
float eyeZ = 0.0;
float gunPosX = 0.0;
float gunPosY = 6.0;
float gunPosZ = 28.0;
float lookAtX = 0.0;
float lookAtY = 0.0;
float lookAtZ = 0.0;
float cannonAngle = 0.0;

// Anim Variables
bool stop = false;
bool currWalking = false;
bool walkReturn = false;

// Blitz Spawn Vars
int blitzIDCount = 0;
float spawnX;
float spawnZ = -300.0;

// BASE 1.0 glutCube

float cubeVertices[] = {
	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,

	-0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f
};

//GOD HELP ME

// BoundingBox Struct (Found in AS1 Skel Code)
typedef struct BoundingBox {
	VECTOR3D min;
	VECTOR3D max;
} BBox;

// SPHERES
GLUquadric* blitzEye;
GLUquadric* blitzHead;
GLUquadric* blitzBody;
GLUquadric* laserProjectile;

// VAO AND VBO
GLuint cubeVBO, cubeVAO;

// Projectile
typedef struct Projectile
{
	float projX, projY, projZ;
	float dirX, dirY, dirZ;
	float speed = 0.75;
	bool projAlive;
} Proj;

// BlitzSTRUCT
typedef struct BlitzInst {

	//ID
	int blitzID;

	// BB
	BoundingBox BlitzBox;
	float bPosX = 0.0, bPosY = 0.0, bPosZ = 0.0;

	std::vector<Projectile> blitzProj;
	float beamTimer = 0.0;

	// Health and Such
	int bHP = 5;
	bool isAlive = true;
	bool isHit = false;
	bool deathAnim = false;
	float deathTimer = 0.0f;
	float hitTimer = 0.0f;

	// Anim
	float robotAngle = 0.0;
	float shoulderAngle = -90;
	float gunRotateAngle = 0.0;
	float kneeAngle = 0.0;
	float footAngle = 0.0;
	float legAngleL = 0.0;
	float legAngleR = 0.0;
	bool isAnim = false;
	bool currWalking = false;
	bool walkReturn = false;
	int cycleAnim = 0;

} BlitzInst;

std::vector<Proj> projectiles;
std::vector<BlitzInst> blitzcranks;

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

// WORLD

GLfloat cliff_mat_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat cliff_mat_diffuse[] = { 0.9f, 0.6f, 0.9f, 1.0f };
GLfloat cliff_mat_specular[] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat cliff_mat_shininess[] = { 10.0F };

GLfloat water_mat_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat water_mat_diffuse[] = { 0.0f, 0.0f, 0.9f, 1.0f };
GLfloat water_mat_specular[] = { 0.974597f, 0.974597f, 0.974597f, 1.0f };
GLfloat water_mat_shininess[] = { 100.0f };

GLfloat bridge_mat_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat bridge_mat_diffuse[] = { 0.95f, 0.6f, 0.4f, 1.0f };
GLfloat bridge_mat_specular[] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat bridge_mat_shininess[] = { 10.0F };

// BASE LASER

GLfloat proj_mat_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat proj_mat_specular[] = { 0.45f,0.55f,0.45f,1.0f };
GLfloat proj_mat_diffuse[] = { 1.0f,0.25f,0.1f,1.0f };
GLfloat proj_mat_shininess[] = { 32.0F };

// Light properties

GLfloat light_position0[] = { -4.0F, 8.0F, 8.0F, 1.0F };
GLfloat light_position1[] = { 4.0F, 8.0F, 8.0F, 1.0F };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_ambient[] = { 0.2F, 0.2F, 0.2F, 1.0F };

// Prototypes for functions in this module
void reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void functionKeys(int key, int x, int y);
void animationHandler(int param);

// My Functions for Blitzcrank

// These cubes are basically glutSolidCube(1.0). I am a genius, I know. Even I, scare myself sometimes.
void initCube();
void drawCube();

void drawRobot(const BlitzInst& blitz);
void drawBody(const BlitzInst& blitz);
void drawHead();
void drawLowerBody();
void drawLeftArm(const BlitzInst& blitz);
void drawRightArm(const BlitzInst& blitz);
void drawBodyShoulder();
void drawRightLowerArm();
void drawLeftLowerArm();
void drawFist();
void drawEye();
void drawLeftUpperLeg(const BlitzInst& blitz);
void drawRightUpperLeg(const BlitzInst& blitz);
void drawFoot();

// Environment Prototypes

void drawEnvironment();
void drawCliffs();
void drawBridge();
void drawWater();

// Cannon Prototype
void drawCannon();
void drawProjectile();

void initCube()
{
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// BOUNDING BOX STUFF

void updateBoundingBox(BlitzInst& blitz)
{
	float halfWidth = 20.0;
	float halfHeight = 20.0f;
	float halfDepth = 20.0f;

	blitz.BlitzBox.min = VECTOR3D(blitz.bPosX - halfWidth, blitz.bPosY - halfHeight, blitz.bPosZ - halfDepth);
	blitz.BlitzBox.max = VECTOR3D(blitz.bPosX + halfWidth, blitz.bPosY + halfHeight, blitz.bPosZ + halfDepth);
}

bool isCollision(const VECTOR3D& point, const BoundingBox& box)
{
	return (point.x >= box.min.x && point.x <= box.max.x && point.y >= box.min.y && point.y <= box.max.y && point.z >= box.min.z && point.z <= box.max.z);
}


// MESH DRAWING

void drawCube()
{
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_QUADS, 0, 24);
	glBindVertexArray(0);
}

void drawRobot(const BlitzInst& blitz) {

	glPushMatrix();
	glTranslatef(0.0, blitz.bPosY, 0.0); // Idk why this fixes this tbh

	drawBody(blitz);
	drawHead();
	drawLeftArm(blitz);
	drawRightArm(blitz);
	drawLowerBody();
	drawLeftUpperLeg(blitz);
	drawRightUpperLeg(blitz);
	glPopMatrix();

	glPushMatrix();

	glPopMatrix();

	glPopMatrix();
}


void drawBody(const BlitzInst& blitz)
{
	if (!blitz.isHit)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);
	
	}
	else if (blitz.isHit)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, proj_mat_ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, proj_mat_specular);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, proj_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SHININESS, proj_mat_shininess);
	}


	glPushMatrix();
	blitzBody = gluNewQuadric();
	gluSphere(blitzBody, robotBodyRadius, 20.0, 20.0);
	glPopMatrix();
}

void drawBodyShoulder()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);

	glScalef(0.75 * robotBodyRadius, -0.35 * robotBodyRadius, 0.70 * robotBodyDepth);
	drawCube();

}


void drawHead()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);

	glPushMatrix();
	glTranslatef(0.0, 1.15 * robotBodyRadius + 0.5 * headRadius, headRadius);

	// Build Head
	glPushMatrix();
	blitzHead = gluNewQuadric();
	gluSphere(blitzHead, headRadius, 20.0, 20.0);

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
	blitzEye = gluNewQuadric();
	gluSphere(blitzEye, 0.25 * headRadius, 20.0, 20.0);
}

void drawLeftArm(const BlitzInst& blitz)
{
	glPushMatrix();

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
	drawCube();
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
	drawCube();

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
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);

	glScalef(0.35 * robotBodyRadius, -0.575 * robotBodyRadius, 0.25 * robotBodyDepth);
	drawCube();
}

void drawRightArm(const BlitzInst& blitz)
{
	glPushMatrix();

	glTranslatef(-(0.5 * robotBodyRadius + 0.5 * limbWidth), 0.5 * robotBodyRadius, 0.0);
	glRotatef(blitz.shoulderAngle, 1.0, 0.0, 0.0);

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
	glTranslatef(-4.0 * limbWidth, -0.5 * limbLength, 0.0);
	glScalef(limbWidth, limbLength, limbWidth);
	drawCube();
	glPopMatrix();

	// Right Lower Arm
	glPushMatrix();
	glTranslatef(-4.0 * limbWidth, -1.4 * limbLength, 0.0);
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
	drawCube();

	//  Gun
	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, gun_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, gun_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, gun_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, gun_mat_shininess);

	glTranslatef(0.0, -(0.23 * limbLength + 0.5 * gunLength), 0.0);

	// rotate gun
	glRotatef(gunAngle, 1.0, 0.0, 0.0);

	// Build Gun 
	GLUquadricObj* blitzGun;
	blitzGun = gluNewQuadric();
	gluCylinder(blitzGun, limbWidth, limbWidth, limbWidth, 20, 20);

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

	// Actual start of PELVIS
	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);

	glTranslatef(0, 0.45 * robotBodyRadius, 0.0);

	glScalef(0.13 * robotBodyRadius, 0.025 * robotBodyRadius, 0.13 * robotBodyDepth);
	glScalef(robotBodyRadius, robotBodyRadius, robotBodyRadius);
	drawCube();

	glPopMatrix();

	glPopMatrix();
}

void drawLeftUpperLeg(const BlitzInst& blitz)
{
	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);
	glTranslatef(2.7 * limbWidth, -1.0 * robotBodyRadius, 0.0);
	glRotatef(blitz.legAngleL, 1.0, 0.0, 0.0);

	glPushMatrix(); // UPPER LEG

	// Drawing of Leg

	glTranslatef(0.0, -limbLength, 0.0);
	glScalef(limbWidth, 1.35 * limbLength, limbWidth);
	drawCube();
	glPopMatrix(); // Upper Leg END

	// Lower Leg

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, robotTorso_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotTorso_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotTorso_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotTorso_mat_shininess);

	glTranslatef(0.0, -1.7 * limbLength, 0.0);
	glRotatef(blitz.kneeAngle, 1.0, 0.0, 0.0);

	glPushMatrix();
	glTranslatef(0.0, -0.46 * limbLength, 0.0);
	glScalef(limbWidth, limbLength, limbWidth);
	drawCube();
	glPopMatrix(); // Lower Leg END

	// Draw Foot under Lower Leg!

	glPushMatrix();
	glTranslatef(0.0, -1.0 * limbLength, 0.50 * limbWidth);
	glRotatef(blitz.footAngle, 1.0, 0.0, 0.0);
	drawFoot();
	glPopMatrix(); // Foot End


	glPopMatrix();

	glPopMatrix(); // Whole Left Leg END
}


void drawRightUpperLeg(const BlitzInst& blitz)
{
	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, robotBody_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, robotBody_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, robotBody_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, robotBody_mat_shininess);
	glTranslatef(-2.7 * limbWidth, -1.0 * robotBodyRadius, 0.0);
	glRotatef(blitz.legAngleR, 1.0, 0.0, 0.0);

	glPushMatrix(); // UPPER LEG

	// Drawing of Leg

	glTranslatef(0.0, -limbLength, 0.0);
	glScalef(limbWidth, 1.35 * limbLength, limbWidth);
	drawCube();
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
	drawCube();
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
	drawCube();
}

////////////
// ENV
///////////

void drawEnvironment()
{
	glPushMatrix();
	drawWater();

	glPushMatrix();
	glTranslatef(0.0, -35.0, -400.0);
	drawCliffs();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, -35.0, 80.0);
	drawCliffs();
	glPopMatrix();

	drawBridge();

	glPopMatrix();
}

void drawWater()
{
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, water_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, water_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, water_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, water_mat_shininess);
	glTranslatef(0.0, -35.0, 0.0);
	glScalef(1000.0, 2.0, 1000.0);
	drawCube();
	glPopMatrix();
}

void drawCliffs()
{
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, cliff_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, cliff_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cliff_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, cliff_mat_shininess);
	glScalef(1000.0, 35.0, 200.0);
	drawCube();
	glPopMatrix();
}

void drawBridge()
{
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, bridge_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, bridge_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, bridge_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, bridge_mat_shininess);
	glTranslatef(0.0, -35.0, -160.0);
	glScalef(100.0, 35.0, 280.0);

	drawCube();
	glPopMatrix();
}

//////////////////
// WEAPONS
/////////////////

float cannonLookAtX, cannonLookAtY, cannonLookAtZ;
float cannonRadius = 5.0f;

void updateCannonLookAt()
{
	cannonLookAtX = gunPosX + cannonRadius * cos(yaw) * cos(pitch);
	cannonLookAtY = gunPosY + cannonRadius * sin(pitch);
	cannonLookAtZ = gunPosZ + cannonRadius * sin(yaw) * cos(pitch);
}

void drawProjectile()
{
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, proj_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, proj_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, proj_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, proj_mat_shininess);
	laserProjectile = gluNewQuadric();
	gluSphere(laserProjectile, 1.0, 20.0, 20.0);
	glPopMatrix();
}

void drawCannon()
{
	glPushMatrix();

	glTranslatef(gunPosX, gunPosY, gunPosZ);
	glRotatef(-15.0 * yaw, 0.0, 1.0, 0.0);
	glRotatef(15.0 * pitch, 1.0, 0.0, 0.0);

	glTranslatef(1.0, -2.5, -3.0);

	// Base
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, bridge_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, bridge_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, bridge_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, bridge_mat_shininess);
	glScalef(2.0f, 2.0f, 2.0f);
	drawCube();
	glPopMatrix();

	// Barrel
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, gun_mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, gun_mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, gun_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, gun_mat_shininess);
	glTranslatef(0.0f, 0.0f, -2.0f);
	glRotatef(canAngle, 1.0, 0.0, 0.0);
	glScalef(1.5f, 1.5f, 5.0f);

	drawCube();
	glPopMatrix();

	glPopMatrix();
}

//////////////////////
// PROJECTILES
/////////////////////

void shootProjectile()
{
	Projectile newProj;

	newProj.projX = gunPosX;
	newProj.projY = gunPosY;
	newProj.projZ = gunPosZ;

	newProj.dirX = cos(yaw) * cos(pitch);
	newProj.dirY = sin(pitch);
	newProj.dirZ = sin(yaw) * cos(pitch);

	newProj.speed = 1.0f;
	newProj.projAlive = true;

	projectiles.push_back(newProj);
}

void blitzShootProjectile(BlitzInst& blitz)
{
	Projectile newProj;

	newProj.projX = blitz.bPosX - 6.0f;
	newProj.projY = blitz.bPosY + 4.0f;
	newProj.projZ = blitz.bPosZ;

	newProj.dirX = 0.0f;
	newProj.dirY = 0.0f;
	newProj.dirZ = 1.0f;
	newProj.speed = 2.0f;
	newProj.projAlive = true;

	blitz.blitzProj.push_back(newProj);
}

void updateProjectiles()
{
	for (auto& proj : projectiles) 
	{
		if (proj.projAlive) 
		{
			proj.projX += proj.dirX * proj.speed * curRound;
			proj.projY += proj.dirY * proj.speed * curRound;
			proj.projZ += proj.dirZ * proj.speed * curRound;

			if (proj.projZ > 100.0f || proj.projZ < -100.0f || proj.projX > 100.0f || proj.projX < -100.0f) 
			{
				proj.projAlive = false;
			}

		}
	}
}

void updateBlitzShooting(std::vector<BlitzInst>& blitzcranks)
{

	for (auto& blitz : blitzcranks)
	{
		if (stop)
		{
			blitz.blitzProj.clear();
		}
		else if (blitz.isAlive && (!stop) && (!blitz.deathAnim))
		{
			blitz.beamTimer += 0.016;

			if (blitz.beamTimer >= 1.5)
			{
				blitzShootProjectile(blitz);
				blitz.beamTimer = 0.0;
			}

			for (auto& proj : blitz.blitzProj)
			{

				if (proj.projAlive)
				{
					proj.projZ += proj.dirZ * proj.speed;

					if (proj.projZ >= 25.0f)
					{
						playerHP--;
						proj.projAlive = false;
						if (playerHP == 0)
						{
							glutTimerFunc(10, animationHandler, 10);
						}
					}

				}
			}
		}


	}
}

// There are many for loops and ifs in here but basically the first two checks per alive projectile, then
// next two iterates over every Blitzcrank Instance. There's really no way outside of O(n^2) I think huhu
void checkCollisions(std::vector<Proj>& projectiles, std::vector<BlitzInst>& blitzcranks) {
	for (auto& proj : projectiles)
	{
		if (proj.projAlive)
		{
			for (auto& blitz : blitzcranks)
			{
				if (blitz.isAlive)
				{
					VECTOR3D projPos(proj.projX, proj.projY, proj.projZ);
					if (isCollision(projPos, blitz.BlitzBox)) {
						blitz.bHP--;
						blitz.isHit = true;
						blitz.hitTimer = 0.0f;

						if (blitz.bHP <= 0)
						{
							blitz.currWalking = false;
							blitz.blitzProj.clear();

							blitz.legAngleL = 0;
							blitz.kneeAngle = 0;
							blitz.currWalking = false;
							blitz.walkReturn = false;

							blitz.deathAnim = true;
							glutTimerFunc(10, animationHandler, 4);
						}


						proj.projAlive = false;
						break;
					}
				}
			}
		}
	}
}

//////////////////////////////
/// BLITZ INST STUFF
/////////////////////////////

void spawnBlitzcrank() 
{
	BlitzInst newBlitz;

	// static cast helped fix some bug too idk what was the problem
	spawnX = -15.0 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (15.0 - (-15.0))));

	newBlitz.blitzID = blitzIDCount;
	newBlitz.bPosX = spawnX;
	newBlitz.bPosZ = spawnZ;

	newBlitz.legAngleL = 0.0;
	newBlitz.kneeAngle = 0.0;
	newBlitz.currWalking = true;
	newBlitz.cycleAnim = 1;

	newBlitz.bHP *= curRound;
	glutTimerFunc(10, animationHandler, 3);
	blitzcranks.push_back(newBlitz);
	blitzIDCount += 1;
}

void updateBlitzPositions(std::vector<BlitzInst>& blitzcranks)
{
	if (!stop)
	{
		for (auto& blitz : blitzcranks)
		{
			if (blitz.isAlive == false)
			{
				
			}
			else if (blitz.isAlive && blitz.currWalking)
			{
				blitz.bPosZ += 0.30;
				updateBoundingBox(blitz);
				if (blitz.bPosZ >= 23.0)
				{
					glutTimerFunc(10, animationHandler, 10);
				}
			}
			if (blitz.isHit)
			{
				blitz.hitTimer += 0.016;

				if (blitz.hitTimer >= 2.0) 
				{
					blitz.isHit = false;
					blitz.hitTimer = 0.0;
				}
			}
		}
	}

}

void checkClear() 
{
	roundClear = true;
	for (const auto& blitz : blitzcranks)
	{
		if (blitz.isAlive) 
		{
			roundClear = false;
			break;
		}
	}	
}

//////////////////
// HANDLERS
/////////////////

void updateHandler(int param)
{
	updateBlitzPositions(blitzcranks);
	updateProjectiles();
	updateBlitzShooting(blitzcranks);
	checkCollisions(projectiles, blitzcranks);
	checkClear();
	glutPostRedisplay();
	glutTimerFunc(16, updateHandler, 0);
}


void animationHandler(int param)
{
	if (!stop) 
	{
		// PLAYER DEATH CANON GONE
		if (param == 10)
		{
			if (canAngle > -25.0)
			{
				canAngle -= 0.5f;
			}
			else if (canAngle == -25.0)
			{
				inRound = false;
				stop = true;
				printf("You Lost! Press R to Retry.");
			}

		}
		for (auto& blitz : blitzcranks) 
		{
			switch (param) {
			case 3:
				if (blitz.currWalking) 
				{
					if (!blitz.walkReturn) 
					{
						if (blitz.legAngleL > -70.0) 
						{
							blitz.legAngleL -= 2.0;
							blitz.kneeAngle += 2.0;
						}
						else 
						{
							blitz.walkReturn = true;
						}
					}
					else 
					{
						if (blitz.kneeAngle > 0.0) 
						{
							blitz.kneeAngle -= 2.0;
							blitz.legAngleL += 2.0;
						}
						else 
						{
							blitz.walkReturn = false;
							//blitz.currWalking = false;
						}
					}
				}
				break;
			case 4:
				if (blitz.deathAnim)
				{
					blitz.deathTimer += 0.016f;

					if (blitz.shoulderAngle < 0) 
					{
						blitz.shoulderAngle += 2.0f;
					}

					if (blitz.legAngleR >= -90) 
					{
						blitz.legAngleR -= 2.0f;
						blitz.legAngleL -= 2.0f;
					}

					if (blitz.bPosY > -8.0f) 
					{
						blitz.bPosY -= 0.25f;
					}

					if (blitz.deathTimer >= 5.0) 
					{
						blitz.deathAnim = false;
						blitz.deathTimer = 0.0f;
						blitz.isAlive = false;
					}
				}
				
				break;
			}
		}
		glutTimerFunc(16, animationHandler, param);
		glutPostRedisplay();
	}
}

//////////////////
// GAME CONTROl
/////////////////

void resetGame() 
{
	curRound = 1;
	playerHP = 10;
	canAngle = 0.0f;
	stop = false;

	blitzcranks.clear();
	projectiles.clear();
	blitzIDCount = 0;
	inRound = false;
	yaw = 0.0f;
	pitch = 0.0f;
}


