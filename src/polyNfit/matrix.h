///////////////////////////////////////////////////////////////////////////////
//
// Simple class to store a rectangular matrix (x_00...x_nm) as a 
// 1D array of doubles in the form
// (x_00, x_10...x_n0, x_01, x_11...x_nn)
//
// (c) Daniel Tang 2009
//
///////////////////////////////////////////////////////////////////////////////

#ifndef POLYFIT_MATRIX_H
#define POLYFIT_MATRIX_H

#include <cmath>
#include <new>
#include <iostream>

class Matrix {
public:
  Matrix(int isize, int jsize);
  Matrix(double *, int isize, int jsize);
  ~Matrix();

  int 		jsize() const		       	{return(N);}
  int		isize() const 			{return(SIZE/N);}
  int 		size() const		       	{return(SIZE);}
  void		LUdecomp();
  //void		setToBasis(double (*)(int,double *), double *, int);
  void		resize(int, int);
  bool		repackFrom(int,int);
  double *	array() 			{return(x);}
  void		clear();

  double &	val(int i, int j)		{
    //    if(i*N+j >= SIZE) throw("Subscript out of range in Matrix.");
      return(x[i*N+j]);
  }
  double 	val(int i, int j) const		{
    //    if(i*N+j >= SIZE) throw("Subscript out of range in const Matrix.");
      return(x[i*N+j]);
  }
  double &	operator()(int i, int j)	{return(val(i,j));}
  double 	operator()(int i, int j) const	{return(val(i,j));}

protected:
  double * 	x;
  int 		N;
  int		SIZE;
  bool		allocatedMemory;
};

std::ostream &operator <<(std::ostream &, const Matrix &);

#endif
