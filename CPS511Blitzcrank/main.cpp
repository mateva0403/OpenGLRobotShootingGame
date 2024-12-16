/*

BLITZCRANK ARAM
By: Matthew Evangelista 501158179

*/

#include <stdio.h>
#include <windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <gl/glut.h>
#include "blitzcrank.h"
#include "surfaceModeller.h"
#include "subdivcurve.h"

// PROTOTYPES

void initOpenGL(int w, int h);
void mouse(int button, int state, int x, int y);
void mouseMotionHandler(int xMouse, int yMouse);
void display(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);

// NOT WORKY initShaders(); 

// VARS
// VP/CAM
GLint windowWidth = 1280;
GLint windowHeight = 720;
GLint vWidth = windowWidth;
GLint vHeight = windowHeight;

// SHADERS (DISREGARD)
GLuint program = 0;
GLuint texture = 0;

// M&K
int currentButton;

int main(int argc, char* argv[])
{
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(vWidth, vHeight);
	glutInitWindowPosition(200, 30);
	glutCreateWindow("BLITZCRANK");
	glewInit();
	initOpenGL(vWidth, vHeight);
	glutWarpPointer(windowWidth / 2, windowHeight / 2);
	initCube();
	/*
	
	ORIGINALLY: IMPORTED CUSTOM MESH. MEMORY ACCESS ERROR I HAVE NO IDEA WHY AND I CANNOT BE BOTHERED ANYMORE.

	loadCannonOBJ("exportedMesh.obj");
	initCannonVAO

	*/
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(mouseMotionHandler);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(functionKeys);
	glutTimerFunc(16, updateHandler, 0);
	glutMainLoop();
	return 0;
}

void initOpenGL(int w, int h)
{

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	glViewport(0, 0, vWidth, vHeight);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.5F, 0.5F, 1.0F, 0.0F);
	glClearDepth(1.0f);
	glEnable(GL_NORMALIZE); 
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
}

// This was here for debug.
bool camOn = true;

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	lookAtX = gunPosX + cos(yaw) * cos(pitch);
	lookAtY = gunPosY + sin(pitch);
	lookAtZ = gunPosZ + sin(yaw) * cos(pitch);
	
	gluLookAt(gunPosX, gunPosY, gunPosZ, lookAtX, lookAtY, lookAtZ, 0.0, 1.0, 0.0);

	// NO WORKY renderCannon();
	drawCannon();
	drawEnvironment();

	// GAME/ROUND 

	if (!inRound)
	{
		printf("Round: %d\n", curRound);
		blitzcranks.clear();
		projectiles.clear();
		for (int i = 0; i < curRound; i++)
		{
			spawnBlitzcrank();
		}
		inRound = true;
		roundClear = false;
	}

	if (roundClear)
	{
		inRound = false;
		playerHP += 5;
		curRound++;
	}

	for (const auto& proj : projectiles) 
	{
		if (proj.projAlive) {
			glPushMatrix();
			glTranslatef(proj.projX, proj.projY, proj.projZ);
			drawProjectile();
			glPopMatrix();
		}
	}

	for (const BlitzInst& blitz : blitzcranks) {

			if (blitz.isAlive)
			{
				glPushMatrix();
				glTranslatef(blitz.bPosX, 0.0, blitz.bPosZ);
				drawRobot(blitz);
				glPopMatrix();
			}
			
			for (const auto& proj : blitz.blitzProj) {
				if (proj.projAlive) {
					glPushMatrix();
					glTranslatef(proj.projX, proj.projY, proj.projZ);
					drawProjectile();
					glPopMatrix();
				}
			}
	}
	updateBlitzPositions(blitzcranks);
	glutSwapBuffers();
}

void mouse(int button, int state, int x, int y)
{
	
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)w / h, 0.2, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(gunPosX, gunPosY, gunPosZ, lookAtX, lookAtY, lookAtZ, 0.0, 1.0, 0.0);
}

// Mouse TEMPs
int lastX, lastY;
void mouseMotionHandler(int x, int y)
{
	static int lastX = x, lastY = y;
	if (camOn == true) {

		int deltaX = x - lastX;
		int deltaY = y - lastY;

		yaw += deltaX * 0.01f; 
		pitch -= deltaY * 0.01f;

		if (yaw < 0.0f) yaw += 360.0f;
		if (yaw >= 360.0f) yaw -= 360.0f;

		lastX = x;
		lastY = y;

		updateCannonLookAt();
	}
	
	glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case ' ':
		if (playerHP > 0)
		{
			shootProjectile();
		}
		break;
	case 'r':
		resetGame();
		break;
	case 's':
		glutTimerFunc(10, animationHandler, 1);
		break;
	case 'S':
		stop = true;
		break;
	case 'c':
		glutTimerFunc(10, animationHandler, 2);
		break;
	case 'W':
		for (auto& blitz : blitzcranks) {
			blitz.legAngleL = 0;
			blitz.kneeAngle = 0;
			blitz.currWalking = false;
			blitz.walkReturn = false;
		}
		break;
	case 'n':
		break;
	case 'm':
		blitzcranks[0].isAlive = false;
		break;
	case 'p': 
		if (!blitzcranks.empty()) {
			BlitzInst& blitz = blitzcranks[0];
			blitz.legAngleL = 0; 
			blitz.kneeAngle = 0;
			blitz.currWalking = false; 
			blitz.walkReturn = false;
			blitz.cycleAnim = 0;
		}
		break;
	}
	
	glutPostRedisplay(); 
}

void functionKeys(int key, int x, int y)
{
	switch (key) 
	{
		case GLUT_KEY_LEFT:
			yaw -= 0.1f; 
			if (yaw < 0.0f) yaw += 360.0f;
			break;

		case GLUT_KEY_RIGHT:
			yaw += 0.1f;   
			if (yaw >= 360.0f) yaw -= 360.0f;
			break;

		case GLUT_KEY_UP: 
			pitch += 0.1f;
			if (pitch > 89.0f) pitch = 89.0f;
			break;

		case GLUT_KEY_DOWN:
			pitch -= 0.1f;
			if (pitch < -89.0f) pitch = -89.0f; 
			break;
	}
	glutPostRedisplay();
}