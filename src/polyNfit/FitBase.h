#pragma once
///////////////////////////////////////////////////////////////////////////////
//
// Class to solve Ax = y for two values of y, where A is a matrix made up
// of bais functions a_ij = sum_x f_i(x)f_j(x). As is the case when
// finding the least squares fit polynomial.
//
// (c) 2009 Daniel Tang
//
///////////////////////////////////////////////////////////////////////////////

#include "matrix.h"
#include "pfitDataSet.h"
#include <vector>

template <class T>
class FitBase
{
public:
	FitBase(int bases, int indim, int outdim);
	~FitBase();
	
    void                init(pfitDataSet<T> &dataSet);
    
	void				solve(int, T *, unsigned int &nCoefficients);
	int					nBases() { return _bases; };
	virtual T			basis(int n, pfitDataPoint<T> const &x) = 0;
	
	unsigned short		_indim, _outdim;

protected:
	int					_bases;
	
	// Solves Ax = y for x
	Matrix<T>							A;	
	std::vector<std::vector<T> >		y;	// store OUTDIM values of y
	
};