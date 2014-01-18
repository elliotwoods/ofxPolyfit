#pragma once
//
//  ofxPolyFitUtils.h
//

#include "ofMain.h"
#include "../libs/PolyNFit/src/Data/pfitDataSet.h"

namespace ofxPolyfit {
	pfitDataSetf makeDataSet(const vector<ofVec3f> &input, const vector<ofVec3f> &output);
}