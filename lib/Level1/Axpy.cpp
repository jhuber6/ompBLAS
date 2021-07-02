#include "ompBLAS/Level1/Axpy.h"

namespace {

template <typename IndexType, typename T>
void axpy(const IndexType N, const T Alpha, const T *X, const IndexType INCX,
          T *Y, const IndexType INCY) {
#pragma omp target teams distribute parallel for map(to : X [0:N])             \
  map(tofrom : Y [0:N]) firstprivate(X)
  for (IndexType i = 0; i < N; ++i)
    Y[i * INCY] = Alpha * X[i * INCX] + Y[i * INCY];
}

} // namespace

extern "C" {

void saxpy(const int N, const float Alpha, const float *X, const int INCX,
           float *Y, const int INCY) {
  axpy(N, Alpha, X, INCX, Y, INCY);
}

void daxpy(const int N, const double Alpha, const double *X, const int INCX,
           double *Y, const int INCY) {
  axpy(N, Alpha, X, INCX, Y, INCY);
}

void caxpy(const int N, const std::complex<float> Alpha,
           const std::complex<float> *X, const int INCX, std::complex<float> *Y,
           const int INCY) {
  axpy(N, Alpha, X, INCX, Y, INCY);
}

void zaxpy(const int N, const std::complex<double> Alpha,
           const std::complex<double> *X, const int INCX,
           std::complex<double> *Y, const int INCY) {
  axpy(N, Alpha, X, INCX, Y, INCY);
}
}
