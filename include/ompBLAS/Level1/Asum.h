#ifndef ASUM_H
#define ASUM_H

#include <complex>

extern "C" {

float sasum(const int N, const float *X, const int INCX);

double dasum(const int N, const double *X, const int INCX);

float scasum(const int N, const std::complex<float> *X, const int INCX);

double dzasum(const int N, const std::complex<double> *X, const int INCX);

}

#endif
