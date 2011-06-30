#include "Utils.h"

void setFrameRateAsWindowTitle() {
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

// 60 FPS, vertical sync, dark background
void setupOF(int fps) {
	ofSetFrameRate(fps);
	if(fps <= 60) {
		ofSetVerticalSync(true);
	}
	ofBackground(33,33,33);
}