#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "glut.h"
#include "glm\glm.hpp"
#include "font.h"
#include "Ball.h"
using namespace glm;
#define BALL_MAX 256
ivec2 windowSize = {800,600};

bool keys[256];

Ball balls[256];
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT); //GLbitfield mask
	glMatrixMode(GL_PROJECTION); //GLenum mode
	glLoadIdentity();
	gluOrtho2D(
		0,//GLdouble left,
		windowSize.x,//GLdouble right,
		windowSize.y,//GLdouble bottom,
		0//GLdouble top
	);
	glMatrixMode(GL_MODELVIEW); //GLenum mode
	glLoadIdentity();

	unsigned char colors[6][3] = {
		{ 0xff,0x00,0x00},
		{ 0x00,0xff,0x00 },
		{ 0x00,0x00,0xff },
		{ 0xff,0xff,0x00 },
		{ 0x00,0xff,0xff },
		{ 0xff,0x00,0xff }
	};

	for (int i = 0; i < BALL_MAX; i++)
	{
		glColor3ub(
			colors[i % 6][0],
			colors[i % 6][1],
			colors[i % 6][2]
			);
		balls[i].draw();
	}
	
	fontBegin();
	fontSetColor(0, 0xff, 0);
	fontSetSize(FONT_DEFAULT_SIZE);
	float lineHeight = fontGetSize() * 1.5;
	float y = windowSize.y - lineHeight * 2;
	fontSetPosition(0,y );
	fontSetWeight(fontGetWeightMin());
//	fontDraw("min:%f", fontGetWeightMin());
	
	fontEnd();
	glutSwapBuffers();
}
void idle(void) {
	for (int i = 0; i < BALL_MAX; i++) {
		balls[i].update();
		if (balls[i].m_position.y < 0) {
			balls[i].m_position = balls[i].m_lastPosition;
			balls[i].m_speed.y = fabs(balls[i].m_speed.y);
		}
		if (balls[i].m_position.x < 0) {
			balls[i].m_position = balls[i].m_lastPosition;
			balls[i].m_speed.x = fabs(balls[i].m_speed.x);
		}
		if (balls[i].m_position.x >= windowSize.x) {
			balls[i].m_position = balls[i].m_lastPosition;
			balls[i].m_speed.x = -fabs(balls[i].m_speed.x);
		}
		if (balls[i].m_position.y >= windowSize.y) {
			balls[i].m_position = balls[i].m_lastPosition;
			balls[i].m_speed.y = -fabs(balls[i].m_speed.y);
		}
	}
	glutPostRedisplay();
}
void reshape(int width, int height) {
	//printf("reshape: width:%d height:%d\n", width, height);
	glViewport(
		0,0,//GLint x, GLint y, 
		width, height//GLsizei width, GLsizei height
	);
	windowSize = ivec2(width,height);	
}
void (keyboard)(unsigned char key, int x, int y) {
	if (key == 0x1b)
		exit(0);
//	printf("keyboard: \'%c\'(%#x)\n",key,key);
	keys[key] = true;
}
void keyboardUp(unsigned char key, int x, int y) {
//	printf("keyboardUp: \'%c\'(%#x)\n", key, key);
	keys[key] = false;
}
int main(int argc, char *argv[]) {
	srand(time(NULL));
	for (int i = 0; i < BALL_MAX; i++) {
		balls[i].m_position = vec2(
			rand() % windowSize.x,
			rand() % windowSize.y
		);
		balls[i].m_speed = 
			normalize(vec2((float)rand()/RAND_MAX -.5f,
			(float)rand()/RAND_MAX-.5f));

	}
	glutInit(
		&argc, //int *argcp
		argv); //char **argv
	glutInitDisplayMode(GL_DOUBLE); //unsigned int mode
	glutInitWindowPosition(640,0); //int x, y
	glutInitWindowSize(windowSize.x, windowSize.y);//int width, height
	glutCreateWindow("title");//const char *title
	glutDisplayFunc(display);//void (GLUTCALLBACK *func)(void)
	glutIdleFunc(idle); //void (GLUTCALLBACK *func)(void)
	glutReshapeFunc(reshape); //void (GLUTCALLBACK *func)(int width, int height)
	glutKeyboardFunc(keyboard);//void (GLUTCALLBACK *func)(unsigned char key, int x, int y)
	glutIgnoreKeyRepeat(GL_TRUE); //int ignore
	glutKeyboardUpFunc(keyboardUp); //void (GLUTCALLBACK *func)(unsigned char key, int x, int y)
	glutMainLoop();
	return 0;
}