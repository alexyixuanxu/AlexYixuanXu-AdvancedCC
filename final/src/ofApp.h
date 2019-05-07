#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxThreadedImageLoader.h"

// single-person smile strength detector
// not robust against rotation
class SmileDetector {
protected:
    ofRectangle roi;
    
public:
    ofxCv::ObjectFinder faceFinder, smileFinder;
    
    void setup() {
        faceFinder.setup("haarcascade_frontalface_default.xml");
        faceFinder.setPreset(ofxCv::ObjectFinder::Accurate);
        faceFinder.setFindBiggestObject(true);
        smileFinder.setup("smiled_05.xml");
        smileFinder.setPreset(ofxCv::ObjectFinder::Sensitive);
        smileFinder.setMinNeighbors(0);
    }
    template <class T>
    void update(T& img) {
        update(ofxCv::toCv(img));
    }
    void update(const cv::Mat& mat) {
        faceFinder.update(mat);
        if(faceFinder.size()) {
			
            roi = faceFinder.getObject(0);
            float lowerRatio = .35;
            roi.y += roi.height * (1 - lowerRatio);
            roi.height *= lowerRatio;
            cv::Mat faceMat(mat, ofxCv::toCv(roi));
			
            smileFinder.update(faceMat);
		
        }
    }
	/*
    void draw() const {
        faceFinder.draw();
        if(faceFinder.size()) {
            ofPushMatrix();
            ofTranslate(roi.position);
            smileFinder.draw();
            ofPopMatrix();
        }
	}
	*/
    bool getFaceFound() const {
        return faceFinder.size();
    }
    ofRectangle getFace() const {
        return faceFinder.getObject(0);
    }
    int getSmileAmount() const {
        if(faceFinder.size()) {
            return smileFinder.size();
        } else {
            return 0;
        }
    }
};

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
	void mouseReleased(int x, int y, int button);

	void displayScene(string sceneText);
	void resetScene();
	void testSmile(float smileReq, float smileAmt);

	// text
	vector<string> text;

	// images
	vector<ofImage> images;
	ofxThreadedImageLoader loader;
	int imgSize = 20; // always need 1 more image than text
	float moveAmt = 1;

	// countour finder
	ofxCv::ContourFinder contourFinder;
	float minArea = 1;
	float maxArea = 500;
	float threshold = 130;
	bool holes = true;

	// sounds
	ofSoundPlayer bgm;
	
    // smile detection
    ofVideoGrabber cam;
	bool camOn = false;
    SmileDetector smile;
	float smileAmt;
	// text for directing smiles
	string smileText = "Smile.";
	vector<string> smileReqTexts;

	// font
	ofTrueTypeFont textFont;
	ofTrueTypeFont titleFont;
	ofTrueTypeFont buttonFont;

	// program manager
	int stage = 0; // 0 is home screen, 1 starts

	ofRectangle startButton;
	float startButtonX;
	float startButtonY;

	bool next = false;
	ofRectangle nextButton;
	float nextButtonX;
	float nextButtonY;

	ofRectangle replayButton;
	float replayButtonX;
	float replayButtonY;
	
	// text timer
	float nextResetTime = 0;
	float resetIntervalTime = 0.2;
	int wordIndex = 0;

	// smile text timer;
	float nextSmileResetTime = 0;
	float resetSmileIntervalTime = 1;
};