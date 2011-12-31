#pragma once
//
//  pfitDataPoint.h
//  ofxPolyfit
//

#include <string>
#include <sstream>
#include <set>

using namespace std;
typedef unsigned int pfitIndex;
typedef set<pfitIndex> pfitIndexSet;

template<typename DataType>
class pfitDataPoint
{
public:
	///Blank constructor
	pfitDataPoint();
	///Remote mirror constructor
	pfitDataPoint(int inputDimensions, int outputDimensions, DataType* inputData, DataType* outputData, bool* activeData = &pfitDataPoint::_activeTrue);
	///Locally allocated constructor
	pfitDataPoint(int inputDimensions, int outputDimensions);
	
	~pfitDataPoint();
	
	void                    operator++();
	pfitDataPoint<DataType> operator+(const pfitIndex &RHS);
	bool                    operator==(pfitDataPoint const &RHS);
	bool                    operator!=(pfitDataPoint const &RHS);
	
	
	void        clearOutput();
	
	void        throwIfNotReady(int inDimensions, int outDimensions) const;
	
	//if you want to store inside here
	void        localAllocate();
	void        localRelease(); //delete data
	
	DataType*   getInput() const;
	DataType*   getOutput() const;
	DataType*   getInput();
	DataType*   getOutput();
	
	bool        getActive() const;
	void		setActive(const bool b);
	
	string      toString() const;
	
	pfitDataPoint<DataType> makeCopy() const;

protected:
	int			_inDimensions;
	int			_outDimensions;
	
	DataType*	_inputData;
	DataType*	_outputData;
	
	bool*		_activeData;
	
	bool		_locallyAllocated;
	
private:
	static bool _activeTrue;
};