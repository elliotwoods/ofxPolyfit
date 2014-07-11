#pragma once
//
//  pfitDataSet.h
//  ofxPolyFit
//

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <set>
#include <sstream>
#include <fstream>
#include <iostream>

#include "pfitDataPoint.h"

using namespace std;

template<typename DataType>
class pfitDataSet
{
public:
	pfitDataSet();
	pfitDataSet(const pfitDataSet<DataType> &other);
	~pfitDataSet();

	pfitDataPoint<DataType> operator[]( pfitIndex i );
	pfitDataPoint<DataType> operator[]( pfitIndex i ) const;

	void    init(int inputDimensions, int outputDimensions, pfitIndex size = 0);
	void    clear();
	void    resize(pfitIndex const size);
	void    allocate(pfitIndex const allocation);

	void    wrapData(DataType* x, DataType* y);

	//vector style access
	pfitDataPoint<DataType>     begin() const;
	pfitDataPoint<DataType>     end() const;
	pfitIndex					size() const { return _nDataPoints; };

	///Direct access to data
	DataType*			getInput();
	const DataType*		getInput() const;
	DataType*			getOutput();
	const DataType*		getOutput() const;
	bool*				getActive();
	const bool*			getActive() const;

	///Copy data from existing array
	void		setInput(const DataType* data);
	void		setOutput(const DataType* data);
	void		setActive(const bool* data);
	
	int		getInputDimensions() const;
	int		getOutputDimensions() const;

	void    throwIfNotReady(int inDimensions, int outDimensions, int nBases=0) const;

	pfitIndexSet		getActiveIndices() const;
	pfitIndex           getActiveCount() const;
	void				setActiveIndices(const pfitIndexSet& set);
	void				setActiveAll();
	void				setActiveNone();

	string	toString() const;
	
	void	load(string filename);
	void	save(string filename);

protected:
	void    deAllocate();

	void    checkAllocated() const;

	int         _inDimensions;
	int         _outDimensions;

	pfitIndex   _nDataPoints;
	pfitIndex   _nDataPointsAllocated;

	DataType*   _inData; // stored XYZXYZXYZ
	DataType*   _outData;
	bool*       _activeData;

	bool        _dataAllocated;
};

typedef pfitDataSet<double> pfitDataSetd;
typedef pfitDataPoint<double> pfitDataPointd;

typedef pfitDataSet<float> pfitDataSetf;
typedef pfitDataPoint<float> pfitDataPointf;

