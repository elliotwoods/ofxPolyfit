/*
 *  ofxPolyFit.cpp
 *  ofxPolyFit example
 *
 *  Created by Elliot Woods on 03/11/2010.
 *  Copyright 2010 Kimchi and Chips. All rights reserved.
 *
 */

#include "ofxPolyFit.h"

template<typename T>
ofxPolyFit_<T>::ofxPolyFit_() {
	_isInitialised = false;
	_success = false;
	nBases = 0;
}

template<typename T>
ofxPolyFit_<T>::~ofxPolyFit_() {
	if (_isInitialised)
    {
        for (int i=0; i<_fit->_outdim; i++)
            delete[] coefficients[i];

		delete _fit;
    }
}

template<typename T>
void ofxPolyFit_<T>::init(int order, int dimensionsIn, int dimensionsOut, pfitBasisType basisType) {
	if (_isInitialised)
		uninitialise();
	
	_fit = new polyNfit<T>(order, dimensionsIn, dimensionsOut, basisType);
	
	basisIndices = &(_fit->vecBasisIndices);
	
	//create space for coefficients
	coefficients.resize(_fit->_outdim);
	for (int i=0; i<_fit->_outdim; i++)
	{
		coefficients[i] = new T[_fit->_nBases];
		for (int iBasis=0; iBasis<_fit->_nBases; iBasis++)
			coefficients[i][iBasis] = 0;
	}
    
    nBases = _fit->_nBases;
	
	_success = false;
	_isInitialised = true;
}

template<typename T>
void ofxPolyFit_<T>::uninitialise() {
    for (int i=0; i<_fit->_outdim; i++)
        delete[] coefficients[i];
    coefficients.clear();
    delete _fit;
    nBases = 0;
    
    _isInitialised = false;
    _success = false;
}

template<typename T>
void ofxPolyFit_<T>::correlate(pfitDataSet<T> &dataSet)
{
	if (!checkInitialised())
        return;
    try
    {
		float startTime = ofGetElapsedTimef();

        //////////////////////////////////////////////////////
        // BASIC ERROR CHECKING
        //////////////////////////////////////////////////////
        //
        dataSet.throwIfNotReady(_fit->_indim, _fit->_outdim, nBases);
        //
        //////////////////////////////////////////////////////

        
        //////////////////////////////////////////////////////
        // RECREATE FIT
        //////////////////////////////////////////////////////
        //
        polyNfit<T> *newFit = new polyNfit<T>(_fit->_order, _fit->_indim, _fit->_outdim, _fit->_basesShape);
        delete _fit;
        _fit = newFit;
        //
        //////////////////////////////////////////////////////
        
        //////////////////////////////////////////////////////
        // PERFORM FIT
        //////////////////////////////////////////////////////
        //
		ofLogNotice() << "ofxPolyFit : initialising polyfit";
        _fit->init(dataSet);
        //
        //////////////////////////////////////////////////////
        
        
        //////////////////////////////////////////////////////
        // READ IN COEFFICIENTS
        //////////////////////////////////////////////////////
        //
		ofLogNotice() << "ofxPolyFit : solving polyfit";
        for (int iDimOut = 0; iDimOut < _fit->_outdim; iDimOut++)
            _fit->solve(iDimOut, coefficients[iDimOut], nBases);
        //
        //////////////////////////////////////////////////////
        
		float endTime = ofGetElapsedTimef();
		ofLogNotice() << "ofxPolyFit : fit complete in " << (endTime-startTime) << "s";
        _success = true;
    }
    
    catch (char* message)
    {
          ofLog(OF_LOG_ERROR, "ofxPolyFit: " + string(message));
        
        _success = false;
    }
}

template<typename T>
void ofxPolyFit_<T>::evaluate(pfitDataPoint<T> &dataPoint, bool checkData) const
{
    /////////////////////////
    // Check we're ready
    /////////////////////////
    //
    if (checkData)
    {
        try
        {
            checkInitialised();
            dataPoint.throwIfNotReady(_fit->_indim, _fit->_outdim);
        }
        catch (char* msg)
        {
            ofLogError() << "ofxPolyFit_<T>::evaluate : " << msg;
            return;
        }
    }
    //
    /////////////////////////
    
    
    //clear output to 0
    dataPoint.clearOutput();
	
    
    ///////////////////////////
    // Evaluate per output dim
    ///////////////////////////
    //
	T basis, coeff;
	for (int iDimOut=0; iDimOut< _fit->_outdim; iDimOut++)
	{	
        
        T *output = dataPoint.getOutput();
        
		for (int iBasis=0; iBasis<nBases; iBasis++)
		{
			basis=_fit->basis(iBasis, dataPoint);
			coeff = coefficients[iDimOut][iBasis];
            //
			output[iDimOut]+= basis * coeff;
		}
        
	}
    //
    ///////////////////////////
}
template <typename T>
void ofxPolyFit_<T>::evaluate(pfitDataSet<T> &dataSet, bool checkData) const
{
	if (checkData) {
		/////////////////////////
		// Check we're ready
		/////////////////////////
		//
		try
		{
			checkInitialised();
			dataSet.throwIfNotReady(_fit->_indim, _fit->_outdim);
		}
		catch (char* msg)
		{
			ofLogError() << "ofxPolyFit_<T>::evaluate : " << msg;
			return;
		}
		//
		/////////////////////////
    }
	
    
    /////////////////////////
    // Evaluate points
    /////////////////////////
    //
	int i;
	pfitDataPoint<T> pt;
	for (pt = dataSet.begin(); pt != dataSet.end(); ++pt)
		if (pt.getActive())
			evaluate(pt, false);
    //
    /////////////////////////
}

template <typename T>
T ofxPolyFit_<T>::evaluate(T input) const
{
    checkInitialised();
    
    //1D special case
    
    if (_fit->_outdim != 1 || _fit->_indim != 1)
    {
        ofLogError() << "Cannot use ofxPolyFit_<T>::evaluate(<T>) unless indim=1, outdim=1.";
        return 0;
    }
    
    T output;
    
	bool enabled = true;
    pfitDataPoint<T> point(1, 1, &input, &output, &enabled);
    
    evaluate(point, false);
    
    return output;
}

template <typename T>
T ofxPolyFit_<T>::residualSquared(pfitDataPoint<T> const &dataPoint, bool checkData)
{
    /////////////////////////
    // Check we're ready
    /////////////////////////
    //
    if (checkData)
    {
        try
        {
            dataPoint.throwIfNotReady(_fit->_indim, _fit->_outdim);
        }
        catch (char* msg)
        {
            ofLogError() << "ofxPolyFit_<T>::evaluate : " << msg;
            return 0;
        }
    }
    //
    /////////////////////////
    
    
    ///////////////////////////////
    // Evaluate to seperate memory
    ///////////////////////////////
    //
    T *outResult = new T[_fit->_outdim];
    
	bool enabled = true;
    pfitDataPoint<T> pointResult(_fit->_indim, _fit->_outdim, dataPoint.getInput(), outResult, &enabled);
    
    evaluate(pointResult);
    //
    ///////////////////////////////
    
    
    ///////////////////////////////
    // Calculate residual squared
    ///////////////////////////////
    //
    T *outData = dataPoint.getOutput();
    
    T residualall = 0;
    T residualx;
    
    for (int i=0; i<_fit->_outdim; i++)
    {
        residualx = outData[i] - outResult[i];
        residualall += residualx * residualx;
    }
    //
    ///////////////////////////////
    
	delete[] outResult;
    return residualall;
    
}

template<typename T>
T ofxPolyFit_<T>::residualRMS(pfitDataSet<T> const &dataSet)
{
    /////////////////////////
    // Check we're ready
    /////////////////////////
    //
    try
    {
        dataSet.throwIfNotReady(_fit->_indim, _fit->_outdim);
    }
    catch (char* msg)
    {
        ofLogError() << "ofxPolyFit_<T>::evaluate : " << msg;
        return 0;
    }
    //
    /////////////////////////
    
    
    /////////////////////////
    // Go through points
    /////////////////////////
    //
    T residualTotal = 0;
    
    for (pfitDataPoint<T> point = dataSet.begin(); point != dataSet.end(); ++point)
    {
        if (point.getActive())
            residualTotal += residualSquared(point);
    }
    //
    /////////////////////////
    
    
    /////////////////////////
    // Mean squared
    /////////////////////////
    //
    residualTotal /= T(dataSet.getActiveCount());
    return sqrt(residualTotal);
    //
    /////////////////////////
    
    
}

template<typename T>
void ofxPolyFit_<T>::save(string filename)
{
#ifdef TARGET_WIN32
	filename = ".\\data\\" + filename;
#else
	filename = "../../../data/" + filename;
#endif
	ofstream fileout(filename.c_str(), ios::out | ios::binary | ios::trunc);
	
	//write:
	//order, dimensionsIn, dimensionsOut, basisType, nBases
	//bases[iBasis,iDimensionIn]
	//coefficients[iDimensionOut,
	
	if (!fileout.is_open())
	{
		ofLog(OF_LOG_ERROR, "ofxPolyFit: cannot open output file " + filename);
		return;
	}
	
	fileout.write((char*) &_fit->_order, 2);
	fileout.write((char*) &_fit->_indim, 2);
	fileout.write((char*) &_fit->_outdim, 2);
	fileout.write((char*) &_fit->_basesShape, 1);
	fileout.write((char*) &nBases, 4);
	
	basisIndices = &(_fit->vecBasisIndices);
	if (_fit->_basesShape < BASIS_SHAPE_PADE_FIRST)
		for (int iBasis=0; iBasis<nBases; iBasis++)
			fileout.write((char*) basisIndices->at(iBasis),
						  sizeof(unsigned int) * _fit->_indim);
	
	for (int iDimOut=0; iDimOut<_fit->_outdim; iDimOut++)
		fileout.write((char*) coefficients[iDimOut],
					  sizeof(T) * nBases);
    
	ofLog(OF_LOG_VERBOSE, "ofxPolyFit: Fit saved");
}

template<typename T>
void ofxPolyFit_<T>::load(string filename)
{
#ifdef TARGET_WIN32
	filename = ".\\data\\" + filename;
#else
	filename = "../../../data/" + filename;
#endif
	ifstream filein(filename.c_str(), ios::binary | ios::in);
	
	if (!filein.is_open())
	{
		ofLog(OF_LOG_ERROR, "ofxPolyFit: failed to load file " + filename);
		return;
	}
	unsigned short order, indim, outdim;
    unsigned char basesShape;
    
	filein.read((char*) &order, 2);
	filein.read((char*) &indim, 2);
	filein.read((char*) &outdim, 2);
	filein.read((char*) &basesShape, 1);
    
    init(order, indim, outdim, pfitBasisType(basesShape));
    
	filein.read((char*) &nBases, 4);
	
	if (_fit->_basesShape < BASIS_SHAPE_PADE_FIRST)
		for (int iBasis=0; iBasis<nBases; iBasis++)
			filein.read((char*) basisIndices->at(iBasis),
						  _fit->_indim * 4);
	
	for (int iDimOut=0; iDimOut<_fit->_outdim; iDimOut++)
		filein.read((char*) coefficients[iDimOut],
					  sizeof(T) * nBases);
	
	_success = true;
}

template<typename T>
void ofxPolyFit_<T>::RANSAC(pfitDataSet<T> &dataSet, int maxIterations, float selectionProbability, float residualThreshold, float inclusionThreshold)
{
    checkInitialised();
	
	//////////////////////////////////////////////////////////////////
    //taken from pseudocode at http://en.wikipedia.org/wiki/RANSAC
    //////////////////////////////////////////////////////////////////
    // parameters:
    //
    // selectionProbability = 0.0..1.0  ;   Possibility of each data
    //                                      point being within
    //                                      "maybeInlier" set at
    //                                      the start of each
    //                                      iteration.
    //
    // residualThreshold = 0.0..+INF    ;   Maximum residual a data
    //                                      point can have to be
    //                                      added to the consensus
    //                                      set.
    //
    // inclusionThreshold = 0.0..1.0    ;   What percentage of data
    //                                      points must be included
    //                                      in the consenus set.
    //////////////////////////////////////////////////////////////////
    
    int inclusionThresholdCount = inclusionThreshold * float(dataSet.size());
    bestError = + 1e37;
	pfitIndexSet bestConsensus;
    
    T *bestModel = new T[_fit->_outdim * nBases];
    
    pfitIndexSet currentConsensus;
    set<int>::iterator idxIt;
    double currentError;
    
    float startTime;
    
    //loop through allowed number of iterations
	pfitDataPoint<T> pt;
	
	//
	
    for (int iteration=0; iteration<maxIterations; iteration++) {
        startTime = ofGetElapsedTimef();
        
        //////////////////////////////////
        // Randomly select maybe inliers
        //////////////////////////////////
        //
        for (pt = dataSet.begin(); pt != dataSet.end(); ++pt)
			pt.setActive(ofRandomuf() < selectionProbability);
        
        if (dataSet.getActiveCount() < nBases)
            continue;
        //
        //////////////////////////////////
        
        
        ////////////////////////////////////
        // Perform a fit with maybe inliers
        ////////////////////////////////////
        //
		try {
			correlate(dataSet);
		} catch (...) {
			continue;
		}
        
        //
        ////////////////////////////////////
        
        
        ////////////////////////////////////
        // Build consensus set
        ////////////////////////////////////
        //
        currentConsensus = dataSet.getActiveIndices();
        
		pfitIndexSet::iterator itIdx;
		pfitDataPoint<T> ptTest;
        for (int iPoint=0; iPoint<dataSet.size(); iPoint++) {
            //first check if already in set
            if (currentConsensus.count(iPoint) != 0)
                continue;
            
			//
			ptTest = dataSet[iPoint];
            
            //if residual for this point is less than threshold
            //then add it to the consensus set
            if (residualSquared(ptTest) < residualThreshold)
                currentConsensus.insert(iPoint);
            
        }
        //
        ////////////////////////////////////
        
        
        ////////////////////////////////////////
        // Check if we meet inclusion threshold
        ////////////////////////////////////////
        //
        if (currentConsensus.size() < inclusionThresholdCount)
        {
            cout << "Iteration #" << iteration << " achieved " << currentConsensus.size() << " points out of the necessary " << inclusionThresholdCount << " and therefore failed after " <<  (ofGetElapsedTimef() - startTime) << "s.\n";
            continue;
        }
        //
        ////////////////////////////////////////
        
        
        ////////////////////////////////////
        // Check to see if this iteration
        // has the lowest error so far
        ////////////////////////////////////
        //
		//perform a fit with consensus set
		dataSet.setActiveIndices(currentConsensus);
		try {
			correlate(dataSet);
		} catch (...) {
			//we should never get here to be honest.
			continue;
		}
		
        currentError = residualRMS(dataSet);
        
        if (currentError < bestError)
        {
            //we've got a better fit
            bestError = currentError;
            bestConsensus = currentConsensus;
            
            for (int iOutDim=0; iOutDim<_fit->_outdim; iOutDim++)
                for (int iBasis=0; iBasis<nBases; iBasis++)
                {
                    bestModel[iOutDim * nBases + iBasis] = coefficients[iOutDim][iBasis];
                }
        }
        //        
        ////////////////////////////////////
        
        cout << "Iteration #" << iteration << " took " << (ofGetElapsedTimef() - startTime) << "s to complete. bestError=" << bestError << " bestConsensus count=" << bestConsensus.size() << "\n";
    }

    //reload best consensus
	dataSet.setActiveIndices(bestConsensus);
    
    ////////////////////////////////////
    // Reload best model as current fit
    ////////////////////////////////////
    //
    for (int iOutDim=0; iOutDim<_fit->_outdim; iOutDim++)
        for (int iBasis=0; iBasis<nBases; iBasis++)
        {
            coefficients[iOutDim][iBasis] = bestModel[iOutDim * nBases + iBasis];
        }
	delete[] bestModel;
    //
    ////////////////////////////////////
}

template<typename T>
pfitDataSetf ofxPolyFit_<T>::makeDataSet(const vector<ofVec3f> &input, const vector<ofVec3f> &output) {
	int size = input.size();
	if (size != output.size())
		throw("ofxPolyfit::makeDataSet - vector input size mismatch error");
		
	pfitDataSetf set;
	set.init(3, 3, size);
	memcpy(set.getInput(), &input[0], sizeof(ofVec3f) * size);
	memcpy(set.getOutput(), &output[0], sizeof(ofVec3f) * size);
		
	return set;
}

template<typename T>
pfitDataSetf ofxPolyFit_<T>::makeDataSet(const vector<ofVec2f> &input, const vector<ofVec2f> &output) {
	int size = input.size();
	if (size != output.size())
		throw("ofxPolyfit::makeDataSet - vector input size mismatch error");
	
	pfitDataSetf set;
	set.init(2, 2, size);
	memcpy(set.getInput(), &input[0], sizeof(ofVec2f) * size);
	memcpy(set.getOutput(), &output[0], sizeof(ofVec2f) * size);
	
	return set;
}

template<typename T>
bool ofxPolyFit_<T>::checkInitialised() const
{
    if (!_isInitialised)
    {
        ofLogError() << "ofxPolyFit : We're not yet initialised";
        return false;
    } else
        return true;
}

template class ofxPolyFit_<double>;
template class ofxPolyFit_<float>;