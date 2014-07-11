#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	this->keyPressed(' ');
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	ofMatrix4x4 transform;
	transform.postMultTranslate(ofRandomf(), ofRandomf(), ofRandomf());
	transform.postMultRotate(90, ofRandomf(), ofRandomf(), ofRandomf());
	
	vector<ofVec3f> source;
	vector<ofVec3f> target;
	for(int i=0; i<10000; i++) {
		source.push_back(ofVec3f(ofRandomf() * 100.0f, ofRandomf() * 100.0f, ofRandomf() * 100.0f));
		target.push_back(source.back() * transform);
	}
	cout << source.size() << " points created" << endl;

	for(int i=0; i<1000; i++) {
		auto dataSet = ofxPolyFit::makeDataSet(source, target);
		cout << dataSet.size() << ",";
	}
	cout << endl;

	auto dataSet = ofxPolyFit::makeDataSet(source, target);

	for(int i=0; i<1000; i++) {
		ofxPolyFit fit;
		fit.init(1, 3, 3, BASIS_SHAPE_TRIANGLE);
		fit.correlate(dataSet);
		cout << fit.residualRMS(dataSet) << ",";
	}
	cout << endl;
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

}
