#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "EnvelopeGenerator.hpp"
#include "audioTools.h"
#include "Star.h"

// listening port
#define PORT 8000

// max number of strings to display
#define NUM_MSG_STRINGS 20

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
    void exit();
    void audioOut(float* buffer, int bufferSize, int nChannels);


	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
    
    //SOUND
    //make a new envelope
    EnvelopeGenerator* envelope;
    //control the frequency with a smooth value
    smoothValue frequency;
    //make a oscillator
    oscillator osc;
    //convert MIDI note number to frequency
    float mtof(float midiVal) {
        //return expr 440*(pow (2, (($f1-69)/12)))
        return 440 * pow(2, (midiVal - 69) / 12.f);
    }
    
    void playsound(int i);
    void stopsound();
    string currentWave = "sine";

	// OSC
	ofxOscReceiver receiver;
	int currentMsgString;
	string msgStrings[NUM_MSG_STRINGS];
	float timers[NUM_MSG_STRINGS];

	vector<float> xyz;
	
	// VISUAL
	//vector<glm::vec3> points;

	vector<Star> stars;
	float xspd;
	float yspd;
	float cspd;

	int numStars = 500;

	// EXPORT
	bool oneShot;
	bool pdfRendering;

};
