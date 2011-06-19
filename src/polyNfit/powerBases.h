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


#define BASIS_SHAPE_TRIANGLE 0
#define BASIS_SHAPE_SQUARE 1
#define BASIS_SHAPE_SQUARE_MINUS_HIGHEST 2
#define BASIS_SHAPE_PADE_FIRST 3


class powerBases
{
public:
	powerBases(int basesShape);
	int							updateBasisIndicies(int nDimensions, int nPowerOrder);

	std::vector<unsigned int*>	vecBasisIndices;
	unsigned char				_basesShape; //0=triangle, 1=square
	unsigned int				_nBases;
	
private:
	void						calcBases();
	void						clearBases();
	
	int							_nDimensions, _nPowerOrder;
	int							_renderedNDimensions, _renderedNPowerOrder;
	
};