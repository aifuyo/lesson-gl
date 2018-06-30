#pragma once
#define FONT_DEFAULT_WIDTH (80.f)
#define FONT_DEFAULT_HEIGHT (100.f)
void fontBegin();
void fontEnd();

void fontSetPosition(float _x, float _y);
void fontSetHeight(float _height);
float fontGetHeight();

float fontGetWeightMin();
float fontGetWeightMax();
void fontSetWeight(float _weight);
float fontGetWeight();

void fontSetColor(unsigned char _red, unsigned char _green,
	unsigned char _blue);
void fontDraw(const char* _format, ...);