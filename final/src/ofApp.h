#pragma once

#include "ofMain.h"
#include "ofxCv.h"

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
    void draw() const {
        faceFinder.draw();
        if(faceFinder.size()) {
            ofPushMatrix();
            ofTranslate(roi.position);
            smileFinder.draw();
            ofPopMatrix();
        }
	}
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
    
    ofVideoGrabber cam;
    SmileDetector smile;

	float smileAmt;

	int num = 0;
	vector<float> xs;
	vector<float> ys;

	float nextResetTime = 0;
	float resetIntervalTime = 1;
	ofImage face;
};
