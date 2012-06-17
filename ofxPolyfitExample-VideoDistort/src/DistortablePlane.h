#pragma once
//
//  DistortablePlane.h
//  ofxPolyFitExample-VideoDistort
//

#include "ofMain.h"
#include "ofxPolyFit.h"

// a distortable video plane with 9 control points in a grid (2nd order fit)

class DistortablePlane {
public:
	DistortablePlane();
	void draw();

	void setSource(ofBaseHasTexture & source);
	void setCalibrateMode(const bool calibrateMode);
	void setGridResolution(int resolution);
	
	//remember to call this if you change the video's resolution, or control points
	void updateGrid();
	void resetCalibration();
	
	void nextHandle();
	
	pfitDataSetf calibration;
	pfitDataPointf selection;
	
	ofVec2f & getHandle();
	
	void keyPressed(ofKeyEventArgs &args);
	void keyReleased(ofKeyEventArgs &args);
	void mousePressed(ofMouseEventArgs &args);
	void mouseDragged(ofMouseEventArgs &args);
	
	void save(string filename);
	void load(string filename);
	
	void setBounds(const ofRectangle & bounds);
	
protected:
	ofxPolyFit fit;
	ofMesh mesh;
	ofRectangle defaultBounds;
	
	ofBaseHasTexture * source;
	
	bool calibrateMode;
	int gridResolution;
	
	const float radius;
	bool hit;
	ofVec2f startDragDiff;
	
	bool shift;
};