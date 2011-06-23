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


DataType polyNfit::basis(int n, pfitDataPoint<DataType> const &x)
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
	 
	 rerender the indicies table (if needed)
	 
	 create output double = 1
	 
	 fill xPowerSpace using indicies from other program
	 
	 loop through dimensions
	 output = product of all powers denoted in xPowerSpace
	 i.e. output *= pow(x[iDimension],xPowerSpace[iDimension]);
	 
	 return output
	 */
    
	DataType output = 1;
	DataType *xData = x.getInput();
	
	for (int iDimension=0; iDimension<_indim; iDimension++)
	{
		output *= pow(xData[iDimension],(double)vecBasisIndices.at(n)[iDimension]);
	}
	
	return output;
	//1D solution
	//return(pow(x[0], n));
    
    
}
