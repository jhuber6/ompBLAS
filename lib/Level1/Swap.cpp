#include "ompBLAS/Level1/Swap.h"

namespace {

template <typename IndexType, typename T>
void swap(const IndexType N, T *X, IndexType INCX, T *Y, const IndexType INCY) {
#pragma omp target teams distribute parallel for                               \
  map(tofrom : X [0:N]) map(tofrom : Y [0:N])
  for (IndexType i = 0; i < N; ++i) {
    std::swap(X[i * INCX], Y[i * INCY]);
  }
}

} // namespace

extern "C" {

void sswap(const int N, float *X, const int INCX, float *Y, const int INCY) {
  swap(N, X, INCX, Y, INCY);
}

void dswap(const int N, double *X, const int INCX, double *Y, const int INCY) {
  swap(N, X, INCX, Y, INCY);
}

void cswap(const int N, std::complex<float> *X, const int INCX,
           std::complex<float> *Y, const int INCY) {
  swap(N, X, INCX, Y, INCY);
}

void zswap(const int N, std::complex<double> *X, const int INCX,
           std::complex<double> *Y, const int INCY) {
  swap(N, X, INCX, Y, INCY);
}
}
