#pragma once
/*
 *  polyNfit.h
 *  polyncpp-test
 *
 *  Created by Elliot Woods on 28/03/2010.
 *  Copyright 2010 Kimchi and Chips. All rights reserved.
 *
 */

#include "FitBase.h"
#include "powerBases.h"


class polyNfit : public powerBases, public FitBase
{
public:
	polyNfit(int order, int indim, int outdim, pfitBasisType basesShape) :
 		_order(order),
		FitBase(updateBasisIndicies(indim,order), indim, outdim),
		powerBases(basesShape)
		{
		};

    DataType		basis(int n, pfitDataPoint<DataType> const &x);
	unsigned short	_order;
};