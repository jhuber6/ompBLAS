#include "ompBLAS/Level1/Asum.h"

#include <complex>
#include <cmath>

namespace {

template <typename IndexType, typename T>
T asum(const IndexType N, const T *X, const IndexType INCX) {
  T Sum = 0.0;

#pragma omp target teams distribute parallel for map(to : X[0 : N]) \
  reduction(+ : Sum)
  for (IndexType i = 0; i < N; ++i)
    Sum += std::abs(X[i * INCX]);

  return Sum;
}

template <typename IndexType, typename T>
T asum(const IndexType N, const std::complex<T> *X, const IndexType INCX) {
  T Sum = 0.0;

#pragma omp target teams distribute parallel for map(to : X[0 : N]) \
  reduction(+ : Sum)
  for (IndexType i = 0; i < N; ++i)
    Sum += std::abs(X[i * INCX].real()) + std::abs(X[i * INCX].imag());

  return Sum;
}

}

extern "C" {

float sasum(const int N, const float *X, const int INCX) {
  return asum(N, X, INCX);
}

double dasum(const int N, const double *X, const int INCX) {
  return asum(N, X, INCX);
}

float scasum(const int N, const std::complex<float> *X, const int INCX) {
  return asum(N, X, INCX);
}

double dzasum(const int N, const std::complex<double> *X, const int INCX) {
  return asum(N, X, INCX);
}

}
