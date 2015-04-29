#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	kinect.open();

	// IF WE ARE GOING TO ACCESS THE MESH
	// WE NEED TO INIT ALL THREE STREAMS
	kinect.initColorSource();
	kinect.initBodySource();
	kinect.initDepthSource();

	// MOVING THE CAMERA CLOSER
	camera.setDistance(10);

	ofSetWindowShape(1920, 1080);
}

//--------------------------------------------------------------
void testApp::update(){
	// UDPATE ALL INITIALIZED KINECT STREAMS
	kinect.update();

	// GRAB THE MESH FROM THE ADDON. THESE SETTINGS WILL RETURN A POINT CLOUD
	mesh = kinect.getDepthSource()->getMesh(
		false,
		ofxKinectForWindows2::Source::Depth::PointCloudOptions::TextureCoordinates::ColorCamera);
}

//--------------------------------------------------------------
void testApp::draw(){

	// SETTING THE BACKGROUND A MUTED BLUE
	ofBackground(75, 95, 115);

	// DRAW THE COLOR CAMERA TO MAKE SURE THE KINECT IS WORKING
	ofSetColor(255);
	kinect.getColorSource()->draw(0, 0, 320, 180);

	// WE ARE NOW SWITCHING TO THE 3D VIEW
	camera.begin();

	ofPushStyle();
	// FOR THIS MESH, I SCALED THE Z NEGATIVE TO MATCH DEPTH
	// THIS IS DEVELOPER PREFERENCE, BUT IT'S HOW I CONCEIVE
	// THE MESH...
	ofScale(10, 10, -10);

	mesh.draw();

	ofPopStyle();

	camera.end();

}

