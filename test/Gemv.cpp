#include "Common.h"
#include "ompBLAS/ompBLAS.h"

extern "C" {
void sgemv_(const char *Trans, const int *M, const int *N, const float *Alpha,
            const float *A, const int *LDA, const float *X, const int *INCX,
            const float *Beta, float *Y, const int *INCY);

void dgemv_(const char *Trans, const int *M, const int *N, const double *Alpha,
            const double *A, const int *LDA, const double *X, const int *INCX,
            const double *Beta, double *Y, const int *INCY);

void cgemv_(const char *Trans, const int *M, const int *N,
            const std::complex<float> *Alpha, const std::complex<float> *A,
            const int *LDA, const std::complex<float> *X, const int *INCX,
            const std::complex<float> *Beta, std::complex<float> *Y,
            const int *INCY);

void zgemv_(const char *Trans, const int *M, const int *N,
            const std::complex<double> *Alpha, const std::complex<double> *A,
            const int *LDA, const std::complex<double> *X, const int *INCX,
            const std::complex<double> *Beta, std::complex<double> *Y,
            const int *INCY);
}

template <typename T, typename F, typename G>
void randomStimulus(F &&Ref, G &&Test) {
  int M = irand() % 256 + 256;
  int N = irand() % 256 + 256;
  int INCX = 1;
  int INCY = 1;
  T alpha = T(2.0) * random<T>() - T(1.0);
  T beta = T(2.0) * random<T>() - T(1.0);

  std::vector<T> A = getRandomVector<T>(M * N);
  std::vector<T> X = getRandomVector<T>(N);
  std::vector<T> Y = getRandomVector<T>(M);

  std::vector<T> YBlas = Y;
  std::vector<T> YTest = Y;
  Ref("N", &M, &N, &alpha, A.data(), &M, X.data(), &INCX, &beta, YBlas.data(),
      &INCX);
  Test(Order::ColMajor, Transpose::NoTrans, M, N, alpha, A.data(), M, X.data(),
       INCX, beta, YTest.data(), INCY);

  checkResult(YBlas, YTest);

  A = getRandomVector<T>(N * M);
  X = getRandomVector<T>(M);
  Y = getRandomVector<T>(N);

  YBlas = Y;
  YTest = Y;
  Ref("T", &M, &N, &alpha, A.data(), &M, X.data(), &INCX, &beta, YBlas.data(),
      &INCX);
  Test(Order::ColMajor, Transpose::Trans, M, N, alpha, A.data(), M, X.data(),
       INCX, beta, YTest.data(), INCY);

  checkResult(YBlas, YTest);

  A = getRandomVector<T>(N * M);
  X = getRandomVector<T>(M);
  Y = getRandomVector<T>(N);

  YBlas = Y;
  YTest = Y;
  Ref("C", &M, &N, &alpha, A.data(), &M, X.data(), &INCX, &beta, YBlas.data(),
      &INCX);
  Test(Order::ColMajor, Transpose::ConjTrans, M, N, alpha, A.data(), M,
       X.data(), INCX, beta, YTest.data(), INCY);

  checkResult(YBlas, YTest);
}

int main() {
  rnd.seed(std::random_device{}());
  printf("Testing DGEMV...\n");
  randomStimulus<double>(dgemv_, dgemv);
  printf("Pass!\n");
  printf("Testing SGEMV...\n");
  randomStimulus<float>(sgemv_, sgemv);
  printf("Pass!\n");
  printf("Testing ZGEMV...\n");
  randomStimulus<std::complex<double>>(zgemv_, zgemv);
  printf("Pass!\n");
  printf("Testing CGEMV...\n");
  randomStimulus<std::complex<float>>(cgemv_, cgemv);
  printf("Pass!\n");
  return 0;
}
