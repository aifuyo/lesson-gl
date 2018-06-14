#include "glut.h"
void display(void) {
	glutWireTeapot(1); //GLdouble size
	glFlush();
}
int main(int argc, char *argv[]) {
	glutInit(
		&argc, //int *argcp
		argv); //char **argv
	glutInitWindowPosition(640,0); //int x, y
	glutInitWindowSize(640,640);//int width, height
	glutCreateWindow("title");//const char *title
	glutDisplayFunc(display);//void (GLUTCALLBACK *func)(void)
	glutMainLoop();
	return 0;
}