//
//  pfitDataSet.cpp
//  ofxPolyFit
//
//  Created by Elliot Woods on 19/06/2011.
//  Copyright 2011 Kimchi and Chips. All rights reserved.
//

#include "pfitDataSet.h"

///////////////////////////////////////////////////////
// pfitDataPoint
//
template<typename T>
pfitDataPoint<T>::pfitDataPoint() : 
_inDimensions(0),
_outDimensions(0),
_enabled(true),
_locallyAllocated(false)
{
}

template<typename T>
pfitDataPoint<T>::pfitDataPoint(int inputDimensions, int outputDimensions, T* inputData, T* outputData, bool enabled) : 
_inDimensions(inputDimensions),
_outDimensions(outputDimensions),
_inputData(inputData),
_outputData(outputData),
_enabled(enabled),
_locallyAllocated(false)
{
}

template<typename T>
pfitDataPoint<T>::pfitDataPoint(int inputDimensions, int outputDimensions, bool enabled = true) :
_inDimensions(inputDimensions),
_outDimensions(outputDimensions),
_enabled(enabled),
_locallyAllocated(false)
{
	localAllocate();
}

template<typename T>
pfitDataPoint<T>::~pfitDataPoint()
{
    if (_locallyAllocated)
        localRelease();
}

template<typename T>
void pfitDataPoint<T>::operator++()
{
    _inputData += _inDimensions;
    _outputData += _outDimensions;
}

template<typename T>
pfitDataPoint<T> pfitDataPoint<T>::operator+(const pfitIndex &RHS)
{
    return pfitDataPoint(_inDimensions, _outDimensions,
            _inputData + _inDimensions * RHS,
            _outputData + _outDimensions * RHS,
            _enabled);            
}

template<typename T>
bool pfitDataPoint<T>::operator==(pfitDataPoint const &RHS)
{
    //presume if input is same, then output is same
    return (this->getInput() == RHS.getInput());
}

template<typename T>
bool pfitDataPoint<T>::operator!=(pfitDataPoint const &RHS)
{
    //perhaps this should reference == 
    
    //presume if input is same, then output is same
    return (this->getInput() != RHS.getInput());

}

template<typename T>
void pfitDataPoint<T>::clearOutput()
{
    for (int i=0; i<_outDimensions; i++)
        _outputData[i] = 0;
}

template <typename T>
void pfitDataPoint<T>::throwIfNotReady(int inDimensions, int outDimensions) const
{
    if (_inDimensions != inDimensions)
        throw("pfitDataPoint : input dimensions of data set do not match fit");
    
    if (_outDimensions != outDimensions)
        throw("pfitDataPoint : output dimensions of data set do not match fit");
}

////

template <typename T>
void pfitDataPoint<T>::localAllocate()
{
    if (_locallyAllocated)
        localRelease();
    
    
    _inputData = new T[_inDimensions];
    _outputData = new T[_outDimensions];
    
    
    _locallyAllocated = true;
}

template <typename T>
void pfitDataPoint<T>::localRelease()
{
    if (!_locallyAllocated)
        throw("pfitDataPoint: cannot release data as we have nothing locally allocated");
    
    delete[] _inputData;
    delete[] _outputData;
    
}

////

template <typename T>
T* pfitDataPoint<T>::getInput() const
{
    return _inputData;    
}

template <typename T>
T* pfitDataPoint<T>::getOutput() const
{
    return _outputData;    
}


template <typename T>
T* pfitDataPoint<T>::getInput()
{
    return _inputData;    
}

template <typename T>
T* pfitDataPoint<T>::getOutput()
{
    return _outputData;    
}

template <typename T>
bool pfitDataPoint<T>::getEnabled() const
{
    return _enabled;    
}

////

template <typename T>
string pfitDataPoint<T>::toString() const
{
    stringstream output;
    
    output.precision(3);
    
    //x
    output << "(";
    for (int dim=0; dim<_inDimensions; dim++) {
        if (dim>0)
            output << ", ";
        
        output << _inputData[dim];
    }
    output << ") \t=>";

    //y
    output << "\t(";
    for (int dim=0; dim<_outDimensions; dim++) {
        if (dim>0)
            output << ", ";
        
        output << _outputData[dim];
    }
    output << ") ";
    
    //enabed
    output << " \t[" << (_enabled ? "x" : " ") << "]";

    
    return output.str();    
}


//
///////////////////////////////////////////////////////







///////////////////////////////////////////////////////
// pfitDataSet
//

template<typename T>
pfitDataSet<T>::pfitDataSet() :
_dataAllocated(false),
_nDataPoints(0),
_nDataPointsAllocated(0),
_inDimensions(0),
_outDimensions(0)
{
    
}

template<typename T>
pfitDataSet<T>::~pfitDataSet()
{
    clear();
}

template<typename T>
pfitDataPoint<T> pfitDataSet<T>::operator[]( pfitIndex i )
{
    checkAllocated();
    
    if (i < _nDataPoints)
        return pfitDataPoint<T>(_inDimensions, _outDimensions, _inData + i*_inDimensions, _outData + i*_outDimensions, _enabledData[i]);
    else
        throw("pfitDataSet::[] : index out of range");
}

template<typename T>
pfitDataPoint<T> pfitDataSet<T>::operator[]( pfitIndex i ) const
{
    checkAllocated();
    
    if (i < _nDataPoints)
        return pfitDataPoint<T>(_inDimensions, _outDimensions, _inData + i*_inDimensions, _outData + i*_outDimensions, _enabledData[i]);
    else
        throw("pfitDataSet::[] : index out of range");
}

template<typename T>
void pfitDataSet<T>::init(int inputDimensions, int outputDimensions, pfitIndex size)
{
    deAllocate();
    
    _inDimensions = inputDimensions;
    _outDimensions = outputDimensions;
    
    resize(size);
}

template<typename T>
void pfitDataSet<T>::clear()
{
    deAllocate();
}

template<typename T>
void pfitDataSet<T>::resize(pfitIndex const size)
{
    //save time by only copying relevant points
    //during allocation
    _nDataPoints = min(_nDataPoints, size);
    
    if (size > _nDataPointsAllocated)
    {
        //reallocate to fit new size
        allocate(1 << int(ceil(log(double(size+1))/log((double)2.0))));
        
    }
    
    pfitIndex quarterOfAllocation = _nDataPointsAllocated >> 2;
    if (size < quarterOfAllocation)
    {
        //reallocate to fit new size
        allocate(1 << int(ceil(log(double(size+1))/log((double)2.0))));
    }
    
    _nDataPoints = size;
}

template<typename T>
void pfitDataSet<T>::allocate(pfitIndex const allocation)
{
    if (allocation == 0)
    {
        deAllocate();
        return;        
    }
    
    T *newDataAreaIn = new T[allocation * _inDimensions];
    T *newDataAreaOut = new T[allocation * _outDimensions];
    bool *newDataAreaEnabled = new bool[allocation];

    //set default enabled to 'true'
    memset(newDataAreaEnabled, true, allocation * sizeof(bool));
    
    //reduce dataset size if outside of allocation
    _nDataPoints = min(allocation, _nDataPoints);
    
    /////////////////////
    // MOVE DATA
    /////////////////////
    //
    if (_dataAllocated)
    {
        //move to new memory area
        memcpy(newDataAreaIn, _inData, _nDataPoints * sizeof(T) * _inDimensions);
        memcpy(newDataAreaOut, _outData, _nDataPoints * sizeof(T) * _outDimensions);
        memcpy(newDataAreaEnabled, _enabledData, _nDataPoints * sizeof(bool));
        
        //delete old memory area
        delete[] _inData;
        delete[] _outData;
        delete[] _enabledData;
    }
    //
    /////////////////////
    
    //move pointer to new area
    _inData = newDataAreaIn;
    _outData = newDataAreaOut;
    _enabledData = newDataAreaEnabled;
    
    _nDataPointsAllocated = allocation;
    _dataAllocated = true;
}

template<typename T>
void pfitDataSet<T>::deAllocate()
{
    if (_dataAllocated)
    {
        delete[] _inData;
        delete[] _outData;
        delete[] _enabledData;
        _dataAllocated = false;
    }
    
    _nDataPoints = 0;
    _nDataPointsAllocated = 0;
}

////

template<typename T>
T* pfitDataSet<T>::getInput()
{
    return _inData;
}

template<typename T>
T* pfitDataSet<T>::getOutput()
{
    return _outData;
}

////

template<typename T>
pfitDataPoint<T> pfitDataSet<T>::begin() const
{
    return operator[](0);
}

template<typename T>
pfitDataPoint<T> pfitDataSet<T>::end() const
{
    return operator[](_nDataPoints-1) + 1;
}

////

template<typename T>
set<pfitIndex> pfitDataSet<T>::getActiveIndices() const
{
    checkAllocated();
    
    set<pfitIndex> activeIndices;
    
    for (pfitIndex i=0; i<_nDataPoints; i++)
    {
        if (_enabledData[i])
            activeIndices.insert(i);
    }

	return activeIndices;
}

template<typename T>
pfitIndex pfitDataSet<T>::getActiveIndicesCount() const
{
    checkAllocated();
    
    pfitIndex count = 0;
    
    for (pfitIndex i=0; i<_nDataPoints; i++)
    {
        if (_enabledData[i])
            count++;
    }
    
    return count;
}

////

template<typename T>
string pfitDataSet<T>::toString() const
{
    stringstream output;
    
    for (pfitIndex i=0; i<_nDataPoints; i++)
        output << operator[](i).toString() << endl;
    
    return output.str();
}

/////////////////////////
// protected

template<typename T>
void pfitDataSet<T>::checkAllocated() const
{
    if (!_dataAllocated)
        throw ("pfitDataSet: data not allocated");
}

template <typename T>
void pfitDataSet<T>::throwIfNotReady(int inDimensions, int outDimensions, int nBases) const
{
    if (_inDimensions != inDimensions)
        throw("pfitDataSet : input dimensions of data set do not match fit");
    
    if (_outDimensions != outDimensions)
        throw("pfitDataSet : output dimensions of data set do not match fit");
    
    if (getActiveIndicesCount() < nBases)
        throw("pfitDataSet : insufficient datapoints to fill all bases");
}
//
///////////////////////////////////////////////////////

template class pfitDataPoint<double>;
template class pfitDataPoint<float>;

template class pfitDataSet<double>;
template class pfitDataSet<float>;
