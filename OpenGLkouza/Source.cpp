#include <stdlib.h>
#include <stdio.h>
#include "glut.h"
#include "glm\glm.hpp"
#include "font.h"
#include "Rect.h"
using namespace glm;
ivec2 windowSize = {800,600};

bool keys[256];

Rect rect1 = Rect(vec2(100,100),vec2(100,200));
Rect rect2 = Rect(vec2(windowSize.x/2, windowSize.y/2), vec2(200, 100));
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
	if (rect1.intersect(rect2))
		glColor3ub(0xff, 0x00, 0x00);
	else 
		glColor3ub(0x00, 0x00, 0xff);
	
	rect1.draw();
	glColor3ub(0x00, 0xff, 0x00);
	rect2.draw();

	
	fontBegin();
	fontSetColor(0, 0xff, 0);
	fontSetSize(FONT_DEFAULT_SIZE);
	float lineHeight = fontGetSize() * 1.5;
	float y = windowSize.y - lineHeight * 2;
	fontSetPosition(0,y );
	fontSetWeight(fontGetWeightMin());
	fontDraw("min:%f", fontGetWeightMin());
	
	fontSetWeight(fontGetWeightMax());
	fontSetPosition(0, y+=lineHeight);
	fontDraw("max:%f", fontGetWeightMax());
	fontEnd();
	glutSwapBuffers();
}
void idle(void) {
	float f = 2;
	if (keys['w']) rect1.m_position.y -= f;
	if (keys['s'])rect1.m_position.y += f;
	if (keys['a'])rect1.m_position.x -= f;
	if (keys['d'])rect1.m_position.x += f;
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