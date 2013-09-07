#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(50);
	ofEnableSmoothing();
	
	camera.initGrabber(640, 480);
	
	plane.setSource(camera);
	plane.setCalibrateMode(true);
	plane.setGridResolution(10);
	plane.load("warp.bin");
}

//--------------------------------------------------------------
void ofApp::update(){
	camera.update();
    video.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	plane.draw();
	
	if (plane.getCalibrateMode()) {
		int y = 20;
		ofDrawBitmapString("Drag a movie file onto this app", 20, y+=15);
		ofDrawBitmapString("Use the handles to warp and move the video", 20, y+=15);
		ofDrawBitmapString("[SPACE] = toggle calibration mode", 20, y+=10);
		ofDrawBitmapString("Use mouse to select points and warp the video", 20, y+=15);
		ofDrawBitmapString("[TAB] = select points", 20, y+=15);
		ofDrawBitmapString("[UP]/[DOWN]/[LEFT]/RIGHT = move the selected point", 20, y+=15);
		ofDrawBitmapString("[SHIFT] + arrow key = move the selected point faster", 20, y+=15);
		ofDrawBitmapString("[r] = reset calibration", 20, y+=15);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed (int key){
	if (key == ' ') {
		plane.toggleCalibrateMode();
		if (plane.getCalibrateMode())
			ofBackground(50);
		else
			ofBackground(0);
	}
	if (key == 's')
		plane.save("warp.bin");
	if (key == 'l')
		plane.load("warp.bin");
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
	for(auto file : dragInfo.files) {
		video.loadMovie(file);
		video.play();
		plane.setSource(video);
	}
}
