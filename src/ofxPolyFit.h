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
	
	void		init(int order, int dimensionsIn, int dimensionsOut, pfitBasisType basisType);
	void        uninitialise();
    
    //perform correlation
    void        correlate(pfitDataSetd &dataSet);
    
    //general case evaluate
	void		evaluate(pfitDataPointd &dataPoint, bool checkData=true) const;
	void		evaluate(pfitDataSetd &dataSet) const;
    
    //special 1D case
    DataType    evaluate(DataType input) const;
    
    //residuals
    DataType      residualSquared(pfitDataPointd const &dataPoint, bool checkData=true) const;
    DataType      residualRMS(pfitDataSetd const &dataSet) const;
    
    bool        getSuccess() { return _success; }
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
    bool    checkInitialised() const;

	polyNfit		*_fit;
	
	bool			_isInitialised;
	bool			_success;
};