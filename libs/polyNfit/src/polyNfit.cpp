/*
 *  polyNfit.cpp
 *  polyncpp-test
 *
 *  Created by Elliot Woods on 28/03/2010.
 *  Copyright 2010 Kimchi and Chips. All rights reserved.
 *
 */

#include "polyNfit.h"

// ---------- Define the basis we're working in
// --------------------------------------------

template <class T>
T polyNfit<T>::basis(int n, pfitDataPoint<T> const &x)
{
	//n denotes the index of the basis that we're working with
	//for 1D, we just have power x^n
	//for >=2D, we must translate n into powers for each index.
	
	
	/* PSEUDOCODE
	 
	 INPUTS
	 n = 1D index of current basis/coefficient
	 d = number of dimensions
	 x = array of x[d]
	 
	 OUTPUTS
	 scaler sum of dimension powers for this index
	 
	 ALGORITHM
	 
	 rerender the Indices table (if needed)
	 
	 create output double = 1
	 
	 fill xPowerSpace using Indices from other program
	 
	 loop through dimensions
	 output = product of all powers denoted in xPowerSpace
	 i.e. output *= pow(x[iDimension],xPowerSpace[iDimension]);
	 
	 return output
	 */
    
	T output = 1;
	T *xData = x.getInput();
	
	for (int iDimension=0; iDimension<FitBase<T>::_indim; iDimension++)
		output *= pow(xData[iDimension],(T)vecBasisIndices.at(n)[iDimension]);
	
	return output;
	//1D solution
	//return(pow(x[0], n));
    
}

template class polyNfit<double>;
template class polyNfit<float>;