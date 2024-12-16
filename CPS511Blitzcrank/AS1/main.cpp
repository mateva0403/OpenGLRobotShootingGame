/*
#include <windows.h>
#include <gl/glut.h>
#include <math.h>
#include <stdio.h>

void initOpenGL(void);
void draw(void);

GLint windowWidth = 500;
GLint windowHeight = 500;
GLint viewportWidth = windowWidth;
GLint viewportHeight = windowHeight;

int main(int argc, char* argv[])
{
	glutInit(&argc, (char**)argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("MY FIRST ONE BRO");

	glutDisplayFunc(draw);

	initOpenGL();

	glutMainLoop();

	return 0;
}

void initOpenGL(void)
{
	glViewport(0, 0, viewportWidth, viewportHeight);
	glMatrixMode()
}

void draw(void)
{


}
*/