#pragma once
//
//  ofxPolyFitUtils.h
//

#include "ofMain.h"
#include "pfitDataSet.h"

namespace ofxPolyfit {
	pfitDataSetf makeDataSet(const vector<ofVec3f> &input, const vector<ofVec3f> &output);
}