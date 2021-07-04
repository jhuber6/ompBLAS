#ifndef SCAL_H
#define SCAL_H

#include <complex>

extern "C" {

void sscal(const int N, const float Alpha, float *X, const int INCX);

void dscal(const int N, const double Alpha, double *X, const int INCX);

void cscal(const int N, const std::complex<float> Alpha, std::complex<float> *X,
           const int INCX);

void zscal(const int N, const std::complex<double> Alpha,
           std::complex<double> *X, const int INCX);
}

#endif
