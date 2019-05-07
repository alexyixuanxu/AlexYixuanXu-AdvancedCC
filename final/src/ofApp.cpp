#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
	ofBackground(10);

	// cam and smile detection
    ofSetVerticalSync(false);
    ofSetFrameRate(60);
    cam.setup(640, 480);
    smile.setup();

	// text
	text.push_back("ONE ONE ONE ONE ONE");
	text.push_back("TWO TWO TWO TWO");
	text.push_back("THREE THREE THREE");

	// images
	images.resize(imgSize);
	for (int i = 0; i < imgSize; i++) {
		loader.loadFromDisk(images[i], "images/"+ofToString(i)+".png");
	}

	// font
	textFont.load("font/manic-depressive.ttf", 24);
}

void ofApp::update() {
	if (stage >= imgSize) {
		stage = 0;
	}
	if (camOn) {
		cam.update();

		if (cam.isFrameNew()) {
			smile.update(cam);
			if (smile.getFaceFound()) {
				smileAmt = smile.getSmileAmount();
				//ofLog() << smileAmt << endl;
				// 10 - 200
			}
		}
	}

}

void ofApp::draw() {
	images[stage].draw(ofRandom(-3, 3), ofRandom(-3, 3), ofGetScreenWidth(), ofGetScreenHeight());
	if (stage == 0) { // start screen

		if (mouseX == 50 && mouseY == 60) { // start button

		}
	}
	else if (stage > 0 && stage < images.size()) { // text screen
		if (stage == 11) {
			camOn = true;
			displayScene(text[stage]);
			// how much smile indicator
			//ofDrawBitmapString(ofToString(ofGetFrameRate()), 50, 50);
			//ofDrawBitmapString(ofToString(smileAmt), 50, 100);
		}
		else if (stage == images.size()-1) {
			//cam.draw(ofGetScreenWidth()/2+cam.getWidth()/2, ofGetScreenHeight()/2-cam.getHeight()/2, -cam.getWidth(), cam.getHeight());
		}
		else {
			camOn = false;
			displayScene(text[stage]);
		}
	}
	else if (stage == images.size()) { // end screen
		// restart button
	}

	if (nextButton) {
		// draw next button

		// when mouse hover, draw next button bigger

	}
}

void ofApp::mouseReleased(int x, int y, int button) {
	//stage += 1;
	if (stage == 0) {
		if (mouseX == 50 && mouseY == 60) { // start button
			stage += 1;
			resetScene();
		}
	}
	else if (stage > 0 && stage < images.size()) {
		if (nextButton) {
			if (mouseX == 50 && mouseY == 60) {
				stage += 1;
				resetScene();
			}
		}
	}
	else if (stage == images.size()) {

	}
}
void ofApp::displayScene(string sceneText) {
	vector<string> splitText = ofSplitString(sceneText, " ");
	string tempText = "";
	for (int j = 0; j < wordIndex; j++) {
		tempText += splitText[j]+" ";
	}
	float now = ofGetElapsedTimef();
	if (now > nextResetTime){
		if (wordIndex < splitText.size()) {
			wordIndex += 1;
		}
		else if (wordIndex >= splitText.size()) {
			nextButton = true;
			wordIndex = splitText.size();
		}
		nextResetTime = now + resetIntervalTime;
	}
	ofSetColor(255);
	textFont.drawString(tempText, ofGetScreenWidth() / 10, ofGetScreenHeight() - ofGetScreenHeight() / 5);
}

void ofApp::resetScene() {
	nextButton = false;

	nextResetTime = 0;
	resetIntervalTime = 0.2;
	wordIndex = 0;
}
