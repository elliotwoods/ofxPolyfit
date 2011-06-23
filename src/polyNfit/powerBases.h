#pragma once
/*
 *  powerBases.h
 *  polyncpp-test
 *
 *  Created by Elliot Woods on 26/03/2010.
 *  Copyright 2010 Kimchi and Chips. All rights reserved.
 *
 */

#include <vector>
#include <math.h>


typedef enum  {
    BASIS_SHAPE_TRIANGLE = 0,
    BASIS_SHAPE_SQUARE,
    BASIS_SHAPE_SQUARE_MINUS_HIGHEST,
    BASIS_SHAPE_PADE_FIRST,
    BASIS_SHAPE_PADE_SECOND
} pfitBasisType;


class powerBases
{
public:
	powerBases(pfitBasisType basesShape);
	int							updateBasisIndicies(int nDimensions, int nPowerOrder);

	std::vector<unsigned int*>	vecBasisIndices;
	pfitBasisType				_basesShape; //0=triangle, 1=square
	unsigned int				_nBases;
	
private:
	void						calcBases();
	void						clearBases();
	
	int							_nDimensions, _nPowerOrder;
	int							_renderedNDimensions, _renderedNPowerOrder;
	
};