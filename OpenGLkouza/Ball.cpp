#include "Ball.h"
#include "glut.h"
void Ball::update() {
	m_lastPosition = m_position;
	m_position += m_speed;
}

void Ball::draw() {
	glPushMatrix();
	{
		glTranslatef(m_position.x, m_position.y,0); //GLfloat x, y, z
		glScalef(16,16,0); //GLfloat x, y, z
		glutSolidSphere(
			1, 16, 16
		); //GLdouble radius, GLint slices, GLint stacks
	}
	glPopMatrix();
}