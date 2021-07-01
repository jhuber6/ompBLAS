#include "ompBLAS/ompBLAS.h"
#include "Common.h"

extern "C" {
void dgemm_(const char *transa, const char *transb, const int *m, const int *n,
            const int *k, const double *alpha, const double *a, const int *lda,
            const double *b, const int *ldb, const double *beta, double *c,
            const int *ldc);
void sgemm_(const char *transa, const char *transb, const int *m, const int *n,
            const int *k, const float *alpha, const float *a, const int *lda,
            const float *b, const int *ldb, const float *beta, float *c,
            const int *ldc);
void cgemm_(const char *transa, const char *transb, const int *m, const int *n,
            const int *k, const void *alpha, const void *a, const int *lda,
            const void *b, const int *ldb, const void *beta, void *c,
            const int *ldc);
void zgemm_(const char *transa, const char *transb, const int *m, const int *n,
            const int *k, const void *alpha, const void *a, const int *lda,
            const void *b, const int *ldb, const void *beta, void *c,
            const int *ldc);
}

template <typename T, typename F, typename G>
void randomStimulus(F &&Ref, G &&Test) {
  int M = irand() % 256;
  int N = irand() % 256;
  int K = irand() % 256;
  T alpha = T(2.0) * random<T>() - T(1.0);
  T beta = T(2.0) * random<T>() - T(1.0);

  std::vector<T> A = getRandomVector<T>(M * K);
  std::vector<T> B = getRandomVector<T>(K * N);
  std::vector<T> C = getRandomVector<T>(M * N);

  std::vector<T> CBlas = C;
  std::vector<T> CTest = C;
  Ref("N", "N", &M, &N, &K, &alpha, A.data(), &M, B.data(), &K, &beta,
      CBlas.data(), &M);
  Test(Order::ColMajor, Transpose::NoTrans, Transpose::NoTrans, M, N, K, alpha,
       A.data(), M, B.data(), K, beta, CTest.data(), M);

  checkResult(CBlas, CTest);

  A = getRandomVector<T>(M * M);
  B = getRandomVector<T>(M * M);
  C = getRandomVector<T>(M * M);

  CBlas = C;
  CTest = C;
  Ref("T", "T", &M, &M, &M, &alpha, A.data(), &M, B.data(), &M, &beta,
      CBlas.data(), &M);
  Test(Order::ColMajor, Transpose::Trans, Transpose::Trans, M, M, M, alpha,
       A.data(), M, B.data(), M, beta, CTest.data(), M);

  checkResult(CBlas, CTest);
}

int main(int argc, char **argv) {
  rnd.seed(std::random_device{}());
  printf("Testing DGEMM...\n");
  randomStimulus<double>(dgemm_, dgemm);
  printf("Pass!\n");
  printf("Testing SGEMM...\n");
  randomStimulus<float>(sgemm_, sgemm);
  printf("Pass!\n");
  printf("Testing ZGEMM...\n");
  randomStimulus<std::complex<double>>(zgemm_, zgemm);
  printf("Pass!\n");
  printf("Testing CGEMM...\n");
  randomStimulus<std::complex<float>>(cgemm_, cgemm);
  printf("Pass!\n");
  return 0;
}
