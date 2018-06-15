#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "font.h"
#include "glut.h"
#include "glm\glm.hpp"
using namespace glm;
static vec2 position;
static float size = FONT_DEFAULT_SIZE;
static unsigned char color[3];
static float weight = 1;
void fontBegin() {
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS); //GLbitfield mask
	glMatrixMode(GL_PROJECTION); //GLenum mode
	glLoadIdentity();
	GLint viewport[4];
	glGetIntegerv(
		GL_VIEWPORT,//GLenum pname, 
		viewport //GLint *params
	);
	gluOrtho2D(
		0,//GLdouble left,
		viewport[2],//GLdouble right,
		viewport[3],//GLdouble bottom,
		0//GLdouble top
	);
	glMatrixMode(GL_MODELVIEW); //GLenum mode
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST); //GLenum cap
	glDisable(GL_LIGHTING); //GLenum cap
	glDisable(GL_TEXTURE_2D); //GLenum cap
}
void fontEnd() {
	glPopMatrix();
	glPopAttrib();
}

void fontSetPosition(float _x, float _y) {
	position = vec2(_x, _y);
}
void fontSetSize(float _size) {
	size = _size;
}
float fontGetSize() {
	return size;
}
float fontGetWeightMin() {
	GLfloat weight[2];
	glGetFloatv(
		GL_LINE_WIDTH_RANGE,
		weight
	);
	return weight[0];
}
float fontGetWeightMax() {
	GLfloat weight[2];
	glGetFloatv(
		GL_LINE_WIDTH_RANGE,
		weight
	);
	return weight[1];
}
void fontSetWeight(float _weight) {
	weight = _weight;
}
void fontSetColor(unsigned char _red, unsigned char _green,
	unsigned char _blue) {
	color[0] = _red;
	color[1] = _green;
	color[2] = _blue;
}
void fontDraw(const char* _format, ...) {
	va_list argList;
	va_start(argList, _format);
	char str[256];
	vsprintf_s(str, _format, argList);
	va_end(argList);

	glLineWidth(weight);

	glColor3ub(color[0], color[1], color[2]); //GLubyte red, GLubyte green, GLubyte blue
	glPushMatrix();
	{
		glTranslatef(position.x, position.y+size, 0);
		float s = size/FONT_DEFAULT_SIZE;
		glScalef(s, -s, s); //GLfloat x, GLfloat y, GLfloat z
		for (char *p = str; *p != '\0'; p++)
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *p); 
	}
	glPopMatrix();
//	printf("%s\n", str);
}