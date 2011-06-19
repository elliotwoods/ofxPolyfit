#pragma once
/*
 *  ofxPolyFit.h
 *  ofxPolyFit example
 *
 *  Created by Elliot Woods on 03/11/2010.
 *  Copyright 2010 Kimchi and Chips. All rights reserved.
 *
 */

#include "ofMain.h"
#include <fstream>
#include <set>

#include "polyNfit.h"

//for the time being, we asset doubles
typedef double DataType;

class ofxPolyFit
{
public:
	ofxPolyFit();
	~ofxPolyFit();
	
	void		init(int order, int dimensionsIn, int dimensionsOut, int basisType);
	void        uninitialise();
    
    //perform correlation
    void        correlate(pfitDataSetd &dataSet);
    
    //general case evaluate
	void		evaluate(pfitDataPointd &dataPoint, bool checkData=true);
	void		evaluate(pfitDataSetd &dataSet);
    
    //special 1D case
    DataType    evaluate(DataType input);
    
    //residuals
    DataType      residualSquared(pfitDataPointd dataPoint, bool checkData=true);
    DataType      residualRMS(pfitDataSetd dataSet);
    
	void        save(string filename);
	void		load(string filename);
    
	vector<unsigned int*>				*basisIndicies;
	vector<DataType*>					coefficients;
	
	unsigned int						nBases;
    
    //RANSAC (experimental)
    void        RANSAC(double* input, double* output, int nDataPoints, int maxIterations, float selectionProbability, float residualThreshold, float inclusionThreshold);
    
    double bestError;
    set<int> bestConsensus;
    double *bestModel;
protected:
    bool    checkInitialised();

	polyNfit		*_fit;
	
	bool			_isInitialised;
	bool			_success;
};