#include "ompBLAS/Gemm.h"
#include "Common.h"

#include <cstdlib>

extern "C" {
void dgemm_(const char *transa, const char *transb, const int *m, const int *n,
            const int *k, const double *alpha, const double *a, const int *lda,
            const double *b, const int *ldb, const double *beta, double *c,
            const int *ldc);
}

void randomStimulus() {
  int M = irand() % 400 + 100;
  int N = irand() % 400 + 100;
  int K = irand() % 400 + 100;
  double alpha = 2.0 * drand() - 1.0;
  double beta = 2.0 * drand() - 1.0;

  std::vector<double> A(M * K);
  std::vector<double> B(K * N);
  std::vector<double> C(M * N);

  std::generate(A.begin(), A.end(), []() { return 2.0 * drand() - 1.0; });
  std::generate(B.begin(), B.end(), []() { return 2.0 * drand() - 1.0; });
  std::generate(C.begin(), C.end(), []() { return 2.0 * drand() - 1.0; });

  auto CBlas = C;
  auto CTest = C;
  dgemm_("N", "N", &M, &N, &K, &alpha, A.data(), &M, B.data(), &K, &beta,
         CBlas.data(), &M);
  dgemm(Order::ColMajor, Transpose::NoTrans, Transpose::NoTrans, M, N, K, alpha,
        A.data(), M, B.data(), K, beta, CTest.data(), M);

  if (!(getError(CBlas, CTest) < 1.0E-12))
    exit(1);
}

void checkTranspose() {
  int M = irand() % 50 + 50;
  double alpha = 2.0 * drand() - 1.0;
  double beta = 2.0 * drand() - 1.0;

  std::vector<double> A(M * M);
  std::vector<double> B(M * M);
  std::vector<double> C(M * M);

  std::generate(A.begin(), A.end(), []() { return 2.0 * drand() - 1.0; });
  std::generate(B.begin(), B.end(), []() { return 2.0 * drand() - 1.0; });
  std::generate(C.begin(), C.end(), []() { return 2.0 * drand() - 1.0; });

  auto CBlas = C;
  auto CTest = C;
  dgemm_("T", "T", &M, &M, &M, &alpha, A.data(), &M, B.data(), &M, &beta,
         CBlas.data(), &M);
  dgemm(Order::ColMajor, Transpose::Trans, Transpose::Trans, M, M, M, alpha,
        A.data(), M, B.data(), M, beta, CTest.data(), M);

  if (!(getError(CBlas, CTest) < 1.0E-12))
    exit(1);
}

int main(int argc, char **argv) {
  rnd.seed(std::random_device{}());
  randomStimulus();
  checkTranspose();
  return 0;
}
