#include "ompBLAS/Level3/Gemm.h"

namespace {

template <int MC, int NC, int KC, int MR, int NR, typename IndexType,
          typename T>
void gemm(const IndexType M, const IndexType N, const IndexType K,
          const T Alpha, const T *A, const IndexType LDAC, const IndexType LDAR,
          const T *B, const IndexType LDBC, const IndexType LDBR, const T Beta,
          T *C, const IndexType LDCC, const IndexType LDCR) {

#pragma omp target teams distribute collapse(2)                                \
  map(to: A [0:M * K])                                                         \
  map(to: B [0:K * N])                                                         \
  map(tofrom: C [0:M * N])                                                     \
  firstprivate(Alpha) firstprivate(Beta)
  for (IndexType i0 = 0; i0 < M; i0 += MC) {
    for (IndexType j0 = 0; j0 < N; j0 += NC) {
      T sA[MC][KC];
      T sB[KC][NC];

      const IndexType MB = (i0 + MC <= M) ? MC : M % MC;
      const IndexType NB = (j0 + NC <= N) ? NC : N % NC;

#pragma omp parallel firstprivate(i0, j0, A, B, C, Alpha, Beta, MB, NB, LDAR,  \
                                  LDAC, LDBR, LDBC, LDCR, LDCC, M, N, K)       \
    shared(sA, sB)
      for (IndexType k0 = 0; k0 < K; k0 += KC) {
        const T Beta_ = (k0 == 0) ? Beta : T(1.0);
        const IndexType KB = (k0 + KC <= K) ? KC : K % KC;

#pragma omp barrier

#pragma omp for collapse(2) nowait
        for (IndexType i1 = 0; i1 < MC; ++i1)
          for (IndexType k1 = 0; k1 < KC; ++k1)
            sA[i1][k1] = (i1 + i0 < M && k0 + k1 < K)
                             ? A[(i0 + i1) * LDAR + (k0 + k1) * LDAC]
                             : T(0.0);

#pragma omp for collapse(2) nowait
        for (IndexType k1 = 0; k1 < KC; ++k1)
          for (IndexType j1 = 0; j1 < NC; ++j1)
            sB[k1][j1] = (k0 + k1 < K && j0 + j1 < N)
                             ? B[(k0 + k1) * LDBR + (j0 + j1) * LDBC]
                             : T(0.0);

#pragma omp barrier

#pragma omp for collapse(2) nowait
        for (IndexType i1 = 0; i1 < MB; i1 += MR) {
          for (IndexType j1 = 0; j1 < NB; j1 += NR) {
            const IndexType MS = (i1 + MR <= MB) ? MR : MB % MR;
            const IndexType NS = (j1 + NR <= NB) ? NR : NB % NR;
            T rAB[MR][NR];
            T rA[MR];
            T rB[NR];

            for (IndexType i2 = 0; i2 < MR; ++i2)
              for (IndexType j2 = 0; j2 < NR; ++j2)
                rAB[i2][j2] = T(0.0);

            for (IndexType k1 = 0; k1 < KB; ++k1) {
              for (IndexType i2 = 0; i2 < MR; ++i2)
                rA[i2] = sA[i1 + i2][k1];

              for (IndexType j2 = 0; j2 < NR; ++j2)
                rB[j2] = sB[k1][j1 + j2];

              for (IndexType i2 = 0; i2 < MR; ++i2)
                for (IndexType j2 = 0; j2 < NR; ++j2)
                  rAB[i2][j2] += rA[i2] * rB[j2];
            }

            for (IndexType i2 = 0; i2 < MS; ++i2)
              for (IndexType j2 = 0; j2 < NS; ++j2)
                C[(i0 + i1 + i2) * LDCR + (j0 + j1 + j2) * LDCC] =
                    Alpha * rAB[i2][j2] +
                    Beta_ * C[(i0 + i1 + i2) * LDCR + (j0 + j1 + j2) * LDCC];
          } 
        }
      }
    }
  }
}

template <int MC, int NC, int KC, int MR, int NR, typename IndexType,
          typename T>
void gemmImpl(const Order Ordering, const Transpose TransA,
              const Transpose TransB, const IndexType M, const IndexType N,
              const IndexType K, const T Alpha, const T *A, const IndexType LDA,
              const T *B, const IndexType LDB, const T Beta, T *C,
              const IndexType LDC) {

  // TODO: Support conjugate.
  if (Ordering == Order::ColMajor) {
    if (TransA == Transpose::NoTrans && TransB == Transpose::NoTrans)
      gemm<MC, NC, KC, MR, NR>(M, N, K, Alpha, A, LDA, 1, B, LDB, 1, Beta, C,
                               LDC, 1);
    else if (TransA == Transpose::Trans && TransB == Transpose::NoTrans)
      gemm<MC, NC, KC, MR, NR>(M, N, K, Alpha, A, 1, LDA, B, LDB, 1, Beta, C,
                               LDC, 1);
    else if (TransA == Transpose::NoTrans && TransB == Transpose::Trans)
      gemm<MC, NC, KC, MR, NR>(M, N, K, Alpha, A, LDA, 1, B, 1, LDB, Beta, C,
                               LDC, 1);
    else
      gemm<MC, NC, KC, MR, NR>(M, N, K, Alpha, A, 1, LDA, B, 1, LDB, Beta, C,
                               LDC, 1);
  } else if (Ordering == Order::RowMajor) {
    if (TransA == Transpose::NoTrans && TransB == Transpose::NoTrans)
      gemm<MC, NC, KC, MR, NR>(M, N, K, Alpha, A, 1, LDA, B, 1, LDB, Beta, C, 1,
                               LDC);
    else if (TransA == Transpose::Trans && TransB == Transpose::NoTrans)
      gemm<MC, NC, KC, MR, NR>(M, N, K, Alpha, A, LDA, 1, B, 1, LDB, Beta, C, 1,
                               LDC);
    else if (TransA == Transpose::NoTrans && TransB == Transpose::Trans)
      gemm<MC, NC, KC, MR, NR>(M, N, K, Alpha, A, 1, LDA, B, LDB, 1, Beta, C, 1,
                               LDC);
    else
      gemm<MC, NC, KC, MR, NR>(M, N, K, Alpha, A, LDA, 1, B, LDB, 1, Beta, C, 1,
                               LDC);
  }
}

} // namespace

extern "C" {

void sgemm(const Order Ordering, const Transpose TransA, const Transpose TransB,
           const int M, const int N, const int K, const float Alpha,
           const float *A, const int LDA, const float *B, const int LDB,
           const float Beta, float *C, const int LDC) {
  gemmImpl<48, 32, 64, 8, 4>(Ordering, TransA, TransB, M, N, K, Alpha, A, LDA,
                             B, LDB, Beta, C, LDC);
}

void dgemm(const Order Ordering, const Transpose TransA, const Transpose TransB,
           const int M, const int N, const int K, const double Alpha,
           const double *A, const int LDA, const double *B, const int LDB,
           const double Beta, double *C, const int LDC) {
  gemmImpl<48, 32, 64, 4, 2>(Ordering, TransA, TransB, M, N, K, Alpha, A, LDA,
                             B, LDB, Beta, C, LDC);
}

void cgemm(const Order Ordering, const Transpose TransA, const Transpose TransB,
           const int M, const int N, const int K,
           const std::complex<float> Alpha, const std::complex<float> *A,
           const int LDA, const std::complex<float> *B, const int LDB,
           const std::complex<float> Beta, std::complex<float> *C,
           const int LDC) {
  gemmImpl<48, 48, 48, 4, 2>(Ordering, TransA, TransB, M, N, K, Alpha, A, LDA,
                             B, LDB, Beta, C, LDC);
}

void zgemm(const Order Ordering, const Transpose TransA, const Transpose TransB,
           const int M, const int N, const int K,
           const std::complex<double> Alpha, const std::complex<double> *A,
           const int LDA, const std::complex<double> *B, const int LDB,
           const std::complex<double> Beta, std::complex<double> *C,
           const int LDC) {
  gemmImpl<24, 16, 32, 2, 2>(Ordering, TransA, TransB, M, N, K, Alpha, A, LDA,
                             B, LDB, Beta, C, LDC);
}
}
