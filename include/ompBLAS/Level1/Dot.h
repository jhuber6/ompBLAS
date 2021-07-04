#ifndef DOT_H
#define DOT_H

#include <complex>

extern "C" {

float sdot(const int N, const float *X, const int INCX, const float *Y,
           const int INCY);

double ddot(const int N, const double *X, const int INCX, const double *Y,
            const int INCY);

std::complex<float> cdotu(const int N, const std::complex<float> *X,
                          const int INCX, const std::complex<float> *Y,
                          const int INCY);

std::complex<double> zdotu(const int N, const std::complex<double> *X,
                           const int INCX, const std::complex<double> *Y,
                           const int INCY);

std::complex<float> cdotc(const int N, const std::complex<float> *X,
                          const int INCX, const std::complex<float> *Y,
                          const int INCY);

std::complex<double> zdotc(const int N, const std::complex<double> *X,
                           const int INCX, const std::complex<double> *Y,
                           const int INCY);

double sdsdot(const int N, const float SB, const float *X, const int INCX,
              const float *Y, const int INCY);
}
#endif
