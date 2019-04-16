#ifndef _STAR // if this class hasn't been defined, the program can define it
#define _STAR // by using this if statement you prevent the class to be called more than once which would confuse the compiler
#include "ofMain.h"

class Star
{
public: 
	void setup(float size);
	void update(float xspd, float yspd, float cspd);
	void draw(); 

	// variables
	float x;
	float y;

	float r;
	float g;
	float b;

	float size;

	int numTraces = 6;
	deque<float> prevx;
	deque<float> prevy;

	// constructor
	Star();
	~Star();
};

#endif
