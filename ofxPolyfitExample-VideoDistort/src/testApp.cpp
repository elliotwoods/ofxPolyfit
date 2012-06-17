#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(50);
	ofEnableSmoothing();
	
	video.loadMovie("movies/fingers.mov");
	video.play();
	
	plane.setSource(video);
	plane.setCalibrateMode(true);
}

//--------------------------------------------------------------
void testApp::update(){
    video.idleMovie();
}

//--------------------------------------------------------------
void testApp::draw(){
	plane.draw();
	
	int y = 20;
	ofDrawBitmapString("Use the handles to warp and move the video", 20, y+=10);
	ofDrawBitmapString("[SPACE] = toggle calibration mode", 20, y+=10);
	ofDrawBitmapString("Use mouse to select points and warp the video", 20, y+=10);
	ofDrawBitmapString("[TAB] = select points", 20, y+=10);
	ofDrawBitmapString("[UP]/[DOWN]/[LEFT]/RIGHT = move the selected point", 20, y+=10);
	ofDrawBitmapString("[SHIFT] + arrow key = move the selected point faster", 20, y+=10);
	ofDrawBitmapString("[r] = reset calibration", 20, y+=10);
	
}

//--------------------------------------------------------------
void testApp::keyPressed (int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}


//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
