#include <iostream>
#include <chrono>         // To access timing calipers 
#include <bits/stdc++.h>
#include <stdio.h> //for rand

#include "matar.h"

//-----likwid preprocessors----
#ifdef LIKWID_PERFMON
#include "likwid.h"
#else
#define LIKWID_MARKER_INIT
#define LIKWID_MARKER_THREADINIT
#define LIKWID_MARKER_REGISTER(regionTag)
#define LIKWID_MARKER_START(regionTag)
#define LIKWID_MARKER_STOP(regionTag)
#define LIKWID_MARKER_CLOSE
#endif

int main() {

	LIKWID_MARKER_INIT;

    int size_i = 5, size_j = 4, size_k = 3;
	const int size1 = 16777216;
	const int size3 = 256;
	const int repeat = 1;

	std::cout<<"Size of 1D problem: "<<size1<<"\n";
	std::cout<<"Size of 3D problem (each dimension): "<<size3<<"\n";
	std::cout<<"Performing each kernel "<<repeat<<" times.\n";
	std::cout<<"============================================================\n";
	

	//=========================================================================
	//	cpu stream benchmarks
	//=========================================================================	

	//1. create carrays

	//1D
	auto c_arr1 = CArray<double>(size1);
	auto c_arr2 = CArray<double>(size1);
	auto c_arr3 = CArray<double>(size1);
	auto c_arr4 = CArray<double>(size1);

	//3D
	auto c_arr1_3d = CArray<double>(size3, size3, size3);
	auto c_arr2_3d = CArray<double>(size3, size3, size3);
	auto c_arr3_3d = CArray<double>(size3, size3, size3);
	auto c_arr4_3d = CArray<double>(size3, size3, size3);

	//2. create regular arrays

	//1D
	double* reg_arr1_1d = new double[size1];
	double* reg_arr2_1d = new double[size1];
	double* reg_arr3_1d = new double[size1];
	double* reg_arr4_1d = new double[size1];

	//3D
	double*** reg_arr1_3d = new double**[size3];
	double*** reg_arr2_3d = new double**[size3];
	double*** reg_arr3_3d = new double**[size3];
	double*** reg_arr4_3d = new double**[size3];

	for(size_t i = 0; i < size3; i++) {
	 reg_arr1_3d[i] = new double*[size3];	
	 reg_arr2_3d[i] = new double*[size3];	
	 reg_arr3_3d[i] = new double*[size3];	
	 reg_arr4_3d[i] = new double*[size3];	
	  for(size_t j = 0; j < size3; j++) {
		reg_arr1_3d[i][j] = new double[size3];
		reg_arr2_3d[i][j] = new double[size3];
		reg_arr3_3d[i][j] = new double[size3];
		reg_arr4_3d[i][j] = new double[size3];
	   }
	  }

	//3. create view arrays or traditional arrays
	
	//1D
	auto v1r_1d = ViewCArray<double>(reg_arr1_1d, size1);
	auto v2r_1d = ViewCArray<double>(reg_arr2_1d, size1);
	auto v3r_1d = ViewCArray<double>(reg_arr3_1d, size1);
	auto v4r_1d = ViewCArray<double>(reg_arr4_1d, size1);
	
	
	//3D
	auto v1r_3d = ViewCArray<double>(reg_arr1_1d, size3, size3, size3);
	auto v2r_3d = ViewCArray<double>(reg_arr2_1d, size3, size3, size3);
	auto v3r_3d = ViewCArray<double>(reg_arr3_1d, size3, size3, size3);
	auto v4r_3d = ViewCArray<double>(reg_arr4_1d, size3, size3, size3);
	

	//=========================================================================
	//	initialize
	//=========================================================================

	//1D
	std::cout<<"initializing 1D arrays\n";
#pragma omp simd
	for(size_t i = 0; i < size1; i++) {
		c_arr1(i) = 1.0;
		reg_arr1_1d[i] = 1.0;
	  }

	//3D
	std::cout<<"initializing 3D arrays\n";
#pragma omp simd collapse(3)
	for(size_t i = 0; i < size3; i++) {
	 for(size_t j = 0; j < size3; j++) {
	  for(size_t k = 0; k < size3; k++) {
		c_arr1_3d(i,j,k) = 1.0;
		reg_arr1_3d[i][j][k] = 1.0;
		}
	   }
	  }

	
	//=========================================================================
	//	1D CArray Tests
	//=========================================================================



	//=======================
	//	COPY
	// arr2 = arr1
	//=======================	
	
	double carr_copy_times[repeat];	//array to hold times for carray copy
	LIKWID_MARKER_START("1D_CARRAY_COPY");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_copy_c_1d = std::chrono::steady_clock::now();
#pragma omp simd	   
	   for(size_t i = 0; i < size1; i++) {
		 c_arr2(i) = c_arr1(i);
		}
		auto end_copy_c_1d = std::chrono::steady_clock::now();
		std::chrono::duration<double> tot_copy_c_1d = end_copy_c_1d - start_copy_c_1d;
		carr_copy_times[t] = tot_copy_c_1d.count();
	}
	LIKWID_MARKER_STOP("1D_CARRAY_COPY");


	/* N will be used in the function to get min and max elements in the array*/	
	//int N = sizeof(carr_copy_times) / sizeof(carr_copy_times[0]);
	int N = repeat;
	//std::cout<<"N = "<<N<<"\n";

	//calculate average
	double avg_copy_c, tot_copy_c;
	for(size_t i = 1; i < repeat; i++) {
		tot_copy_c += carr_copy_times[i];
		//std::cout<<"carr_copy_times["<<i<<"] = "<<carr_copy_times[i]<<"\n";
	}	
	avg_copy_c = tot_copy_c / (double) (repeat-1);
	double min1dc_copy;
	double max1dc_copy;
	min1dc_copy = *std::min_element(carr_copy_times +1, carr_copy_times + N);
	max1dc_copy = *std::max_element(carr_copy_times +1, carr_copy_times + N);

	//=======================
	//	SCALE
	// arr3 = 2.0*arr2
	//=======================	
	
	double c_scale1d_times[repeat];
	LIKWID_MARKER_START("1D_CARRAY_SCALE");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_scale1dc = std::chrono::steady_clock::now();
#pragma omp simd
	   for(size_t i = 0; i < size1; i++) {
		 c_arr3(i) = 2.0*c_arr2(i);
		}
	   auto end_scale1dc = std::chrono::steady_clock::now();
	   std::chrono::duration<double> tot_scale1dc = end_scale1dc - start_scale1dc;
	   c_scale1d_times[t] = tot_scale1dc.count();
	}
	LIKWID_MARKER_STOP("1D_CARRAY_SCALE");

	//compute average
	double avg_scale1dc, tot_scale_1dc;
	for(size_t i = 1; i < repeat; i++) {
		tot_scale_1dc += c_scale1d_times[i];
	}
	avg_scale1dc = tot_scale_1dc / (double) (repeat-1);
	double min1dc_scale, max1dc_scale;
	min1dc_scale = *std::min_element(c_scale1d_times + 1, c_scale1d_times + N);
	max1dc_scale = *std::max_element(c_scale1d_times + 1, c_scale1d_times + N);

	//=======================
	//	SUM
	// arr4 = arr2 + arr1
	//=======================	

	double sum_1dc[repeat];
	LIKWID_MARKER_START("1D_CARRAY_SUM");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_sum1dc = std::chrono::steady_clock::now();
#pragma omp simd
	   for(size_t i = 0; i < size1; i++) {
			c_arr4(i) = c_arr1(i) + c_arr2(i);
		}
	   auto end_sum1dc = std::chrono::steady_clock::now();
	   std::chrono::duration<double> tot_1dc_sum = end_sum1dc - start_sum1dc;
	   sum_1dc[t] = tot_1dc_sum.count();
	}
	LIKWID_MARKER_STOP("1D_CARRAY_SUM");

	//compute average
	double avg_sum1dc, tot_sum_1dc;
	for(size_t i = 1; i < repeat; i++) {
		tot_sum_1dc += sum_1dc[i];
	}
	avg_sum1dc = tot_sum_1dc / (double) (repeat-1);
	double min1dc_sum, max1dc_sum;
	min1dc_sum = *std::min_element(sum_1dc + 1, sum_1dc + N);
	max1dc_sum = *std::max_element(sum_1dc + 1, sum_1dc + N);

	//=======================
	//	TRIAD
	// arr4 = 2.0*arr1 + arr2
	//=======================	
	
	double triad_1dc[repeat];
	LIKWID_MARKER_START("1D_CARRAY_TRIAD");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_triad1dc = std::chrono::steady_clock::now();
#pragma omp simd
	   for(size_t i = 0; i < size1; i++) {
		c_arr4(i) = 2.0*c_arr1(i) + c_arr2(i);
		}
	   auto end_triad1dc = std::chrono::steady_clock::now();
	   std::chrono::duration<double> tot_triad1dc = end_triad1dc - start_triad1dc;
	   triad_1dc[t] = tot_triad1dc.count();
	}
	LIKWID_MARKER_STOP("1D_CARRAY_TRIAD");

	//compute average
	double avg_triad_1dc, tot_1dctriad;
	for(size_t i = 1; i < repeat; i++) {
		tot_1dctriad += triad_1dc[i];
	}
	avg_triad_1dc = tot_1dctriad / (double) (repeat-1);
	double min1dc_triad, max1dc_triad;
	min1dc_triad = *std::min_element(triad_1dc + 1, triad_1dc + N);
	max1dc_triad = *std::max_element(triad_1dc + 1, triad_1dc + N);

	//=======================
	//	DOT PRODUCT
	//=======================	

	double dot_1dc[repeat];
	double dotsum1dc = 0.0;
	LIKWID_MARKER_START("1D_CARRAY_DOTPRODUCT");
	for(size_t t = 0; t < repeat; t++) {
	   double dot_sum = 0.0;
	   auto start_1dcdot = std::chrono::steady_clock::now();
	   for(size_t i = 0; i < size1; i++) {
		dot_sum += c_arr1(i)*c_arr2(i);
	   }
	   dotsum1dc += dot_sum;
	   auto end_1dcdot = std::chrono::steady_clock::now();
	   std::chrono::duration<double>tot_1dcdot = end_1dcdot - start_1dcdot;
	   dot_1dc[t] = tot_1dcdot.count();
	}
	LIKWID_MARKER_STOP("1D_CARRAY_DOTPRODUCT");

	//do some random operation so compiler doesnt ignore loop
	double rand1 = dotsum1dc + 1.0;
	std::cout<<"rand1 = "<<rand1<<"\n";

	//compute averages, min, max
	double avg_1dcdot, tot_dot1dc;
	for(size_t i = 1; i < repeat; i++) {
		tot_dot1dc += dot_1dc[i];
	}
	avg_1dcdot = tot_dot1dc / (double) (repeat-1);
	double min1dc_dot, max1dc_dot;
	min1dc_dot = *std::min_element(dot_1dc + 1, dot_1dc + N);
	max1dc_dot = *std::max_element(dot_1dc + 1, dot_1dc + N);



	//table for 1D CArray output
	std::cout<<"=============================================================\n";
	std::cout<<"	1D CArray Timings Output\n";
	std::cout<<"=============================================================\n";
	
	std::cout<<"Test           Avg.          Min.     Max.   \n";
	std::cout<<"-------------------------------------------\n";
	std::cout<<"Copy         "<<avg_copy_c<<"        "<<min1dc_copy<<"      "<<max1dc_copy<<"\n";
	std::cout<<"Scale        "<<avg_scale1dc<<"      "<<min1dc_scale<<"       "<<max1dc_scale<<"\n";
	std::cout<<"Sum          "<<avg_sum1dc<<"        "<<min1dc_sum<<"        "<<max1dc_sum<<"\n";
	std::cout<<"Triad        "<<avg_triad_1dc<<"     "<<min1dc_triad<<"        "<<max1dc_triad<<"\n";
	std::cout<<"Dot Prod.    "<<avg_1dcdot<<"        "<<min1dc_dot<<"     "<<max1dc_dot<<"\n";
	std::cout<<"=============================================================\n";
	std::cout<<"=============================================================\n";

	std::cout<<"\n";
	std::cout<<"=============================================================\n";


	//=========================================================================
	//	1D Regular Array Tests
	//=========================================================================


	//=======================
	//	COPY
	// arr2 = arr1
	//=======================	

	double reg_copy_times[repeat];	//array to hold times for carray copy
	LIKWID_MARKER_START("1D_REG_COPY");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_copy_r_1d = std::chrono::steady_clock::now();
#pragma omp simd
	   for(size_t i = 0; i < size1; i++) {
		  reg_arr2_1d[i] = reg_arr1_1d[i];
		}
	   auto end_copy_r_1d = std::chrono::steady_clock::now();
	   std::chrono::duration<double> tot_copy_r_1d = end_copy_r_1d - start_copy_r_1d;
	   reg_copy_times[t] = tot_copy_r_1d.count();
	}
	LIKWID_MARKER_STOP("1D_REG_COPY");	

	//calculate average
	double avg_copy_r, tot_copy_r;
	for(size_t i = 1; i < repeat; i++) {
		tot_copy_r += reg_copy_times[i];
	}	
	avg_copy_r = tot_copy_r / (double) (repeat-1);
	double min1dr_copy, max1dr_copy;
	min1dr_copy = *std::min_element(reg_copy_times + 1, reg_copy_times + N);
	max1dr_copy = *std::max_element(reg_copy_times + 1, reg_copy_times + N);

	//=======================
	//	SCALE
	// arr3 = 2.0*arr2
	//=======================	

	double reg_scale1d_times[repeat];
	LIKWID_MARKER_START("1D_REG_SCALE");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_scale1dr = std::chrono::steady_clock::now();
#pragma omp simd
	   for(size_t i = 0; i < size1; i++) {
		 reg_arr3_1d[i] = 2.0*reg_arr2_1d[i];
		}
	   auto end_scale1dr = std::chrono::steady_clock::now();
	   std::chrono::duration<double> tot_scale1dr = end_scale1dr - start_scale1dr;
	   reg_scale1d_times[t] = tot_scale1dr.count();
	}
	LIKWID_MARKER_STOP("1D_REG_SCALE");

	//compute average
	double avg_scale1dr, tot_scale_1dr;
	for(size_t i = 1; i < repeat; i++) {
		tot_scale_1dr += reg_scale1d_times[i];
	}
	avg_scale1dr = tot_scale_1dr / (double) (repeat-1);
	double min1dr_scale, max1dr_scale;
	min1dr_scale = *std::min_element(reg_scale1d_times + 1, reg_scale1d_times + N);
	max1dr_scale = *std::max_element(reg_scale1d_times + 1, reg_scale1d_times + N);

	//=======================
	//	SUM
	//=======================	

	double sum_1dr[repeat];
	LIKWID_MARKER_START("1D_REG_SUM");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_sum1dr = std::chrono::steady_clock::now();
#pragma omp simd
	   for(size_t i = 0; i < size1; i++) {
			reg_arr4_1d[i] = reg_arr1_1d[i] + reg_arr2_1d[i];
		}
	   auto end_sum1dr = std::chrono::steady_clock::now();
	   std::chrono::duration<double> tot_1dr_sum = end_sum1dr - start_sum1dr;
	   sum_1dr[t] = tot_1dr_sum.count();
	}
	LIKWID_MARKER_STOP("1D_REG_SUM");

	//compute average
	double avg_sum1dr, tot_sum_1dr;
	for(size_t i = 1; i < repeat; i++) {
		tot_sum_1dr += sum_1dr[i];
	}
	avg_sum1dr = tot_sum_1dr / (double) (repeat-1);
	double min1dr_sum, max1dr_sum;
	min1dr_sum = *std::min_element(sum_1dr + 1, sum_1dr + N);
	max1dr_sum = *std::max_element(sum_1dr + 1, sum_1dr + N);

	//=======================
	//	TRIAD
	//=======================	

	double triad_1dr[repeat];
	LIKWID_MARKER_START("1D_REG_TRIAD");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_triad1dr = std::chrono::steady_clock::now();
#pragma omp simd
	   for(size_t i = 0; i < size1; i++) {
		reg_arr4_1d[i] = 2.0*reg_arr1_1d[i] + reg_arr2_1d[i];
		}
	   auto end_triad1dr = std::chrono::steady_clock::now();
	   std::chrono::duration<double> tot_triad1dr = end_triad1dr - start_triad1dr;
	   triad_1dr[t] = tot_triad1dr.count();
	}
	LIKWID_MARKER_STOP("1D_REG_TRIAD");

	//compute average
	double avg_triad_1dr, tot_1drtriad;
	for(size_t i = 1; i < repeat; i++) {
		tot_1drtriad += triad_1dr[i];
	}
	avg_triad_1dr = tot_1drtriad / (double) (repeat-1);
	double min1dr_triad, max1dr_triad;
	min1dr_triad = *std::min_element(triad_1dr + 1, triad_1dr + N);
	max1dr_triad = *std::max_element(triad_1dr + 1, triad_1dr + N);

	//=======================
	//	DOT PRODUCT
	//=======================	

	double dot_1dr[repeat];
	double dotsum = 0;
	LIKWID_MARKER_START("1D_REG_DOTPRODUCT");
	for(size_t t = 0; t < repeat; t++) {
	   double dot_sum = 0.0;
	   auto start_1drdot = std::chrono::steady_clock::now();
	   for(size_t i = 0; i < size1; i++) {
		dot_sum += reg_arr1_1d[i]*reg_arr2_1d[i];
	   }
	   dotsum += dot_sum;
	   auto end_1drdot = std::chrono::steady_clock::now();
	   std::chrono::duration<double>tot_1drdot = end_1drdot - start_1drdot;
	   dot_1dr[t] = tot_1drdot.count();
	}
	LIKWID_MARKER_STOP("1D_REG_DOTPRODUCT");

	//rand operation so compiler doesnt igore loop 
	std::cout<<"Dotsum = "<<dotsum<<"\n";

	//compute averages, min, max
	double avg_1drdot, tot_dot1dr;
	for(size_t i = 1; i < repeat; i++) {
		tot_dot1dr += dot_1dr[i];
	}
	avg_1drdot = tot_dot1dr / (double) (repeat-1);
	double min1dr_dot, max1dr_dot;
	min1dr_dot = *std::min_element(dot_1dr + 1, dot_1dr + N);
	max1dr_dot = *std::max_element(dot_1dr + 1, dot_1dr + N);

	//1D regular arrays tables
	std::cout<<"=============================================================\n";
	std::cout<<"	1D Regular Timings Output\n";
	std::cout<<"=============================================================\n";
	
	std::cout<<"Test           Avg.          Min.     Max.   \n";
	std::cout<<"-------------------------------------------\n";
	std::cout<<"Copy         "<<avg_copy_r<<"        "<<min1dr_copy<<"      "<<max1dr_copy<<"\n";
	std::cout<<"Scale        "<<avg_scale1dr<<"      "<<min1dr_scale<<"       "<<max1dr_scale<<"\n";
	std::cout<<"Sum          "<<avg_sum1dr<<"        "<<min1dr_sum<<"        "<<max1dr_sum<<"\n";
	std::cout<<"Triad        "<<avg_triad_1dr<<"     "<<min1dr_triad<<"        "<<max1dr_triad<<"\n";
	std::cout<<"Dot Prod.    "<<avg_1drdot<<"        "<<min1dr_dot<<"     "<<max1dr_dot<<"\n";
	std::cout<<"=============================================================\n";
	std::cout<<"=============================================================\n";

	std::cout<<"\n";


	//=========================================================================
	//	1D VIEW Tests
	//=========================================================================



	//=======================
	//	COPY
	//	v2r_1d = v1r_1d
	//=======================	
	
	double view_copy_times[repeat];	//array to hold times for carray copy
	LIKWID_MARKER_START("1D_VIEW_COPY");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_copy_v_1d = std::chrono::steady_clock::now();
#pragma omp simd	   
	   for(size_t i = 0; i < size1; i++) {
		 v2r_1d(i) = v1r_1d(i);
		}
		auto end_copy_v_1d = std::chrono::steady_clock::now();
		std::chrono::duration<double> tot_copy_v_1d = end_copy_v_1d - start_copy_v_1d;
		view_copy_times[t] = tot_copy_v_1d.count();
	}
	LIKWID_MARKER_STOP("1D_VIEW_COPY");


	//calculate average
	double avg_copy_v, tot_copy_v;
	for(size_t i = 1; i < repeat; i++) {
		tot_copy_v += view_copy_times[i];
	}	
	avg_copy_v = tot_copy_v / (double) (repeat-1);
	double min1dv_copy;
	double max1dv_copy;
	min1dv_copy = *std::min_element(view_copy_times + 1, view_copy_times + N);
	max1dv_copy = *std::max_element(view_copy_times + 1, view_copy_times + N);

	//=======================
	//	SCALE
	//=======================	
	
	double v_scale1d_times[repeat];
	LIKWID_MARKER_START("1D_VIEW_SCALE");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_scale1dv = std::chrono::steady_clock::now();
#pragma omp simd
	   for(size_t i = 0; i < size1; i++) {
		 v3r_1d(i) = 2.0*v2r_1d(i);
		}
	   auto end_scale1dv = std::chrono::steady_clock::now();
	   std::chrono::duration<double> tot_scale1dv = end_scale1dv - start_scale1dv;
	   v_scale1d_times[t] = tot_scale1dv.count();
	}
	LIKWID_MARKER_STOP("1D_VIEW_SCALE");
	
	//compute average, min, max
	double avg_scale1dv, tot_scale_1dv;
	for(size_t i = 1; i < repeat; i++) {
		tot_scale_1dv += v_scale1d_times[i];
	}
	avg_scale1dv = tot_scale_1dv / (double) (repeat-1);
	double min1dv_scale, max1dv_scale;
	min1dv_scale = *std::min_element(v_scale1d_times + 1, v_scale1d_times + N);
	max1dv_scale = *std::max_element(v_scale1d_times + 1, v_scale1d_times + N);

	//=======================
	//	SUM
	//=======================	

	double sum_1dv[repeat];
	LIKWID_MARKER_START("1D_VIEW_SUM");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_sum1dv = std::chrono::steady_clock::now();
#pragma omp simd
	   for(size_t i = 0; i < size1; i++) {
			v4r_1d(i) = v2r_1d(i) + v1r_1d(i);
		}
	   auto end_sum1dv = std::chrono::steady_clock::now();
	   std::chrono::duration<double> tot_1dv_sum = end_sum1dv - start_sum1dv;
	   sum_1dv[t] = tot_1dv_sum.count();
	}
	LIKWID_MARKER_STOP("1D_VIEW_SUM");

	//compute average
	double avg_sum1dv, tot_sum_1dv;
	for(size_t i = 1; i < repeat; i++) {
		tot_sum_1dv += sum_1dv[i];
	}
	avg_sum1dv = tot_sum_1dv / (double) (repeat-1);
	double min1dv_sum, max1dv_sum;
	min1dv_sum = *std::min_element(sum_1dv + 1, sum_1dv + N);
	max1dv_sum = *std::max_element(sum_1dv + 1, sum_1dv + N);

	//=======================
	//	TRIAD
	//=======================	
	
	double triad_1dv[repeat];
	LIKWID_MARKER_START("1D_VIEW_TRIAD");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_triad1dv = std::chrono::steady_clock::now();
#pragma omp simd
	   for(size_t i = 0; i < size1; i++) {
		v4r_1d(i) = 2.0*v1r_1d(i) + v2r_1d(i);
		}
	   auto end_triad1dv = std::chrono::steady_clock::now();
	   std::chrono::duration<double> tot_triad1dv = end_triad1dv - start_triad1dv;
	   triad_1dv[t] = tot_triad1dv.count();
	}
	LIKWID_MARKER_STOP("1D_VIEW_TRIAD");

	//compute average
	double avg_triad_1dv, tot_1dvtriad;
	for(size_t i = 1; i < repeat; i++) {
		tot_1dvtriad += triad_1dv[i];
	}
	avg_triad_1dv = tot_1dvtriad / (double) (repeat-1);
	double min1dv_triad, max1dv_triad;
	min1dv_triad = *std::min_element(triad_1dv + 1, triad_1dv + N);
	max1dv_triad = *std::max_element(triad_1dv + 1, triad_1dv + N);

	//=======================
	//	DOT PRODUCT
	//=======================	

	double dot_1dv[repeat];
	double dotsum1dv = 0.0;
	LIKWID_MARKER_START("1D_VIEW_DOTPRODUCT");
	for(size_t t = 0; t < repeat; t++) {
	   double dot_sum = 0.0;
	   auto start_1dvdot = std::chrono::steady_clock::now();
	   for(size_t i = 0; i < size1; i++) {
		dot_sum += v1r_1d(i)*v2r_1d(i);
	   }
	   dotsum1dv += dot_sum;
	   auto end_1dvdot = std::chrono::steady_clock::now();
	   std::chrono::duration<double>tot_1dvdot = end_1dvdot - start_1dvdot;
	   dot_1dv[t] = tot_1dvdot.count();
	}
	LIKWID_MARKER_STOP("1D_VIEW_DOTPRODUCT");

	//do some random operation so compiler doesnt ignore loop
	double rand3 = dotsum1dv + 1.0;
	std::cout<<"rand3 = "<<rand3<<"\n";

	//compute averages, min, max
	double avg_1dvdot, tot_dot1dv;
	for(size_t i = 1; i < repeat; i++) {
		tot_dot1dv += dot_1dv[i];
	}
	avg_1dvdot = tot_dot1dv / (double) (repeat-1);
	double min1dv_dot, max1dv_dot;
	min1dv_dot = *std::min_element(dot_1dv + 1, dot_1dv + N);
	max1dv_dot = *std::max_element(dot_1dv + 1, dot_1dv + N);

	//table for 1D View

	std::cout<<"=============================================================\n";
	std::cout<<"	1D View Timings Output\n";
	std::cout<<"=============================================================\n";
	
	std::cout<<"Test           Avg.          Min.     Max.   \n";
	std::cout<<"-------------------------------------------\n";
	std::cout<<"Copy         "<<avg_copy_v<<"        "<<min1dv_copy<<"      "<<max1dv_copy<<"\n";
	std::cout<<"Scale        "<<avg_scale1dv<<"      "<<min1dv_scale<<"       "<<max1dv_scale<<"\n";
	std::cout<<"Sum          "<<avg_sum1dv<<"        "<<min1dv_sum<<"        "<<max1dv_sum<<"\n";
	std::cout<<"Triad        "<<avg_triad_1dv<<"     "<<min1dv_triad<<"        "<<max1dv_triad<<"\n";
	std::cout<<"Dot Prod.    "<<avg_1dvdot<<"        "<<min1dv_dot<<"     "<<max1dv_dot<<"\n";
	std::cout<<"=============================================================\n";
	std::cout<<"=============================================================\n";

	std::cout<<"\n";

	// compute average percent difference w.r.t regular array
	// e.g the %difference for the carray (w.r.t regular array)
	// c_copy = ( ( avg_carray - avg_regarray) / avg_regarray ) * 100.0
	// c = carray, v = view
	double c_copy, c_scale, c_sum, c_triad, c_dot;
	double v_copy, v_scale, v_sum, v_triad, v_dot;
	double hund = 100.0;

	//---carray's---
	c_copy = ( (avg_copy_c - avg_copy_r) / avg_copy_r ) * hund;
	c_scale = ( (avg_scale1dc - avg_scale1dr) / avg_scale1dr) * hund;
	c_sum = ( (avg_sum1dc - avg_sum1dr) / avg_sum1dr) * hund;
	c_triad = ( (avg_triad_1dc - avg_triad_1dr) / avg_triad_1dr) * hund;
	c_dot = ( (avg_1dcdot - avg_1drdot) / avg_1drdot) * hund;

	//---views------

	v_copy = ( (avg_copy_v - avg_copy_r) / avg_copy_r ) * hund;
	v_scale = ( (avg_scale1dv - avg_scale1dr) / avg_scale1dr) * hund;
	v_sum = ( (avg_sum1dv - avg_sum1dr) / avg_sum1dr) * hund;
	v_triad = ( (avg_triad_1dv - avg_triad_1dr) / avg_triad_1dr) * hund;
	v_dot = ( (avg_1dvdot - avg_1drdot) / avg_1drdot) * hund;



	std::cout<<"=============================================================\n";
	std::cout<<"	Percent Difference Table for 1D Case\n";
	std::cout<<"=============================================================\n";
	
	std::cout<<"Test           CArray           View\n";
	std::cout<<"-------------------------------------------\n";
	std::cout<<"Copy         "<<c_copy<<"       "<<v_copy<<"\n";
	std::cout<<"Scale        "<<c_scale<<"      "<<v_scale<<"\n";
	std::cout<<"Sum          "<<c_sum<<"        "<<v_sum<<"\n";
	std::cout<<"Triad        "<<c_triad<<"      "<<v_triad<<"\n";
	std::cout<<"Dot Prod.    "<<c_dot<<"        "<<v_dot<<"\n";
	std::cout<<"=============================================================\n";


	//=========================================================================
	//	3D CArray Tests
	//=========================================================================


	//=============================
	//	COPY
	//=============================	

	double carr_copy_3dtimes[repeat]; 
	LIKWID_MARKER_START("3D_CARRAY_COPY");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_copy_c_3d = std::chrono::steady_clock::now();
#pragma omp simd collapse(3)
	   for(size_t i = 0; i < size3; i++) {
		for(size_t j = 0; j < size3; j++) {
		 for(size_t k = 0; k < size3; k++) {
			c_arr2_3d(i,j,k) = c_arr1_3d(i,j,k);
		   }
		  }
		 }
	   auto end_copy_c_3d = std::chrono::steady_clock::now();
	   std::chrono::duration<double> tot_copy_c_3d = end_copy_c_3d - start_copy_c_3d;
	   carr_copy_3dtimes[t] = tot_copy_c_3d.count();
	}
	LIKWID_MARKER_STOP("3D_CARRAY_COPY");

	//calculate average
	double avg_copy_3dc, tot_copy_3dc;
	for(size_t i = 1; i < repeat; i++) {
		tot_copy_3dc += carr_copy_3dtimes[i];
	}
	avg_copy_3dc = tot_copy_3dc / (double) (repeat-1);
	double min3dc_copy, max3dc_copy;
	min3dc_copy = *std::min_element(carr_copy_3dtimes + 1, carr_copy_3dtimes + N);
	max3dc_copy = *std::max_element(carr_copy_3dtimes + 1, carr_copy_3dtimes + N);

	//=============================
	//	SCALE
	//=============================	

	double c_scale3d_times[repeat];
	LIKWID_MARKER_START("3D_CARRAY_SCALE");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_scale3dc = std::chrono::steady_clock::now();
#pragma omp simd collapse(3)
	   for(size_t i = 0; i < size3; i++) {
		for(size_t j = 0; j < size3; j++) {
		 for(size_t k = 0; k < size3; k++) {
			c_arr3_3d(i,j,k) = 2.0*c_arr2_3d(i,j,k);
		   }
		  }
		 }
	   auto end_scale3dc = std::chrono::steady_clock::now();
	   std::chrono::duration<double> tot_scale3dc = end_scale3dc - start_scale3dc;
	   c_scale3d_times[t] = tot_scale3dc.count();
	}
	LIKWID_MARKER_STOP("3D_CARRAY_SCALE");

	//compute average
	double avg_scale3dc, tot_scale_3dc;
	for(size_t i = 1; i < repeat; i++) {
		tot_scale_3dc += c_scale3d_times[i];
	}
	avg_scale3dc = tot_scale_3dc / (double) (repeat-1);
	double min3dc_scale, max3dc_scale;
	min3dc_scale = *std::min_element(c_scale3d_times + 1, c_scale3d_times + N);
	max3dc_scale = *std::max_element(c_scale3d_times + 1, c_scale3d_times + N);
	
	//=============================
	//	SUM
	//=============================	

	double sum_3dc[repeat];
	LIKWID_MARKER_START("3D_CARRAY_SUM");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_sum3dc = std::chrono::steady_clock::now();
#pragma omp simd collapse(3)
	   for(size_t i = 0; i < size3; i++) {
		for(size_t j = 0; j < size3; j++) {
		 for(size_t k = 0; k < size3; k++) {
			c_arr4_3d(i,j,k) = c_arr2_3d(i,j,k) + c_arr1_3d(i,j,k);
		  }
		 }
		}
	   auto end_sum3dc = std::chrono::steady_clock::now();
	   std::chrono::duration<double> tot_3dc_sum = end_sum3dc - start_sum3dc;
	   sum_3dc[t] = tot_3dc_sum.count();
	}
	LIKWID_MARKER_STOP("3D_CARRAY_SUM");

	//compute average
	double avg_sum3dc, tot_sum_3dc;
	for(size_t i = 1; i < repeat; i++) {
		tot_sum_3dc += sum_3dc[i];
	}
	avg_sum3dc = tot_sum_3dc / (double) (repeat-1);
	double min3dc_sum, max3dc_sum;
	min3dc_sum = *std::min_element(sum_3dc + 1, sum_3dc + N);
	max3dc_sum = *std::max_element(sum_3dc + 1, sum_3dc + N);
	std::cout<<"Averape elapsed time for 3D CArray sum = "<<avg_sum3dc<<"s\n";
	std::cout<<"Min 3D Carray sum time = "<<min3dc_sum<<"s\n";
	std::cout<<"Max 3D Carray sum time = "<<max3dc_sum<<"s\n";

	//=============================
	//	TRIAD
	//=============================	

	double triad_3dc[repeat];
	LIKWID_MARKER_START("3D_CARRAY_TRIAD");
	for(size_t t = 0; t < repeat; t++ ) {
	   auto start_triad3dc = std::chrono::steady_clock::now();
#pragma omp simd collapse(3)
	   for(size_t i = 0; i < size3; i++) {
		for(size_t j = 0; j < size3; j++) {
		 for(size_t k = 0; k < size3; k++) {
			c_arr4_3d(i,j,k) = 2.0*c_arr1_3d(i,j,k) + c_arr2_3d(i,j,k);
		   }
		  }
		 }
		auto end_triad3dc = std::chrono::steady_clock::now();
		std::chrono::duration<double> tot_triad3dc = end_triad3dc - start_triad3dc;
		triad_3dc[t] = tot_triad3dc.count();
	}
	LIKWID_MARKER_STOP("3D_CARRAY_TRIAD");
	
	//compute average
	double avg_triad_3dc, tot_3dctriad;
	for(size_t i = 1; i < repeat; i++) {
		tot_3dctriad += triad_3dc[i];
	}
	avg_triad_3dc = tot_3dctriad / (double) (repeat-1);
	double min3dc_triad, max3dc_triad;
	min3dc_triad = *std::min_element(triad_3dc + 1, triad_3dc + N);
	max3dc_triad = *std::max_element(triad_3dc + 1, triad_3dc + N);

	//=============================
	//	DOT PRODUCT
	//=============================	

	double dot_3dc[repeat];
	double dot_3dcsum = 0.0;
	double dotsum3dc = 0.0;
	LIKWID_MARKER_START("3D_CARRAY_DOTPRODUCT");
	for(size_t t = 0; t < repeat; t++ ) {
	   auto start_dot3dc = std::chrono::steady_clock::now();
	   for(size_t i = 0; i < size3; i++) {
		for(size_t j = 0; j < size3; j++) {
		 for(size_t k = 0; k < size3; k++) {
			dotsum3dc += c_arr1_3d(i,j,k) * c_arr2_3d(i,j,k);
		   }
		  }
		 }
		auto end_dot3dc = std::chrono::steady_clock::now();
		std::chrono::duration<double> tot_dot3dc = end_dot3dc - start_dot3dc;
		dot_3dc[t] = tot_dot3dc.count();
		dot_3dcsum  += dotsum3dc;
	}
	LIKWID_MARKER_STOP("3D_CARRAY_DOTPRODUCT");	

	std::cout<<"dot_sum3dc = "<<dot_3dcsum<<"\n";

	//compute average
	double avg_dot_3dc, tot_3dcdot;
	for(size_t i = 1; i < repeat; i++) {
		tot_3dcdot += dot_3dc[i];
	}
	avg_dot_3dc = tot_3dcdot / (double) (repeat-1);
	double min3dc_dot, max3dc_dot;
	min3dc_dot = *std::min_element(dot_3dc + 1, dot_3dc + N);
	max3dc_dot = *std::max_element(dot_3dc + 1, dot_3dc + N);


	//table for 3D CArray output
	std::cout<<"=============================================================\n";
	std::cout<<"	3D CArray Timings Output\n";
	std::cout<<"=============================================================\n";
	
	std::cout<<"Test           Avg.          Min.     Max.   \n";
	std::cout<<"-------------------------------------------\n";
	std::cout<<"Copy         "<<avg_copy_3dc<<"        "<<min3dc_copy<<"      "<<max3dc_copy<<"\n";
	std::cout<<"Scale        "<<avg_scale3dc<<"      "<<min3dc_scale<<"       "<<max3dc_scale<<"\n";
	std::cout<<"Sum          "<<avg_sum3dc<<"        "<<min3dc_sum<<"        "<<max3dc_sum<<"\n";
	std::cout<<"Triad        "<<avg_triad_3dc<<"     "<<min3dc_triad<<"        "<<max3dc_triad<<"\n";
	std::cout<<"Dot Prod.    "<<avg_dot_3dc<<"        "<<min3dc_dot<<"     "<<max3dc_dot<<"\n";
	std::cout<<"=============================================================\n";
	std::cout<<"=============================================================\n";

	std::cout<<"\n";
	std::cout<<"=============================================================\n";


	//=========================================================================
	//	3D TRAD Tests
	//=========================================================================


	//=============================
	//	COPY
	//=============================	

	double reg_copy_3dtimes[repeat]; 
	LIKWID_MARKER_START("3D_REG_COPY");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_copy_r_3d = std::chrono::steady_clock::now();
#pragma omp simd collapse(3)
	   for(size_t i = 0; i < size3; i++) {
		for(size_t j = 0; j < size3; j++) {
		 for(size_t k = 0; k < size3; k++) {
			reg_arr2_3d[i][j][k] = reg_arr1_3d[i][j][k];
		   }
		  }
		 }
	   auto end_copy_r_3d = std::chrono::steady_clock::now();
	   std::chrono::duration<double> tot_copy_r_3d = end_copy_r_3d - start_copy_r_3d;
	   reg_copy_3dtimes[t] = tot_copy_r_3d.count();
	}
	LIKWID_MARKER_STOP("3D_REG_COPY");

	//calculate average
	double avg_copy_3dr, tot_copy_3dr;
	for(size_t i = 1; i < repeat; i++) {
		tot_copy_3dr += reg_copy_3dtimes[i];
	}
	avg_copy_3dr = tot_copy_3dr / (double) (repeat-1);
	double min3dr_copy, max3dr_copy;
	min3dr_copy = *std::min_element(reg_copy_3dtimes + 1, reg_copy_3dtimes + N);
	max3dr_copy = *std::max_element(reg_copy_3dtimes + 1, reg_copy_3dtimes + N);


	//=============================
	//	SCALE
	//=============================	

	double reg_scale3d_times[repeat];
	LIKWID_MARKER_START("3D_REG_SCALE");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_scale3dr = std::chrono::steady_clock::now();
#pragma omp simd collapse(3)
	   for(size_t i = 0; i < size3; i++) {
		for(size_t j = 0; j < size3; j++) {
		 for(size_t k = 0; k < size3; k++) {
			reg_arr3_3d[i][j][k] = 2.0*reg_arr2_3d[i][j][k];
		   }
		  }
		 }
	   auto end_scale3dr = std::chrono::steady_clock::now();
	   std::chrono::duration<double> tot_scale3dr = end_scale3dr - start_scale3dr;
	   reg_scale3d_times[t] = tot_scale3dr.count();
	}
	LIKWID_MARKER_STOP("3D_REG_SCALE");

	//compute average
	double avg_scale3dr, tot_scale_3dr;
	for(size_t i = 1; i < repeat; i++) {
		tot_scale_3dr += reg_scale3d_times[i];
	}
	avg_scale3dr = tot_scale_3dr / (double) (repeat-1);
	double min3dr_scale, max3dr_scale;
	min3dr_scale = *std::min_element(reg_scale3d_times + 1, reg_scale3d_times + N);
	max3dr_scale = *std::max_element(reg_scale3d_times + 1, reg_scale3d_times + N);


	//=============================
	//	SUM
	//=============================	

	double sum_3dr[repeat];
	LIKWID_MARKER_START("3D_REG_SUM");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_sum3dr = std::chrono::steady_clock::now();
#pragma omp simd collapse(3)
	   for(size_t i = 0; i < size3; i++) {
		for(size_t j = 0; j < size3; j++) {
		 for(size_t k = 0; k < size3; k++) {
			reg_arr4_3d[i][j][k] = reg_arr2_3d[i][j][k] + reg_arr1_3d[i][j][k];
		  }
		 }
		}
	   auto end_sum3dr = std::chrono::steady_clock::now();
	   std::chrono::duration<double> tot_3dr_sum = end_sum3dr - start_sum3dr;
	   sum_3dr[t] = tot_3dr_sum.count();
	}
	LIKWID_MARKER_STOP("3D_REG_SUM");

	//compute average
	double avg_sum3dr, tot_sum_3dr;
	for(size_t i = 1; i < repeat; i++) {
		tot_sum_3dr += sum_3dr[i];
	}
	avg_sum3dr = tot_sum_3dr / (double) (repeat-1);
	double min3dr_sum, max3dr_sum;
	min3dr_sum = *std::min_element(sum_3dr + 1, sum_3dr + N);
	max3dr_sum = *std::max_element(sum_3dr + 1, sum_3dr + N);


	//=============================
	//	TRIAD
	//=============================	

	double triad_3dr[repeat];
	LIKWID_MARKER_START("3D_REG_TRIAD");
	for(size_t t = 0; t < repeat; t++ ) {
	   auto start_triad3dr = std::chrono::steady_clock::now();
#pragma omp simd collapse(3)
	   for(size_t i = 0; i < size3; i++) {
		for(size_t j = 0; j < size3; j++) {
		 for(size_t k = 0; k < size3; k++) {
			reg_arr4_3d[i][j][k] = 2.0*reg_arr1_3d[i][j][k] + reg_arr2_3d[i][j][k];
		   }
		  }
		 }
		auto end_triad3dr = std::chrono::steady_clock::now();
		std::chrono::duration<double> tot_triad3dr = end_triad3dr - start_triad3dr;
		triad_3dr[t] = tot_triad3dr.count();
	}
	LIKWID_MARKER_STOP("3D_REG_TRIAD");	

	//compute average
	double avg_triad_3dr, tot_3drtriad;
	for(size_t i = 1; i < repeat; i++) {
		tot_3drtriad += triad_3dr[i];
	}
	avg_triad_3dr = tot_3drtriad / (double) (repeat-1);
	double min3dr_triad, max3dr_triad;
	min3dr_triad = *std::min_element(triad_3dr + 1, triad_3dr + N);
	max3dr_triad = *std::max_element(triad_3dr + 1, triad_3dr + N);


	//=============================
	//	DOT PRODUCT
	//============================	

	double dot_3dr[repeat];
	double dot_3drsum = 0.0;
	double dotsum3dr = 0.0;
	LIKWID_MARKER_START("3D_REG_DOTPRODUCT");
	for(size_t t = 0; t < repeat; t++ ) {
	   auto start_dot3dr = std::chrono::steady_clock::now();
	   for(size_t i = 0; i < size3; i++) {
		for(size_t j = 0; j < size3; j++) {
		 for(size_t k = 0; k < size3; k++) {
			dotsum3dr += reg_arr1_3d[i][j][k] * reg_arr2_3d[i][j][k];
		   }
		  }
		 }
		auto end_dot3dr = std::chrono::steady_clock::now();
		std::chrono::duration<double> tot_dot3dr = end_dot3dr - start_dot3dr;
		dot_3dr[t] = tot_dot3dr.count();
		dot_3drsum  += dotsum3dr;
	}
	LIKWID_MARKER_STOP("3D_REG_DOTPRODUCT");	

	std::cout<<"dot_sum3dr = "<<dot_3drsum<<"\n";

	//compute average
	double avg_dot_3dr, tot_3drdot;
	for(size_t i = 1; i < repeat; i++) {
		tot_3drdot += dot_3dr[i];
	}
	avg_dot_3dr = tot_3drdot / (double) (repeat-1);
	double min3dr_dot, max3dr_dot;
	min3dr_dot = *std::min_element(dot_3dr + 1, dot_3dr + N);
	max3dr_dot = *std::max_element(dot_3dr + 1, dot_3dr + N);


	//table for 3D regular output
	std::cout<<"=============================================================\n";
	std::cout<<"	3D Regular Timings Output\n";
	std::cout<<"=============================================================\n";
	
	std::cout<<"Test           Avg.          Min.     Max.   \n";
	std::cout<<"-------------------------------------------\n";
	std::cout<<"Copy         "<<avg_copy_3dr<<"        "<<min3dr_copy<<"      "<<max3dr_copy<<"\n";
	std::cout<<"Scale        "<<avg_scale3dr<<"      "<<min3dr_scale<<"       "<<max3dr_scale<<"\n";
	std::cout<<"Sum          "<<avg_sum3dr<<"        "<<min3dr_sum<<"        "<<max3dr_sum<<"\n";
	std::cout<<"Triad        "<<avg_triad_3dr<<"     "<<min3dr_triad<<"        "<<max3dr_triad<<"\n";
	std::cout<<"Dot Prod.    "<<avg_dot_3dr<<"        "<<min3dr_dot<<"     "<<max3dr_dot<<"\n";
	std::cout<<"=============================================================\n";
	std::cout<<"=============================================================\n";

	std::cout<<"\n";

	std::cout<<"=============================================================\n";


	//=========================================================================
	//	3D VIEW Tests
	//=========================================================================



	//=======================
	//	COPY
	//	v2r_3d = v1r_3d
	//=======================	
	
	double view_copy_times3d[repeat];	//array to hold times for carray copy
	LIKWID_MARKER_START("3D_VIEW_COPY");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_copy_v_3d = std::chrono::steady_clock::now();
#pragma omp simd collapse(3)	   
	   for(size_t i = 0; i < size3; i++) {
		for(size_t j = 0; j < size3; j++) { 
		 for(size_t k = 0; k < size3; k++) {
			v2r_3d(i,j,k) = v1r_3d(i,j,k);
		   }
		  }
		 }
		auto end_copy_v_3d = std::chrono::steady_clock::now();
		std::chrono::duration<double> tot_copy_v_3d = end_copy_v_3d - start_copy_v_3d;
		view_copy_times3d[t] = tot_copy_v_3d.count();
	}
	LIKWID_MARKER_STOP("3D_VIEW_COPY");


	//calculate average
	double avg_copy_3dv, tot_copy_3dv;
	for(size_t i = 1; i < repeat; i++) {
		tot_copy_3dv += view_copy_times3d[i];
	}	
	avg_copy_3dv = tot_copy_3dv / (double) (repeat-1);
	double min3dv_copy;
	double max3dv_copy;
	min3dv_copy = *std::min_element(view_copy_times3d + 1, view_copy_times3d + N);
	max3dv_copy = *std::max_element(view_copy_times3d + 1, view_copy_times3d + N);

	//=======================
	//	SCALE
	//=======================	
	
	double v_scale3d_times[repeat];
	LIKWID_MARKER_START("3D_VIEW_SCALE");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_scale3dv = std::chrono::steady_clock::now();
#pragma omp simd collapse(3)
	   for(size_t i = 0; i < size3; i++) {
		for(size_t j = 0; j < size3; j++) {
		 for(size_t k = 0; k < size3; k++) {
			 v3r_3d(i,j,k) = 2.0*v2r_3d(i,j,k);
			}
		   }
		  }
	   auto end_scale3dv = std::chrono::steady_clock::now();
	   std::chrono::duration<double> tot_scale3dv = end_scale3dv - start_scale3dv;
	   v_scale3d_times[t] = tot_scale3dv.count();
	}
	LIKWID_MARKER_STOP("3D_VIEW_SCALE");
	
	//compute average, min, max
	double avg_scale3dv, tot_scale_3dv;
	for(size_t i = 1; i < repeat; i++) {
		tot_scale_3dv += v_scale3d_times[i];
	}
	avg_scale3dv = tot_scale_3dv / (double) (repeat-1);
	double min3dv_scale, max3dv_scale;
	min3dv_scale = *std::min_element(v_scale3d_times + 1, v_scale3d_times + N);
	max3dv_scale = *std::max_element(v_scale3d_times + 1, v_scale3d_times + N);

	//=======================
	//	SUM
	//=======================	

	double v_sum3d_times[repeat];
	LIKWID_MARKER_START("3D_VIEW_SUM");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_sum3dv = std::chrono::steady_clock::now();
#pragma omp simd collapse(3)
	   for(size_t i = 0; i < size3; i++) {
		for(size_t j = 0; j < size3; j++) {
		 for(size_t k = 0; k < size3; k++) {
			 v4r_3d(i,j,k) = v2r_3d(i,j,k) + v1r_3d(i,j,k);
			}
		   }
		  }
	   auto end_sum3dv = std::chrono::steady_clock::now();
	   std::chrono::duration<double> tot_sum3dv = end_sum3dv - start_sum3dv;
	   v_sum3d_times[t] = tot_sum3dv.count();
	}
	LIKWID_MARKER_STOP("3D_VIEW_SUM");
	
	//compute average, min, max
	double avg_sum3dv, tot_sum_3dv;
	for(size_t i = 1; i < repeat; i++) {
		tot_sum_3dv += v_sum3d_times[i];
	}
	avg_sum3dv = tot_sum_3dv / (double) (repeat-1);
	double min3dv_sum, max3dv_sum;
	min3dv_sum = *std::min_element(v_sum3d_times + 1, v_sum3d_times + N);
	max3dv_sum = *std::max_element(v_sum3d_times + 1, v_sum3d_times + N);

	//=======================
	//	TRIAD
	//=======================	
	
	double v_triad3d_times[repeat];
	LIKWID_MARKER_START("3D_VIEW_TRIAD");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_triad3dv = std::chrono::steady_clock::now();
#pragma omp simd collapse(3)
	   for(size_t i = 0; i < size3; i++) {
		for(size_t j = 0; j < size3; j++) {
		 for(size_t k = 0; k < size3; k++) {
			 v4r_3d(i,j,k) = v2r_3d(i,j,k) + 2.0*v1r_3d(i,j,k);
			}
		   }
		  }
	   auto end_triad3dv = std::chrono::steady_clock::now();
	   std::chrono::duration<double> tot_triad3dv = end_triad3dv - start_triad3dv;
	   v_triad3d_times[t] = tot_triad3dv.count();
	}
	LIKWID_MARKER_STOP("3D_VIEW_TRIAD");
	
	//compute average, min, max
	double avg_triad3dv, tot_triad_3dv;
	for(size_t i = 1; i < repeat; i++) {
		tot_triad_3dv += v_triad3d_times[i];
	}
	avg_triad3dv = tot_triad_3dv / (double) (repeat-1);
	double min3dv_triad, max3dv_triad;
	min3dv_triad = *std::min_element(v_triad3d_times + 1, v_triad3d_times + N);
	max3dv_triad = *std::max_element(v_triad3d_times + 1, v_triad3d_times + N);

	//=======================
	//	DOT PRODUCT
	//=======================	

	double v_dotp3d_times[repeat];
	double sum_dotp = 0.0;
	LIKWID_MARKER_START("3D_VIEW_DOTPROD");
	for(size_t t = 0; t < repeat; t++) {
	   double temp_dotp = 0.0;
	   auto start_dotp3dv = std::chrono::steady_clock::now();
#pragma omp simd collapse(3)
	   for(size_t i = 0; i < size3; i++) {
		for(size_t j = 0; j < size3; j++) {
		 for(size_t k = 0; k < size3; k++) {
			 temp_dotp += v2r_3d(i,j,k) * v1r_3d(i,j,k);
			}
		   }
		  }
	   sum_dotp += temp_dotp;
	   auto end_dotp3dv = std::chrono::steady_clock::now();
	   std::chrono::duration<double> tot_dotp3dv = end_dotp3dv - start_dotp3dv;
	   v_dotp3d_times[t] = tot_dotp3dv.count();
	}
	LIKWID_MARKER_STOP("3D_VIEW_DOTPROD");

	std::cout<<"Dot product total sum 3d View = "<<sum_dotp<<"\n";
	
	//compute average, min, max
	double avg_dotp3dv, tot_dotp_3dv;
	for(size_t i = 1; i < repeat; i++) {
		tot_dotp_3dv += v_dotp3d_times[i];
	}
	avg_dotp3dv = tot_dotp_3dv / (double) (repeat-1);
	double min3dv_dotp, max3dv_dotp;
	min3dv_dotp = *std::min_element(v_dotp3d_times + 1, v_dotp3d_times + N);
	max3dv_dotp = *std::max_element(v_dotp3d_times + 1, v_dotp3d_times + N);



	//table for 3D view output
	std::cout<<"=============================================================\n";
	std::cout<<"	3D View Timings Output\n";
	std::cout<<"=============================================================\n";
	
	std::cout<<"Test           Avg.          Min.     Max.   \n";
	std::cout<<"-------------------------------------------\n";
	std::cout<<"Copy         "<<avg_copy_3dv<<"        "<<min3dv_copy<<"      "<<max3dv_copy<<"\n";
	std::cout<<"Scale        "<<avg_scale3dv<<"      "<<min3dv_scale<<"       "<<max3dv_scale<<"\n";
	std::cout<<"Sum          "<<avg_sum3dv<<"        "<<min3dv_sum<<"        "<<max3dv_sum<<"\n";
	std::cout<<"Triad        "<<avg_triad3dv<<"     "<<min3dv_triad<<"        "<<max3dv_triad<<"\n";
	std::cout<<"Dot Prod.    "<<avg_dotp3dv<<"        "<<min3dv_dotp<<"     "<<max3dv_dotp<<"\n";
	std::cout<<"=============================================================\n";
	std::cout<<"=============================================================\n";


	// compute average percent difference w.r.t regular array
	// e.g the %difference for the carray (w.r.t regular array)
	// c_copy = ( ( avg_carray - avg_regarray) / avg_regarray ) * 100.0
	// c = carray, v = view
	double c_copy3d, c_scale3d, c_sum3d, c_triad3d, c_dot3d;
	double v_copy3d, v_scale3d, v_sum3d, v_triad3d, v_dot3d;

	//---carray's---
	c_copy3d = ( (avg_copy_3dc - avg_copy_3dr) / avg_copy_3dr ) * hund;
	c_scale3d = ( (avg_scale3dc - avg_scale3dr) / avg_scale3dr) * hund;
	c_sum3d = ( (avg_sum3dc - avg_sum3dr) / avg_sum3dr) * hund;
	c_triad3d = ( (avg_triad_3dc - avg_triad_3dr) / avg_triad_3dr) * hund;
	c_dot3d = ( (avg_dot_3dc - avg_dot_3dr) / avg_dot_3dr) * hund;

	//---views------

	v_copy3d = ( (avg_copy_3dv - avg_copy_3dr) / avg_copy_3dr ) * hund;
	v_scale3d = ( (avg_scale3dv - avg_scale3dr) / avg_scale3dr) * hund;
	v_sum3d = ( (avg_sum3dv - avg_sum3dr) / avg_sum3dr) * hund;
	v_triad3d = ( (avg_triad3dv - avg_triad_3dr) / avg_triad_3dr) * hund;
	v_dot3d = ( (avg_dotp3dv - avg_dot_3dr) / avg_dot_3dr) * hund;


	std::cout<<"=============================================================\n";
	std::cout<<"	Percent Difference Table for 3D Case\n";
	std::cout<<"=============================================================\n";
	
	std::cout<<"Test           CArray           View\n";
	std::cout<<"-------------------------------------------\n";
	std::cout<<"Copy         "<<c_copy3d<<"       "<<v_copy3d<<"\n";
	std::cout<<"Scale        "<<c_scale3d<<"      "<<v_scale3d<<"\n";
	std::cout<<"Sum          "<<c_sum3d<<"        "<<v_sum3d<<"\n";
	std::cout<<"Triad        "<<c_triad3d<<"      "<<v_triad3d<<"\n";
	std::cout<<"Dot Prod.    "<<c_dot3d<<"        "<<v_dot3d<<"\n";
	std::cout<<"=============================================================\n";


	//=========================================================================
	//	RAGGED-RIGHT TESTS
	//=========================================================================	

	std::cout<<"Start ragged right tests!\n";

	// dimensions or rag-right
	const int rows = 64000;

	//create strides array
	std::cout<<"Creating strides CArray...\n";
	//auto strides = CArray<size_t>(rows);
	size_t strides[rows];	

	int temp;
	std::cout<<"Initializing stride array...\n";
	//initialize the strides randomly between 5 and 85k
	for(size_t i = 0; i < rows; i++) {
		temp = ( rand() % 20000 ) + 5;
		strides[i] = temp;
	}


	//statistics on strides array: avg, min, max
	double avg_stride, tot_stride, min_stride, max_stride;
	for(int i = 0; i < rows; i++){
	 tot_stride += strides[i];
	}
	avg_stride = tot_stride / (double)rows;
	min_stride = *std::min_element(strides, strides + rows);
	max_stride = *std::max_element(strides, strides + rows);

	std::cout<<"Average stride length = " <<avg_stride<<".\n";
	std::cout<<"Min stride length = "<<min_stride<<".\n";
	std::cout<<"Max stride length = "<<max_stride<<".\n";
	//create the ragged-right
	//1. the traditional c++ array
	double** reg_rr1 = new double*[rows];
	double** reg_rr2 = new double*[rows];
	double** reg_rr3 = new double*[rows];
	double** reg_rr4 = new double*[rows];
	
	for(size_t i = 0; i < rows; i++) {
		int temp = strides[i];
		reg_rr1[i] = new double[ temp ];
		reg_rr2[i] = new double[ temp ];
		reg_rr3[i] = new double[ temp ];
		reg_rr4[i] = new double[ temp ];
	}

	//2. create MATAR ragged-right
	auto rr1 = RaggedRightArray<double>(strides, rows);
	auto rr2 = RaggedRightArray<double>(strides, rows);
	auto rr3 = RaggedRightArray<double>(strides, rows);
	auto rr4 = RaggedRightArray<double>(strides, rows);

	//3. Initialize only first ragged right for trad and MATAR
	std::cout<<"Initializing ragged-right1s ... \n";
	for(size_t i = 0; i < rows; i++) {
#pragma omp simd
	  for(size_t j = 0; j < strides[i]; j++) {
		rr1(i,j) = 1.0;
		reg_rr1[i][j] = 1.0;
		}
	}
	std::cout<<"finished initializing\n";

	std::cout<<"=============================================================\n";
	std::cout<<"~~~~~~~~~~~~~~~~~~~RAGGED-RIGHT CARRAY TESTS~~~~~~~~~~~~~~~~~\n";
	std::cout<<"=============================================================\n";

	//======================
	//	COPY
	//	rr2 = rr1
	//======================	
	
	std::cout<<"start rr copy!\n";
	double rr_times[repeat];
	LIKWID_MARKER_START("RR_CARRAY_COPY");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_rrcopy = std::chrono::steady_clock::now();
	   for(size_t i = 0; i< rows; i++) {
#pragma omp simd
		for(size_t j = 0; j < rr1.stride(i); j++ ){
			rr2(i,j) = rr1(i,j);
		}
	   }
	   auto end_rrcopy = std::chrono::steady_clock::now();
	   std::chrono::duration<double> tot_rrcopy = end_rrcopy - start_rrcopy;
	   rr_times[t] = tot_rrcopy.count();
	}
	LIKWID_MARKER_STOP("RR_CARRAY_COPY");

	//compute average, max, min
	double avg_rrcopy, tot_copyrr, min_rrcopy, max_rrcopy;
	for(size_t i = 1; i < repeat; i++) {
		tot_copyrr += rr_times[i];
	}
	avg_rrcopy = tot_copyrr / (double) (repeat-1);
	min_rrcopy = *std::min_element(rr_times + 1, rr_times + N);
	max_rrcopy = *std::max_element(rr_times + 1, rr_times + N);
	std::cout<<"Average time for RR copy = "<<avg_rrcopy<<"s\n";
	std::cout<<"Min time for RR copy = "<<min_rrcopy<<"s\n";
	std::cout<<"Max time for RR copy = "<<max_rrcopy<<"s\n";



	//======================
	//	SCALE
	//	r3 = 2*r2
	//======================	

	double rr_scale_times[repeat];
	LIKWID_MARKER_START("RR_CARRAY_SCALE");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_rrscale = std::chrono::steady_clock::now();
	   for(size_t i = 0; i< rows; i++) {
#pragma omp simd
		for(size_t j = 0; j < rr2.stride(i); j++ ){
			rr3(i,j) = 2.0*rr2(i,j);
		}
	   }
	   auto end_rrscale = std::chrono::steady_clock::now();
	   std::chrono::duration<double> tot_rrscale = end_rrscale - start_rrscale;
	   rr_scale_times[t] = tot_rrscale.count();
	}
	LIKWID_MARKER_STOP("RR_CARRAY_SCALE");

	//compute average, max, min
	double avg_rrscale, tot_scale_rr, min_rrscale, max_rrscale;
	for(size_t i = 1; i < repeat; i++) {
		tot_scale_rr += rr_scale_times[i];
	}
	avg_rrscale = tot_scale_rr / (double) (repeat-1);
	min_rrscale = *std::min_element(rr_scale_times + 1, rr_scale_times + N);
	max_rrscale = *std::max_element(rr_scale_times + 1, rr_scale_times + N);
	std::cout<<"Average time for RR scale = "<<avg_rrscale<<"s\n";
	std::cout<<"Min time for RR scale = "<<min_rrscale<<"s\n";
	std::cout<<"Max time for RR scale = "<<max_rrscale<<"s\n";

	//======================
	//	SUM
	//	rr4 = rr2 + rr1
	//======================	

	double rr_sum_times[repeat];
	LIKWID_MARKER_START("RR_CARRAY_SUM");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_rrsum = std::chrono::steady_clock::now();
	   for(size_t i = 0; i< rows; i++) {
#pragma omp simd
		for(size_t j = 0; j < rr1.stride(i); j++ ){
			rr4(i,j) = rr1(i,j) + rr2(i,j);
		}
	   }
	   auto end_rrsum = std::chrono::steady_clock::now();
	   std::chrono::duration<double> tot_rrsum = end_rrsum - start_rrsum;
	   rr_sum_times[t] = tot_rrsum.count();
	}
	LIKWID_MARKER_STOP("RR_CARRAY_SUM");

	//compute average, max, min
	double avg_rrsum, tot_sum_rr, min_rrsum, max_rrsum;
	for(size_t i = 1; i < repeat; i++) {
		tot_sum_rr += rr_sum_times[i];
	}
	avg_rrsum = tot_sum_rr / (double) (repeat-1);
	min_rrsum = *std::min_element(rr_sum_times + 1, rr_sum_times + N);
	max_rrsum = *std::max_element(rr_sum_times + 1, rr_sum_times + N);
	std::cout<<"Average time for RR sum = "<<avg_rrsum<<"s\n";
	std::cout<<"Min time for RR sum = "<<min_rrsum<<"s\n";
	std::cout<<"Max time for RR sum = "<<max_rrsum<<"s\n";

	//======================
	//	TRIAD
	//	rr4 = 2.0*rr1 + rr2
	//======================	

	double rr_triad_times[repeat];
	LIKWID_MARKER_START("RR_CARRAY_TRIAD");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_rrtriad = std::chrono::steady_clock::now();
	   for(size_t i = 0; i< rows; i++) {
#pragma omp simd
		for(size_t j = 0; j < rr1.stride(i); j++ ){
			rr4(i,j) = 2.0*rr1(i,j) + rr2(i,j);
		}
	   }
	   auto end_rrtriad = std::chrono::steady_clock::now();
	   std::chrono::duration<double> tot_rrtriad = end_rrtriad - start_rrtriad;
	   rr_triad_times[t] = tot_rrtriad.count();
	}
	LIKWID_MARKER_STOP("RR_CARRAY_TRIAD");

	//compute average, max, min
	double avg_rrtriad, tot_triad_rr, min_rrtriad, max_rrtriad;
	for(size_t i = 1; i < repeat; i++) {
		tot_triad_rr += rr_triad_times[i];
	}
	avg_rrtriad = tot_triad_rr / (double) (repeat-1);
	min_rrtriad = *std::min_element(rr_triad_times + 1, rr_triad_times + N);
	max_rrtriad = *std::max_element(rr_triad_times + 1, rr_triad_times + N);
	std::cout<<"Average time for RR triad = "<<avg_rrtriad<<"s\n";
	std::cout<<"Min time for RR triad = "<<min_rrtriad<<"s\n";
	std::cout<<"Max time for RR triad = "<<max_rrtriad<<"s\n";
	std::cout<<"=============================================================\n";

	std::cout<<"rr4(1,1) = "<<rr4(1,1)<<"\n";

	std::cout<<"=============================================================\n";
	std::cout<<"~~~~~~~~~~~~~~~~~~~RAGGED-RIGHT REGULAR TESTS~~~~~~~~~~~~~~~~~\n";
	std::cout<<"=============================================================\n";

	//======================
	//	COPY
	//	rr2 = rr1
	//======================	
	
	double reg_rr_times[repeat];
	LIKWID_MARKER_START("RR_REG_COPY");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_reg_rrcopy = std::chrono::steady_clock::now();
	   for(size_t i = 0; i< rows; i++) {
#pragma omp simd
		for(size_t j = 0; j < strides[i]; j++ ){
			reg_rr2[i][j] = reg_rr1[i][j];
		}
	   }
	   auto end_reg_rrcopy = std::chrono::steady_clock::now();
	   std::chrono::duration<double> tot_reg_rrcopy = end_reg_rrcopy - start_reg_rrcopy;
	   reg_rr_times[t] = tot_reg_rrcopy.count();
	}
	LIKWID_MARKER_STOP("RR_REG_COPY");

	//compute average, max, min
	double avg_reg_rrcopy, tot_reg_copyrr, min_reg_rrcopy, max_reg_rrcopy;
	for(size_t i = 1; i < repeat; i++) {
		tot_reg_copyrr += reg_rr_times[i];
	}
	avg_reg_rrcopy = tot_reg_copyrr / (double) (repeat-1);
	min_reg_rrcopy = *std::min_element(reg_rr_times + 1, reg_rr_times + N);
	max_reg_rrcopy = *std::max_element(reg_rr_times + 1, reg_rr_times + N);
	std::cout<<"Average time for trad RR copy = "<<avg_reg_rrcopy<<"s\n";
	std::cout<<"Min time for trad RR copy = "<<min_reg_rrcopy<<"s\n";
	std::cout<<"Max time for trad RR copy = "<<max_reg_rrcopy<<"s\n";



	//======================
	//	SCALE
	//	r3 = 2*r2
	//======================	

	double reg_rr_scale[repeat];
	LIKWID_MARKER_START("RR_REG_SCALE");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_reg_rrscale = std::chrono::steady_clock::now();
	   for(size_t i = 0; i< rows; i++) {
#pragma omp simd
		for(size_t j = 0; j < strides[i]; j++ ){
			reg_rr3[i][j] = 2.0*reg_rr2[i][j];
		}
	   }
	   auto end_reg_rrscale = std::chrono::steady_clock::now();
	   std::chrono::duration<double> tot_reg_rrscale = end_reg_rrscale - start_reg_rrscale;
	   reg_rr_scale[t] = tot_reg_rrscale.count();
	}
	LIKWID_MARKER_STOP("RR_REG_SCALE");

	//compute average, max, min
	double avg_reg_rrscale, tot_reg_scalerr, min_reg_rrscale, max_reg_rrscale;
	for(size_t i = 1; i < repeat; i++) {
		tot_reg_scalerr += reg_rr_scale[i];
	}
	avg_reg_rrscale = tot_reg_scalerr / (double) (repeat-1);
	min_reg_rrscale = *std::min_element(reg_rr_scale + 1, reg_rr_scale + N);
	max_reg_rrscale = *std::max_element(reg_rr_scale + 1, reg_rr_scale + N);
	std::cout<<"Average time for trad RR scale = "<<avg_reg_rrscale<<"s\n";
	std::cout<<"Min time for trad RR scale = "<<min_reg_rrscale<<"s\n";
	std::cout<<"Max time for trad RR scale = "<<max_reg_rrscale<<"s\n";


	//======================
	//	sum
	//	rr4 = rr1 + rr2
	//======================	

	double reg_rr_sum[repeat];
	LIKWID_MARKER_START("RR_REG_SUM");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_reg_rrsum = std::chrono::steady_clock::now();
	   for(size_t i = 0; i< rows; i++) {
#pragma omp simd
		for(size_t j = 0; j < strides[i]; j++ ){
			reg_rr3[i][j] = reg_rr1[i][j] + reg_rr2[i][j];
		}
	   }
	   auto end_reg_rrsum = std::chrono::steady_clock::now();
	   std::chrono::duration<double> tot_reg_rrsum = end_reg_rrsum - start_reg_rrsum;
	   reg_rr_sum[t] = tot_reg_rrsum.count();
	}
	LIKWID_MARKER_STOP("RR_REG_SUM");
	
	//compute average, max, min
	double avg_reg_rrsum, tot_reg_sumrr, min_reg_rrsum, max_reg_rrsum;
	for(size_t i = 1; i < repeat; i++) {
		tot_reg_sumrr += reg_rr_sum[i];
	}
	avg_reg_rrsum = tot_reg_sumrr / (double) (repeat-1);
	min_reg_rrsum = *std::min_element(reg_rr_sum + 1, reg_rr_sum + N);
	max_reg_rrsum = *std::max_element(reg_rr_sum + 1, reg_rr_sum + N);
	std::cout<<"Average time for trad RR sum = "<<avg_reg_rrsum<<"s\n";
	std::cout<<"Min time for trad RR sum = "<<min_reg_rrsum<<"s\n";
	std::cout<<"Max time for trad RR sum = "<<max_reg_rrsum<<"s\n";



	//======================
	//	TRIAD
	//	rr4 = 2.0*rr1 + rr2
	//======================	

	double reg_rr_triad[repeat];
	LIKWID_MARKER_START("RR_REG_TRIAD");
	for(size_t t = 0; t < repeat; t++) {
	   auto start_reg_rrtriad = std::chrono::steady_clock::now();
	   for(size_t i = 0; i< rows; i++) {
#pragma omp simd
		for(size_t j = 0; j < strides[i]; j++ ){
			reg_rr4[i][j] = 2.0*reg_rr1[i][j] + reg_rr2[i][j];
		}
	   }
	   auto end_reg_rrtriad = std::chrono::steady_clock::now();
	   std::chrono::duration<double> tot_reg_rrtriad = end_reg_rrtriad - start_reg_rrtriad;
	   reg_rr_triad[t] = tot_reg_rrtriad.count();
	}
	LIKWID_MARKER_STOP("RR_REG_TRIAD");

	//compute average, max, min
	double avg_reg_rrtriad, tot_reg_triadrr, min_reg_rrtriad, max_reg_rrtriad;
	for(size_t i = 1; i < repeat; i++) {
		tot_reg_triadrr += reg_rr_triad[i];
	}
	avg_reg_rrtriad = tot_reg_triadrr / (double) (repeat-1);
	min_reg_rrtriad = *std::min_element(reg_rr_triad + 1, reg_rr_triad + N);
	max_reg_rrtriad = *std::max_element(reg_rr_triad + 1, reg_rr_triad + N);
	std::cout<<"Average time for trad RR triad = "<<avg_reg_rrtriad<<"s\n";
	std::cout<<"Min time for trad RR triad = "<<min_reg_rrtriad<<"s\n";
	std::cout<<"Max time for trad RR triad = "<<max_reg_rrtriad<<"s\n";


	std::cout<<"=============================================================\n";


	std::cout<<"reg_rr4(1,1) = "<<reg_rr4[1][1]<<"\n";

	std::cout<<"==========DONE WITH STREAMBENCHMARK CPU TESTS!!!=============\n";








	std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~GPU  TESTS~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

    // Kokkos GPU test
   
    Kokkos::initialize();
    {

    using policy2D = Kokkos::MDRangePolicy< Kokkos::Rank<2> >;
    policy2D array_type = policy2D({0,0}, {size_i, size_j});
    policy2D matrix_type = policy2D({1,1}, {size_i+1, size_j+1});
    policy2D splice_type = policy2D({0,0}, {size_j, size_k});
    using policy3D = Kokkos::MDRangePolicy< Kokkos::Rank<3> >;
    policy3D array_type3 = policy3D({0,0,0}, {size_i, size_j, size_k});
    policy3D matrix_type3 = policy3D({1,1,1}, {size_i+1, size_j+1, size_k+1});
    
    /*------------------------- Stream Benchmarks ----------------------------------------*/

    // Kokkos stream benchmark test (for FMatrixKokkos and CArrayKokkos objects)
    printf("\n Kokkos stream benchmark CArrayKokkos\n");
    
    double scalar = 3.0;
    size_t nsize = 64 * 64 * 64 * 64;

    auto arr1 = CArrayKokkos <real_t> (nsize);
    auto arr2 = CArrayKokkos <real_t> (nsize);
    auto arr3 = CArrayKokkos <real_t> (nsize);

    printf("Stream benchmark with %d elements.\n", nsize);
    
    Kokkos::parallel_for("Initialize", nsize, KOKKOS_LAMBDA(const int i) {
            arr1(i) = 1.0;
            arr2(i) = 2.0;
            arr3(i) = 0.0;
            });
    Kokkos::fence();
   
    // Perform copy benchmark
    auto begin = std::chrono::high_resolution_clock::now();
     
    Kokkos::parallel_for("Copy", nsize, KOKKOS_LAMBDA(const int i) {
            arr3(i) = arr1(i);
            });
    Kokkos::fence();
    

    std::chrono::duration<double> copy_time = std::chrono::high_resolution_clock::now() - begin;

    std::cout << "Copy time: " << copy_time.count() << " s." << std::endl;

    // Perform scaling benchmark
    begin = std::chrono::high_resolution_clock::now();
    
    Kokkos::parallel_for("Scale", nsize, KOKKOS_LAMBDA(const int i) {
            arr2(i) = scalar * arr3(i);
            });
    Kokkos::fence();
    
    std::chrono::duration<double> scale_time = std::chrono::high_resolution_clock::now() - begin;

    std::cout << "Scale time: " << scale_time.count() << " s." << std::endl;

    // Perform sum benchmark
    begin = std::chrono::high_resolution_clock::now();
    
    Kokkos::parallel_for("Sum", nsize, KOKKOS_LAMBDA(const int i) {
            arr3(i) = arr1(i) + arr2(i);
            });
    Kokkos::fence();
    
    std::chrono::duration<double> sum_time = std::chrono::high_resolution_clock::now() - begin; 

    std::cout << "Sum time: " << sum_time.count() << " s." << std::endl;

    // Perform triad benchmark
    begin = std::chrono::high_resolution_clock::now();
    
    Kokkos::parallel_for("Triad", nsize, KOKKOS_LAMBDA(const int i) {
            arr1(i) = arr2(i) + 
                                  (scalar * arr3(i));
            });
    Kokkos::fence();
    
    std::chrono::duration<double> triad_time = std::chrono::high_resolution_clock::now() - begin;

    std::cout << "Triad time: " << triad_time.count() << " s." << std::endl;


    // Perform matrix matrix multiply benchmark

	//for some reason, with likwid markers mmm gets hung up.
	//comment out for now
	
	/*
    int matrix_size = 64 * 64;
    auto mat1 = CArrayKokkos <real_t> (matrix_size, matrix_size);
    auto mat2 = CArrayKokkos <real_t> (matrix_size, matrix_size);
    auto mat3 = CArrayKokkos <real_t> (matrix_size, matrix_size);

    policy2D mmm_type = policy2D({0,0}, {matrix_size,matrix_size});
    Kokkos::parallel_for("MatrixInit", mmm_type, KOKKOS_LAMBDA(const int i, const int j) {
            mat1(i, j) = (real_t) (i + 1) * (j + 1);
            mat2(i, j) = (real_t) (i + 2) * (j + 2);
            //printf("Mat1 (%d, %d) %lf\n", i, j, mat1(i, j));
            //printf("Mat2 (%d, %d) %lf\n", i, j, mat2(i, j));
        });
    Kokkos::fence();
	

    begin = std::chrono::high_resolution_clock::now();

    Kokkos::parallel_for ("RaggedDownTeam", TeamPolicy(matrix_size, Kokkos::AUTO), KOKKOS_LAMBDA (const TeamPolicy::member_type &teamMember) {
            const int i = teamMember.league_rank();

            Kokkos::parallel_for (Kokkos::TeamThreadRange (teamMember, matrix_size), [=] (const int j) {
                double temp_var = 0.0;

                Kokkos::parallel_reduce (Kokkos::ThreadVectorRange (teamMember, matrix_size), [=] (const int k, double &mat_val) {
                    mat_val += mat1(i, k) * mat2(k, j);
                }, temp_var);

                mat3(i, j) = temp_var;
                //printf("Mat3 (%d, %d) %lf\n", i, j, mat3(i, j));
            });
        });
    
    Kokkos::fence();
    
    std::chrono::duration<double> mmm_time = std::chrono::high_resolution_clock::now() - begin;

    std::cout << "MMM time: " << mmm_time.count() << " s." << std::endl;

	*/

    
    }
    Kokkos::finalize();

	LIKWID_MARKER_CLOSE;

    printf("--- finished ---\n");

    return 0;
}
