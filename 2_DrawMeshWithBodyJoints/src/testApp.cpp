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

	// NOW DRAW OUR JOINTS IN THE SAME 3D SPACE
	drawJoints3D();

	ofPopStyle();

	camera.end();

}


void testApp::drawJoints3D()
{
	// DRAW THE JOINTS IN A SALMON COLOR
	ofSetColor(240, 120, 90);

	ofVec3f pos;

	// THERE IS A MAXIMUM OF 6 BODIES TRACKED BY KINECT
	for (int i = 0; i<6; i++){
		// IF THE BODY IS BEING TRACKED...
		if (this->kinect.getBodySource()->getBodies()[i].tracked){

			auto b = this->kinect.getBodySource()->getBodies()[i];

			// ITERATE THROUGH ALL JOINTS IN THE TRACKED BODY...
			for (std::map<JointType, ofxKFW2::Data::Joint>::iterator it = b.joints.begin(); it != b.joints.end(); ++it)
			{
				if (it->second.getTrackingState() == TrackingState_Tracked)
				{
					// GRAB THE JOINT'S 3D POSITION
					pos = it->second.getPosition();

					// AND RENDER. JOINTS AND THE MESH POINTS SHARE THE 
					// SAME DEPTH SPACE COORDINATES SO NO COORDINATE MAPPING
					// IS NEEDED FOR THIS. 
					ofBox(pos.x, pos.y, pos.z, .05, .05, .05);
				}
			}
		}
	}
	ofSetColor(255);
}

