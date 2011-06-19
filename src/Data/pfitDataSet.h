//
//  pfitDataSet.h
//  ofxPolyFit
//
//  Created by Elliot Woods on 19/06/2011.
//  Copyright 2011 Kimchi and Chips. All rights reserved.
//

#include <vector>
#include <set>

using namespace std;

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
    
        void        throwIfNotReady(int inDimensions, int outDimensions);
    
        //if you want to store inside here
        void        localAllocate();
        void        localRelease(); //delete data
    
        DataType*   getInput() const;
        DataType*   getOutput() const;
        DataType*   getInput();
        DataType*   getOutput();
    
        bool        getEnabled() const;
        
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
        
        void    init(int inputDimensions, int outputDimensions, pfitIndex size = 0);
        void    clear();
        void    resize(unsigned int size);
        void    allocate(unsigned int const allocation);
    
        void    wrapData(DataType* x, DataType* y);
    
        //vector style access
        pfitDataPoint<DataType>     begin();
        pfitDataPoint<DataType>     end();
        
        DataType*   getInput();
        DataType*   getOutput();
    
        void    throwIfNotReady(int inDimensions, int outDimensions, int nBases=0);
    
        set<pfitIndex>      getActiveIndices();
        pfitIndex           getActiveIndicesCount();
        
    protected:
        void    deAllocate();
    
        void    checkAllocated();
    
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
