#include "glut.h"
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT); //GLbitfield mask
	glRotatef(
		1,//GLfloat angle, 
		0, 0, 1 //GLfloat x, GLfloat y, GLfloat z
	);
	glutWireTeapot(1); //GLdouble size
	glutSwapBuffers();
}
void idle(void) {
	glutPostRedisplay();
}
int main(int argc, char *argv[]) {
	glutInit(
		&argc, //int *argcp
		argv); //char **argv
	glutInitDisplayMode(GL_DOUBLE); //unsigned int mode
	glutInitWindowPosition(640,0); //int x, y
	glutInitWindowSize(640,640);//int width, height
	glutCreateWindow("title");//const char *title
	glutDisplayFunc(display);//void (GLUTCALLBACK *func)(void)
	glutIdleFunc(idle); //void (GLUTCALLBACK *func)(void)
	glutMainLoop();
	return 0;
}