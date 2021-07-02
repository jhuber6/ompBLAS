#ifndef IAMAX_H
#define IAMAX_H

#include <complex>

extern "C" {

int isamax(const int N, const float *X, const int INCX);

int idamax(const int N, const double *X, const int INCX);

int icamax(const int N, const std::complex<float> *X, const int INCX);

int izamax(const int N, const std::complex<double> *X, const int INCX);
}

#endif
