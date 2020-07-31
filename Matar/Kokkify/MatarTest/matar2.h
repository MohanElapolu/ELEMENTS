#ifndef MATAR_H
#define MATAR_H

// Order
//
//  Standard (non-Kokkos data structures)
//   1. FArray
//   2. ViewFArray
//   3. FMatrix
//   4. ViewFMatrix
//   5. CArray
//   6. ViewCArray
//   7. CMatrix
//   8. ViewCMatrix
//   9. RaggedRightArray
//   10. RaggedDownArray
//   11. DynamicRaggedRightArray
//   12. DynamicRaggedDownArray
//   13. SparseRowArray
//   14. SparseColArray
//
//   Kokkos Data structures
//   15. FArrayKokkos
//   16. ViewFArrayKokkos
//   17. FMatrixKokkos
//   18. ViewFMatrixKokkos
//   19. CArrayKokkos
//   20. ViewCArrayKokkos
//   21. CMatrixKokkos
//   22. ViewCMatrixKokkos
//   23. RaggedRightArrayKokkos
//   24. RaggedDownArrayKokkos
//   25. DynamicRaggedRightArrayKokkos
//   26. DynamicRaggedDownArrayKokkos
//   27. SparseRowArrayKokkos
//   28. SparseColArrayKokkos


#include <stdio.h>
#include <stdlib.h>

#include <assert.h>
#include "kokkos_alias.h"

//To disable asserts, uncomment the following line
//#define NDEBUG

//---Begin Standard Data Structures---

//1. FArray
template <typename T>
class FArray {
    
private:
    size_t dim1_, dim2_, dim3_, dim4_, dim5_, dim6_, length_;
    T * this_array;
    
public:
    
    // default constructor
   FArray ();
   
    //overload constructors from 1D to 6D
     
   FArray(size_t some_dim1);
   FArray(size_t some_dim1, size_t some_dim2);
   FArray(size_t some_dim1, size_t some_dim2, size_t some_dim3);
   FArray(size_t some_dim1, 
          size_t some_dim2,
          size_t some_dim3,
          size_t some_dim4);
    
   FArray(size_t some_dim1,
          size_t some_dim2,
          size_t some_dim3,
          size_t some_dim4,
          size_t some_dim5);

   FArray(size_t some_dim1,
          size_t some_dim2,
          size_t some_dim3,
          size_t some_dim4,
          size_t some_dim5,
          size_t some_dim6);

    // overload operator() to access data as array(i,....,n);
    T& operator()(size_t i);
    T& operator()(size_t i, size_t j);
    T& operator()(size_t i, size_t j, size_t k);
    T& operator()(size_t i, size_t j, size_t k, size_t l);
    T& operator()(size_t i, size_t j, size_t k, size_t l, size_t m);
    T& operator()(size_t i, size_t j, size_t k, size_t l, size_t m, size_t n);

    //overload = operator
    FArray& operator=(const FArray& temp);

    // deconstructor
    ~FArray ( );
    
}; // end of f_array_t

//---FArray class definnitions----

//constructors
template <typename T>
FArray<T>::FArray(){}

//1D
template <typename T>
FArray<T>::FArray(size_t some_dim1) {
    dim1_ = some_dim1;
    length_ = dim1_;
    this_array = new T[length_];
}

template <typename T>
FArray<T>::FArray(size_t some_dim1, size_t some_dim2) {
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    length_ = dim1_*dim2_;
    this_array = new T[length_];
}

//3D
template <typename T>
FArray<T>::FArray(size_t some_dim1, size_t some_dim2, size_t some_dim3) {
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    dim3_ = some_dim3;
    length_ = dim1_*dim2_*dim3_;
    this_array = new T[length_];
}

//4D
template <typename T>
FArray<T>::FArray(size_t some_dim1, size_t some_dim2, size_t some_dim3, size_t some_dim4) {
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    dim3_ = some_dim3;
    dim4_ = some_dim4;
    length_ = dim1_*dim2_*dim3_*dim4_;
    this_array = new T[length_];
}

//5D
template <typename T>
FArray<T>::FArray(size_t some_dim1, size_t some_dim2, size_t some_dim3, size_t some_dim4, size_t some_dim5) {
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    dim3_ = some_dim3;
    dim4_ = some_dim4;
    dim5_ = some_dim5;
    length_ = dim1_*dim2_*dim3_*dim4_*dim5_;
    this_array = new T[length_];
}

//6D
template <typename T>
FArray<T>::FArray(size_t some_dim1,size_t some_dim2, size_t some_dim3, size_t some_dim4, size_t some_dim5, size_t some_dim6) {
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    dim3_ = some_dim3;
    dim4_ = some_dim4;
    dim5_ = some_dim5;
    dim6_ = some_dim6;
    length_ = dim1_*dim2_*dim3_*dim4_*dim5_*dim6_;
    this_array = new T[length_];
}

//overload operator () for 1D to 6D
//indices are from [0:N-1]

//1D
template <typename T>
T& FArray<T>::operator()(size_t i)
{
    assert( i < dim1_ && "i is out of bounds in FArray 1D!");
    return this_array[i];
}

//2D
template <typename T>
T& FArray<T>::operator()(size_t i, size_t j)
{
    assert( i < dim1_ && "i is out of bounds in FArray 2D!");
    assert( j < dim2_ && "j is out of bounds in FArray 2D!");
    return this_array[i + j*dim1_];
}

//3D
template <typename T>
T& FArray<T>::operator()(size_t i, size_t j, size_t k)
{
    assert( i < dim1_ && "i is out of bounds in FArray 3D!");
    assert( j < dim2_ && "j is out of bounds in Farray 3D!");
    assert( k < dim3_ && "k is out of bounds in FArray 3D!");
    return this_array[i + j*dim1_ + k*dim1_*dim2_];
}

//4D
template <typename T>
T& FArray<T>::operator()(size_t i, size_t j, size_t k, size_t l)
{
    assert( i < dim1_ && "i is out of bounds in FArray 4D!");
    assert( j < dim2_ && "j is out of bounds in FArray 4D!");
    assert( k < dim3_ && "k is out of bounds in FArray 4D!");
    assert( l < dim4_ && "l is out of bounds in FArray 4D!");
    return this_array[ i + j*dim1_ + k*dim1_*dim2_ + l*dim1_*dim2_*dim3_];
}

//5D
template <typename T>
T& FArray<T>::operator()(size_t i, size_t j, size_t k, size_t l, size_t m)
{
    assert( i < dim1_ && "i is out of bounds in FArray 5D!");
    assert( j < dim2_ && "j is out of bounds in FArray 5D!");
    assert( k < dim3_ && "k is out of bounds in FArray 5D!");
    assert( l < dim4_ && "l is out of bounds in FArray 5D!");
    assert( m < dim5_ && "m is out of bounds in FArray 5D!");
    return this_array[i + j*dim1_ + k*dim1_*dim2_ + l*dim1_*dim2_*dim3_ + m*dim1_*dim2_*dim3_*dim4_];
}

//6D
template <typename T>
T& FArray<T>::operator()(size_t i, size_t j, size_t k, size_t l, size_t m, size_t n)
{

    assert( i < dim1_ && "i is out of bounds in FArray 5D!");
    assert( j < dim2_ && "j is out of bounds in FArray 5D!");
    assert( k < dim3_ && "k is out of bounds in FArray 5D!");
    assert( l < dim4_ && "l is out of bounds in FArray 5D!");
    assert( m < dim5_ && "m is out of bounds in FArray 5D!");
    assert( n < dim6_ && "n is out of bounds in FArray 6D!");
    return this_array[i + j*dim1_ + k*dim1_*dim2_ + l*dim1_*dim2_*dim3_ + m*dim1_*dim2_*dim3_*dim4_ + n*dim1_*dim2_*dim3_*dim4_*dim5_];
}

// = operator
//THIS = FArray <> TEMP(n,m,...)
template <typename T>
FArray<T>& FArray<T>::operator= (const FArray& temp)
{
	if(this != & temp) {
	  dim1_ = temp.dim1_;
	  dim2_ = temp.dim2_;
	  dim3_ = temp.dim3_;
	  dim4_ = temp.dim4_;
	  dim5_ = temp.dim5_;
	  dim6_ = temp.dim6_;
	  length_ = temp.length_;
	  this_array = new T[length_];
	}
  return *this;
} 

//delete FArray
template <typename T>
FArray<T>::~FArray(){
    delete [] this_array;
}

//---end of FArray class definitions----

//2. ViewFArray
template <typename T>
class ViewFArray {

private:
    size_t dim1_, dim2_, dim3_, dim4_, dim5_, dim6_;
    T * this_array;
    
public:
    
    // default constructor
    ViewFArray ();

    //---1D array---
    ViewFArray(T *some_array, size_t some_dim1);
    T& operator()(size_t i);
    
    //--- 2D array ---
    
    // overloaded constructor
    ViewFArray (T *some_array, size_t some_dim1, size_t some_dim2);
    T& operator()(size_t i, size_t j);
    
    //--- 3D array ---
    ViewFArray (T *some_array, size_t some_dim1, size_t some_dim2, size_t some_dim3);
    T& operator()(size_t i, size_t j, size_t k);
    
    //--- 4D array ---
    // overloaded constructor
    ViewFArray (T *some_array, size_t some_dim1, size_t some_dim2, size_t some_dim3, size_t some_dim4);
    T& operator()(size_t i, size_t j, size_t k, size_t l);

    //--- 5D array ---
    // overloaded constructor
    ViewFArray (T *some_array, size_t some_dim1, size_t some_dim2, size_t some_dim3, size_t some_dim4, size_t some_dim5);
    T& operator()(size_t i, size_t j, size_t k, size_t l, size_t m);
   
    //------6D -----

    ViewFArray (T *some_array,size_t some_dim1, size_t some_dim2, size_t some_dim3, size_t some_dim4, size_t some_dim5, size_t some_dim6);
    T& operator()(size_t i, size_t j, size_t k, size_t l, size_t m, size_t n);

    
}; // end of viewFArray

//class definitions for viewFArray

//~~~~constructors for viewFArray for 1D to 6D~~~~~~~

//no dimension
template <typename T>
ViewFArray<T>::ViewFArray(){}

//1D
template <typename T>
ViewFArray<T>::ViewFArray(T *some_array, size_t some_dim1){
	dim1_ = some_dim1;
	this_array = some_array;
}

//2D
template <typename T>
ViewFArray<T>::ViewFArray(T *some_array, size_t some_dim1, size_t some_dim2) {
	dim1_ = some_dim1;
	dim2_ = some_dim2;
	this_array = some_array;
}

//3D
template <typename T>
ViewFArray<T>::ViewFArray(T *some_array, size_t some_dim1, size_t some_dim2, size_t some_dim3){
	dim1_ = some_dim1;
	dim2_ = some_dim2;
	dim3_ = some_dim3;
	this_array = some_array;
}

//4D
template <typename T>
ViewFArray<T>::ViewFArray(T *some_array, size_t some_dim1, size_t some_dim2, size_t some_dim3, size_t some_dim4){
	dim1_ = some_dim1;
	dim2_ = some_dim2;
	dim3_ = some_dim3;
	dim4_ = some_dim4;
	this_array = some_array;
}

//5D
template <typename T>
ViewFArray<T>::ViewFArray(T *some_array, size_t some_dim1, size_t some_dim2, size_t some_dim3, size_t some_dim4, size_t some_dim5){
	dim1_ = some_dim1;
	dim2_ = some_dim2;
	dim3_ = some_dim3;
	dim4_ = some_dim4;
	dim5_ = some_dim5;
	this_array = some_array;
}

//6D
template <typename T>
ViewFArray<T>::ViewFArray(T *some_array, size_t some_dim1, size_t some_dim2, size_t some_dim3, size_t some_dim4, size_t some_dim5, size_t some_dim6){
	dim1_ = some_dim1;
	dim2_ = some_dim2;
	dim3_ = some_dim3;
	dim4_ = some_dim4;
	dim5_ = some_dim5;
	dim6_ = some_dim6;
	this_array = some_array;
}

//~~~~~~operator () overload 
//for dimensions 1D to 6D
//indices for array are from 0...N-1

//1D
template <typename T>
T& ViewFArray<T>::operator()(size_t i)
{
	assert(i < dim1_ && "i is out of bounds in ViewFArray 1D!");
	return this_array[i];
}

//2D
template <typename T>
T& ViewFArray<T>::operator()(size_t i, size_t j) 
{
	assert( i < dim1_ && "i is out of bounds in ViewFArray 2D!");
	assert( j < dim2_ && "j is out of bounds in ViewFArray 2D!");
	return this_array[i + j*dim1_];
}

//3D
template <typename T>
T& ViewFArray<T>::operator()(size_t i, size_t j,size_t k)
{
	assert(i < dim1_ && "i is out of bounds in ViewFArray 3D!");
	assert(j < dim2_ && "j is out of bounds in ViewFArray 3D!");
	assert(k < dim3_ && "k is out of bounds in ViewFArray 3D!");
	return this_array[i + j*dim1_ + k*dim1_*dim2_];
}

//4D
template <typename T>
T& ViewFArray<T>::operator()(size_t i, size_t j, size_t k, size_t l)
{
	assert(i < dim1_ && "i is out of bounds in ViewFArray 4D!");
	assert(j < dim2_ && "j is out of bounds in ViewFArray 4D!");
	assert(k < dim3_ && "k is out of bounds in ViewFArray 4D!");
	assert(l < dim4_ && "l is out of bounds in ViewFArray 4D!");
	return this_array[i + j*dim1_ + k*dim1_*dim2_ + l*dim1_*dim2_*dim3_];
}

//5D
template <typename T>
T& ViewFArray<T>::operator()(size_t i, size_t j, size_t k, size_t l, size_t m)
{
	assert(i < dim1_ && "i is out of bounds in ViewFArray 5D!");
	assert(j < dim2_ && "j is out of bounds in ViewFArray 5D!");
	assert(k < dim3_ && "k is out of bounds in ViewFArray 5D!");
	assert(l < dim4_ && "l is out of bounds in ViewFArray 5D!");
	assert(m < dim5_ && "m is out of bounds in ViewFArray 5D!");
	return this_array[i + j*dim1_ + k*dim1_*dim2_ + l*dim1_*dim2_*dim3_ + m*dim1_*dim2_*dim3_*dim4_];
}

//6D
template <typename T>
T& ViewFArray<T>:: operator()(size_t i, size_t j, size_t k, size_t l, size_t m, size_t n)
{
	assert(i < dim1_ && "i is out of bounds in ViewFArray 6D!");
	assert(j < dim2_ && "j is out of bounds in ViewFArray 6D!");
	assert(k < dim3_ && "k is out of bounds in ViewFArray 6D!");
	assert(l < dim4_ && "l is out of bounds in ViewFArray 6D!");
	assert(m < dim5_ && "m is out of bounds in ViewFArray 6D!");
	assert(n < dim6_ && "n is out of bounds in ViewFArray 6D!");
	return this_array[i + j*dim1_ + k*dim1_*dim2_ + l*dim1_*dim2_*dim3_ + m*dim1_*dim2_*dim3_*dim4_ + n*dim1_*dim2_*dim3_*dim4_*dim5_];
}

//---end of ViewFArray class definitions---

//3. FMatrix
template <typename T>
class FMatrix {
private:
    size_t dim1_, dim2_, dim3_, dim4_, dim5_, dim6_;
    size_t length_; // Length of 1D array
    T* this_matrix_;

public:
    // Default constructor
    FMatrix ();

    // --- 1D matrix ---
    
    // Overloaded constructor
    FMatrix (size_t some_dim1);

    // Overload operator() to access data as matrix(i), where i = [1:N]
    T& operator() (size_t i) const;

    // --- 2D matrix ---
    
    // Overloaded constructor
    FMatrix (size_t some_dim1, size_t some_dim2);

    // Overload operator() to access data as matrix(i, j),
    // where i = [1:N], j = [1:N]
    T& operator() (size_t i, size_t j) const;

    // --- 3D matrix ---
    
    // Overloaded constructor
    FMatrix (size_t some_dim1, size_t some_dim2, size_t some_dim3);

    // Overload operator() to access data as matrix(i, j, k),
    // where i = [1:N], j = [1:N], k = [1:N]
    T& operator() (size_t i, size_t j, size_t k) const;

    // --- 4D matrix ---
    
    // Overloaded constructor
    FMatrix (size_t some_dim1, size_t some_dim2, size_t some_dim3, 
               size_t some_dim4);

    // Overload operator() to access data as matrix(i, j, k, l),
    // where i = [1:N], j = [1:N], k = [1:N], l = [1:N]
    T& operator() (size_t i, size_t j, size_t k, size_t l) const;

    // --- 5D matrix ---
    
    // Overloaded constructor
    FMatrix (size_t some_dim1, size_t some_dim2, size_t some_dim3,
               size_t some_dim4, size_t some_dim5);

    // Overload operator() to access data as matrix(i, j, k, l, m),
    // where i = [1:N], j = [1:N], k = [1:N], l = [1:N],
    // m = [1:N]
    T& operator() (size_t i, size_t j, size_t k, size_t l,
                          size_t m) const;

    // --- 6D matrix ---
    
    // Overloaded constructor
    FMatrix (size_t some_dim1, size_t some_dim2, size_t some_dim3,
               size_t some_dim4, size_t some_dim5, size_t some_dim6);

    // Overload operator() to access data as matrix(i, j, k, l, m, n),
    // where i = [1:N], j = [1:N], k = [1:N], l = [1:N], 
    // m = [1:N], n = [1:N]
    T& operator() (size_t i, size_t j, size_t k, size_t l,
                          size_t m, size_t n) const;

    // Overload copy assignment operator
    FMatrix& operator=(const FMatrix& temp);

    size_t size() const;

    // Deconstructor
    ~FMatrix ();

}; // End of FMatrix

//---FMatrix class definitions---

//constructors

//1D
template <typename T>
FMatrix<T>::FMatrix(size_t some_dim1) {
    // assert(some_dim1 > 0);
    dim1_ = some_dim1;
    length_ = dim1_;
    this_matrix_ = new T[length_];
}

//2D
template <typename T>
FMatrix<T>::FMatrix(size_t some_dim1, size_t some_dim2) {
    // assert(some_dim1 > 0);
    // assert(some_dim2 > 0);
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    length_ = dim1_ * dim2_;
    this_matrix_ = new T[length_];
}

//3D
template <typename T>
FMatrix<T>::FMatrix(size_t some_dim1, size_t some_dim2, size_t some_dim3) {
    // assert(some_dim1 > 0);
    // assert(some_dim2 > 0);
    // assert(some_dim3 > 0);
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    dim3_ = some_dim3;
    length_ = dim1_ * dim2_ * dim3_;
    this_matrix_ = new T[length_];
}

//4D
template <typename T>
FMatrix<T>::FMatrix(size_t some_dim1, size_t some_dim2, size_t some_dim3, 
                        size_t some_dim4) {
    // assert(some_dim1 > 0);
    // assert(some_dim2 > 0);
    // assert(some_dim3 > 0);
    // assert(some_dim4 > 0);
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    dim3_ = some_dim3;
    dim4_ = some_dim4;
    length_ = dim1_ * dim2_ * dim3_ * dim4_;
    this_matrix_ = new T[length_];
}

//5D
template <typename T>
FMatrix<T>::FMatrix(size_t some_dim1, size_t some_dim2, size_t some_dim3,
                        size_t some_dim4, size_t some_dim5) {
    // assert(some_dim1 > 0);
    // assert(some_dim2 > 0);
    // assert(some_dim3 > 0);
    // assert(some_dim4 > 0):
    // assert(some_dim5 > 0);
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    dim3_ = some_dim3;
    dim4_ = some_dim4;
    dim5_ = some_dim5;
    length_ = dim1_ * dim2_ * dim3_ * dim4_ * dim5_;
    this_matrix_ = new T[length_];
}

//6D
template <typename T>
FMatrix<T>::FMatrix(size_t some_dim1, size_t some_dim2, size_t some_dim3,
                        size_t some_dim4, size_t some_dim5, size_t some_dim6) {
    // assert(some_dim1 > 0);
    // assert(some_dim2 > 0);
    // assert(some_dim3 > 0);
    // assert(some_dim4 > 0):
    // assert(some_dim5 > 0);
    // assert(some_dim6 > 0);
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    dim3_ = some_dim3;
    dim4_ = some_dim4;
    dim5_ = some_dim5;
    dim6_ = some_dim6;
    length_ = dim1_ * dim2_ * dim3_ * dim4_ * dim5_ * dim6_;
    this_matrix_ = new T[length_];

}

//overload operators

//1D
template <typename T>
inline T& FMatrix<T>::operator() (size_t i) const {
    assert(i >= 1 && i <= dim1_);
    return this_matrix_[i - 1];
}

//2D
template <typename T>
inline T& FMatrix<T>::operator() (size_t i, size_t j) const {
    assert(i >= 1 && i <= dim1_);
    assert(j >= 1 && j <= dim2_);
    return this_matrix_[(i - 1) + ((j - 1) * dim1_)];
}

//3D
template <typename T>
inline T& FMatrix<T>::operator() (size_t i, size_t j, size_t k) const {
    assert(i >= 1 && i <= dim1_);
    assert(j >= 1 && j <= dim2_);
    assert(k >= 1 && k <= dim3_);
    return this_matrix_[(i - 1) + ((j - 1) * dim1_) 
                                + ((k - 1) * dim1_ * dim2_)];
}

//4D
template <typename T>
inline T& FMatrix<T>::operator() (size_t i, size_t j, size_t k, size_t l) const {
    assert(i >= 1 && i <= dim1_);
    assert(j >= 1 && j <= dim2_);
    assert(k >= 1 && k <= dim3_);
    assert(l >= 1 && l <= dim4_);
    return this_matrix_[(i - 1) + ((j - 1) * dim1_)  
                                + ((k - 1) * dim1_ * dim2_)  
                                + ((l - 1) * dim1_ * dim2_ * dim3_)];
}


//5D
template <typename T>
inline T& FMatrix<T>::operator() (size_t i, size_t j, size_t k, size_t l,
                                    size_t m) const {
    assert(i >= 1 && i <= dim1_);
    assert(j >= 1 && j <= dim2_);
    assert(k >= 1 && k <= dim3_);
    assert(l >= 1 && l <= dim4_);
    assert(m >= 1 && m <= dim5_);
    return this_matrix_[(i - 1) + ((j - 1) * dim1_)  
                                + ((k - 1) * dim1_ * dim2_)  
                                + ((l - 1) * dim1_ * dim2_ * dim3_) 
                                + ((m - 1) * dim1_ * dim2_ * dim3_ * dim4_)];
}

//6D
template <typename T>
FMatrix<T>::FMatrix(size_t some_dim1, size_t some_dim2, size_t some_dim3,
                        size_t some_dim4, size_t some_dim5, size_t some_dim6) {
    // assert(some_dim1 > 0);
    // assert(some_dim2 > 0);
    // assert(some_dim3 > 0);
    // assert(some_dim4 > 0):
    // assert(some_dim5 > 0);
    // assert(some_dim6 > 0);
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    dim3_ = some_dim3;
    dim4_ = some_dim4;
    dim5_ = some_dim5;
    dim6_ = some_dim6;
    length_ = dim1_ * dim2_ * dim3_ * dim4_ * dim5_ * dim6_;
    this_matrix_ = new T[length_];

}

template <typename T>
inline T& FMatrix<T>::operator() (size_t i, size_t j, size_t k, size_t l,
                                    size_t m, size_t n) const {
    assert(i >= 1 && i <= dim1_);
    assert(j >= 1 && j <= dim2_);
    assert(k >= 1 && k <= dim3_);
    assert(l >= 1 && l <= dim4_);
    assert(m >= 1 && m <= dim5_);
    assert(n >= 1 && n <= dim6_);
    return this_matrix_[(i - 1) + ((j - 1) * dim1_)  
                                + ((k - 1) * dim1_ * dim2_)  
                                + ((l - 1) * dim1_ * dim2_ * dim3_)  
                                + ((m - 1) * dim1_ * dim2_ * dim3_ * dim4_)  
                                + ((n - 1) * dim1_ * dim2_ * dim3_ * dim4_ * dim5_)];
}

template <typename T>
inline FMatrix<T>& FMatrix<T>::operator= (const FMatrix& temp)
{
    // Do nothing if assignment is of the form x = x
    if (this != &temp) {
        dim1_ = temp.dim1_;
        dim2_ = temp.dim2_;
        dim3_ = temp.dim3_;
        dim4_ = temp.dim4_;
        dim5_ = temp.dim5_;
        dim6_ = temp.dim6_;
        length_ = temp.length_;
        this_matrix_ = new T[length_];
    }
    
    return *this;
}

template <typename T>
inline size_t FMatrix<T>::size() const {
    return length_;
}

template <typename T>
FMatrix<T>::~FMatrix() {
    delete[] this_matrix_;
}

//----end of FMatrix class definitions----

//4. ViewFMatrix
template <typename T>
class ViewFMatrix {

private:
    size_t dim1_, dim2_, dim3_, dim4_, dim5_, dim6_;
    size_t length_; // Length of 1D array
    T * this_matrix_;
    
public:
    
    // Default constructor
    ViewFMatrix ();
    
    //--- 1D matrix ---
    
    // Overloaded constructor
    ViewFMatrix(T *some_matrix,
                size_t some_dim1);
    
    // Overload  operator() to access data as matrix(i,j),
    // where i = [1:N], j = [1:N]
    T& operator()(size_t i) const;
    
    //--- 2D matrix ---
    
    // Overloaded constructor
    ViewFMatrix(T *some_matrix,
                size_t some_dim1,
                size_t some_dim2);
    
    // Overload operator() to access data as matrix(i,j),
    //  where i=[1:N], j=[1:N]
    T& operator()(size_t i, size_t j) const;
    
    //--- 3D matrix ---
    
    // Overloaded constructor
    ViewFMatrix(T *some_matrix,
                size_t some_dim1,
                size_t some_dim2,
                size_t some_dim3);
    
    // Overload operator() to access data as matrix(i,j,k),
    // where i = [1:N], j = [1:N], k = [1:N]
    T& operator()(size_t i, size_t j, size_t k) const;
    
    //--- 4D matrix ---
    
    // Overloaded constructor
    ViewFMatrix(T *some_matrix,
                size_t some_dim1,
                size_t some_dim2,
                size_t some_dim3,
                size_t some_dim4);
        
    // Overload operator() to access data as matrix(i, j, k, l),
    // where i = [0:n-1], j = [1:N], k = [1:N], l = [1:N]
    T& operator()(size_t i, size_t j, size_t k, size_t l) const;
    
    //--- 5D matrix ---
    
    // Overloaded constructor
    ViewFMatrix (T *some_matrix,
                 size_t some_dim1,
                 size_t some_dim2,
                 size_t some_dim3,
                 size_t some_dim4,
                 size_t some_dim5);
        
    // Overload operator() to access data as matrix(i,j,k,l,m),
    // where i = [1:N], j = [1:N], k = [1:N], l = [1:N], m = [1:N]
    T& operator() (size_t i, 
                   size_t j, 
                   size_t k, 
                   size_t l, 
                   size_t m) const;
    
    //--- 6D matrix ---
    
    // Overloaded constructor
    ViewFMatrix (T *some_matrix,
                 size_t some_dim1,
                 size_t some_dim2,
                 size_t some_dim3,
                 size_t some_dim4,
                 size_t some_dim5,
                 size_t some_dim6);
        
    // Overload operator() to access data as matrix(i,j,k,l,m,n),
    // where i = [1:N], j = [1:N], k = [1:N], l = [1:N], m = [1:N]
    T& operator()(size_t i, 
                  size_t j, 
                  size_t k, 
                  size_t l, 
                  size_t m, 
                  size_t n) const;

    size_t size() const;
}; // end of ViewFMatrix

//constructors

//no dimension
template <typename T>
ViewFMatrix<T>::ViewFMatrix() {}

//1D
template <typename T>
ViewFMatrix<T>::ViewFMatrix(T *some_matrix,
                            size_t some_dim1)
{
    dim1_ = some_dim1;
    length_ = dim1_;
    this_matrix_ = some_matrix;
}

//2D
template <typename T>
ViewFMatrix<T>::ViewFMatrix(T *some_matrix,
                            size_t some_dim1,
                            size_t some_dim2)
{
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    length_ = dim1_ * dim2_;
    this_matrix_ = some_matrix;
}

//3D
template <typename T>
ViewFMatrix<T>::ViewFMatrix (T *some_matrix,
                             size_t some_dim1,
                             size_t some_dim2,
                             size_t some_dim3)
{
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    dim3_ = some_dim3;
    length_ = dim1_ * dim2_ * dim3_;
    this_matrix_ = some_matrix;
}

//4D
template <typename T>
ViewFMatrix<T>::ViewFMatrix(T *some_matrix,
                            size_t some_dim1,
                            size_t some_dim2,
                            size_t some_dim3,
                            size_t some_dim4)
{
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    dim3_ = some_dim3;
    dim4_ = some_dim4;
    length_ = dim1_ * dim2_ * dim3_ * dim4_;
    this_matrix_ = some_matrix;
}

//5D
template <typename T>
ViewFMatrix<T>::ViewFMatrix(T *some_matrix,
                            size_t some_dim1,
                            size_t some_dim2,
                            size_t some_dim3,
                            size_t some_dim4,
                            size_t some_dim5)
{
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    dim3_ = some_dim3;
    dim4_ = some_dim4;
    dim5_ = some_dim5;
    length_ = dim1_ * dim2_ * dim3_ * dim4_ * dim5_;
    this_matrix_ = some_matrix;
}

//6D
template <typename T>
ViewFMatrix<T>::ViewFMatrix(T *some_matrix,
                            size_t some_dim1,
                            size_t some_dim2,
                            size_t some_dim3,
                            size_t some_dim4,
                            size_t some_dim5,
                            size_t some_dim6)
{
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    dim3_ = some_dim3;
    dim4_ = some_dim4;
    dim5_ = some_dim5;
    dim6_ = some_dim6;
    length_ = dim1_ * dim2_ * dim3_ * dim4_ * dim5_ * dim6_;
    this_matrix_ = some_matrix;
}

//overload operator ()

//1D
template <typename T>
inline T& ViewFMatrix<T>::operator()(size_t i) const
{
    assert(i >= 1 && i <= dim1_ && "i is out of bounds in c_matrix 1D");  // die if >= dim1
        
    return this_matrix_[(i - 1)];
}

//2D
template <typename T>
inline T& ViewFMatrix<T>::operator()(size_t i, 
                                     size_t j) const
{
       
    assert(i >= 1 && i <= dim1_ && "i is out of bounds in c_matrix 2D");  // die if >= dim1
    assert(j >= 1 && j <= dim2_ && "j is out of bounds in c_matrix 2D");  // die if >= dim2
        
    return this_matrix_[(i - 1) + ((j - 1) * dim1_)];
}

//3D
template <typename T>
inline T& ViewFMatrix<T>::operator()(size_t i, 
                                     size_t j, 
                                     size_t k) const
{
    assert(i >= 1 && i <= dim1_ && "i is out of bounds in c_matrix 3D");  // die if >= dim1
    assert(j >= 1 && j <= dim2_ && "j is out of bounds in c_matrix 3D");  // die if >= dim2
    assert(k >= 1 && k <= dim3_ && "k is out of bounds in c_matrix 3D");  // die if >= dim3
        
    return this_matrix_[(i - 1) + ((j - 1) * dim1_) 
                                + ((k - 1) * dim1_ * dim2_)];
}

//4D
template <typename T>
inline T& ViewFMatrix<T>::operator()(size_t i, 
                                     size_t j, 
                                     size_t k, 
                                     size_t l) const
{
    assert(i >= 1 && i <= dim1_ && "i is out of bounds in c_matrix 4D");  // die if >= dim1
    assert(j >= 1 && j <= dim2_ && "j is out of bounds in c_matrix 4D");  // die if >= dim2
    assert(k >= 1 && k <= dim3_ && "k is out of bounds in c_matrix 4D");  // die if >= dim3
    assert(l >= 1 && l <= dim4_ && "l is out of bounds in c_matrix 4D");  // die if >= dim4
        
    return this_matrix_[(i - 1) + ((j - 1) * dim1_) 
                                + ((k - 1) * dim1_ * dim2_) 
                                + ((l - 1) * dim1_ * dim2_ * dim3_)];
}

//5D
template <typename T>
inline T& ViewFMatrix<T>::operator()(size_t i, 
                                     size_t j, 
                                     size_t k, 
                                     size_t l, 
                                     size_t m) const
{
    assert(i >= 1 && i <= dim1_ && "i is out of bounds in c_matrix 5D");  // die if >= dim1
    assert(j >= 1 && j <= dim2_ && "j is out of bounds in c_matrix 5D");  // die if >= dim2
    assert(k >= 1 && k <= dim3_ && "k is out of bounds in c_matrix 5D");  // die if >= dim3
    assert(l >= 1 && l <= dim4_ && "l is out of bounds in c_matrix 5D");  // die if >= dim4
    assert(m >= 1 && m <= dim5_ && "m is out of bounds in c_matrix 5D");  // die if >= dim5
       
    return this_matrix_[(i - 1) + ((j - 1) * dim1_) 
                                + ((k - 1) * dim1_ * dim2_) 
                                + ((l - 1) * dim1_ * dim2_ * dim3_)
                                + ((m - 1) * dim1_ * dim2_ * dim3_ * dim4_)];
}

//6D
template <typename T>
inline T& ViewFMatrix<T>::operator()(size_t i, size_t j, size_t k, size_t l, size_t m, size_t n) const
{
    assert(i >= 1 && i <= dim1_ && "i is out of bounds in c_matrix 6D");  // die if >= dim1
    assert(j >= 1 && j <= dim2_ && "j is out of bounds in c_matrix 6D");  // die if >= dim2
    assert(k >= 1 && k <= dim3_ && "k is out of bounds in c_matrix 6D");  // die if >= dim3
    assert(l >= 1 && l <= dim4_ && "l is out of bounds in c_matrix 6D");  // die if >= dim4
    assert(m >= 1 && m <= dim5_ && "m is out of bounds in c_matrix 6D");  // die if >= dim5
    assert(n >= 1 && n <= dim6_ && "n is out of bounds in c_matrix 6D");  // die if >= dim6
    return this_matrix_[(i - 1) + ((j - 1) * dim1_) 
                                + ((k - 1) * dim1_ * dim2_) 
                                + ((l - 1) * dim1_ * dim2_ * dim3_)
                                + ((m - 1) * dim1_ * dim2_ * dim3_ * dim4_)
                                + ((n - 1) * dim1_ * dim2_ * dim3_ * dim4_ * dim5_)];
}

//-----end ViewFMatrix-----

//5. CArray
template <typename T>
class CArray {
private:
    size_t dim1_, dim2_, dim3_, dim4_, dim5_, dim6_;
    size_t length_; // Length of 1D array
    T* this_array_;

public:
    // Default constructor
    CArray ();

    // --- 1D array ---
    
    // Overloaded constructor
    CArray (size_t some_dim1);

    // Overload operator() to access data as array(i), where i = [0:N-1]
    T& operator() (size_t i) const;

    // --- 2D array ---
    
    // Overloaded constructor
    CArray (size_t some_dim1, size_t some_dim2);

    // Overload operator() to access data as array(i, j),
    // where i = [0:N-1], j = [0:N-1]
    T& operator() (size_t i, size_t j) const;

    // --- 3D array ---
    
    // Overloaded constructor
    CArray (size_t some_dim1, size_t some_dim2, size_t some_dim3);

    // Overload operator() to access data as array(i, j, k),
    // where i = [0:N-1], j = [0:N-1], k = [0:N-1]
    T& operator() (size_t i, size_t j, size_t k) const;

    // --- 4D array ---
    
    // Overloaded constructor
    CArray (size_t some_dim1, size_t some_dim2, size_t some_dim3, 
               size_t some_dim4);

    // Overload operator() to access data as array(i, j, k, l),
    // where i = [0:N-1], j = [0:N-1], k = [0:N-1], l = [0:N-1]
    T& operator() (size_t i, size_t j, size_t k, size_t l) const;

    // --- 5D array ---
    
    // Overloaded constructor
    CArray (size_t some_dim1, size_t some_dim2, size_t some_dim3,
               size_t some_dim4, size_t some_dim5);

    // Overload operator() to access data as array(i, j, k, l, m),
    // where i = [0:N-1], j = [0:N-1], k = [0:N-1], l = [0:N-1],
    // m = [0:N-1]
    T& operator() (size_t i, size_t j, size_t k, size_t l,
                          size_t m) const;

    // --- 6D array ---
    
    // Overloaded constructor
    CArray (size_t some_dim1, size_t some_dim2, size_t some_dim3,
               size_t some_dim4, size_t some_dim5, size_t some_dim6);

    // Overload operator() to access data as array(i, j, k, l, m, n),
    // where i = [0:N-1], j = [0:N-1], k = [0:N-1], l = [0:N-1], 
    // m = [0:N-1], n = [0:N-1]
    T& operator() (size_t i, size_t j, size_t k, size_t l,
                          size_t m, size_t n) const;

    // Overload copy assignment operator
    CArray& operator= (const CArray& temp); 

    size_t size() const;

    // Deconstructor
    ~CArray ();

}; // End of CArray

//---carray class declarations---

//constructors

//no dim
template <typename T>
CArray<T>::CArray() {}

//1D
template <typename T>
CArray<T>::CArray(size_t some_dim1) {
    // assert(some_dim1 > 0);
    dim1_ = some_dim1;
    length_ = dim1_;
    this_array_ = new T[length_];
}

//2D
template <typename T>
CArray<T>::CArray(size_t some_dim1, size_t some_dim2) {
    // assert(some_dim1 > 0);
    // assert(some_dim2 > 0);
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    length_ = dim1_ * dim2_;
    this_array_ = new T[length_];
}

//3D
template <typename T>
CArray<T>::CArray(size_t some_dim1, size_t some_dim2, size_t some_dim3) {
    // assert(some_dim1 > 0);
    // assert(some_dim2 > 0);
    // assert(some_dim3 > 0);
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    dim3_ = some_dim3;
    length_ = dim1_ * dim2_ * dim3_;
    this_array_ = new T[length_];
}

//4D
template <typename T>
CArray<T>::CArray(size_t some_dim1, size_t some_dim2, size_t some_dim3, 
                        size_t some_dim4) {
    // assert(some_dim1 > 0);
    // assert(some_dim2 > 0);
    // assert(some_dim3 > 0);
    // assert(some_dim4 > 0);
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    dim3_ = some_dim3;
    dim4_ = some_dim4;
    length_ = dim1_ * dim2_ * dim3_ * dim4_;
    this_array_ = new T[length_];
}

//5D
template <typename T>
CArray<T>::CArray(size_t some_dim1, size_t some_dim2, size_t some_dim3,
                        size_t some_dim4, size_t some_dim5) {
    // assert(some_dim1 > 0);
    // assert(some_dim2 > 0);
    // assert(some_dim3 > 0);
    // assert(some_dim4 > 0):
    // assert(some_dim5 > 0);
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    dim3_ = some_dim3;
    dim4_ = some_dim4;
    dim5_ = some_dim5;
    length_ = dim1_ * dim2_ * dim3_ * dim4_ * dim5_;
    this_array_ = new T[length_];
}

//6D
template <typename T>
CArray<T>::CArray(size_t some_dim1, size_t some_dim2, size_t some_dim3,
                        size_t some_dim4, size_t some_dim5, size_t some_dim6) {
    // assert(some_dim1 > 0);
    // assert(some_dim2 > 0);
    // assert(some_dim3 > 0);
    // assert(some_dim4 > 0):
    // assert(some_dim5 > 0);
    // assert(some_dim6 > 0);
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    dim3_ = some_dim3;
    dim4_ = some_dim4;
    dim5_ = some_dim5;
    dim6_ = some_dim6;
    length_ = dim1_ * dim2_ * dim3_ * dim4_ * dim5_ * dim6_;
    this_array_ = new T[length_];

}

//overload () operator

//1D
template <typename T>
inline T& CArray<T>::operator() (size_t i) const {
    assert(i < dim1_);
    return this_array_[i];
}

//2D
template <typename T>
inline T& CArray<T>::operator() (size_t i, size_t j) const {
    assert(i < dim1_);
    assert(j < dim2_);
    return this_array_[j + (i * dim2_)];
}

//3D
template <typename T>
inline T& CArray<T>::operator() (size_t i, size_t j, size_t k) const {
    assert(i < dim1_);
    assert(j < dim2_);
    assert(k < dim3_);
    return this_array_[k + (j * dim3_) + (i * dim3_ * dim2_)];
}

//4D
template <typename T>
inline T& CArray<T>::operator() (size_t i, size_t j, size_t k, size_t l) const {
    assert(i < dim1_);
    assert(j < dim2_);
    assert(k < dim3_);
    assert(l < dim4_);
    return this_array_[l + (k * dim4_) 
                         + (j * dim4_ * dim3_)  
                         + (i * dim4_ * dim3_ * dim2_)];
}

//5D
template <typename T>
inline T& CArray<T>::operator() (size_t i, size_t j, size_t k, size_t l,
                                    size_t m) const {
    assert(i < dim1_);
    assert(j < dim2_);
    assert(k < dim3_);
    assert(l < dim4_);
    assert(m < dim5_);
    return this_array_[m + (l * dim5_) 
                         + (k * dim5_ * dim4_) 
                         + (j * dim5_ * dim4_ * dim3_) 
                         + (i * dim5_ * dim4_ * dim3_ * dim2_)];
}

//6D
template <typename T>
inline T& CArray<T>::operator() (size_t i, size_t j, size_t k, size_t l,
                                    size_t m, size_t n) const {
    assert(i < dim1_);
    assert(j < dim2_);
    assert(k < dim3_);
    assert(l < dim4_);
    assert(m < dim5_);
    assert(n < dim6_);
    return this_array_[n + (m * dim6_) 
                         + (l * dim6_ * dim5_)  
                         + (k * dim6_ * dim5_ * dim4_) 
                         + (j * dim6_ * dim5_ * dim4_ * dim3_)  
                         + (i * dim6_ * dim5_ * dim4_ * dim3_ * dim2_)];

}

//overload = operator
template <typename T>
inline CArray<T>& CArray<T>::operator= (const CArray& temp)
{
    // Do nothing if the assignment is of the form x = x
    if (this != &temp) {
        dim1_ = temp.dim1_;
        dim2_ = temp.dim2_;
        dim3_ = temp.dim3_;
        dim4_ = temp.dim4_;
        dim5_ = temp.dim5_;
        dim6_ = temp.dim6_;
        length_ = temp.length_;
        this_array_ = new T[length_];
    }
    return *this;
}

//return size
template <typename T>
inline size_t CArray<T>::size() const {
    return length_;
}

//destructor
template <typename T>
CArray<T>::~CArray() {
    delete[] this_array_;
}

//----endof carray class definitions----

//6. ViewCArray
template <typename T>
class ViewCArray {

private:
    size_t dim1_, dim2_, dim3_, dim4_, dim5_, dim6_;
    size_t length_; // Length of 1D array
    T * this_array_;
    
public:
    
    // Default constructor
    ViewCArray ();
    
    //--- 1D array ---
    
    // Overloaded constructor
    ViewCArray(T *some_array,
               size_t some_dim1);
    
    // Overload  operator() to access data as array(i,j),
    // where i = [0:N-1], j = [0:N-1]
    T& operator()(size_t i) const;
    
    //--- 2D array ---
    
    // Overloaded constructor
    ViewCArray(T *some_array,
               size_t some_dim1,
               size_t some_dim2);
    
    // Overload operator() to access data as array(i,j),
    //  where i=[0:N-1], j=[0:N-1]
    T& operator()(size_t i, size_t j) const;
    
    //--- 3D array ---
    
    // Overloaded constructor
    ViewCArray(T *some_array,
               size_t some_dim1,
               size_t some_dim2,
               size_t some_dim3);
    
    // Overload operator() to access data as array(i,j,k),
    // where i = [0:N-1], j = [0:N-1], k = [0:N-1]
    T& operator()(size_t i, size_t j, size_t k) const;
    
    //--- 4D array ---
    
    // Overloaded constructor
    ViewCArray(T *some_array,
               size_t some_dim1,
               size_t some_dim2,
               size_t some_dim3,
               size_t some_dim4);
        
    // Overload operator() to access data as array(i, j, k, l),
    // where i = [0:n-1], j = [0:N-1], k = [0:N-1], l = [0:N-1]
    T& operator()(size_t i, size_t j, size_t k, size_t l) const;
    
    //--- 5D array ---
    
    // Overloaded constructor
    ViewCArray (T *some_array,
                size_t some_dim1,
                size_t some_dim2,
                size_t some_dim3,
                size_t some_dim4,
                size_t some_dim5);
        
    // Overload operator() to access data as array(i,j,k,l,m),
    // where i = [0:N-1], j = [0:N-1], k = [0:N-1], l = [0:N-1], m = [0:N-1]
    T& operator() (size_t i, size_t j, size_t k, size_t l, size_t m) const;
    
    //--- 6D array ---
    
    // Overloaded constructor
    ViewCArray (T *some_array,
                size_t some_dim1,
                size_t some_dim2,
                size_t some_dim3,
                size_t some_dim4,
                size_t some_dim5,
                size_t some_dim6);
        
    // Overload operator() to access data as array(i,j,k,l,m,n),
    // where i = [0:N-1], j = [0:N-1], k = [0:N-1], l = [0:N-1], m = [0:N-1]
    T& operator()(size_t i, size_t j, size_t k, size_t l, size_t m, size_t n) const;
    
    size_t size() const;
    
}; // end of ViewCArray

//class definitions

//constructors

//no dim
template <typename T>
ViewCArray<T>::ViewCArray() {}

//1D
template <typename T>
ViewCArray<T>::ViewCArray(T *some_array,
                          size_t some_dim1)
{
    dim1_ = some_dim1;
    length_ = dim1_;
    this_array_ = some_array;
}

//2D
template <typename T>
ViewCArray<T>::ViewCArray(T *some_array,
                          size_t some_dim1,
                          size_t some_dim2)
{
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    length_ = dim1_ * dim2_;
    this_array_ = some_array;
}

//3D
template <typename T>
ViewCArray<T>::ViewCArray (T *some_array,
                           size_t some_dim1,
                           size_t some_dim2,
                           size_t some_dim3)
{
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    dim3_ = some_dim3;
    length_ = dim1_ * dim2_ * dim3_;
    this_array_ = some_array;
}

//4D
template <typename T>
ViewCArray<T>::ViewCArray(T *some_array,
                          size_t some_dim1,
                          size_t some_dim2,
                          size_t some_dim3,
                          size_t some_dim4)
{
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    dim3_ = some_dim3;
    dim4_ = some_dim4;
    length_ = (dim1_ * dim2_ * dim3_ * dim4_);
    this_array_ = some_array;
}

//5D
template <typename T>
ViewCArray<T>::ViewCArray(T *some_array,
                          size_t some_dim1,
                          size_t some_dim2,
                          size_t some_dim3,
                          size_t some_dim4,
                          size_t some_dim5)
{
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    dim3_ = some_dim3;
    dim4_ = some_dim4;
    dim5_ = some_dim5;
    length_ = (dim1_ * dim2_ * dim3_ * dim4_ * dim5_);
    this_array_ = some_array;
}

//6D
template <typename T>
ViewCArray<T>::ViewCArray(T *some_array,
                          size_t some_dim1,
                          size_t some_dim2,
                          size_t some_dim3,
                          size_t some_dim4,
                          size_t some_dim5,
                          size_t some_dim6)
{
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    dim3_ = some_dim3;
    dim4_ = some_dim4;
    dim5_ = some_dim5;
    dim6_ = some_dim6;
    length_ = dim1_ * dim2_ * dim3_ * dim4_ * dim5_ * dim6_;
    this_array_ = some_array;
}

//overload () operator

//1D
template <typename T>
inline T& ViewCArray<T>::operator()(size_t i) const
{
    assert(i < dim1_ && "i is out of bounds in c_array 1D");  // die if >= dim1
    
    return this_array_[i];
}

//2D
template <typename T>
inline T& ViewCArray<T>::operator()(size_t i, 
                                    size_t j) const
{
   
    assert(i < dim1_ && "i is out of bounds in c_array 2D");  // die if >= dim1
    assert(j < dim2_ && "j is out of bounds in c_array 2D");  // die if >= dim2
    
    return this_array_[j + (i * dim2_)];
}

//3D
template <typename T>
inline T& ViewCArray<T>::operator()(size_t i, 
                                    size_t j, 
                                    size_t k) const
{
    assert(i < dim1_ && "i is out of bounds in c_array 3D");  // die if >= dim1
    assert(j < dim2_ && "j is out of bounds in c_array 3D");  // die if >= dim2
    assert(k < dim3_ && "k is out of bounds in c_array 3D");  // die if >= dim3
    
    return this_array_[k + (j * dim3_) 
                         + (i * dim3_ * dim2_)];
}

//4D
template <typename T>
inline T& ViewCArray<T>::operator()(size_t i, 
                                    size_t j, 
                                    size_t k, 
                                    size_t l) const
{
    assert(i < dim1_ && "i is out of bounds in c_array 4D");  // die if >= dim1
    assert(j < dim2_ && "j is out of bounds in c_array 4D");  // die if >= dim2
    assert(k < dim3_ && "k is out of bounds in c_array 4D");  // die if >= dim3
    assert(l < dim4_ && "l is out of bounds in c_array 4D");  // die if >= dim4
    
    return this_array_[l + (k * dim4_) 
                         + (j * dim4_ * dim3_) 
                         + (i * dim4_ * dim3_ * dim2_)];
}

//5D
template <typename T>
inline T& ViewCArray<T>::operator()(size_t i, 
                                    size_t j, 
                                    size_t k, 
                                    size_t l, 
                                    size_t m) const
{
    assert(i < dim1_ && "i is out of bounds in c_array 5D");  // die if >= dim1
    assert(j < dim2_ && "j is out of bounds in c_array 5D");  // die if >= dim2
    assert(k < dim3_ && "k is out of bounds in c_array 5D");  // die if >= dim3
    assert(l < dim4_ && "l is out of bounds in c_array 5D");  // die if >= dim4
    assert(m < dim5_ && "m is out of bounds in c_array 5D");  // die if >= dim5
    
    return this_array_[m + (l * dim5_) 
                         + (k * dim5_ * dim4_) 
                         + (j * dim5_ * dim4_ * dim3_)
                         + (i * dim5_ * dim4_ * dim3_ * dim2_)];
}

//6D
template <typename T>
inline T& ViewCArray<T>::operator()(size_t i, size_t j, size_t k, size_t l, size_t m, size_t n) const
{
    assert(i < dim1_ && "i is out of bounds in c_array 6D");  // die if >= dim1
    assert(j < dim2_ && "j is out of bounds in c_array 6D");  // die if >= dim2
    assert(k < dim3_ && "k is out of bounds in c_array 6D");  // die if >= dim3
    assert(l < dim4_ && "l is out of bounds in c_array 6D");  // die if >= dim4
    assert(m < dim5_ && "m is out of bounds in c_array 6D");  // die if >= dim5
    assert(n < dim6_ && "n is out of bounds in c_array 6D");  // die if >= dim6
    
    return this_array_[n + (m * dim6_) 
                         + (l * dim6_ * dim5_) 
                         + (k * dim6_ * dim5_ * dim4_)
                         + (j * dim6_ * dim5_ * dim4_ * dim3_) 
                         + (i * dim6_ * dim5_ * dim4_ * dim3_ * dim2_)];
}

//return size    
template <typename T>
inline size_t ViewCArray<T>::size() const {
    return length_;
}

//---end of ViewCArray class definitions----


//7. CMatrix
template <typename T>
class CMatrix {
        
private:
    size_t dim1_, dim2_, dim3_, dim4_, dim5_, dim6_,length_;
    T * this_matrix;
            
public:
        
       // default constructor
       CMatrix();
       CMatrix(size_t some_dim1);
       CMatrix(size_t some_dim1, size_t some_dim2);
       CMatrix(size_t some_dim1, size_t some_dim2, size_t some_dim3);
       CMatrix(size_t some_dim1,
           size_t some_dim2,
           size_t some_dim3,
           size_t some_dim4);
       CMatrix(size_t some_dim1,
           size_t some_dim2,
           size_t some_dim3,
           size_t some_dim4,
           size_t some_dim5);
       CMatrix (size_t some_dim1,
            size_t some_dim2,
            size_t some_dim3,
            size_t some_dim4,
            size_t some_dim5,
            size_t some_dim6);
           
    //overload operators to access data
       T& operator()(size_t i);
       T& operator()(size_t i, size_t j);
       T& operator()(size_t i, size_t j, size_t k);
       T& operator()(size_t i, size_t j, size_t k, size_t l);
       T& operator()(size_t i, size_t j, size_t k, size_t l, size_t m);
       T& operator()(size_t i, size_t j, size_t k, size_t l, size_t m, size_t n);

       //overload = operator
	CMatrix& operator= (const CMatrix &temp);
            
       // deconstructor
       ~CMatrix( );
        
}; // end of CMatrix

// CMatrix class definitions

//constructors

//no dim

//1D
template <typename T>
CMatrix<T>::CMatrix() {}

//1D
template <typename T>
CMatrix<T>::CMatrix(size_t some_dim1) {
    dim1_ = some_dim1;
    length_ = dim1_;
    this_matrix = new T[length_];
}

//2D
template <typename T>
CMatrix<T>::CMatrix(size_t some_dim1, size_t some_dim2) {
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    length_ = dim1_*dim2_;
    this_matrix = new T[length_];
}

//3D
template <typename T>
CMatrix<T>::CMatrix(size_t some_dim1, size_t some_dim2, size_t some_dim3) {
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    dim3_ = some_dim3;
    length_ = dim1_*dim2_*dim3_;
    this_matrix = new T[length_];
}

//4D
template <typename T>
CMatrix<T>::CMatrix(size_t some_dim1, size_t some_dim2, size_t some_dim3, size_t some_dim4) {
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    dim3_ = some_dim3;
    dim4_ = some_dim4;
    length_ = dim1_*dim2_*dim3_*dim4_;
    this_matrix= new T[length_];
}   

//5D
template <typename T>
CMatrix<T>::CMatrix(size_t some_dim1, size_t some_dim2, size_t some_dim3, size_t some_dim4, size_t some_dim5) {
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    dim3_ = some_dim3;
    dim4_ = some_dim4;
    dim5_ = some_dim5;
    length_ = dim1_*dim2_*dim3_*dim4_*dim5_;
    this_matrix = new T[length_];
}

//6D
template <typename T>
CMatrix<T>::CMatrix(size_t some_dim1, size_t some_dim2, size_t some_dim3, size_t some_dim4, size_t some_dim5, size_t some_dim6) {
    dim1_ = some_dim1;
    dim2_ = some_dim2;
    dim3_ = some_dim3;
    dim4_ = some_dim4;
    dim5_ = some_dim5;
    dim6_ = some_dim6;
    length_ = dim1_*dim2_*dim3_*dim4_*dim5_*dim6_;
    this_matrix = new T[length_];
}


//overload () operator

//1D
template <typename T>
T& CMatrix<T>::operator()(size_t i)
{
    assert( i < dim1_+1 && "i is out of bounds in CMatrix 1D!");
    return this_matrix[i-1];
}

//2D
template <typename T>
T& CMatrix<T>::operator()(size_t i, size_t j)
{
    assert( i < dim1_+1 && "i is out of bounds in CMatrix 2D!");
    assert( j < dim2_+1 && "j is out of bounds in CMatrix 2D!");
    return this_matrix[(j-1) + (i-1)*dim2_];
}

//3D
template <typename T>
T& CMatrix<T>::operator()(size_t i, size_t j, size_t k)
{
    assert( i < dim1_+1 && "i is out of bounds in CMatrix 3D!");
    assert( j < dim2_+1 && "j is out of bounds in CMatrix 3D!");
    assert( k < dim3_+1 && "k is out of bounds in CMatrix 3D!");
    return this_matrix[(k-1) + (j-1)*dim3_ + (i-1)*dim3_*dim2_];
}

//4D
template <typename T>
T& CMatrix<T>::operator()(size_t i, size_t j, size_t k, size_t l)
{
    assert( i < dim1_+1 && "i is out of bounds in CMatrix 4D!");
    assert( j < dim2_+1 && "j is out of bounds in CMatrix 4D!");
    assert( k < dim3_+1 && "k is out of bounds in CMatrix 4D!");
    assert( l < dim4_+1 && "l is out of bounds in CMatrix 4D!");
    return this_matrix[ (l-1) + (k-1)*dim4_ + (j-1)*dim4_*dim3_ + (i-1)*dim4_*dim3_*dim2_];
}

//5D
template <typename T>
T& CMatrix<T>::operator()(size_t i, size_t j, size_t k, size_t l,size_t m)
{
    assert( i < dim1_+1 && "i is out of bounds in CMatrix 5D!");
    assert( j < dim2_+1 && "j is out of bounds in CMatrix 5D!");
    assert( k < dim3_+1 && "k is out of bounds in CMatrix 5D!");
    assert( l < dim4_+1 && "l is out of bounds in CMatrix 5D!");
    assert( m < dim5_+1 && "m is out of bounds in CMatrix 5D!");
    return this_matrix[(m-1) + (l-1)*dim5_ + (k-1)*dim5_*dim4_ + (j-1)*dim5_*dim4_*dim3_ + (i-1)*dim5_*dim4_*dim3_*dim2_];
}

//6D
template <typename T>
T& CMatrix<T>::operator()(size_t i, size_t j, size_t k, size_t l, size_t m, size_t n)
{
    assert( i < dim1_+1 && "i is out of bounds in CMatrix 6D!");
    assert( j < dim2_+1 && "j is out of bounds in CMatrix 6D!");
    assert( k < dim3_+1 && "k is out of bounds in CMatrix 6D!");
    assert( l < dim4_+1 && "l is out of bounds in CMatrix 6D!");
    assert( m < dim5_+1 && "m is out of bounds in CMatrix 6D!");
    assert( n < dim6_+1 && "n is out of bounds in CMatrix 6D!");
    return this_matrix[ (n-1) + (m-1)*dim6_ + (l-1)*dim6_*dim5_ + (k-1)*dim6_*dim5_*dim4_ + (j-1)*dim6_*dim5_*dim4_*dim3_ + (i-1)*dim6_*dim5_*dim4_*dim3_*dim2_];
}

//overload = operator
//THIS = CMatrix<> temp
template <typename T>
CMatrix<T> &CMatrix<T>::operator= (const CMatrix &temp) {
	if(this != &temp) {
        dim1_ = temp.dim1_;
        dim2_ = temp.dim2_;
        dim3_ = temp.dim3_;
        dim4_ = temp.dim4_;
        dim5_ = temp.dim5_;
        dim6_ = temp.dim6_;
        length_ = temp.length_;
        this_matrix = new T[length_];
	}
  return *this;
}

// Destructor
template <typename T>
CMatrix<T>::~CMatrix(){
    delete[] this_matrix;
}

//----end of CMatrix class definitions----

//8. ViewCMatrix
template <typename T>
class ViewCMatrix {

private:
    size_t dim1_, dim2_, dim3_, dim4_, dim5_, dim6_;
     T * this_matrix;
		    
public:
		    
    // default constructor
    ViewCMatrix();
		    
		    
    //--- 1D array ---	   	    
    // overloaded constructor
    ViewCMatrix (T *some_matrix,size_t some_dim1);
    T& operator() (size_t i);
		    
    //--- 2D array ---	    
    // overloaded constructor
    ViewCMatrix (T *some_matrix, size_t some_dim1, size_t some_dim2);
		    
    T& operator() (size_t i, size_t j);
		    
    //--- 3D array ---	    
    // overloaded constructor
    ViewCMatrix (T *some_matrix,
		size_t some_dim1,
		size_t some_dim2,
		size_t some_dim3);
    T& operator() (size_t i, size_t j, size_t k);
		    
    //--- 4D array ---
		    
    // overloaded constructor
    ViewCMatrix (T *some_matrix,
		size_t some_dim1,
		size_t some_dim2,
		size_t some_dim3,
		size_t some_dim4);
		    
    T& operator() (size_t i, size_t j, size_t k, size_t l);

		    
    //--- 5D array ---
    // overloaded constructor
    ViewCMatrix (T *some_matrix,
		size_t some_dim1,
		size_t some_dim2,
		size_t some_dim3,
		size_t some_dim4,
		size_t some_dim5);
    T& operator() (size_t i, size_t j, size_t k, size_t l, size_t m);

    //--- 6D array ---		    
    // overloaded constructor
    ViewCMatrix (T *some_matrix,
		   size_t some_dim1,
		   size_t some_dim2,
		   size_t some_dim3,
		   size_t some_dim4,
		   size_t some_dim5,
		   size_t some_dim6);
		    
   T& operator() (size_t i, size_t j, size_t k, size_t l, size_t m, size_t n);

		    
}; // end of ViewCMatrix

//class definitions

//constructors

//no dim
template <typename T>
ViewCMatrix<T>::ViewCMatrix(){}

//1D
template <typename T>
ViewCMatrix<T>::ViewCMatrix(T *some_matrix,size_t some_dim1) {
	dim1_ = some_dim1;
	this_matrix = some_matrix;
}

//2D
template <typename T>
ViewCMatrix<T>::ViewCMatrix(T *some_matrix, size_t some_dim1, size_t some_dim2) {
	dim1_ = some_dim1;
	dim2_ = some_dim2;
	this_matrix = some_matrix;
}

//3D
template <typename T>
ViewCMatrix<T>::ViewCMatrix(T *some_matrix, size_t some_dim1, size_t some_dim2, size_t some_dim3) {
	dim1_ = some_dim1;
	dim2_ = some_dim2;
	dim3_ = some_dim3;
	this_matrix = some_matrix;
}

//4D
template <typename T>
ViewCMatrix<T>::ViewCMatrix(T *some_matrix, size_t some_dim1, size_t some_dim2, size_t some_dim3, size_t some_dim4){
	dim1_ = some_dim1;
	dim2_ = some_dim2;
	dim3_ = some_dim3;
	dim4_ = some_dim4;
	this_matrix = some_matrix;
}

//5D
template <typename T>
ViewCMatrix<T>::ViewCMatrix(T *some_matrix, size_t some_dim1, size_t some_dim2, size_t some_dim3, size_t some_dim4, size_t some_dim5){
	dim1_ = some_dim1;
	dim2_ = some_dim2;
	dim3_ = some_dim3;
	dim4_ = some_dim4;
	dim5_ = some_dim5;
	this_matrix = some_matrix;
}

//6D
template <typename T>
ViewCMatrix<T>::ViewCMatrix(T *some_matrix, size_t some_dim1, size_t some_dim2, size_t some_dim3, size_t some_dim4, size_t some_dim5, size_t some_dim6) {
	dim1_ = some_dim1;
	dim2_ = some_dim2;
	dim3_ = some_dim3;
	dim4_ = some_dim4;
	dim5_ = some_dim5;
	dim6_ = some_dim6;
	this_matrix = some_matrix;
}

//overload () operator

//1D
template <typename T>
T& ViewCMatrix<T>:: operator() (size_t i)
{
	assert(i < dim1_+1 && "i is out of bounds for ViewCMatrix 1D!");
	return this_matrix[i-1];
}

//2D
template <typename T>
T& ViewCMatrix<T>::operator() (size_t i, size_t j)
{
	assert(i < dim1_+1 && "i is out of bounds for ViewCMatrix 2D!");
	assert(j < dim2_+1 && "j is out of bounds for ViewCMatrix 2D!");
	return this_matrix[(i-1)*dim2_ + (j-1)];
}

//3D
template <typename T>
T& ViewCMatrix<T>::operator () (size_t i, size_t j, size_t k)
{
	assert(i < dim1_+1 && "i is out of bounds for ViewCMatrix 3D!");
	assert(j < dim2_+1 && "j is out of bounds for ViewCMatrix 3D!");
	assert(k < dim3_+1 && "k is out of bounds for ViewCMatrix 3D!");
	return this_matrix[(k-1) + (j-1)*dim3_ + (i-1)*dim3_*dim2_];
}

//4D
template <typename T>
T& ViewCMatrix<T>::operator()(size_t i, size_t j, size_t k, size_t l)
{
	assert(i < dim1_+1 && "i is out of bounds for ViewCMatrix 4D!");
	assert(j < dim2_+1 && "j is out of bounds for ViewCMatrix 4D!");
	assert(k < dim3_+1 && "k is out of bounds for ViewCMatrix 4D!");
	assert(l < dim4_+1 && "l is out of bounds for ViewCMatrix 4D!");
	return this_matrix[(l-1) + (k-1)*dim4_ + (j-1)*dim4_*dim3_ + (i-1)*dim4_*dim3_*dim2_];
}

//5D
template <typename T>
T& ViewCMatrix<T>::operator()(size_t i, size_t j, size_t k,size_t l, size_t m)
{
	assert(i < dim1_+1 && "i is out of bounds for ViewCMatrix 5D!");
	assert(j < dim2_+1 && "j is out of bounds for ViewCMatrix 5D!");
	assert(k < dim3_+1 && "k is out of bounds for ViewCMatrix 5D!");
	assert(l < dim4_+1 && "l is out of bounds for ViewCMatrix 5D!");
	assert(m < dim5_+1 && "m is out of bounds for ViewCMatrix 5D!");
	return this_matrix[(m-1) + (l-1)*dim5_ + (k-1)*dim5_*dim4_ + (j-1)*dim5_*dim4_*dim3_ + (i-1)*dim5_*dim4_*dim3_*dim2_];
}

//6D
template <typename T>
T& ViewCMatrix<T>::operator()(size_t i, size_t j, size_t k, size_t l, size_t m, size_t n)
{
	assert(i < dim1_+1 && "i is out of bounds for ViewCMatrix 6D!");
	assert(j < dim2_+1 && "j is out of bounds for ViewCMatrix 6D!");
	assert(k < dim3_+1 && "k is out of bounds for ViewCMatrix 6D!");
	assert(l < dim4_+1 && "l is out of bounds for ViewCMatrix 6D!");
	assert(m < dim5_+1 && "m is out of bounds for ViewCMatrix 6D!");
	assert(n < dim6_+1 && "n is out of bounds for ViewCMatrix 6D!");
	return this_matrix[(n-1)+ (m-1)*dim6_ + (l-1)*dim5_*dim6_ + (k-1)*dim6_*dim5_*dim4_ + (j-1)*dim6_*dim5_*dim4_*dim3_ + (i-1)*dim5_*dim6_*dim4_*dim3_*dim2_];
}


//----end of ViewCMatrix class definitions----

//9. RaggedRightArray
template <typename T>
class RaggedRightArray {
private:
    size_t *start_index_;
    T * array_;
    
    size_t dim1_, length_;
    
public:
    // Default constructor
    RaggedRightArray ();
    
    //--- 2D array access of a ragged right array ---
    
    // Overload constructor for a CArray
    RaggedRightArray (CArray<size_t> &strides_array);
    
    // Overload constructor for a ViewCArray
    RaggedRightArray (ViewCArray<size_t> &strides_array);
    
    // Overloaded constructor for a traditional array
    RaggedRightArray (size_t *strides_array, size_t some_dim1);
    
    // A method to return the stride size
    size_t stride(size_t i) const;
    
    // Overload operator() to access data as array(i,j)
    // where i=[0:N-1], j=[stride(i)]
    T& operator()(size_t i, size_t j) const;

    RaggedRightArray& operator= (const RaggedRightArray &temp);

    // Destructor
    ~RaggedRightArray ( );
}; // End of RaggedRightArray


// Overloaded constructor with CArray
template <typename T>
RaggedRightArray<T>::RaggedRightArray (CArray<size_t> &strides_array){
    // The length of the stride array is some_dim1;
    //dim1_  = strides_array.size();
    
    // Create and initialize the starting index of the entries in the 1D array
    start_index_ = new size_t[(dim1_ + 1)];  // note the dim1+1
    start_index_[0] = 0; // the 1D array starts at 0
    
    // Loop over to find the total length of the 1D array to
    // represent the ragged-right array and set the starting 1D index
    size_t count = 0;
    for (size_t i = 0; i < dim1_; i++){
        count += strides_array(i);
        start_index_[(i + 1)] = count;
    } // end for i
    
    array_ = new T[count];
} // End constructor

// Overloaded constructor with a view c array
template <typename T>
RaggedRightArray<T>::RaggedRightArray (ViewCArray<size_t> &strides_array) {
    // The length of the stride array is some_dim1;
    //dim1_  = strides_array.size();
    
    // Create and initialize the starting index of the entries in the 1D array
    start_index_ = new size_t[(dim1_ + 1)];  // note the dim1+1
    start_index_[0] = 0; // the 1D array starts at 0
    
    // Loop over to find the total length of the 1D array to
    // represent the ragged-right array and set the starting 1D index
    size_t count = 0;
    for (size_t i = 0; i < dim1_; i++){
        count += strides_array(i);
        start_index_[(i + 1)] = count;
    } // end for i
    
    array_ = new T[count];
} // End constructor

// Overloaded constructor with a regular cpp array
template <typename T>
RaggedRightArray<T>::RaggedRightArray (size_t *strides_array, size_t dim1){
    // The length of the stride array is some_dim1;
    dim1_ = dim1;
    
    // Create and initialize the starting index of the entries in the 1D array
    start_index_ = new size_t[(dim1_ + 1)];  // note the dim1+1
    start_index_[0] = 0; // the 1D array starts at 0
    
    // Loop over to find the total length of the 1D array to
    // represent the ragged-right array and set the starting 1D index
    size_t count = 0;
    for (size_t i = 0; i < dim1_; i++){
        count += strides_array[i];
        start_index_[(i + 1)] = count;
    } // end for i
    
    array_ = new T[count];
} // End constructor

// A method to return the stride size
template <typename T>
inline size_t RaggedRightArray<T>::stride(size_t i) const {
    // Ensure that i is within bounds
    assert(i < (dim1_ + 1) && "i is greater than dim1_ in RaggedRightArray");

    return start_index_[(i + 1)] - start_index_[i];
}

// Overload operator() to access data as array(i,j)
// where i=[0:N-1], j=[0:stride(i)]
template <typename T>
inline T& RaggedRightArray<T>::operator()(size_t i, size_t j) const {
    // get the 1D array index
    size_t start = start_index_[i];
    
    // asserts
    assert(i < dim1_ && "i is out of dim1 bounds in RaggedRightArray");  // die if >= dim1
    assert(j < stride(i) && "j is out of stride bounds in RaggedRightArray");  // die if >= stride
    
    return array_[j + start];
} // End operator()

//overload = operator
template <typename T>
RaggedRightArray<T> & RaggedRightArray<T>::operator= (const RaggedRightArray &temp) {

    if( this != &temp) {
        dim1_ = temp.dim1_;
        length_ = temp.length_;
        start_index_ = new size_t[dim1_ + 1];
        for (int j = 0; j < dim1_; j++) {
            start_index_[j] = temp.start_index_[j];  
        }
        array_ = new T[length_];
    }
	
    return *this;
}

// Destructor
template <typename T>
RaggedRightArray<T>::~RaggedRightArray () {
    delete[] array_;
    delete[] start_index_;
}

//----end of RaggedRightArray class definitions----

//10. RaggedDownArray
template <typename T>
class RaggedDownArray { 
private:
    size_t *start_index_;
	T * array_;

	size_t dim2_;
    size_t length_;

public:
    //default constructor
    RaggedDownArray() ;

    //~~~~2D`~~~~
	//overload constructor with CArray
	RaggedDownArray(CArray<size_t> &strides_array);

	//overload with ViewCArray
	RaggedDownArray(ViewCArray <size_t> &strides_array);

	//overload with traditional array
	RaggedDownArray(size_t *strides_array, size_t dome_dim1);

	//method to return stride size
	size_t stride(size_t j);

	//overload () operator to access data as array (i,j)
	T& operator()(size_t i, size_t j);

    // method to return total size
    size_t size();

    //overload = operator
    RaggedDownArray& operator= (const RaggedDownArray &temp);

    //destructor
    ~RaggedDownArray();

}; //~~~~~end of RaggedDownArray class declarations~~~~~~~~	

//no dims
template <typename T>
RaggedDownArray<T>::RaggedDownArray() {}

//overload constructor with CArray 
template <typename T>
RaggedDownArray<T>::RaggedDownArray( CArray <size_t> &strides_array) {
    // Length of stride array
    //dim2_ = strides_array.size();

    // Create and initialize startding indices
    start_index_ = new size_t[dim2_+1]; //theres a plus 1, because 
    start_index_[0] = 0; //1D array starts at 0

		
	//length of strides
	dim2_ = strides_array.size();

    // Loop to find total length of 1D array
    size_t count = 0;
    for(size_t j = 0; j < dim2_ ; j++) { 
        count += strides_array(j);
        start_index_[j+1] = count;
    } 
    length_ = count;

    array_ = new T[count];

} // End constructor 

// Overload constructor with ViewCArray
template <typename T>
RaggedDownArray<T>::RaggedDownArray( ViewCArray <size_t> &strides_array) {
    // Length of strides
    //dim2_ = strides_array.size();

    //create array for holding start indices
    start_index_ = new size_t[dim2_+1];
    start_index_[0] = 0;

    size_t count = 0;
    // Loop over to get total length of 1D array
    for(size_t j = 0; j < dim2_ ;j++ ) {
        count += strides_array(j);
        start_index_[j+1] = count;
    }
    length_ = count;	
    array_ = new T[length_];

} // End constructor 

// Overload constructor with regualar array
template <typename T>
RaggedDownArray<T>::RaggedDownArray( size_t *strides_array, size_t dim2){
    // Length of stride array
    dim2_ = dim2;

    // Create and initialize starting index of entries
    start_index_ = new size_t[dim2_+1];
    start_index_[0] = 0;

    // Loop over to find length of 1D array
    // Represent ragged down array and set 1D index
    size_t count = 0;
    for(size_t j = 0; j < dim2_; j++) {
        count += strides_array[j];
        start_index_[j+1] = count;
	}

    length_ = count;	
    array_ = new T[length_];

} //end construnctor

// Check the stride size
template <typename T>
size_t RaggedDownArray<T>::stride(size_t j) {
    return start_index_[j+1] - start_index_[j];
}

//return size
template <typename T>
size_t RaggedDownArray<T>::size() {
    return length_;
}

// overload operator () to access data as an array(i,j)
// Note: i = 0:stride(j), j = 0:N-1
template <typename T>
T& RaggedDownArray<T>::operator()(size_t i, size_t j) {
    // Where is the array starting?
    // look at start index
    size_t start = start_index_[j]; 

    // Make sure we are within array bounds
    assert(i < stride(j) && "i is out of bounds in RaggedDownArray");
    assert(j < dim2_ && "j is out of dim2_ bounds in RaggedDownArray");
		
    return array_[i + start];

} // End () operator

//overload = operator
template <typename T>
RaggedDownArray<T> & RaggedDownArray<T>::operator= (const RaggedDownArray &temp) {

    if( this != &temp) {
        dim2_ = temp.dim2_;
        length_ = temp.length_;
        start_index_ = new size_t[dim2_ + 1];
        for (int j = 0; j < dim2_; j++) {
            start_index_[j] = temp.start_index_[j];  
        }
        array_ = new T[length_];
    }
	
    return *this;
}

// Destructor
template <typename T>
RaggedDownArray<T>::~RaggedDownArray() {
    delete[] array_;
    delete[] start_index_;

} // End destructor


//----end of RaggedDownArray----

//11. DynamicRaggedRightArray
/*
template <typename T>
class DynamicRaggedRightArrayKokkos {
private:
    size_t *stride_;
    T * array_;
    
    size_t dim1_;
    size_t dim2_;
    size_t length_;
    
public:
    // Default constructor
    DynamicRaggedRightArray ();
    
    //--- 2D array access of a ragged right array ---
    
    // overload constructor
    DynamicRaggedRightArray (size_t dim1, size_t dim2);
    
    // A method to return or set the stride size
    size_t& stride(size_t i) const;
    
    // A method to increase the stride size
    void push_back(size_t i) const;
    
    // Overload operator() to access data as array(i,j),
    // where i=[0:N-1], j=[stride(i)]
    T& operator()(size_t i, size_t j) const;
    
    // Destructor
    ~DynamicRaggedRightArray ();
}; 

// Overloaded constructor
template <typename T>
DynamicRaggedRightArray<T>::DynamicRaggedRightArray (size_t dim1, size_t dim2) {
    // The dimensions of the array;
    dim1_  = dim1;
    dim2_  = dim2;
    length_ = dim1*dim2;
    
    // Create memory on the heap for the values
    array_ = new T[dim1*dim2];
    
    // Create memory for the stride size in each row
    stride_ = new size_t[dim1];
    
    // Initialize the stride
    for (int i=0; i<dim1_; i++){
        stride_[i] = 0;
    }
    
    // Start index is always = j + i*dim2
} 

// A method to set the stride size for row i
template <typename T>
size_t& DynamicRaggedRightArray<T>::stride(size_t i) const {
    return stride_[i];
}


// A method to increase the stride size for row i
template <typename T>
void DynamicRaggedRightArray<T>::push_back(size_t i) const {
    stride_[i]++;
}

// Overload operator() to access data as array(i,j),
// where i=[0:N-1], j=[0:stride(i)]
template <typename T>
inline T& DynamicRaggedRightArray<T>::operator()(size_t i, size_t j) const {
    // Asserts
    assert(i < dim1_ && "i is out of dim1 bounds in DynamicRaggedRight");  // die if >= dim1
    assert(j < dim2_ && "j is out of dim2 bounds in DynamicRaggedRight");  // die if >= dim2
    assert(j < stride_[i] && "j is out of stride bounds in DynamicRaggedRight");  // die if >= stride
    
    return array_[j + i*dim2_];
}

// Destructor
template <typename T>
DynamicRaggedRightArray<T>::~DynamicRaggedRightArray() {
    delete[] array_;
    delete[] stride_;
}
*/



//----end DynamicRaggedRightArray class definitions----

//12. DynamicRaggedDownArray


//----end of DynamicRaggedDownArray class definitions-----

//13. SparseRowArray
template <typename T>
class SparseRowArray {
private:
    size_t *start_index_;
    size_t *column_index_;
    
    T * array_;
    
    size_t dim1_;
    
public:
    // Default constructor
    SparseRowArray ();
    
    //--- 2D array access of a ragged right array ---
    
    // Overload constructor for a CArray
    SparseRowArray (CArray<size_t> &strides_array);
    
    // Overload constructor for a ViewCArray
    SparseRowArray (ViewCArray<size_t> &strides_array);
    
    // Overloaded constructor for a traditional array
    SparseRowArray (size_t *strides_array, size_t some_dim1);
    
    // A method to return the stride size
    size_t stride(size_t i) const;
    
    // A method to return the column index as array.column_index(i,j)
    size_t column_index(size_t i, size_t j) const;
    
    // A method to access data as array.value(i,j),
    // where i=[0:N-1], j=[stride(i)]
    T& value(size_t i, size_t j) const;
    
    // Destructor
    ~SparseRowArray ();
}; 


// Overloaded constructor
template <typename T>
SparseRowArray<T>::SparseRowArray (CArray<size_t> &strides_array) {
    // The length of the stride array is some_dim1;
    dim1_  = strides_array.size();
    
    // Create and initialize the starting index of the entries in the 1D array
    start_index_ = new size_t[dim1_+1];  // note the dim1+1
    start_index_[0] = 0; // the 1D array starts at 0
    
    // Loop over to find the total length of the 1D array to
    // represent the ragged-right array and set the starting 1D index
    size_t count = 0;
    for (size_t i = 0; i < dim1_; i++){
        count += strides_array(i);
        start_index_[i+1] = count;
    } // end for i
    
    array_ = new T[count];
    column_index_ = new size_t[count];
} 


// Overloaded constructor
template <typename T>
SparseRowArray<T>::SparseRowArray (ViewCArray<size_t> &strides_array) {
    // The length of the stride array is some_dim1;
    dim1_  = strides_array.size();
    
    // Create and initialize the starting index of the entries in the 1D array
    start_index_ = new size_t[dim1_+1];  // note the dim1+1
    start_index_[0] = 0; // the 1D array starts at 0
    
    // Loop over to find the total length of the 1D array to
    // represent the ragged-right array and set the starting 1D index
    size_t count = 0;
    for (size_t i = 0; i < dim1_; i++){
        count += strides_array(i);
        start_index_[i+1] = count;
    } // end for i
    
    array_ = new T[count];
    column_index_ = new size_t[count];
} 

// Overloaded constructor
template <typename T>
SparseRowArray<T>::SparseRowArray (size_t *strides_array, size_t dim1) {
    // The length of the stride array is some_dim1;
    dim1_ = dim1;
    
    // Create and initialize the starting index of the entries in the 1D array
    start_index_ = new size_t[dim1_+1];  // note the dim1+1
    start_index_[0] = 0; // the 1D array starts at 0
    
    // Loop over to find the total length of the 1D array to
    // represent the ragged-right array and set the starting 1D index
    size_t count = 0;
    for (size_t i = 0; i < dim1_; i++){
        count += strides_array[i];
        start_index_[i+1] = count;
    } // end for i
    
    array_ = new T[count];
    column_index_ = new size_t[count];
} 


// A method to return the stride size
template <typename T>
size_t SparseRowArray<T>::stride(size_t i) const {
    return start_index_[i+1] - start_index_[i];
}

// A method to return the column index
template <typename T>
size_t SparseRowArray<T>::column_index(size_t i, size_t j) const {
    // Get the 1D array index
    size_t start = start_index_[i];
    
    // Asserts
    assert(i < dim1_ && "i is out of dim1 bounds in SparseRowArray");  // die if >= dim1
    assert(j < stride(i) && "j is out of stride bounds in SparseRowArray");  // die if >= stride
    
    return column_index_[j + start];
}

// Access data as array.value(i,j), 
// where i=[0:N-1], j=[0:stride(i)]
template <typename T>
inline T& SparseRowArray<T>::value(size_t i, size_t j) const {
    // Get the 1D array index
    size_t start = start_index_[i];
    
    // Asserts
    assert(i < dim1_ && "i is out of dim1 bounds in sparseRowArray");  // die if >= dim1
    assert(j < stride(i) && "j is out of stride bounds in sparseRowArray");  // die if >= stride
    
    return array_[j + start];
} 

// Destructor
template <typename T>
SparseRowArray<T>::~SparseRowArray() {
    delete[] array_;
    delete[] start_index_;
    delete[] column_index_;
}

//---- end of SparseRowArray class definitions-----



//14. SparseColArray
template <typename T>
class SparseColArray {

private:
	size_t *start_index_;
	size_t *row_index_;
	T * array_;

	size_t dim2;

public:

	//default constructor
	SparseColArray ();

	//constructor with CArray
	SparseColArray(CArray<size_t> &strides_array);

	//constructor with ViewCArray
	SparseColArray(ViewCArray<size_t> &strides_array);

	//constructor with regular array
	SparseColArray(size_t *strides_array, size_t some_dim1);

	//method return stride size
	size_t stride(size_t j) const;

	//method return row index ass array.row_index(i,j)
	size_t& row_index(size_t i, size_t j) const;

	//method access data as an array
	T& value(size_t i, size_t j) const;

	//destructor
	~SparseColArray();
};

//overload constructor with CArray
template <typename T>
SparseColArray<T>::SparseColArray(CArray<size_t> &strides_array) {

	dim2_ = strides_array.size();

	start_index_ = new size_t[dim2_+1];
	start_index_[0] = 0;

	//loop over to find total length of the 1D array
	size_t count = 0;
	for(size_t j = 0; j < dim2_; j++) {
	  count+= strides_array(j);
	  start_index_[j+1] = count;
	}

	array_ = new T[count];
	row_index_ = new T[count];

} //end constructor with CArray


//overload constructor with ViewCArray
template <typename T>
SparseColArray<T>::SparseColArray(ViewCArray<size_t> &strides_array) {

	dim2_ = strides_array.size();

	//create and initialize starting index of 1D array
	start_index_ = new size_t[dim2_+1];
	start_index_[0] = 0;

	//loop over to find total length of 1D array
	size_t count = 0;
	for(size_t j = 0; j < dim2_ ; j++) {
	  count += strides_array(j);
	  start_index_[j+1] = count;
	}

	array_ = new T[count];
	row_index_ = new T[count];

} //end constructor

//overload constructor with traditional array
template <typename T>
SparseColArray<T>::SparseColArray(size_t *strides_array, size_t dim2) {

	dim2_ = dim2;

	//create and initialize the starting index 
	start_index_ = new size_t[dim2_ +1];
	start_index_[0] = 0;

	//loop over to find the total length of the 1D array
	size_t count = 0;
	for(size_t j = 0; j < dim2_; j++) {
	  count += strides_array[j];
	  start_index_[j+1] = count;
	}

	array_ = new T[count];
	row_index_ = new T[count];

} //end constructor

//method to return stride size
template <typename T>
size_t SparseColArray<T>::stride(size_t j) const{
	return start_index_[j+1] - start_index_[j];
}

//acces data ass arrow.row_index(i,j)
// where i = 0:stride(j), j = 0:N-1
template <typename T>
size_t& SparseColArray<T>::row_index(size_t i, size_t j) const {

	//get 1D array index
	size_t start = start_index_[j];

	//asserts to make sure we are in bounds
	assert(i < stride(j) && "i is out of stride bounnds in SparseColArray!");
	assert(j < dim2_ && "j is out of dim1 bounds in SparseColArray");

	return row_index_[i + start];

} //end row index method	


//access values as array.value(i,j)
// where i = 0:stride(j), j = 0:N-1
template <typename T>
T& SparseColArray<T>::value(size_t i, size_t j) const {

	size_t start = start_index_[j];

	//asserts
	assert(i < stride(j) && "i is out of stride boundns in SparseColArray");
	assert(j < dim2_ && "j is out of dim1 bounds in SparseColArray");

	return array_[i + start];

}

//destructor
template <typename T>
SparseColArray<T>::~SparseColArray() {
	delete [] array_;
	delete [] start_index_;
	delete [] row_inndex_;
}

//----end SparseColArray----





//=======================================================================
//	end of standard MATAR data-types
//========================================================================













