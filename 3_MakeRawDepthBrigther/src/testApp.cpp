#include "testApp.h"

#define DEPTH_WIDTH			512
#define DEPTH_HEIGHT		424
#define	MIN_DEPTH_CLIP		50.0f
#define MAX_DEPTH_CLIP		10000.0f

//--------------------------------------------------------------
void testApp::setup(){

	ofSetWindowShape(1060, 530);

	kinect.open();
	kinect.initColorSource();
	kinect.initDepthSource();

	depthDisplay.allocate(DEPTH_WIDTH, DEPTH_HEIGHT, OF_IMAGE_COLOR);

	nearClipping = 50.0;
	farClipping = 2000.0;
	updateRawDepthLookupTable();

}

//--------------------------------------------------------------
void testApp::update(){
	kinect.update();
	generateDepthDisplayImage();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch (key)
	{
		case 'n':
			nearClipping -= 10.0f;
			if (nearClipping < MIN_DEPTH_CLIP) nearClipping = MIN_DEPTH_CLIP;
			updateRawDepthLookupTable();
			break;
		case 'm':
			nearClipping += 10.0f;
			if (nearClipping > (farClipping - 255)) nearClipping = farClipping - 255;
			updateRawDepthLookupTable();
			break;
		case 'f':
			farClipping += 50.0f;
			if (farClipping > MAX_DEPTH_CLIP) farClipping = MAX_DEPTH_CLIP;
			updateRawDepthLookupTable();
			break;
		case 'd':
			farClipping -= 50.0f;
			if (farClipping < (nearClipping + 255)) farClipping = nearClipping + 255;
			updateRawDepthLookupTable();
			break;
	}
}

//--------------------------------------------------------------
void testApp::generateDepthDisplayImage() {

	// MAKE SURE WE HAVE A DEPTH SOURCE BEFORE WE ATTEMPT TO MANIPULATE
	if (nullptr == kinect.getDepthSource()) return;

	unsigned short* rawdepth = kinect.getDepthSource()->getPixels();

	int rawindex = 0;
	int displayindex = 0;

	for (int y = 0; y < DEPTH_HEIGHT; y++) {
		for (int x = 0; x < DEPTH_WIDTH; x++) {
			unsigned char greyValue = depthLookupTable[rawdepth[rawindex++]];

			depthDisplay.getPixels()[displayindex++] = greyValue;
			depthDisplay.getPixels()[displayindex++] = greyValue;
			depthDisplay.getPixels()[displayindex++] = greyValue;
		}
	}

	depthDisplayImage.setFromPixels(depthDisplay);

}


//--------------------------------------------------------------
void testApp::draw(){

	ofBackground(100);

	kinect.getDepthSource()->getTextureReference().draw(10, 10);
	ofDrawBitmapString("RAW DEPTH PIXELS", 10, 460);

	depthDisplayImage.draw(536, 10);

	stringstream ss;
	ss << "DISPLAY READY IMAGE" << endl;
	ss << "nearClipping [- n / + m]: " << ofToString(nearClipping) << endl;
	ss << "farClipping  [+ f / - d]: " << ofToString(farClipping) << endl;
	ofDrawBitmapString(ss.str(), 536, 460);

}


// ***********************************************************************
// A USEFUL FUNCTION REPURPOSED FROM THE 'ofxKinect2' ADDON THAT WAS BUILT
// USING THE DEPRECATED KinectCommonBridge DLL.
// ***********************************************************************

//--------------------------------------------------------------
void testApp::updateRawDepthLookupTable(){

	// IF YOU WANT NEAR TO BE BLACK AND FAR TO BE WHITE, SWITCH THESE
	unsigned char nearColor = 255;
	unsigned char farColor = 0;

	// HOW FAR BACK WE WANT TO GRADIENT BEFORE HITTING THE MAX DISTANCE.
	// EVERYTHING PAST THE MAX DISTANCE WILL BE CLAMPPED TO 'farColor'
	unsigned int maxDepthLevels = 5001;
	depthLookupTable.resize(maxDepthLevels);

	depthLookupTable[0] = 0;
	for (unsigned int i = 1; i < maxDepthLevels; i++)
	{
		depthLookupTable[i] = ofMap(i, nearClipping, farClipping, nearColor, farColor, true);
	}
}
