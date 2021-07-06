#include "ompBLAS/Common.h"

#include <complex>

extern "C" {

void sgemv(const Order Ordering, const Transpose Trans, const int M,
           const int N, const float Alpha, const float *A, const int LDA,
           const float *X, const int INCX, const float Beta, float *Y,
           const int INCY);

void dgemv(const Order Ordering, const Transpose Trans, const int M,
           const int N, const double Alpha, const double *A, const int LDA,
           const double *X, const int INCX, const double Beta, double *Y,
           const int INCY);

void cgemv(const Order Ordering, const Transpose Trans, const int M,
           const int N, const std::complex<float> Alpha,
           const std::complex<float> *A, const int LDA,
           const std::complex<float> *X, const int INCX,
           const std::complex<float> Beta, std::complex<float> *Y,
           const int INCY);

void zgemv(const Order Ordering, const Transpose Trans, const int M,
           const int N, const std::complex<double> Alpha,
           const std::complex<double> *A, const int LDA,
           const std::complex<double> *X, const int INCX,
           const std::complex<double> Beta, std::complex<double> *Y,
           const int INCY);
}
