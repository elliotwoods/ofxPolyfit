//
//  pfitDataPoint.cpp
//  ofxLevMar-example
//

#include "pfitDataPoint.h"

template<typename DataType>
bool pfitDataPoint<DataType>::_activeTrue = true;

///blank constructor
template<typename DataType>
pfitDataPoint<DataType>::pfitDataPoint() : 
_inDimensions(0),
_outDimensions(0),
_locallyAllocated(0) {
}

///remote mirror constructor
template<typename DataType>
pfitDataPoint<DataType>::pfitDataPoint(int inputDimensions, int outputDimensions, DataType* inputData, DataType* outputData, bool* activeData) : 
_inDimensions(inputDimensions),
_outDimensions(outputDimensions),
_inputData(inputData),
_outputData(outputData),
_activeData(activeData),
_locallyAllocated(false) {
}

///locally allocated constructor
template<typename DataType>
pfitDataPoint<DataType>::pfitDataPoint(int inputDimensions, int outputDimensions) :
_inDimensions(inputDimensions),
_outDimensions(outputDimensions),
_locallyAllocated(false) {
	localAllocate();
}

///Destructor
template<typename DataType>
pfitDataPoint<DataType>::~pfitDataPoint() {
    if (_locallyAllocated)
        localRelease();
}

template<typename DataType>
void pfitDataPoint<DataType>::operator++() {
    _inputData += _inDimensions;
    _outputData += _outDimensions;
	_activeData += 1;
}

template<typename DataType>
pfitDataPoint<DataType> pfitDataPoint<DataType>::operator+(const pfitIndex &RHS) {
    return pfitDataPoint(_inDimensions, _outDimensions,
						 _inputData + _inDimensions * RHS,
						 _outputData + _outDimensions * RHS,
						 _activeData + RHS);
}

template<typename DataType>
bool pfitDataPoint<DataType>::operator==(pfitDataPoint const &RHS) {
    //presume if input is same, then output is same
    return (this->getInput() == RHS.getInput());
}

template<typename DataType>
bool pfitDataPoint<DataType>::operator!=(pfitDataPoint const &RHS) {
    //perhaps this should reference == 
    
    //presume if input is same, then output is same
    return (this->getInput() != RHS.getInput());
	
}

template<typename DataType>
void pfitDataPoint<DataType>::clearOutput() {
    for (int i=0; i<_outDimensions; i++)
        _outputData[i] = 0;
}

template <typename DataType>
void pfitDataPoint<DataType>::throwIfNotReady(int inDimensions, int outDimensions) const {
    if (_inDimensions != inDimensions)
        throw("pfitDataPoint : input dimensions of data set do not match fit");
    
    if (_outDimensions != outDimensions)
        throw("pfitDataPoint : output dimensions of data set do not match fit");
}

////

template <typename DataType>
void pfitDataPoint<DataType>::localAllocate() {
    if (_locallyAllocated)
        localRelease();
    
    
    _inputData = new DataType[_inDimensions];
    _outputData = new DataType[_outDimensions];
	_activeData = new bool;
	*_activeData = true;
    
    _locallyAllocated = true;
}

template <typename DataType>
void pfitDataPoint<DataType>::localRelease() {
    if (!_locallyAllocated)
        throw("pfitDataPoint: cannot release data as we have nothing locally allocated");
    
    delete[] _inputData;
    delete[] _outputData;
    delete _activeData;
}

////

template <typename DataType>
DataType* pfitDataPoint<DataType>::getInput() const {
    return _inputData;    
}

template <typename DataType>
DataType* pfitDataPoint<DataType>::getOutput() const {
    return _outputData;    
}


template <typename DataType>
DataType* pfitDataPoint<DataType>::getInput() {
    return _inputData;    
}

template <typename DataType>
DataType* pfitDataPoint<DataType>::getOutput() {
    return _outputData;    
}

template <typename DataType>
bool pfitDataPoint<DataType>::getActive() const {
    return *_activeData;    
}

template <typename DataType>
void  pfitDataPoint<DataType>::setActive(const bool b) {
    *_activeData = b;
}

////

template <typename DataType>
string pfitDataPoint<DataType>::toString() const {
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
    output << " \t[" << (*_activeData ? "x" : " ") << "]";
	
    
    return output.str();    
}


template <typename DataType>
pfitDataPoint<DataType> pfitDataPoint<DataType>::makeCopy() const {
	pfitDataPoint<DataType> p(this->_inDimensions, this->_outDimensions);
	
	memcpy(p.getInput(), this->getInput(), _inDimensions * sizeof(DataType));
	memcpy(p.getOutput(), this->getOutput(), _outDimensions * sizeof(DataType));
	p.setActive(this->getActive());
	
	return p;
}

////

template class pfitDataPoint<double>;
template class pfitDataPoint<float>;
