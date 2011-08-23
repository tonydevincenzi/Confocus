#ifndef _OF_BUBBLE		// by using this if statement you prevent the class to be called more than once - in other words what we're saying here is 
#define _OF_BUBBLE		//if the class has NOT been defined then define it

class talkBubble {
	
	
	
public:   
	
	void update();
	void draw();
	void updatePosition(float x, float y, int z);
	void updateAttributes(int active, string name, int talkTime);
	void setPosition();
	
	talkBubble(float x, float y, int z,int orientation, string message, int talkTime);
	
	ofImage	backgroundImage;
	ofImage	backgroundImage_r;
	ofTrueTypeFont font;
	
	float x;
	float y;
	int z;
	int scale;
	int orientation;
	
	int min, sec, secsInMins;
	string cleanSeconds;
	
	int absoluteX, absoluteY, absoluteZ;
	int bubbleState; // Determines state of bubble (has it been set?)

	int active;
	string name;
	int talkTime;
	string message;
	
	

private:
	
	
};						//don't forget the semicolon in the end of the class definition

#endif 


