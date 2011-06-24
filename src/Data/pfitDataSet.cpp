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

template<typename DataType>
pfitDataPoint<DataType>::pfitDataPoint(int inputDimensions, int outputDimensions, DataType* inputData, DataType* outputData, bool enabled) : 
_inDimensions(inputDimensions),
_outDimensions(outputDimensions),
_inputData(inputData),
_outputData(outputData),
_enabled(enabled),
_locallyAllocated(false)
{
}

template<typename DataType>
pfitDataPoint<DataType>::~pfitDataPoint()
{
    if (_locallyAllocated)
        localRelease();
}

template<typename DataType>
void pfitDataPoint<DataType>::operator++()
{
    _inputData += _inDimensions;
    _outputData += _outDimensions;
}

template<typename DataType>
pfitDataPoint<DataType> pfitDataPoint<DataType>::operator+(const pfitIndex &RHS)
{
    return pfitDataPoint(_inDimensions, _outDimensions,
            _inputData + _inDimensions * RHS,
            _outputData + _outDimensions * RHS,
            _enabled);            
}

template<typename DataType>
bool pfitDataPoint<DataType>::operator==(pfitDataPoint const &RHS)
{
    //presume if input is same, then output is same
    return (this->getInput() == RHS.getInput());
}

template<typename DataType>
bool pfitDataPoint<DataType>::operator!=(pfitDataPoint const &RHS)
{
    //perhaps this should reference == 
    
    //presume if input is same, then output is same
    return (this->getInput() != RHS.getInput());

}

template<typename DataType>
void pfitDataPoint<DataType>::clearOutput()
{
    for (int i=0; i<_outDimensions; i++)
        _outputData[i] = 0;
}

template <typename DataType>
void pfitDataPoint<DataType>::throwIfNotReady(int inDimensions, int outDimensions) const
{
    if (_inDimensions != inDimensions)
        throw("pfitDataPoint : input dimensions of data set do not match fit");
    
    if (_outDimensions != outDimensions)
        throw("pfitDataPoint : output dimensions of data set do not match fit");
}

////

template <typename DataType>
void pfitDataPoint<DataType>::localAllocate()
{
    if (_locallyAllocated)
        localRelease();
    
    
    _inputData = new DataType[_inDimensions];
    _outputData = new DataType[_outDimensions];
    
    
    _locallyAllocated = true;
}

template <typename DataType>
void pfitDataPoint<DataType>::localRelease()
{
    if (!_locallyAllocated)
        throw("pfitDataPoint: cannot release data as we have nothing locally allocated");
    
    delete[] _inputData;
    delete[] _outputData;
    
}

////

template <typename DataType>
DataType* pfitDataPoint<DataType>::getInput() const
{
    return _inputData;    
}

template <typename DataType>
DataType* pfitDataPoint<DataType>::getOutput() const
{
    return _outputData;    
}


template <typename DataType>
DataType* pfitDataPoint<DataType>::getInput()
{
    return _inputData;    
}

template <typename DataType>
DataType* pfitDataPoint<DataType>::getOutput()
{
    return _outputData;    
}

template <typename DataType>
bool pfitDataPoint<DataType>::getEnabled() const
{
    return _enabled;    
}

////

template <typename DataType>
string pfitDataPoint<DataType>::toString() const
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

template<typename DataType>
pfitDataSet<DataType>::pfitDataSet() :
_dataAllocated(false),
_nDataPoints(0),
_nDataPointsAllocated(0),
_inDimensions(0),
_outDimensions(0)
{
    
}

template<typename DataType>
pfitDataSet<DataType>::~pfitDataSet()
{
    clear();
}

template<typename DataType>
pfitDataPoint<DataType> pfitDataSet<DataType>::operator[]( pfitIndex i )
{
    checkAllocated();
    
    if (i < _nDataPoints)
        return pfitDataPoint<DataType>(_inDimensions, _outDimensions, _inData + i*_inDimensions, _outData + i*_outDimensions, _enabledData[i]);
    else
        throw("pfitDataSet::[] : index out of range");
}

template<typename DataType>
pfitDataPoint<DataType> pfitDataSet<DataType>::operator[]( pfitIndex i ) const
{
    checkAllocated();
    
    if (i < _nDataPoints)
        return pfitDataPoint<DataType>(_inDimensions, _outDimensions, _inData + i*_inDimensions, _outData + i*_outDimensions, _enabledData[i]);
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
void pfitDataSet<DataType>::clear()
{
    deAllocate();
}

template<typename DataType>
void pfitDataSet<DataType>::resize(pfitIndex const size)
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

template<typename DataType>
void pfitDataSet<DataType>::allocate(pfitIndex const allocation)
{
    if (allocation == 0)
    {
        deAllocate();
        return;        
    }
    
    DataType *newDataAreaIn = new DataType[allocation * _inDimensions];
    DataType *newDataAreaOut = new DataType[allocation * _outDimensions];
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
        memcpy(newDataAreaIn, _inData, _nDataPoints * sizeof(DataType) * _inDimensions);
        memcpy(newDataAreaOut, _inData, _nDataPoints * sizeof(DataType) * _outDimensions);
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

template<typename DataType>
void pfitDataSet<DataType>::deAllocate()
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

template<typename DataType>
DataType* pfitDataSet<DataType>::getInput()
{
    return _inData;
}

template<typename DataType>
DataType* pfitDataSet<DataType>::getOutput()
{
    return _outData;
}

////

template<typename DataType>
pfitDataPoint<DataType> pfitDataSet<DataType>::begin() const
{
    return operator[](0);
}

template<typename DataType>
pfitDataPoint<DataType> pfitDataSet<DataType>::end() const
{
    return operator[](_nDataPoints-1) + 1;
}

////

template<typename DataType>
set<pfitIndex> pfitDataSet<DataType>::getActiveIndices() const
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

template<typename DataType>
pfitIndex pfitDataSet<DataType>::getActiveIndicesCount() const
{
    checkAllocated();
    
    pfitIndex count;
    
    for (pfitIndex i=0; i<_nDataPoints; i++)
    {
        if (_enabledData[i])
            count++;
    }
    
    return count;
}

////

template<typename DataType>
string pfitDataSet<DataType>::toString() const
{
    stringstream output;
    
    for (pfitIndex i=0; i<_nDataPoints; i++)
        output << operator[](i).toString() << endl;
    
    return output.str();
}

/////////////////////////
// protected

template<typename DataType>
void pfitDataSet<DataType>::checkAllocated() const
{
    if (!_dataAllocated)
        throw ("pfitDataSet: data not allocated");
}

template <typename DataType>
void pfitDataSet<DataType>::throwIfNotReady(int inDimensions, int outDimensions, int nBases) const
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
