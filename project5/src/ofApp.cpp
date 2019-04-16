#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup() {
	receiver.setup(PORT);

	// pdf rendering bools
	oneShot = false;
	pdfRendering = false;

	ofSetFrameRate(60);
	ofBackground(0);
	ofSetBackgroundColor(0);
	//ofSetBackgroundAuto(false);
    
    //SOUND
    //set up oscillator: 440 frequency, 0 phase, 1.0 amplitude
    //48000 sample rate
    osc.setup(440, 0, 1.0, 48000);
    //set up an envelope with default value
    envelope = new EnvelopeGenerator(2, 100, 0.6, 300, 48000);
    //set up the frequency
    frequency.currentValue = frequency.targetValue = 440;
    //smooth the frequency to remove unwanted clicks
    frequency.smoother = new parameterSmoother(100, 48000);
    //set up sound stream: 2 output channels, 0 input channel,
    //48000 sample rate, 512 buffersize, 4 buffers
    ofSoundStreamSetup(2, 0, 48000, 512, 4);

	//VISUAL
	for (int i = 0; i < 3; i++) {
		xyz.push_back(0);
	}

	// initiate stars
	for (int i = 0; i < numStars; i++) {
		Star currentStar;
		stars.push_back(currentStar);
		currentStar.setup(ofRandom(2, 8));
	}
}
//--------------------------------------------------------------
void ofApp::update() {
	ofSetColor(0);
	ofSetBackgroundColor(0);
	ofBackground(0);
	while (receiver.hasWaitingMessages()) {

		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(m);

		if (m.getAddress() == "/accxyz") {
			auto x = m.getArgAsFloat(0);
			auto y = m.getArgAsFloat(1);
			auto z = m.getArgAsFloat(2);
			ofLogNotice() << "accelerometer x: " << x << "accelerometer y: " << y << "accelerometer z: " << z << endl;

			xyz.at(0) = x;
			xyz.at(1) = y;
			xyz.at(2) = z;

			//glm::vec3 point = glm::vec3(x, y, z);
			//points.push_back(point);

		}
		// there are 12 keys from touchOSC
        if(m.getAddress() == "/1/push1"){
            if(m.getArgAsFloat(0) == 1){
                ofLogNotice() << m << endl;
                frequency.targetValue = mtof(60);
                envelope->setGate(1);
            }
            else{
                envelope->setGate(0);
            }
        }


	}
    frequency.targetValue = frequency.targetValue + (xyz[0] * 100) + (xyz[1] * 10) + (xyz[2] * 10);

	// update visual
	xspd = xyz[0] * 10;
	yspd = xyz[1] * 10;
	cspd = xyz[2] * 5;

	for (int i = 0; i < stars.size(); i++) {
		stars[i].update(xspd, yspd, cspd);
	}
}
//--------------------------------------------------------------
void ofApp::draw() {
	ofSetBackgroundColor(0);
	ofBackground(0);

	if (oneShot) {
		ofBeginSaveScreenAsPDF("screenshot-" + ofGetTimestampString() + ".pdf", false);
	}

	/*
	ofSetColor(255);
	ofBeginShape();
	for (auto &point : points) {
		auto x = point.x * 1000;
		auto y = point.y * 1000;
		auto z = point.z * 1000;
		ofVertex(x, y, z);
	}
	ofEndShape();
	*/
    

	for (int i = 0; i < stars.size(); i++) {
		stars[i].draw();
	}

	if (oneShot) {
		ofEndSaveScreenAsPDF();
		oneShot = false;
	}
}
//--------------------------------------------------------------
void ofApp::audioOut(float* buffer, int bufferSize, int nChannels){
    for(int i = 0; i < bufferSize; i++){
        
        float currentSample = osc.getSample(currentWave);
        //process envelope every sample
        envelope->process();
        //smooth the frequency and set the oscillator with current frequency
        frequency.currentValue = frequency.smoother->process(frequency.targetValue);
        osc.setFrequency(frequency.currentValue);
        
        //currentSample *= current amplitude value
        currentSample *= envelope->getValue();
        //writing to the left channel
        buffer[i*nChannels+0] = currentSample;
        //writing to the right channel
        buffer[i*nChannels+1] = currentSample;
    }
}

void ofApp::playsound(int i) {
    //map the y value of selected pixel to midi note
    int note = ofMap(i, 0, 28, 72, 60);
    //ofLog() << "the note is " << ofToString(note);
    //use this note to set target value for frequency
    frequency.targetValue = mtof(note);
    //turns th envelope on
    envelope->setGate(1);
}

void ofApp::stopsound() {
    //turns the envelope off
    envelope->setGate(0);
}


void ofApp::keyPressed(int key) {
	if (key == 'm') {
		pdfRendering = !pdfRendering;
		if (pdfRendering) {
			ofSetFrameRate(1);  // so it doesn't generate tons of pages
			ofBeginSaveScreenAsPDF("recording-" + ofGetTimestampString() + ".pdf", true);
		}
		else {
			ofSetFrameRate(60);
			ofEndSaveScreenAsPDF();
		}
	}

	if (!pdfRendering && key == 'n') {
		oneShot = true;
	}
    
    switch(key){
        case 'a':
            frequency.targetValue = mtof(60);
            envelope->setGate(1);
            break;
        case 's':
            frequency.targetValue = mtof(62);
            envelope->setGate(1);
            break;
        case 'd':
            frequency.targetValue = mtof(64);
            envelope->setGate(1);
            break;
        case 'f':
            frequency.targetValue = mtof(65);
            envelope->setGate(1);
            break;
        case 'g':
            frequency.targetValue = mtof(67);
            envelope->setGate(1);
            break;
        case 'h':
            frequency.targetValue = mtof(69);
            envelope->setGate(1);
            break;
        case 'j':
            frequency.targetValue = mtof(71);
            envelope->setGate(1);
            break;
        case 'k':
            frequency.targetValue = mtof(72);
            envelope->setGate(1);
            break;
            
            //change sound wave
        case 'z':
            currentWave = "sine";
            break;
        case 'x':
            currentWave = "tri";
            break;
        case 'c':
            currentWave = "saw";
            break;
        case 'v':
            currentWave = "square";
            break;

}
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    switch(key){
        case 'a':
            envelope->setGate(0);
            break;
        case 's':
            envelope->setGate(0);
            break;
        case 'd':
            envelope->setGate(0);
            break;
        case 'f':
            envelope->setGate(0);
            break;
        case 'g':
            envelope->setGate(0);
            break;
        case 'h':
            envelope->setGate(0);
            break;
        case 'j':
            envelope->setGate(0);
            break;
        case 'k':
            envelope->setGate(0);
            break;
    }
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}
//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
}
//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
}
//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
}
//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
}

void ofApp::exit(){
    ofSoundStreamClose();
}
