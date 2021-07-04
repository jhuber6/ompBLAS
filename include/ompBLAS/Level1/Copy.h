#ifndef COPY_H
#define COPY_H

#include <complex>

extern "C" {

void scopy(const int N, const float *X, const int INCX, float *Y,
           const int INCY);

void dcopy(const int N, const double *X, const int INCX, double *Y,
           const int INCY);

void ccopy(const int N, const std::complex<float> *X, const int INCX,
           std::complex<float> *Y, const int INCY);

void zcopy(const int N, const std::complex<double> *X, const int INCX,
           std::complex<double> *Y, const int INCY);
}
#endif
