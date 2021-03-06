#include "Common.h"
#include "ompBLAS/ompBLAS.h"

extern "C" {

int isamax_(const int *N, const float *X, const int *INCX);
int idamax_(const int *N, const double *X, const int *INCX);
int icamax_(const int *N, const std::complex<float> *X, const int *INCX);
int izamax_(const int *N, const std::complex<double> *X, const int *INCX);
}

template <typename T, typename F, typename G>
void randomStimulus(F &&Ref, G &&Test) {
  int N = irand() % 1024 + 1024;
  int M = irand() % 16 + 16;
  int INCX = 1;
  std::vector<T> X = getRandomVector<T>(N);

  int RefIdx = Ref(&N, X.data(), &INCX);
  int TestIdx = Test(N, X.data(), INCX);

  if (RefIdx != TestIdx) {
    printf("*** FAILURE ***\nBLAS: %d\nompBLAS: %d\n", RefIdx, TestIdx);
    exit(1);
  }

  INCX = M;
  X = getRandomVector<T>(N * M);

  RefIdx = Ref(&N, X.data(), &INCX);
  TestIdx = Test(N, X.data(), INCX);

  if (RefIdx != TestIdx) {
    printf("*** FAILURE ***\nBLAS: %d\nompBLAS: %d\n", RefIdx, TestIdx);
    exit(1);
  }
}

int main() {
  rnd.seed(std::random_device{}());
  printf("Testing IDAMAX...\n");
  randomStimulus<double>(idamax_, idamax);
  printf("Pass!\n");
  printf("Testing ISAMAX...\n");
  randomStimulus<float>(isamax_, isamax);
  printf("Pass!\n");
  printf("Testing IZAMAX...\n");
  randomStimulus<std::complex<double>>(izamax_, izamax);
  printf("Pass!\n");
  printf("Testing ICAMAX...\n");
  randomStimulus<std::complex<float>>(icamax_, icamax);
  printf("Pass!\n");
}
