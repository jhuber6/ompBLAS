#include "ompBLAS/Level1/Copy.h"

namespace {

template <typename IndexType, typename T>
void copy(const IndexType N, const T *X, const IndexType INCX, T *Y,
          const IndexType INCY) {
#pragma omp target teams distribute parallel for                               \
  map(to : X [0:N]) map(tofrom : Y [0:N])
  for (IndexType i = 0; i < N; ++i)
    Y[i * INCY] = X[i * INCX];
}

} // namespace

extern "C" {

void scopy(const int N, const float *X, const int INCX, float *Y,
           const int INCY) {
  copy(N, X, INCX, Y, INCY);
}

void dcopy(const int N, const double *X, const int INCX, double *Y,
           const int INCY) {
  copy(N, X, INCX, Y, INCY);
}

void ccopy(const int N, const std::complex<float> *X, const int INCX,
           std::complex<float> *Y, const int INCY) {
  copy(N, X, INCX, Y, INCY);
}

void zcopy(const int N, const std::complex<double> *X, const int INCX,
           std::complex<double> *Y, const int INCY) {
  copy(N, X, INCX, Y, INCY);
}
}
