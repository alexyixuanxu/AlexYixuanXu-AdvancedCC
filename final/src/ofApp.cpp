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
	text.push_back("1 1 1 1 1 1 1 1 1 1");
	text.push_back("Smile.");
	text.push_back("3 3 3 3 3 3 3 3 3 3");
	text.push_back("Smile.");
	text.push_back("end end end");

	// smile text
	smileReqTexts.push_back("Bigger smile.");
	smileReqTexts.push_back("Smile.");
	smileReqTexts.push_back("Bigger.");
	smileReqTexts.push_back("Just smile.");
	smileReqTexts.push_back("Smile, smile, smile.");

	// images
	images.resize(imgSize);
	for (int i = 0; i < imgSize; i++) {
		loader.loadFromDisk(images[i], "images/"+ofToString(i)+".png");
	}

	// sounds
	bgm.load("sounds/limbo.wav");
	bgm.setLoop(true);
	bgm.play();

	// font
	textFont.load("font/manic-depressive.ttf", 24);


	ofSetRectMode(OF_RECTMODE_CORNER);
	// start button
	startButton.x = 500;
	startButton.y = 500;
	startButton.width = 50;
	startButton.height = 50;

	// next button
	nextButton.x = 500;
	nextButton.y = 500;
	nextButton.width = 50;
	nextButton.height = 50;

	// replay button
	replayButton.x = 500;
	replayButton.y = 500;
	replayButton.width = 50;
	replayButton.height = 50;
}

void ofApp::update() {
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
		ofPushStyle();
		if (startButton.getX() <= mouseX <= startButton.getX() + startButton.getWidth() &&
			startButton.getY() <= mouseY <= startButton.getY() + startButton.getHeight()) { // hover start button
			ofSetColor(0, 255, 0);
		}
		else { // draw start button
			ofSetColor(255);
		}
		ofDrawRectangle(startButton);
		ofPopStyle();
	}
	else if (stage > 0 && stage < text.size()) { // text screen
		displayScene(text[stage - 1]);
		if (camOn) {
			testSmile(100, smileAmt);
		}
		if (next) { // display next button
			ofPushStyle();
			// when mouse hover, draw next button bigger
			if (nextButton.getX() <= mouseX <= nextButton.getX() + nextButton.getWidth() &&
				nextButton.getY() <= mouseY <= nextButton.getY() + nextButton.getHeight()) {
				ofSetColor(255, 0, 0);
			}
			else {
				// draw next button
				ofSetColor(255);
			}
			ofDrawRectangle(nextButton);
			ofPopStyle();
		}
	}
	else if (stage == text.size()) { // end screen
		ofPushStyle();
		if (replayButton.getX() <= mouseX <= replayButton.getX() + replayButton.getWidth() &&
			replayButton.getY() <= mouseY <= replayButton.getY() + replayButton.getHeight()) { // hover start button
			ofSetColor(0, 0, 255);
		}
		else { // draw start button
			ofSetColor(255);
		}
		ofDrawRectangle(replayButton);
		ofPopStyle();
	}
}

void ofApp::mouseReleased(int x, int y, int button) {
	if (stage == 0) {
		if (startButton.getX() <= mouseX <= startButton.getX() + startButton.getWidth() &&
			startButton.getY() <= mouseY <= startButton.getY() + startButton.getHeight()) { // start button
			stage += 1;
			resetScene();
		}
	}
	else if (stage > 0 && stage < text.size()) {
		if (next) {
			if (nextButton.getX() <= mouseX <= nextButton.getX() + nextButton.getWidth() &&
				nextButton.getY() <= mouseY <= nextButton.getY() + nextButton.getHeight()) {
				stage += 1;
				resetScene();
			}
		}
	}
	else if (stage >= text.size()) { // ending
		if (replayButton.getX() <= mouseX <= replayButton.getX() + replayButton.getWidth() &&
			replayButton.getY() <= mouseY <= replayButton.getY() + replayButton.getHeight()) { // start button
			stage = 0;
			resetScene();
		}
	}
}
void ofApp::displayScene(string sceneText) {
	// if we are going to use camera
	if (sceneText == "Smile.") {
		ofPushStyle();
		ofSetColor(255);
		textFont.drawString(smileText, ofGetScreenWidth() / 10, ofGetScreenHeight() - ofGetScreenHeight() / 5);
		ofPopStyle();

		camOn = true;
	}
	else {
		camOn = false;
		vector<string> splitText = ofSplitString(sceneText, " ");
		string tempText = "";
		for (int j = 0; j < wordIndex; j++) {
			tempText += splitText[j] + " ";
		}
		float now = ofGetElapsedTimef();
		if (now > nextResetTime) {
			if (wordIndex < splitText.size()) {
				wordIndex += 1;
			}
			else if (wordIndex >= splitText.size()) {
				next = true;
				wordIndex = splitText.size();
			}
			nextResetTime = now + resetIntervalTime;
		}
		ofPushStyle();
		ofSetColor(255);
		textFont.drawString(tempText, ofGetScreenWidth() / 10, ofGetScreenHeight() - ofGetScreenHeight() / 5);
		ofPopStyle();
	}

}

void ofApp::resetScene() {
	next = false;

	nextResetTime = 0;
	resetIntervalTime = 0.2;
	wordIndex = 0;

	float nextSmileResetTime = 0;
	float resetSmileIntervalTime = 1;
}

void ofApp::testSmile(float smileReq, float smileAmt) {
	// how much smile indicator
	ofPushStyle();
	ofSetColor(255);
	if (smileAmt <= smileReq && !next) {
		float now = ofGetElapsedTimef();
		if (now > nextSmileResetTime) {
			smileText = smileReqTexts[ofRandom(smileReqTexts.size())];
			nextSmileResetTime = now + resetSmileIntervalTime;
		}
	}
	else if (smileAmt >= smileReq) {
		smileText = "Good.";
		next = true;
	}
	ofPopStyle();
}
