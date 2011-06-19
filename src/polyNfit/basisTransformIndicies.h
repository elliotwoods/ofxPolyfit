#pragma once
/*
 *  basisTransformIndicies.h
 *  polyncpp-test
 *
 *  Created by Elliot Woods on 26/03/2010.
 *  Copyright 2010 Kimchi and Chips. All rights reserved.
 *
 */

#include <vector>
#include <math.h>

class basisTransformIndicies
{
public:
	basisTransformIndicies() { };
	int							updateBasisIndicies(int nDimensions, int nPowerOrder);
	
	std::vector<unsigned int*>	vecBasisIndices;
	
private:
	void						calc();
	void						clear();
	
	int							_nBases;
	int							_nDimensions, _nPowerOrder;
	int							_renderedNDimensions, _renderedNPowerOrder;
	
};