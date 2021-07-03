#ifndef NRM2_H
#define NRM2_H

#include <complex>

extern "C" {
float snrm2(const int N, const float *X, const int INCX);

double dnrm2(const int N, const double *X, const int INCX);

float scnrm2(const int N, const std::complex<float> *X, const int INCX);

double dznrm2(const int N, const std::complex<double> *X, const int INCX);
}

#endif
