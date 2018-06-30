#pragma once
#include <stdlib.h>
#include "glm\glm.hpp"
#include "Ball.h"
using namespace glm;

struct Paddle {
	float m_height;
	vec2 m_position;

	bool intersectBall(Ball const& _ball);
	void draw();
};