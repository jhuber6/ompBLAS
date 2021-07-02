#include "ompBLAS/Level1/Iamax.h"

#include <limits>

namespace {

template <typename T> T abs1(const std::complex<T> &x) {
  return std::abs(std::real(x)) + std::abs(std::imag(x));
}

template <typename T> T minimum() { return std::numeric_limits<T>::min(); }

template <typename IndexType, typename T>
IndexType iamax(IndexType N, const T *X, const IndexType INCX) {
  std::pair<IndexType, T> AbsMax = {0, std::abs(X[0])};

#pragma omp declare reduction(iamax : std::pair<IndexType, T> :                \
    omp_out = omp_in.second > omp_out.second ? omp_in : omp_out)               \
  initializer(omp_priv = {minimum<IndexType>(), minimum<T>()})
#pragma omp target teams distribute parallel for map(to : X [0:N])             \
    reduction(iamax : AbsMax)
  for (IndexType i = 0; i < N; ++i) {
    if (std::abs(X[i * INCX]) > AbsMax.second) {
      AbsMax.second = std::abs(X[i * INCX]);
      AbsMax.first = i;
    }
  }

  return AbsMax.first + 1;
}

template <typename IndexType, typename T>
IndexType iamax(IndexType N, const std::complex<T> *X, const IndexType INCX) {
  std::pair<IndexType, T> AbsMax = {0, abs1(X[0])};

#pragma omp declare reduction(iamax : std::pair<IndexType, T> :                \
    omp_out = omp_in.second > omp_out.second ? omp_in : omp_out)               \
  initializer(omp_priv = {minimum<IndexType>(), minimum<T>()})
#pragma omp target teams distribute parallel for map(to : X [0:N])             \
    reduction(iamax : AbsMax)
  for (IndexType i = 0; i < N; ++i) {
    if (abs1(X[i * INCX]) > AbsMax.second) {
      AbsMax.second = abs1(X[i * INCX]);
      AbsMax.first = i;
    }
  }

  return AbsMax.first + 1;
}

} // namespace

extern "C" {

int isamax(const int N, const float *X, const int INCX) {
  return iamax(N, X, INCX);
}

int idamax(const int N, const double *X, const int INCX) {
  return iamax(N, X, INCX);
}

int icamax(const int N, const std::complex<float> *X, const int INCX) {
  return iamax(N, X, INCX);
}

int izamax(const int N, const std::complex<double> *X, const int INCX) {
  return iamax(N, X, INCX);
}

}
