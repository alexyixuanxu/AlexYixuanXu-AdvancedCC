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
	text.push_back("\"Oh no I am so, so, so sorry.\" She said."); // img 1
	text.push_back("You looked at your stained dress."); // 2
	text.push_back("I don't want her to feel awkward, you thought."); // 3
	text.push_back("\"Oh it's okay.\" You said."); // 4
	text.push_back("And smiled."); // 5
	text.push_back("Smile."); // 6

	text.push_back("He told a joke."); // 7
	text.push_back("Everyone laughed."); // 8
	text.push_back("So you laughed with them."); // 9
	text.push_back("Smile."); // 10

	text.push_back("The noises were giving you headache.");  // 11
	text.push_back("But you said \"thank you for having me.\""); // 12
	text.push_back("And gave the host a bright smile."); // 13
	text.push_back("Smile."); // 14

	text.push_back("You dragged your body back."); // 15
	text.push_back("You looked into the mirror,"); // 16
	text.push_back("and smile."); // 17
	text.push_back("Smile."); // 18
	text.push_back(" "); // 19

	// smile text
	smileReqTexts.push_back("Bigger smile.");
	smileReqTexts.push_back("Smile.");
	smileReqTexts.push_back("Bigger.");
	smileReqTexts.push_back("Just smile.");
	smileReqTexts.push_back("Smile, smile, smile.");

	// images
	images.resize(imgSize);
	for (int i = 0; i < imgSize; i++) {
		loader.loadFromDisk(images[i], "images/"+ofToString(i)+".jpg");
		//images[i].resize(ofGetScreenWidth(), ofGetScreenHeight());
	}
	// coutour finder
	contourFinder.setMinAreaRadius(minArea);
	contourFinder.setMaxAreaRadius(maxArea);
	contourFinder.setThreshold(threshold);
	contourFinder.setFindHoles(holes);

	// sounds
	bgm.load("sounds/limbo.wav");
	bgm.setLoop(true);
	bgm.play();

	// font
	textFont.load("font/manic-depressive.ttf", 32);
	titleFont.load("font/manic-depressive.ttf", 72);
	buttonFont.load("font/manic-depressive.ttf", 36);

	ofSetRectMode(OF_RECTMODE_CORNER);

	// start button
	startButtonX = ofGetScreenWidth() / 2 -50;
	startButtonY = ofGetScreenHeight() - ofGetScreenHeight() / 7;
	startButton = buttonFont.getStringBoundingBox("START", startButtonX, startButtonY);

	// next button
	nextButtonX = ofGetScreenWidth() - ofGetScreenWidth() / 6;
	nextButtonY = ofGetScreenHeight() - ofGetScreenHeight() / 3;
	nextButton = buttonFont.getStringBoundingBox("NEXT", nextButtonX, nextButtonY);

	// replay button
	replayButtonX = ofGetScreenWidth() / 2;
	replayButtonY = ofGetScreenHeight() - ofGetScreenHeight() / 6;
	replayButton = buttonFont.getStringBoundingBox("REPLAY", replayButtonX, replayButtonY);
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
	if (stage == 0) { // start screen
		moveAmt = 0;

		ofPushStyle();
		//ofSetColor(10);
		//images[stage].draw(ofRandom(-3, 3), ofRandom(-3, 3), ofGetScreenWidth(), ofGetScreenHeight());
		ofSetColor(255);
		contourFinder.findContours(images[stage]);
		ofPushMatrix();
		ofScale(ofGetScreenWidth() / images[stage].getWidth());
		ofTranslate(ofRandom(-moveAmt, moveAmt), ofRandom(-moveAmt, moveAmt));
		contourFinder.draw();
		ofPopMatrix();
		ofPopStyle();

		ofPushStyle();
		float titleX = ofRandom(ofGetScreenWidth() / 4 - 3, ofGetScreenWidth() / 4 + 3);
		float titleY = ofRandom(ofGetScreenHeight() - ofGetScreenHeight() / 3 - 3, ofGetScreenHeight() - ofGetScreenHeight() / 3 + 3);
		titleFont.drawString("SMILE MACHINE", titleX, titleY);
		ofPopStyle();
		ofPushStyle();
		if (startButton.getX() <= mouseX && mouseX <= startButton.getX() + startButton.getWidth() &&
			startButton.getY() <= mouseY && mouseY <= startButton.getY() + startButton.getHeight()) { // hover start button
			ofSetColor(255, 0, 0);
		}
		else { // draw start button
			ofSetColor(255);
		}
		//ofDrawRectangle(startButton);
		buttonFont.drawString("START", startButtonX, startButtonY);
		ofPopStyle();
	}
	else if (stage > 0 && stage < text.size()) {
		if (stage > 0 && stage < 16) { // text screen
			moveAmt = 1;

			ofPushStyle();
			//ofSetColor(10);
			//images[stage].draw(ofRandom(-3, 3), ofRandom(-3, 3), ofGetScreenWidth(), ofGetScreenHeight());
			ofSetColor(255);
			contourFinder.findContours(images[stage]);
			ofPushMatrix();
			ofScale(ofGetScreenWidth() / images[stage].getWidth());
			ofTranslate(ofRandom(-moveAmt, moveAmt), ofRandom(-moveAmt, moveAmt));
			contourFinder.draw();
			ofPopMatrix();
			ofPopStyle();

			if (camOn) {
				testSmile(100, smileAmt);
			}
		}
		else if (stage >= 16 && stage < text.size()) { // mirror
			moveAmt = 1;

			camOn = true;
			ofPushStyle();
			ofSetColor(255);
			contourFinder.findContours(cam);
			ofPushMatrix();
			ofScale(ofGetScreenWidth() / cam.getWidth());
			ofTranslate(ofRandom(-moveAmt, moveAmt), ofRandom(-moveAmt, moveAmt));
			contourFinder.draw();
			ofPopMatrix();
			ofPopStyle();

			if (stage == text.size()-1 && camOn) {
				testSmile(100, smileAmt);
			}
		}

		displayScene(text[stage - 1]);

		if (next) { // display next button
			ofPushStyle();
			// when mouse hover, draw next button bigger
			if (nextButton.getX() <= mouseX && mouseX <= nextButton.getX() + nextButton.getWidth() &&
				nextButton.getY() <= mouseY && mouseY <= nextButton.getY() + nextButton.getHeight()) {
				ofSetColor(255, 0, 0);
			}
			else {
				// draw next button
				ofSetColor(255);
			}
			buttonFont.drawString("NEXT", nextButtonX, nextButtonY);
			ofPopStyle();
		}
		}
	else if (stage == text.size()) { // end screen
		moveAmt = 0;

		ofPushStyle();
		ofSetColor(10);
		images[stage].draw(ofRandom(-moveAmt, moveAmt), ofRandom(-moveAmt, moveAmt), ofGetScreenWidth(), ofGetScreenHeight());
		ofPopStyle();

		ofPushStyle();
		if (replayButton.getX() <= mouseX && mouseX <= replayButton.getX() + replayButton.getWidth() &&
			replayButton.getY() <= mouseY && mouseY <= replayButton.getY() + replayButton.getHeight()) { // hover start button
			ofSetColor(255, 0, 0);
		}
		else { // draw start button
			ofSetColor(255);
		}
		buttonFont.drawString("REPLAY", replayButtonX, replayButtonY);
		ofPopStyle();
	}
}

void ofApp::mouseReleased(int x, int y, int button) {
	if (stage == 0) {
		if (startButton.getX() <= mouseX && mouseX <= startButton.getX() + startButton.getWidth() &&
			startButton.getY() <= mouseY && mouseY <= startButton.getY() + startButton.getHeight()) { // start button
			stage += 1;
			resetScene();
		}
	}
	else if (stage > 0 && stage < text.size()) {
		if (next) {
			if (nextButton.getX() <= mouseX && mouseX <= nextButton.getX() + nextButton.getWidth() &&
				nextButton.getY() <= mouseY && mouseY <= nextButton.getY() + nextButton.getHeight()) {
				stage += 1;
				resetScene();
			}
		}
	}
	else if (stage >= text.size()) { // ending
		if (replayButton.getX() <= mouseX && mouseX <= replayButton.getX() + replayButton.getWidth() &&
			replayButton.getY() <= mouseY && mouseY <= replayButton.getY() + replayButton.getHeight()) { // start button
			stage = 0;
			resetScene();
		}
	}
}
void ofApp::displayScene(string sceneText) {
	// if we are going to use camera
	if (sceneText == "Smile.") {
		ofPushStyle();
		ofSetColor(0, 220);
		ofRectangle smileTextRect = textFont.getStringBoundingBox(smileText, ofGetScreenWidth() / 10, ofGetScreenHeight() - ofGetScreenHeight() / 5);
		ofDrawRectangle(smileTextRect);
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
		// text box
		ofSetColor(0, 220);
		ofRectangle textRect = textFont.getStringBoundingBox(tempText, ofGetScreenWidth() / 10, ofGetScreenHeight() - ofGetScreenHeight() / 5);
		ofDrawRectangle(textRect);
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
