#include "ompBLAS/Level2/Gemv.h"

namespace {

float conj(const float x) { return x; }
double conj(const double x) { return x; }
std::complex<float> conj(const std::complex<float> x) { return std::conj(x); }
std::complex<double> conj(const std::complex<double> x) { return std::conj(x); }

template <typename T> void atomicSet(T *Y, const T Alpha, const T Sum) {
#pragma omp atomic
  *Y += Alpha * Sum;
}

template <typename T>
void atomicSet(std::complex<T> *Y, const std::complex<T> Alpha,
               const std::complex<T> Sum) {
  const std::complex<T> X = Alpha * Sum;
  T *YPtr = reinterpret_cast<T *>(Y);

#pragma omp atomic
  YPtr[0] += X.real();

#pragma omp atomic
  YPtr[1] += X.imag();
}

template <int MC, int NC, typename IndexType, typename T>
void gemv(const IndexType M, const IndexType N, const T Alpha, const T *A,
          const IndexType LDAC, const IndexType LDAR, const T *X,
          const IndexType INCX, const T Beta, T *Y, const IndexType INCY,
          const bool Conj = false) {

#pragma omp target teams distribute parallel for map(tofrom : Y [0:M])
  for (IndexType i0 = 0; i0 < M; ++i0)
    Y[i0 * INCY] = Beta * Y[i0 * INCY];

#pragma omp target teams distribute collapse(2) map(to : A [0:M * N])          \
    map(to : X [0:N]) map(tofrom : Y [0:M])
  for (IndexType i0 = 0; i0 < M; i0 += MC) {
    for (IndexType j0 = 0; j0 < N; j0 += NC) {
      T sX[NC];

#pragma omp parallel firstprivate(Beta, Alpha, i0, j0, M, N, INCX, INCY, LDAC, \
                                  LDAR, X, Y, A, Conj) shared(sX)
      {
        const IndexType MR = i0 + MC <= M ? MC : M % MC;
        const IndexType NR = j0 + NC <= N ? NC : N % NC;

#pragma omp for nowait
        for (IndexType j1 = 0; j1 < NC; ++j1)
          sX[j1] = (j0 + j1 < N) ? X[(j0 + j1) * INCX] : T(0.0);

#pragma omp barrier

#pragma omp for nowait
        for (IndexType i1 = 0; i1 < MR; ++i1) {
          T Sum = T(0.0);
          for (IndexType j1 = 0; j1 < NR; ++j1) {
            T Ax = (Conj) ? conj(A[(i0 + i1) * LDAR + (j0 + j1) * LDAC])
                          : A[(i0 + i1) * LDAR + (j0 + j1) * LDAC];
            Sum += sX[j1] * Ax;
          }

          atomicSet(&Y[(i0 + i1) * INCY], Alpha, Sum);
        }
      }
    }
  }
}

template <int MC, int NC, typename IndexType, typename T>
void gemvImpl(const Order Ordering, const Transpose Trans, const IndexType M,
              const IndexType N, const T Alpha, const T *A, const IndexType LDA,
              const T *X, const IndexType INCX, const T Beta, T *Y,
              const IndexType INCY) {
  if (Ordering == Order::ColMajor) {
    if (Trans == Transpose::NoTrans)
      gemv<MC, NC>(M, N, Alpha, A, LDA, 1, X, INCX, Beta, Y, INCY);
    else if (Trans == Transpose::Trans)
      gemv<MC, NC>(N, M, Alpha, A, 1, LDA, X, INCX, Beta, Y, INCY);
    else
      gemv<MC, NC>(N, M, Alpha, A, 1, LDA, X, INCX, Beta, Y, INCY, true);
  } else if (Ordering == Order::RowMajor) {
    if (Trans == Transpose::NoTrans)
      gemv<MC, NC>(M, N, Alpha, A, 1, LDA, X, INCX, Beta, Y, INCY);
    else if (Trans == Transpose::Trans)
      gemv<MC, NC>(N, M, Alpha, A, LDA, 1, X, INCX, Beta, Y, INCY);
    else
      gemv<MC, NC>(N, M, Alpha, A, LDA, 1, X, INCX, Beta, Y, INCY, true);
  }
}

} // namespace

extern "C" {

void sgemv(const Order Ordering, const Transpose Trans, const int M,
           const int N, const float Alpha, const float *A, const int LDA,
           const float *X, const int INCX, const float Beta, float *Y,
           const int INCY) {
  gemvImpl<128, 256>(Ordering, Trans, M, N, Alpha, A, LDA, X, INCX, Beta, Y,
                     INCY);
}

void dgemv(const Order Ordering, const Transpose Trans, const int M,
           const int N, const double Alpha, const double *A, const int LDA,
           const double *X, const int INCX, const double Beta, double *Y,
           const int INCY) {
  gemvImpl<128, 256>(Ordering, Trans, M, N, Alpha, A, LDA, X, INCX, Beta, Y,
                     INCY);
}

void cgemv(const Order Ordering, const Transpose Trans, const int M,
           const int N, const std::complex<float> Alpha,
           const std::complex<float> *A, const int LDA,
           const std::complex<float> *X, const int INCX,
           const std::complex<float> Beta, std::complex<float> *Y,
           const int INCY) {
  gemvImpl<128, 256>(Ordering, Trans, M, N, Alpha, A, LDA, X, INCX, Beta, Y,
                     INCY);
}

void zgemv(const Order Ordering, const Transpose Trans, const int M,
           const int N, const std::complex<double> Alpha,
           const std::complex<double> *A, const int LDA,
           const std::complex<double> *X, const int INCX,
           const std::complex<double> Beta, std::complex<double> *Y,
           const int INCY) {
  gemvImpl<128, 256>(Ordering, Trans, M, N, Alpha, A, LDA, X, INCX, Beta, Y,
                     INCY);
}
}
