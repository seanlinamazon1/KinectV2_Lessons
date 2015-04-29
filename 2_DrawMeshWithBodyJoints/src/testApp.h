#pragma once

#include "ofMain.h"
#include "ofxKinectForWindows2.h"

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	ofxKFW2::Device		kinect;

	ofEasyCam			camera;
	ofMesh				mesh;

	void drawJoints3D();

};