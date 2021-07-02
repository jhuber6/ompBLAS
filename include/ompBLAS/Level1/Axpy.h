#ifndef AXYPY_H
#define AXYPY_H

#include <complex>

extern "C" {

void saxpy(const int N, const float Alpha, const float *X, const int INCX,
           float *Y, const int INCY);

void daxpy(const int N, const double Alpha, const double *X, const int INCX,
           double *Y, const int INCY);

void caxpy(const int N, const std::complex<float> Alpha,
           const std::complex<float> *X, const int INCX, std::complex<float> *Y,
           const int INCY);

void zaxpy(const int N, const std::complex<double> Alpha,
           const std::complex<double> *X, const int INCX,
           std::complex<double> *Y, const int INCY);
}

#endif
