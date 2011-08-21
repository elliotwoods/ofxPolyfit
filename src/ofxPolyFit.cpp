/*
 *  ofxPolyFit.cpp
 *  ofxPolyFit example
 *
 *  Created by Elliot Woods on 03/11/2010.
 *  Copyright 2010 Kimchi and Chips. All rights reserved.
 *
 */

#include "ofxPolyFit.h"

template<class T>
ofxPolyFit_<T>::ofxPolyFit_() : 
bestModel(0)
{
	_isInitialised = false;
	_success = false;
	nBases = 0;
}

template<class T>
ofxPolyFit_<T>::~ofxPolyFit_()
{
	if (_isInitialised)
    {
        for (int i=0; i<_fit->_outdim; i++)
            delete[] coefficients[i];

		delete _fit;
    }
}

template<class T>
void ofxPolyFit_<T>::init(int order, int dimensionsIn, int dimensionsOut, pfitBasisType basisType)
{
	if (_isInitialised)
		uninitialise();
	
	_fit = new polyNfit<T>(order, dimensionsIn, dimensionsOut, basisType);
	
	basisIndicies = &(_fit->vecBasisIndices);
	
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

template<class T>
void ofxPolyFit_<T>::uninitialise()
{
    for (int i=0; i<_fit->_outdim; i++)
        delete[] coefficients[i];
    coefficients.clear();
    delete _fit;
    nBases = 0;
    
    _isInitialised = false;
    _success = false;
}

template<class T>
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

template<class T>
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
template <class T>
void ofxPolyFit_<T>::evaluate(pfitDataSet<T> &dataSet) const
{
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
    
    
    /////////////////////////
    // Evaluate points
    /////////////////////////
    //
	int i;
	for (i = 0; i<dataSet.size(); ++i)
    {
        evaluate(dataSet[i], false);
    }
    //
    /////////////////////////
    
}

template <class T>
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
    
    pfitDataPoint<T> point(1, 1, &input, &output);
    
    evaluate(point, false);
    
    return output;
}

template <class T>
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
    
    pfitDataPoint<T> pointResult(_fit->_indim, _fit->_outdim, dataPoint.getInput(), outResult);
    
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

template<class T>
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
        if (point.getEnabled())
            residualTotal += residualSquared(point);
    }
    //
    /////////////////////////
    
    
    /////////////////////////
    // Mean squared
    /////////////////////////
    //
    residualTotal /= T(dataSet.getActiveIndicesCount());
    return sqrt(residualTotal);
    //
    /////////////////////////
    
    
}

template<class T>
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
	
	
	if (_fit->_basesShape < BASIS_SHAPE_PADE_FIRST)
		for (int iBasis=0; iBasis<nBases; iBasis++)
			fileout.write((char*) basisIndicies->at(iBasis),
						  sizeof(unsigned int) * _fit->_indim);
	
	for (int iDimOut=0; iDimOut<_fit->_outdim; iDimOut++)
		fileout.write((char*) coefficients[iDimOut],
					  sizeof(T) * nBases);
    
	ofLog(OF_LOG_VERBOSE, "ofxPolyFit: Fit saved");
}

template<class T>
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
			filein.read((char*) basisIndicies->at(iBasis),
						  _fit->_indim * 4);
	
	for (int iDimOut=0; iDimOut<_fit->_outdim; iDimOut++)
		filein.read((char*) coefficients[iDimOut],
					  sizeof(T) * nBases);
	
	_success = true;
}

/*

void ofxPolyFit_<T>::RANSAC(double* input, double* output, int nDataPoints, int maxIterations, float selectionProbability, float residualThreshold, float inclusionThreshold)
{
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
    
    int inclusionThresholdCount = inclusionThreshold * float(nDataPoints);
    bestError = + 1e37;
    bestConsensus.clear();
    
    if (bestModel != 0)
        delete[] bestModel;
    bestModel = new double[_fit->_outdim * nBases];
    
    set<int> maybeInlierIndices;
    set<int> currentConsensus;
    set<int>::iterator idxIt;
    double currentError;
    
    vector<double> vecInputPoint(_fit->_indim);
    vector<double> vecOutputPoint(_fit->_outdim);
    
    float startTime;
    
    //loop through allowed number of iterations
    for (int iteration=0; iteration<maxIterations; iteration++)
    {
        startTime = ofGetElapsedTimef();
        
        //////////////////////////////////
        // Randomly select maybe inliers
        //////////////////////////////////
        //
        maybeInlierIndices.clear();
        for (int iPoint=0; iPoint<nDataPoints; iPoint++)
            if (ofRandomuf() < selectionProbability)
                maybeInlierIndices.insert(iPoint);
        
        if (maybeInlierIndices.size() < nBases)
            continue;
        //
        //////////////////////////////////
        
        
        ////////////////////////////////////
        // Perform a fit with maybe inliers
        ////////////////////////////////////
        //
        correlate(input, output, maybeInlierIndices);
        //
        ////////////////////////////////////
        
        
        ////////////////////////////////////
        // Build consensus set
        ////////////////////////////////////
        //
        currentConsensus = maybeInlierIndices;
        
        for (int iPoint=0; iPoint<nDataPoints; iPoint++)
        {
            //first check if already in set
            if (currentConsensus.count(iPoint) != 0)
                continue;
            
            memcpy(&vecInputPoint[0], input + (iPoint * _fit->_indim), _fit->_indim * sizeof(double)); 
            memcpy(&vecOutputPoint[0], output + (iPoint * _fit->_outdim), _fit->_outdim * sizeof(double)); 
            
            //if residual for this point is less than threshold
            //then add it to the consensus set
            if (residual(vecInputPoint, vecOutputPoint) < residualThreshold)
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
        correlate(input, output, currentConsensus);
        currentError = residual(input, output, currentConsensus);
        
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
    
    
    ////////////////////////////////////
    // Reload best model as current fit
    ////////////////////////////////////
    //
    for (int iOutDim=0; iOutDim<_fit->_outdim; iOutDim++)
        for (int iBasis=0; iBasis<nBases; iBasis++)
        {
            coefficients[iOutDim][iBasis] = bestModel[iOutDim * nBases + iBasis];
        }    
    //
    ////////////////////////////////////
}
            
*/

template<class T>
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