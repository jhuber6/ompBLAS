#ifndef SWAP_H
#define SWAP_H

#include <complex>

extern "C" {

void sswap(const int N, float *X, const int INCX, float *Y, const int INCY);

void dswap(const int N, double *X, const int INCX, double *Y, const int INCY);

void cswap(const int N, std::complex<float> *X, const int INCX,
           std::complex<float> *Y, const int INCY);

void zswap(const int N, std::complex<double> *X, const int INCX,
           std::complex<double> *Y, const int INCY);
}
#endif
