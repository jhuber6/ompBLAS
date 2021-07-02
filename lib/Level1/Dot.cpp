#include "ompBLAS/Level1/Dot.h"

namespace {

float conj(const float x) { return x; }
double conj(const double x) { return x; }
std::complex<float> conj(const std::complex<float> x) { return std::conj(x); }
std::complex<double> conj(const std::complex<double> x) { return std::conj(x); }

template <bool Conj, typename IndexType, typename T>
T dot(const IndexType N, const T *X, const IndexType INCX, const T *Y,
      const IndexType INCY) {
  T Sum = T(0.0);

#pragma omp target teams distribute parallel for                               \
  reduction(+ : Sum) map(to: X[0 : N]) map(to: Y[0 : N])
  for (IndexType i = 0; i < N; ++i) {
    if (Conj)
      Sum += conj(X[i * INCX]) * Y[i * INCY];
    else
      Sum += X[i * INCX] * Y[i * INCY];
  }

  return Sum;
}

} // namespace

extern "C" {

float sdot(const int N, const float *X, const int INCX, const float *Y,
           const int INCY) {
  return dot<false>(N, X, INCX, Y, INCY);
}

double ddot(const int N, const double *X, const int INCX, const double *Y,
            const int INCY) {
  return dot<false>(N, X, INCX, Y, INCY);
}

std::complex<float> cdotu(const int N, const std::complex<float> *X,
                          const int INCX, const std::complex<float> *Y,
                          const int INCY) {
  return dot<false>(N, X, INCX, Y, INCY);
}

std::complex<double> zdotu(const int N, const std::complex<double> *X,
                           const int INCX, const std::complex<double> *Y,
                           const int INCY) {
  return dot<false>(N, X, INCX, Y, INCY);
}

std::complex<float> cdotc(const int N, const std::complex<float> *X,
                          const int INCX, const std::complex<float> *Y,
                          const int INCY) {
  return dot<true>(N, X, INCX, Y, INCY);
}

std::complex<double> zdotc(const int N, const std::complex<double> *X,
                           const int INCX, const std::complex<double> *Y,
                           const int INCY) {
  return dot<true>(N, X, INCX, Y, INCY);
}
}
