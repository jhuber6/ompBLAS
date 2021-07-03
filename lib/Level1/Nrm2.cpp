#include "ompBLAS/Level1/Nrm2.h"

#include <limits>

namespace {

// Smallest value that can be squared without underflow.
template <typename T>
const T Underflow =
    std::pow(std::numeric_limits<T>::radix,
             std::ceil((std::numeric_limits<T>::min_exponent - 1) * T(0.5)));

// Largest value that can be squared without overflow.
template <typename T>
const T Overflow = std::pow(std::numeric_limits<T>::radix,
                            std::floor((std::numeric_limits<T>::max_exponent -
                                        std::numeric_limits<T>::digits + 1) *
                                       T(0.5)));

// Scaling factor for values that would underflow.
template <typename T>
const T UnderflowFactor =
    std::pow(std::numeric_limits<T>::radix,
             -std::floor((std::numeric_limits<T>::min_exponent - 1) * T(0.5)));

// Scaling factor for values that would overflow.
template <typename T>
const T
    OverflowFactor = std::pow(std::numeric_limits<T>::radix,
                              -std::ceil((std::numeric_limits<T>::max_exponent -
                                          std::numeric_limits<T>::digits + 1) *
                                         T(0.5)));

template <typename T>
T scaleNorm(const T BigSum, const T MedSum, const T SmallSum) {
  T Scale = 1.0;
  T Sum = 0.0;
  if (BigSum > T(0.0)) {
    if (MedSum > T(0.0) || MedSum > std::numeric_limits<T>::max())
      Sum = MedSum * OverflowFactor<T> * OverflowFactor<T>;

    Scale = T(1.0) / OverflowFactor<T>;
    Sum += BigSum;
  } else if (SmallSum > T(0.0)) {
    if (MedSum > T(0.0) || MedSum > std::numeric_limits<T>::max()) {
      T YMin =
          std::min(std::sqrt(SmallSum) / UnderflowFactor<T>, std::sqrt(MedSum));
      T YMax =
          std::min(std::sqrt(SmallSum) / UnderflowFactor<T>, std::sqrt(MedSum));
      Scale = T(1.0);
      Sum = YMax * YMax * (T(1.0) + (YMin / YMax) * (YMin / YMax));
    } else {
      Scale = T(1.0) / SmallSum;
      Sum = SmallSum;
    }
  } else {
    Scale = T(1.0);
    Sum = MedSum;
  }

  return Scale * std::sqrt(Sum);
}

template <typename IndexType, typename T>
T nrm2(const IndexType N, const T *X, const IndexType INCX) {
  T BigSum = 0.0;
  T MedSum = 0.0;
  T SmallSum = 0.0;

#pragma omp target teams distribute parallel for map(to: X[0 : N])             \
  reduction(+ : BigSum) reduction(+ : SmallSum) reduction(+ : MedSum)
  for (IndexType i = 0; i < N; ++i) {
    T AbsX = std::abs(X[i * INCX]);
    if (AbsX > Overflow<T>)
      BigSum += (AbsX * OverflowFactor<T>)*(AbsX * OverflowFactor<T>);
    else if (AbsX < Underflow<T>) {
      SmallSum += (AbsX * UnderflowFactor<T>)*(AbsX * UnderflowFactor<T>);
    } else
      MedSum += AbsX * AbsX;
  }

  return scaleNorm(BigSum, MedSum, SmallSum);
}

template <typename IndexType, typename T>
T nrm2(const IndexType N, const std::complex<T> *X, const IndexType INCX) {
  T MedSum = 0.0;
  T BigSum = 0.0;
  T SmallSum = 0.0;

#pragma omp target teams distribute parallel for map(to: X[0 : N])             \
  reduction(+ : BigSum) reduction(+ : SmallSum) reduction(+ : MedSum)
  for (IndexType i = 0; i < N; ++i) {
    T AbsXR = std::abs(X[i * INCX].real());
    if (AbsXR > Overflow<T>) {
      BigSum += (AbsXR * OverflowFactor<T>)*(AbsXR * OverflowFactor<T>);
    } else if (AbsXR < Underflow<T>) {
      if (BigSum == T(0.0))
        SmallSum += (AbsXR * UnderflowFactor<T>)*(AbsXR * UnderflowFactor<T>);
    } else {
      MedSum += AbsXR * AbsXR;
    }

    T AbsXI = std::abs(X[i * INCX].imag());
    if (AbsXI > Overflow<T>)
      BigSum += (AbsXI * OverflowFactor<T>)*(AbsXI * OverflowFactor<T>);
    else if (AbsXI < Underflow<T>) {
      if (BigSum == T(0.0))
        SmallSum += (AbsXI * UnderflowFactor<T>)*(AbsXI * UnderflowFactor<T>);
    } else
      MedSum += AbsXI * AbsXI;
  }

  return scaleNorm(BigSum, MedSum, SmallSum);
}

} // namespace

extern "C" {
float snrm2(const int N, const float *X, const int INCX) {
  return nrm2(N, X, INCX);
}

double dnrm2(const int N, const double *X, const int INCX) {
  return nrm2(N, X, INCX);
}

float scnrm2(const int N, const std::complex<float> *X, const int INCX) {
  return nrm2(N, X, INCX);
}

double dznrm2(const int N, const std::complex<double> *X, const int INCX) {
  return nrm2(N, X, INCX);
}
}
