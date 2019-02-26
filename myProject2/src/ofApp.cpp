#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0, 0, 0);
	mVideo.load("2001ASpaceOdyssey_2.mp4");
	mVideo.setLoopState(OF_LOOP_NONE);
	mVideo.play();
	for (int i = 0; i < myTriangles.size(); i++) {
		myTriangles[i].setup();
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	mVideo.update();
	for (int i = 0; i < myTriangles.size(); i++) {
		myTriangles[i].update();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	//ofBackground(0, 0, 0);
	mVideo.draw(0, 0);

	//get frame information
	totalNumofFrame = mVideo.getTotalNumFrames();
	currentFrame = mVideo.getCurrentFrame();

	//map video playbackrate to how many frame has been played
	//motion will get slower and slower
	playbackspeed = ofMap(currentFrame, 0, totalNumofFrame, 1, 0);
	mVideo.setSpeed(playbackspeed);
	ofLogNotice() << playbackspeed << endl;

	if (mVideo.getCurrentFrame() == 463) {
		playbackspeed = 0;
	}
	for (int i = 0; i < myTriangles.size(); i++) {
		myTriangles[i].draw();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	//option2: change the playback rate by key press
	switch (key) {
	case '1':
		mVideo.setSpeed(0.09);
		break;
	case '2':
		mVideo.setSpeed(0.5);
		break;
	case '3':
		mVideo.setSpeed(-0.8);
		break;
	}
	Tri tempTriangle;
	myTriangles.push_back(tempTriangle);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
