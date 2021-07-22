#include "ompBLAS/Level1/Scal.h"

namespace {

template <typename IndexType, typename T>
void scal(const IndexType N, const T Alpha, T *X, const IndexType INCX) {
#pragma omp target teams distribute parallel for map(tofrom : X[0:N * INCX])
  for (IndexType i = 0; i < N; ++i)
    X[i * INCX] = Alpha * X[i * INCX];
}

} // namespace

extern "C" {

void sscal(const int N, const float Alpha, float *X, const int INCX) {
  scal(N, Alpha, X, INCX);
}

void dscal(const int N, const double Alpha, double *X, const int INCX) {
  scal(N, Alpha, X, INCX);
}

void cscal(const int N, const std::complex<float> Alpha, std::complex<float> *X,
           const int INCX) {
  scal(N, Alpha, X, INCX);
}

void zscal(const int N, const std::complex<double> Alpha,
           std::complex<double> *X, const int INCX) {
  scal(N, Alpha, X, INCX);
}
}
