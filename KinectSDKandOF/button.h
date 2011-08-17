#ifndef _OF_BUTTON
#define _OF_BUTTON

#include <string>

class button{

public:
	//data
	int originX, originY;   //where the button starts
	int bWidth, bHeight;    //the size of the button
	string buttonName;
	ofTrueTypeFont font;
	ofColor selectedC, defaultC;
	int fontSize;
	bool trigger;
	
	//constructor
	button(string _buttonName, int _originX, int _originY, int _bWidth, int _bHeight, bool _trigger);
	
	//functionality
	void drawFont(bool _buttonPressed);
	bool buttonPressed(int x, int y);
};

#endif

