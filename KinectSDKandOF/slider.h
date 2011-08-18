#ifndef _OF_SLIDER
#define _OF_SLIDER

#include <string>

class slider{

public:
	//data
	int originX,originY;
	int sWidth;
	int value;
	int sliderPosX,sliderPosY;
	ofTrueTypeFont font;
	ofImage img;

	slider(int x, int y, int sWidth);

	void drawSlider(int startFrom, int endTo);
	void getSliderPosX(int _mouseX, int _mouseY);
};

#endif
