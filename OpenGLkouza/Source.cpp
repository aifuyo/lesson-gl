#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "glut.h"
#include "glm\glm.hpp"
#include "font.h"
#include "Ball.h"
#include "Paddle.h"
#include "audio.h"
using namespace glm;
#define BALL_MAX 256
#define PLAYER_MAX 2
#define SCORE_MAX 11
#define PADDLE_SPEED 8.f
#define BALL_Y_SPEED_MAX 8.f
ivec2 windowSize = {800,600};

bool keys[256];
Ball ball = {8};
Paddle paddles[PLAYER_MAX];
int scores[PLAYER_MAX];
int wait;
bool started;
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
	
	{
		GLfloat range[2];
		glGetFloatv(
			GL_LINE_WIDTH_RANGE,
			range);
		glLineWidth(range[1]/2);
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(
			range[1],
			0x5555
		);
		glBegin(GL_LINES);
		{
			for (int i = 0; i < 2; i++)
				glVertex2f(windowSize.x/2, windowSize.y*i);
		}
		glEnd();
		glPopAttrib();
	}

	if (wait <=0)
		ball.draw();
	
	if (started)
	for (int i = 0; i < PLAYER_MAX; i++)
		paddles[i].draw();

	fontBegin();
	
	fontSetHeight(FONT_DEFAULT_HEIGHT);
	fontSetPosition(0, 0);
	fontDraw("A\nbb\nccc\nddd");
	fontEnd();
	glutSwapBuffers();
}
void idle(void) {
	audioUpdate();
	if (started) {
		if (wait > 0) {
			wait--;

			if ((wait <= 0)
				&& (
				(scores[0] > SCORE_MAX)
					|| (scores[1] >= SCORE_MAX)
					)
				) {
				started = false;
			}
		}
		{
			if (keys['w']) paddles[0].m_position.y -= PADDLE_SPEED;
			if (keys['s']) paddles[0].m_position.y += PADDLE_SPEED;
			//if (keys['i']) paddles[1].m_position.y -= PADDLE_SPEED;
			//if (keys['k']) paddles[1].m_position.y += PADDLE_SPEED;
			{
				float centerY = paddles[1].m_position.y + paddles[1].m_height / 2;
				if (ball.m_position.y < centerY-PADDLE_SPEED)
					paddles[1].m_position.y -= PADDLE_SPEED;
				if (ball.m_position.y > centerY+PADDLE_SPEED)
					paddles[1].m_position.y += PADDLE_SPEED;
			}

			for (int i = 0; i < PLAYER_MAX; i++) {
				paddles[i].m_position.y = max(paddles[i].m_position.y, 0.f);
				paddles[i].m_position.y = min(paddles[i].m_position.y, windowSize.y - paddles[i].m_height);
			}
		}
	}
	if (wait <= 0) {
		ball.update();

		if (
			(ball.m_position.x < 0)
			|| (ball.m_position.x >= windowSize.x)
			) {
			if (started) {
				audioLength(1000);
				audioDecay(0);
				audioFreq(440/4);
				audioPlay();
				if (ball.m_position.x < 0)
					scores[1]++;
				else
					scores[0]++;
				wait = 60;
				ball.m_position.x = windowSize.x / 2;
				ball.m_lastPosition = ball.m_position;
				ball.m_speed.y = max(ball.m_speed.y, -PADDLE_SPEED);
				ball.m_speed.y = min(ball.m_speed.y, PADDLE_SPEED);
			}
			else {
				ball.m_position = ball.m_lastPosition;
				ball.m_speed.x *= -1;
			}
		}
		if (
			(ball.m_position.y < 0)
			|| (ball.m_position.y >= windowSize.y)
			) {

			if (started) {
				audioDecay(.9);
				audioFreq(440/2);
				audioPlay();
			}
			ball.m_position = ball.m_lastPosition;
			ball.m_speed.y *= -1;
		}

		if (started)
		{
			for (int i = 0; i < PLAYER_MAX; i++) {
				if (paddles[i].intersectBall(ball)) {
					//audioWaveform(AUDIO_WAVEFORM_PULSE_12_5);
					audioDecay(.9);
					audioFreq(440);
					audioPlay();
					ball.m_position = ball.m_lastPosition;
					ball.m_speed.x *= -1;

					float paddleCenterY = paddles[i].m_position.y + paddles[i].m_height / 2;
					float subMax = paddles[i].m_height/2;
					ball.m_speed.y = (ball.m_position.y - paddleCenterY) / subMax * BALL_Y_SPEED_MAX;
				}
			}
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
	printf("keyboard: \'%c\'(%#x)\n",key,key);

	//audioLength(100);
	//audioDecay(.9);
	//audioPitchTarget(.25);
	//audioSweep(.99);
	//audioFreq(440);
	switch (key) {
	case  0x1b:
		exit(0);
		break;
	case 'p':
		audioFreq(440);
		audioSweep(1.01,440*2);
		audioPlay();
		break;
	case 's':
		audioStop();
		break;
	}
	if (!started) {
		for (int i = 0; i < PLAYER_MAX; i++) {
			scores[i] = 0;
			paddles[i].m_height = 32;
			paddles[i].m_position.y = (windowSize.y - paddles[i].m_height) / 2;

		}
		paddles[0].m_position.x = 100;
		paddles[1].m_position.x = windowSize.x - 100;

		ball.m_position =
		ball.m_lastPosition = vec2(windowSize.x / 2, windowSize.y / 2);
		started = true;
	}
	
	/*if ((key >= '1') && (key <= '5')) {
		audioWaveform(key - '1');
		audioStop();
		audioPlay();
	}*/
	if ((key >= '0') && (key <= '9')) {
		audioStop();
		int k = key - '0';
		audioWaveform(AUDIO_WAVEFORM_PULSE_50);
		audioFreq(440 * powf(2, (1+k/12.f)));
		audioPlay();
	}
	keys[key] = true;
}
void keyboardUp(unsigned char key, int x, int y) {
//	printf("keyboardUp: \'%c\'(%#x)\n", key, key);
	keys[key] = false;
}
void passiveMotion(int x, int y) {
	//printf("passiveMotion: x:%d y:%d\n", x,y);
}

int main(int argc, char *argv[]) {
	srand(time(NULL));
	if (audioInit() != 0)
		return 1;
	ball.m_speed = vec2(1, 1)*4.f;
	
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
	glutPassiveMotionFunc(passiveMotion);
	glutKeyboardUpFunc(keyboardUp); //void (GLUTCALLBACK *func)(unsigned char key, int x, int y)
	glutMainLoop();
	return 0;
}