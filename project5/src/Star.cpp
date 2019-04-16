#include "Star.h"

Star::Star()
{
	for (int i = 0; i < numTraces; i++) {
		prevx.push_back(x);
		prevy.push_back(y);
	}
}


Star::~Star()
{
}

void Star::setup(float star_size) {
	x = ofRandom(ofGetScreenWidth());
	y = ofRandom(ofGetScreenHeight());

	size = star_size;

	r = 205;
	g = 205;
	b = 205;
}

void Star::update(float xspd, float yspd, float cspd) {
	// circle gets to very left, move it to right
	// very right, move it left
	// ...
	// ...
	if (x <= -size / 2) {
		x = ofGetScreenWidth() + size / 2;
	}
	else if (x >= ofGetScreenWidth() + size / 2) {
		x = -size / 2;
	}
	if (y <= -size / 2) {
		y = ofGetScreenHeight() + size / 2;
	}
	else if (y >= ofGetScreenHeight() + size / 2) {
		y = -size / 2;
	}

	x += xspd;
	y += yspd;

	prevx.pop_front();
	prevx.push_back(x);
	prevy.pop_front();
	prevy.push_back(y);

	// change color based on z
	r += ofRandom(-abs(cspd), +abs(cspd));
	if (r <= 0) {
		r = 0;
	}
	else if (r >= 255) {
		r = 255;
	}

	g += ofRandom(-abs(cspd), +abs(cspd));
	if (g <= 0) {
		g = 0;
	}
	else if (g >= 255) {
		g = 255;
	}

	b += ofRandom(-abs(cspd), +abs(cspd));
	if (b <= 0) {
		b = 0;
	}
	else if (g >= 255) {
		b = 255;
	}
}

void Star::draw() {
	ofPushStyle();
	ofFill();
	// draw traces
	for (int i = 0; i < prevx.size(); i++) {
		ofSetColor(r, g, b, 255*i / prevx.size());
		//float traceSize = i*size/prevx.size();
		ofDrawCircle(prevx[i], prevy[i], size);
	}
	// draw the star
	ofSetColor(r, g, b, 255);
	ofDrawCircle(x, y, size);
	ofPopStyle();
}
