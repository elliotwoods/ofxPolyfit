//
//  ofxPolyFitUtils.cpp
//
#include "ofxPolyFitUtils.h"

namespace ofxPolyfit {
	pfitDataSetf makeDataSet(const vector<ofVec3f> &input, const vector<ofVec3f> &output) {
		int size = input.size();
		if (size != output.size())
			throw("ofxPolyfit::makeDataSet - vector input size mismatch error");
		
		pfitDataSetf set;
		set.init(3, 3, size);
		memcpy(set.getInput(), &input[0], sizeof(ofVec3f) * size);
		memcpy(set.getOutput(), &output[0], sizeof(ofVec3f) * size);
		
		return set;
	}
}