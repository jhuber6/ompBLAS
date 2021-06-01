#include <omp.h>
#include "ompBLAS/Gemm.h"
#include <complex>
#include <cstdlib>

namespace {

template <typename IndexType, typename T>
void gemm(const IndexType M, const IndexType N, const IndexType K,
          const T Alpha, const T *A, const IndexType LDAC, const IndexType LDAR,
          const T *B, const IndexType LDBC, const IndexType LDBR, const T Beta,
          T *C, const IndexType LDCC, const IndexType LDCR) {

  int isHost;
#pragma omp target map(from: isHost)
  { isHost = omp_is_initial_device(); }
  if (isHost)
    exit(2);
#pragma omp target teams distribute parallel for collapse(2) \
  map(to: A[0 : M * K])\
  map(to: B[0 : K * N])\
  map(tofrom: C[0 : M * N])
  for (IndexType i = 0; i < M; ++i) {
    for (IndexType j = 0; j < N; ++j) {
      T Sum = 0.0;

      for (IndexType k = 0; k < K; ++k)
        Sum += A[i * LDAR + k * LDAC] * B[k * LDBR + j * LDBC];
      C[i * LDCR + j * LDCC] = (Alpha * Sum) + (Beta * C[i * LDCR + j * LDCC]);
    }
  }
}

} // namespace

extern "C" {

void sgemm(const Order Ordering, const Transpose TransA, const Transpose TransB,
           const int M, const int N, const int K, const float Alpha,
           const float *A, const int LDA, const float *B, const int LDB,
           const float Beta, float *C, const int LDC) {

  if (Ordering == Order::ColMajor) {
    if (TransA == Transpose::NoTrans && TransB == Transpose::NoTrans)
      gemm(M, N, K, Alpha, A, LDA, 1, B, LDB, 1, Beta, C, LDC, 1);
    else if (TransA == Transpose::Trans && TransB == Transpose::NoTrans)
      gemm(M, N, K, Alpha, A, 1, LDA, B, LDB, 1, Beta, C, LDC, 1);
    else if (TransA == Transpose::NoTrans && TransB == Transpose::Trans)
      gemm(M, N, K, Alpha, A, LDA, 1, B, 1, LDB, Beta, C, LDC, 1);
    else
      gemm(M, N, K, Alpha, A, 1, LDA, B, 1, LDB, Beta, C, LDC, 1);
  } else if (Ordering == Order::RowMajor) {
    if (TransA == Transpose::NoTrans && TransB == Transpose::NoTrans)
      gemm(M, N, K, Alpha, A, 1, LDA, B, 1, LDB, Beta, C, 1, LDC);
    else if (TransA == Transpose::Trans && TransB == Transpose::NoTrans)
      gemm(M, N, K, Alpha, A, LDA, 1, B, 1, LDB, Beta, C, 1, LDC);
    else if (TransA == Transpose::NoTrans && TransB == Transpose::Trans)
      gemm(M, N, K, Alpha, A, 1, LDA, B, LDB, 1, Beta, C, 1, LDC);
    else
      gemm(M, N, K, Alpha, A, LDA, 1, B, LDB, 1, Beta, C, 1, LDC);
  }
}

void dgemm(const Order Ordering, const Transpose TransA, const Transpose TransB,
           const int M, const int N, const int K, const double Alpha,
           const double *A, const int LDA, const double *B, const int LDB,
           const double Beta, double *C, const int LDC) {

  if (Ordering == Order::ColMajor) {
    if (TransA == Transpose::NoTrans && TransB == Transpose::NoTrans)
      gemm(M, N, K, Alpha, A, LDA, 1, B, LDB, 1, Beta, C, LDC, 1);
    else if (TransA == Transpose::Trans && TransB == Transpose::NoTrans)
      gemm(M, N, K, Alpha, A, 1, LDA, B, LDB, 1, Beta, C, LDC, 1);
    else if (TransA == Transpose::NoTrans && TransB == Transpose::Trans)
      gemm(M, N, K, Alpha, A, LDA, 1, B, 1, LDB, Beta, C, LDC, 1);
    else
      gemm(M, N, K, Alpha, A, 1, LDA, B, 1, LDB, Beta, C, LDC, 1);
  } else if (Ordering == Order::RowMajor) {
    if (TransA == Transpose::NoTrans && TransB == Transpose::NoTrans)
      gemm(M, N, K, Alpha, A, 1, LDA, B, 1, LDB, Beta, C, 1, LDC);
    else if (TransA == Transpose::Trans && TransB == Transpose::NoTrans)
      gemm(M, N, K, Alpha, A, LDA, 1, B, 1, LDB, Beta, C, 1, LDC);
    else if (TransA == Transpose::NoTrans && TransB == Transpose::Trans)
      gemm(M, N, K, Alpha, A, 1, LDA, B, LDB, 1, Beta, C, 1, LDC);
    else
      gemm(M, N, K, Alpha, A, LDA, 1, B, LDB, 1, Beta, C, 1, LDC);
  }
}

void cgemm(const Order Ordering, const Transpose TransA, const Transpose TransB,
           const int M, const int N, const int K, const void *Alpha,
           const void *A, const int LDA, const void *B, const int LDB,
           const void *Beta, void *C, const int LDC) {
  const auto Alpha_ = *reinterpret_cast<const std::complex<float> *>(Alpha);
  const auto Beta_ = *reinterpret_cast<const std::complex<float> *>(Beta);
  const auto *A_ = reinterpret_cast<const std::complex<float> *>(A);
  const auto *B_ = reinterpret_cast<const std::complex<float> *>(B);
  auto *C_ = reinterpret_cast<std::complex<float> *>(C);

  if (Ordering == Order::ColMajor) {
    if (TransA == Transpose::NoTrans && TransB == Transpose::NoTrans)
      gemm(M, N, K, Alpha_, A_, LDA, 1, B_, LDB, 1, Beta_, C_, LDC, 1);
    else if (TransA == Transpose::Trans && TransB == Transpose::NoTrans)
      gemm(M, N, K, Alpha_, A_, 1, LDA, B_, LDB, 1, Beta_, C_, LDC, 1);
    else if (TransA == Transpose::NoTrans && TransB == Transpose::Trans)
      gemm(M, N, K, Alpha_, A_, LDA, 1, B_, 1, LDB, Beta_, C_, LDC, 1);
    else
      gemm(M, N, K, Alpha_, A_, 1, LDA, B_, 1, LDB, Beta_, C_, LDC, 1);
  } else if (Ordering == Order::RowMajor) {
    if (TransA == Transpose::NoTrans && TransB == Transpose::NoTrans)
      gemm(M, N, K, Alpha_, A_, 1, LDA, B_, 1, LDB, Beta_, C_, 1, LDC);
    else if (TransA == Transpose::Trans && TransB == Transpose::NoTrans)
      gemm(M, N, K, Alpha_, A_, LDA, 1, B_, 1, LDB, Beta_, C_, 1, LDC);
    else if (TransA == Transpose::NoTrans && TransB == Transpose::Trans)
      gemm(M, N, K, Alpha_, A_, 1, LDA, B_, LDB, 1, Beta_, C_, 1, LDC);
    else
      gemm(M, N, K, Alpha_, A_, LDA, 1, B_, LDB, 1, Beta_, C_, 1, LDC);
  }
}

void zgemm(const Order Ordering, const Transpose TransA, const Transpose TransB,
           const int M, const int N, const int K, const void *Alpha,
           const void *A, const int LDA, const void *B, const int LDB,
           const void *Beta, void *C, const int LDC) {
  const auto Alpha_ = *reinterpret_cast<const std::complex<double> *>(Alpha);
  const auto Beta_ = *reinterpret_cast<const std::complex<double> *>(Beta);
  const auto *A_ = reinterpret_cast<const std::complex<double> *>(A);
  const auto *B_ = reinterpret_cast<const std::complex<double> *>(B);
  auto *C_ = reinterpret_cast<std::complex<double> *>(C);

  if (Ordering == Order::ColMajor) {
    if (TransA == Transpose::NoTrans && TransB == Transpose::NoTrans)
      gemm(M, N, K, Alpha_, A_, LDA, 1, B_, LDB, 1, Beta_, C_, LDC, 1);
    else if (TransA == Transpose::Trans && TransB == Transpose::NoTrans)
      gemm(M, N, K, Alpha_, A_, 1, LDA, B_, LDB, 1, Beta_, C_, LDC, 1);
    else if (TransA == Transpose::NoTrans && TransB == Transpose::Trans)
      gemm(M, N, K, Alpha_, A_, LDA, 1, B_, 1, LDB, Beta_, C_, LDC, 1);
    else
      gemm(M, N, K, Alpha_, A_, 1, LDA, B_, 1, LDB, Beta_, C_, LDC, 1);
  } else if (Ordering == Order::RowMajor) {
    if (TransA == Transpose::NoTrans && TransB == Transpose::NoTrans)
      gemm(M, N, K, Alpha_, A_, 1, LDA, B_, 1, LDB, Beta_, C_, 1, LDC);
    else if (TransA == Transpose::Trans && TransB == Transpose::NoTrans)
      gemm(M, N, K, Alpha_, A_, LDA, 1, B_, 1, LDB, Beta_, C_, 1, LDC);
    else if (TransA == Transpose::NoTrans && TransB == Transpose::Trans)
      gemm(M, N, K, Alpha_, A_, 1, LDA, B_, LDB, 1, Beta_, C_, 1, LDC);
    else
      gemm(M, N, K, Alpha_, A_, LDA, 1, B_, LDB, 1, Beta_, C_, 1, LDC);
  }
}
}
