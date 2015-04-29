#pragma once

#include "ofMain.h"

#include "ofxKinectForWindows2.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);

		ofxKFW2::Device		kinect;

		float	nearClipping, farClipping;

		void updateRawDepthLookupTable();
		void generateDepthDisplayImage();

		vector<unsigned char>		depthLookupTable;
		ofPixels_<unsigned char>	depthDisplay;
		ofImage						depthDisplayImage;

};
