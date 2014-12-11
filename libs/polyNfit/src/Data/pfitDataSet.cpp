//
//  pfitDataSet.cpp
//  ofxPolyFit
//
//  Created by Elliot Woods on 19/06/2011.
//  Copyright 2011 Kimchi and Chips. All rights reserved.
//

#include "pfitDataSet.h"

#include <algorithm>

template<typename DataType>
pfitDataSet<DataType>::pfitDataSet() :
_dataAllocated(false),
_nDataPoints(0),
_nDataPointsAllocated(0),
_inDimensions(0),
_outDimensions(0),
_inData(NULL),
_outData(NULL),
_activeData(NULL)
{
    
}

template<typename DataType>
pfitDataSet<DataType>::pfitDataSet(const pfitDataSet<DataType> &other) {
	this->_dataAllocated = false;
	
	this->init(other.getInputDimensions(), other.getOutputDimensions(), other.size());
	
	this->setInput(other.getInput());
	this->setOutput(other.getOutput());
	this->setActive(other.getActive());
}

template<typename DataType>
pfitDataSet<DataType>::~pfitDataSet() {
    clear();
}

///////

template<typename DataType>
pfitDataPoint<DataType> pfitDataSet<DataType>::operator[]( pfitIndex i ) {
    checkAllocated();
    
    if (i < _nDataPoints)
        return pfitDataPoint<DataType>(_inDimensions, _outDimensions, _inData + i*_inDimensions, _outData + i*_outDimensions, &_activeData[i]);
    else
        throw("pfitDataSet::[] : index out of range");
}

template<typename DataType>
pfitDataPoint<DataType> pfitDataSet<DataType>::operator[]( pfitIndex i ) const {
    checkAllocated();
    
    if (i < _nDataPoints)
        return pfitDataPoint<DataType>(_inDimensions, _outDimensions, _inData + i*_inDimensions, _outData + i*_outDimensions, &_activeData[i]);
    else
        throw("pfitDataSet::[] : index out of range");
}

template<typename DataType>
void pfitDataSet<DataType>::init(int inputDimensions, int outputDimensions, pfitIndex size)
{
    deAllocate();
    
    _inDimensions = inputDimensions;
    _outDimensions = outputDimensions;
    
    resize(size);
}

template<typename DataType>
void pfitDataSet<DataType>::clear() {
    deAllocate();
}

template<typename DataType>
void pfitDataSet<DataType>::resize(pfitIndex const size) {
    //save time by only copying relevant points
    //during allocation
    _nDataPoints = min(_nDataPoints, size);
    
    if (size > _nDataPointsAllocated)
    {
		int oldAllocationSize = _nDataPointsAllocated;
        //reallocate to fit new size
        allocate(1 << int(ceil(log(double(size+1))/log((double)2.0))));
		
		//default to active for new points
		for(int i=_nDataPoints; i<size; i++) {
			this->_activeData[i] = true;
		}
    }
    
    pfitIndex quarterOfAllocation = _nDataPointsAllocated >> 2;
    if (size < quarterOfAllocation)
    {
        //reallocate to fit new size
        allocate(1 << int(ceil(log(double(size+1))/log((double)2.0))));
    }
    
    _nDataPoints = size;
}

template<typename DataType>
void pfitDataSet<DataType>::allocate(pfitIndex const allocation)
{
	deAllocate();
    
	if (allocation == 0) {
		return;
	}
    
    /////////////////////
    // ALLOCATE DATA
    /////////////////////
    //
    if (_dataAllocated) {
		_inData = (DataType*) realloc(_inData,  allocation * sizeof(DataType) * _inDimensions);
		_outData = (DataType*) realloc(_outData, allocation * sizeof(DataType) * _outDimensions);
		_activeData = (bool*) realloc(_activeData, allocation * sizeof(bool));
    } else {
		_inData = (DataType*) malloc(allocation * sizeof(DataType) * _inDimensions);
		_outData = (DataType*) malloc(allocation * sizeof(DataType) * _outDimensions);
		_activeData = (bool*) malloc(allocation * sizeof(bool));
	}
    //
    /////////////////////
 
	//update allocation flags
	_nDataPointsAllocated = allocation;
    _dataAllocated = true;

	//reduce dataset size if outside of allocation
    _nDataPoints = min(allocation, _nDataPoints);
}

template<typename DataType>
void pfitDataSet<DataType>::deAllocate()
{
    if (_dataAllocated)
    {
        free(_inData);
        free(_outData);
        free(_activeData);
        _dataAllocated = false;
    }
    
    _nDataPoints = 0;
    _nDataPointsAllocated = 0;
}

////

template<typename DataType>
DataType* pfitDataSet<DataType>::getInput() {
    return _inData;
}

template<typename DataType>
const DataType* pfitDataSet<DataType>::getInput() const {
    return _inData;
}

template<typename DataType>
DataType* pfitDataSet<DataType>::getOutput() {
    return _outData;
}

template<typename DataType>
const DataType* pfitDataSet<DataType>::getOutput() const {
    return _outData;
}

template<typename DataType>
bool* pfitDataSet<DataType>::getActive() {
    return _activeData;
}

template<typename DataType>
const bool* pfitDataSet<DataType>::getActive() const {
    return _activeData;
}

////

template<typename DataType>
void pfitDataSet<DataType>::setInput(const DataType* data) {
	memcpy(getInput(), data, size() * sizeof(DataType) * getInputDimensions());
}

template<typename DataType>
void pfitDataSet<DataType>::setOutput(const DataType* data) {
	memcpy(getOutput(), data, size() * sizeof(DataType) * getOutputDimensions());
}

template<typename DataType>
void pfitDataSet<DataType>::setActive(const bool* data) {
	memcpy(getActive(), data, size() * sizeof(bool));
}

////

template<typename DataType>
int pfitDataSet<DataType>::getInputDimensions() const {
	return _inDimensions;
}

template<typename DataType>
int pfitDataSet<DataType>::getOutputDimensions() const {
	return _outDimensions;
}

////

template<typename DataType>
pfitDataPoint<DataType> pfitDataSet<DataType>::begin() const {
    return operator[](0);
}

template<typename DataType>
pfitDataPoint<DataType> pfitDataSet<DataType>::end() const {
    return operator[](_nDataPoints-1) + 1;
}

////

template<typename DataType>
set<pfitIndex> pfitDataSet<DataType>::getActiveIndices() const {
    checkAllocated();
    
    set<pfitIndex> activeIndices;
    
    for (pfitIndex i=0; i<_nDataPoints; i++)
    {
        if (_activeData[i])
            activeIndices.insert(i);
    }

	return activeIndices;
}

template<typename DataType>
pfitIndex pfitDataSet<DataType>::getActiveCount() const {
    checkAllocated();
    
    pfitIndex count = 0;
    
    for (pfitIndex i=0; i<_nDataPoints; i++) {
        if (_activeData[i])
            count++;
    }
    
    return count;
}

template<typename DataType>
void pfitDataSet<DataType>::setActiveIndices(const pfitIndexSet& s) {
    checkAllocated();
    
    for (pfitIndex i=0; i<_nDataPoints; i++)
		_activeData[i] = (s.count(i) != 0);
}

template<typename DataType>
void pfitDataSet<DataType>::setActiveAll() {
    checkAllocated();
	memset(_activeData, true, _nDataPoints * sizeof(bool));
}

template<typename DataType>
void pfitDataSet<DataType>::setActiveNone() {
    checkAllocated();
	memset(_activeData, false, _nDataPoints * sizeof(bool));
}

////

template<typename DataType>
string pfitDataSet<DataType>::toString() const {
    stringstream output;
    
    for (pfitIndex i=0; i<_nDataPoints; i++)
        output << operator[](i).toString() << endl;
    
    return output.str();
}

////

template<typename DataType>
void pfitDataSet<DataType>::load(string filename) {
	if (filename=="")
		return;
	
	ifstream file(filename.c_str(), ios::binary);
	if (file.good()) {
		this->deAllocate();
		
		file.read((char*) &_inDimensions, sizeof(_inDimensions));
		file.read((char*) &_outDimensions, sizeof(_outDimensions));
		file.read((char*) &_nDataPoints, sizeof(_nDataPoints));
		
		this->resize(_nDataPoints);
		
		file.read((char*) _inData, sizeof(DataType) * _nDataPoints * _inDimensions);
		file.read((char*) _outData, sizeof(DataType) * _nDataPoints * _outDimensions);
		
		file.close();
	}
}

template<typename DataType>
void pfitDataSet<DataType>::save(string filename) {
	if (filename=="")
		return;
	
	ofstream file(filename.c_str(), ios::binary);
	if (file.good()) {		
		file.write((char*) &_inDimensions, sizeof(_inDimensions));
		file.write((char*) &_outDimensions, sizeof(_outDimensions));
		file.write((char*) &_nDataPoints, sizeof(_nDataPoints));	
		file.write((char*) _inData, sizeof(DataType) * _nDataPoints * _inDimensions);
		file.write((char*) _outData, sizeof(DataType) * _nDataPoints * _outDimensions);
		file.close();
	}
}

/////////////////////////
// protected

template<typename DataType>
void pfitDataSet<DataType>::checkAllocated() const {
    if (!_dataAllocated)
        throw ("pfitDataSet: data not allocated");
}

template <typename DataType>
void pfitDataSet<DataType>::throwIfNotReady(int inDimensions, int outDimensions, int nBases) const {
    if (_inDimensions != inDimensions)
        throw("pfitDataSet : input dimensions of data set do not match fit");
    
    if (_outDimensions != outDimensions)
        throw("pfitDataSet : output dimensions of data set do not match fit");
    
    if (getActiveCount() < nBases)
        throw("pfitDataSet : insufficient datapoints to fill all bases");
}

template class pfitDataSet<double>;
template class pfitDataSet<float>;
