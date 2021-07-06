#ifndef GEMM_H
#define GEMM_H

#include "ompBLAS/Common.h"
#include <complex>

extern "C" {

void sgemm(const Order Ordering, const Transpose TransA, const Transpose TransB,
           const int M, const int N, const int K, const float Alpha,
           const float *A, const int LDA, const float *B, const int LDB,
           const float Beta, float *C, const int LDC);

void dgemm(const Order Ordering, const Transpose TransA, const Transpose TransB,
           const int M, const int N, const int K, const double Alpha,
           const double *A, const int LDA, const double *B, const int LDB,
           const double Beta, double *C, const int LDC);

void cgemm(const Order Ordering, const Transpose TransA, const Transpose TransB,
           const int M, const int N, const int K,
           const std::complex<float> Alpha, const std::complex<float> *A,
           const int LDA, const std::complex<float> *B, const int LDB,
           const std::complex<float> Beta, std::complex<float> *C,
           const int LDC);

void zgemm(const Order Ordering, const Transpose TransA, const Transpose TransB,
           const int M, const int N, const int K,
           const std::complex<double> Alpha, const std::complex<double> *A,
           const int LDA, const std::complex<double> *B, const int LDB,
           const std::complex<double> Beta, std::complex<double> *C,
           const int LDC);
}

#endif
