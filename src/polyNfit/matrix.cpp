///////////////////////////////////////////////////////////////////////////////
//
// Definitions for matrix class, see header for details
//
// (c) 2009 Daniel Tang
//
///////////////////////////////////////////////////////////////////////////////

#include "matrix.h"


///////////////////////////////////////////////////////////////////////////////
//
// Constructors/Deconstructors
//
///////////////////////////////////////////////////////////////////////////////

Matrix::Matrix(int isize, int jsize) : N(jsize) {
  x = new double [isize*jsize];
  allocatedMemory = true;
  SIZE = isize*jsize;
}


Matrix::Matrix(double *mem, int isize, int jsize) : 
  N(jsize),
  SIZE(isize*jsize)
{
  x = mem;
  allocatedMemory = false;
}


Matrix::~Matrix() {
  if(allocatedMemory) delete [] x;
}


///////////////////////////////////////////////////////////////////////////////
//
// LU-decomposition
//
///////////////////////////////////////////////////////////////////////////////

void Matrix::LUdecomp() {
  int i,j,k;
  double maxVal;
  double tmp;

  for(i=0;i<N;i++) {
    maxVal=0.0;
    for(j=0;j<N;j++) {
		tmp=fabs(val(i,j));
		if (tmp > maxVal) maxVal=tmp;
    }
    if(maxVal == 0.0) 
      throw("LU-decomposition found singular matrix. You must have chosen a vary bad set of points.");
  }

  for(j = 0; j<N; ++j) {
    for(i = 0; i<=j; ++i) {
      for(k = 0; k < i; ++k) {
	val(i,j) -= val(i,k)*val(k,j);
      } 
    }

    if(val(j,j) == 0.0) {
      throw("LU-decomposition found singular matrix. You must have chosen an ambiguous set of points.");
    }


    for(i = j+1; i<N; ++i) {
      for(k = 0; k < j; ++k) {
	val(i,j) -= val(i,k)*val(k,j);
      }
      val(i,j) /= val(j,j);
    }
  
  }   
}


///////////////////////////////////////////////////////////////////////////////
//
// resizes this matrix, invalidating its contents.
//
///////////////////////////////////////////////////////////////////////////////
void Matrix::resize(int isize, int jsize) {
  if(allocatedMemory) delete [] x;
  x = new double [isize*jsize];
  N = jsize;
  SIZE = isize*jsize;
  allocatedMemory = true;
}


///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
void Matrix::clear() {
  for(int i = 0; i<SIZE; ++i) {
    x[i] = 0.0;
  }
}


///////////////////////////////////////////////////////////////////////////////
//
// Interprets the data in this array as that of a matrix of size (is,js),
// where is<=isize() and js<=jsize(), and re-sizes the matrix to be of size
// (isize(),jsize()). Elements that were previously out of range are
// filled with zeroes.
// 
///////////////////////////////////////////////////////////////////////////////
bool Matrix::repackFrom(int is, int js) {
  if(is > isize() || js > jsize()) return(false);
  int i,j;
  
  i = isize()-1;
  while(i>=is) {
    for(j=jsize()-1; j>=0; --j) {
      val(i,j) = 0.0;
    }
    --i;
  }

  while(i>=0) {
    j = jsize()-1;
    while(j>=js) {
      val(i,j) = 0.0;
      --j;
    }
    while(j>=0) {
      val(i,j) = x[i*js + j];
      --j;
    }
    --i;
  }
  return true;
}


///////////////////////////////////////////////////////////////////////////////
//
// Prints the martix to 'out'
//
///////////////////////////////////////////////////////////////////////////////

std::ostream &operator <<(std::ostream &out, const Matrix &M) {
  int i,j;

  out.precision(3);

  for(i=0; (i+1)*M.jsize()<=M.size(); ++i) {
    for(j=0; j<M.jsize(); ++j) {
      out << M(i,j) << "\t";
    }
    out << "\n";
  }
  return(out);
}

