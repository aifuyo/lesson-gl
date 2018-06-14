#include <stdio.h>
#include "glut.h"

int windowWidth = 800;
int windowHeight = 600;

bool keys[256];

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT); //GLbitfield mask
	glMatrixMode(GL_PROJECTION); //GLenum mode
	glLoadIdentity();
	gluOrtho2D(
		0,//GLdouble left,
		windowWidth,//GLdouble right,
		windowHeight,//GLdouble bottom,
		0//GLdouble top
	);
	glMatrixMode(GL_MODELVIEW); //GLenum mode
	glLoadIdentity();
	glTranslatef(windowWidth / 2, windowHeight / 2, 0); //GLfloat x, GLfloat y, GLfloat z
	static float angle;
	//angle += 1;
	if (keys['d'])		angle += 1;
	if (keys['a'])		angle -= 1;
	glRotatef(
		angle,//GLfloat angle, 
		0, 0, 1 //GLfloat x, GLfloat y, GLfloat z
	);
	glScalef(256, 256, 1); //GLfloat x, GLfloat y, GLfloat z
	glutWireTeapot(1); //GLdouble size
	glutSwapBuffers();
}
void idle(void) {
	glutPostRedisplay();
}
void reshape(int width, int height) {
	//printf("reshape: width:%d height:%d\n", width, height);
	glViewport(
		0,0,//GLint x, GLint y, 
		windowWidth, windowHeight//GLsizei width, GLsizei height
	);
	windowWidth = width;
	windowHeight = height;
}
void (keyboard)(unsigned char key, int x, int y) {
	if (key == 0x1b)
		exit(0);
	printf("keyboard: \'%c\'(%#x)\n",key,key);
	keys[key] = true;
}
void keyboardUp(unsigned char key, int x, int y) {
	printf("keyboardUp: \'%c\'(%#x)\n", key, key);
	keys[key] = false;
}
int main(int argc, char *argv[]) {
	glutInit(
		&argc, //int *argcp
		argv); //char **argv
	glutInitDisplayMode(GL_DOUBLE); //unsigned int mode
	glutInitWindowPosition(640,0); //int x, y
	glutInitWindowSize(windowWidth,windowHeight);//int width, height
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