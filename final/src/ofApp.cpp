#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
	ofBackground(255);
    ofSetVerticalSync(false);
    ofSetFrameRate(120);
    cam.setup(640, 480);
    smile.setup();
	face.load("smile.png");
}

void ofApp::update() {
	cam.update();

    if(cam.isFrameNew()) {
        smile.update(cam);
        if(smile.getFaceFound()) {
            smileAmt = smile.getSmileAmount();
			//ofLog() << smileAmt << endl;
        }
    }

	// at an interval
	float now = ofGetElapsedTimef();
	if (now > nextResetTime){
		if (smileAmt == 0) {
			num += 1;
			xs.push_back(ofRandomWidth());
			ys.push_back(ofRandomHeight());
		}
		else {
			if (num > 0) {
				num -= 1;
				xs.pop_back();
				ys.pop_back();
			}
		}
		nextResetTime = now + resetIntervalTime;
	}
}

void ofApp::draw() {
	for (int i = 0; i < num; i++) {
		face.draw(xs[i], ys[i], face.getWidth()/8, face.getHeight()/8);
	}
    ofSetColor(255);
    cam.draw(ofGetScreenWidth()/2+cam.getWidth()/2, ofGetScreenHeight()/2-cam.getHeight()/2, -cam.getWidth(), cam.getHeight());
	smile.draw();
	ofSetColor(0);
	ofDrawBitmapString(ofToString(ofGetFrameRate()), 50, 50);
}
