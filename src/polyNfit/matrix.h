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

template <class T>
class Matrix {
public:
  Matrix(int isize, int jsize);
  Matrix(T *, int isize, int jsize);
  ~Matrix();

  int 		jsize() const		       	{return(N);}
  int		isize() const 			{return(SIZE/N);}
  int 		size() const		       	{return(SIZE);}
  void		LUdecomp();
  //void		setToBasis(double (*)(int,double *), double *, int);
  void		resize(int, int);
  bool		repackFrom(int,int);
  T *		array() 			{return(x);}
  void		clear();

  T &	val(int i, int j)		{
    //    if(i*N+j >= SIZE) throw("Subscript out of range in Matrix.");
      return(x[i*N+j]);
  }
  T 	val(int i, int j) const		{
    //    if(i*N+j >= SIZE) throw("Subscript out of range in const Matrix.");
      return(x[i*N+j]);
  }
  T &	operator()(int i, int j)	{return(val(i,j));}
  T 	operator()(int i, int j) const	{return(val(i,j));}

protected:
  T * 	x;
  int 		N;
  int		SIZE;
  bool		allocatedMemory;
};

template <class T>
std::ostream &operator <<(std::ostream &, const Matrix<T> &);

#endif
