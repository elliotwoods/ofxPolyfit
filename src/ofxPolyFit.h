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

#include "../libs/PolyNFit/src/polyNfit.h"
#include "ofxPolyFitUtils.h"

template <typename T>
class ofxPolyFit_
{
public:
	ofxPolyFit_();
	~ofxPolyFit_();
	
	void		init(int order, int dimensionsIn, int dimensionsOut, pfitBasisType basisType = BASIS_SHAPE_TRIANGLE);
	void        uninitialise();
    
    ///perform correlation
    void        correlate(pfitDataSet<T> &dataSet);
    
    ///general case evaluate
	void		evaluate(pfitDataPoint<T> &dataPoint, bool checkData=true) const;
	void		evaluate(pfitDataSet<T> &dataSet, bool checkData=true) const;
    
    ///special 1D case evaluate
    T		    evaluate(T input) const;
    
    ///residuals
    T			residualSquared(pfitDataPoint<T> const &dataPoint, bool checkData=true);
    T			residualRMS(pfitDataSet<T> const &dataSet);
    
    bool        getSuccess() { return _success; }
	void        save(string filename);
	void		load(string filename);
    
	vector<unsigned int*>				*basisIndices;
	vector<T*>							coefficients;
	
	unsigned int						nBases;
    
    //RANSAC (experimental)
    void        RANSAC(pfitDataSet<T> &dataSet, int maxIterations, float selectionProbability, float residualThreshold, float inclusionThreshold);
    
    T bestError;
protected:
    bool    checkInitialised() const;

	polyNfit<T>		*_fit;
	
	bool			_isInitialised;
	bool			_success;
};

typedef ofxPolyFit_<double> ofxPolyFitd;
typedef ofxPolyFit_<float> ofxPolyFitf;
typedef ofxPolyFitf ofxPolyFit;