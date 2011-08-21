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

template <class T>
class polyNfit : public powerBases, public FitBase<T>
{
public:
	polyNfit(int order, int indim, int outdim, pfitBasisType basesShape) :
 		_order(order),
		FitBase<T>(updateBasisIndicies(indim,order), indim, outdim),
		powerBases(basesShape)
		{
		};

    T				basis(int n, pfitDataPoint<T> const &x);
	unsigned short	_order;
};