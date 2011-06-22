//
//  pfitDataSet.h
//  ofxPolyFit
//
//  Created by Elliot Woods on 19/06/2011.
//  Copyright 2011 Kimchi and Chips. All rights reserved.
//

#include <vector>
#include <set>
#include <sstream>

using namespace std;

//must be an unsigned integer type (e.g. unsigned long)
typedef unsigned int pfitIndex;

template<typename DataType>
class pfitDataPoint
{
    public:
        pfitDataPoint(int inputDimensions, int outputDimensions, DataType* inputData, DataType* outputData, bool enabled = true);
    
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
    
        bool        getEnabled() const;
    
        string      toString() const;
        
    protected:
        int         _inDimensions;
        int         _outDimensions;

        DataType*   _inputData;
        DataType*   _outputData;

        bool        _enabled;
    
        bool        _locallyAllocated;
    
};

template<typename DataType>
class pfitDataSet
{
    public:
        pfitDataSet();
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
        
        DataType*   getInput();
        DataType*   getOutput();
    
        void    throwIfNotReady(int inDimensions, int outDimensions, int nBases=0) const;
    
        set<pfitIndex>      getActiveIndices() const;
        pfitIndex           getActiveIndicesCount() const;
        
        string              toString() const;
        
    protected:
        void    deAllocate();
    
        void    checkAllocated() const;
    
        int         _inDimensions;
        int         _outDimensions;
        
        pfitIndex   _nDataPoints;
        pfitIndex   _nDataPointsAllocated;
    
        DataType*   _inData; // stored XYZXYZXYZ
        DataType*   _outData;
        bool*       _enabledData;
    
        bool        _dataAllocated;
};

typedef pfitDataSet<double> pfitDataSetd;
typedef pfitDataPoint<double> pfitDataPointd;
