#include "Common.h"
#include "ompBLAS/ompBLAS.h"

extern "C" {
float snrm2_(const int *N, const float *X, const int *INCX);

double dnrm2_(const int *N, const double *X, const int *INCX);

float scnrm2_(const int *N, const std::complex<float> *X, const int *INCX);

double dznrm2_(const int *N, const std::complex<double> *X, const int *INCX);
}

template <typename T, typename F, typename G>
void randomStimulus(F &&Ref, G &&Test) {
  int M = irand() % 1024 + 1024;
  int INCX = 1;
  std::vector<T> X = getRandomVector<T>(M);

  auto RefNorm = Ref(&M, X.data(), &INCX);
  auto TestNorm = Test(M, X.data(), INCX);

  checkResult(RefNorm, TestNorm);
}

int main() {
  rnd.seed(std::random_device{}());
  printf("Testing DNRM2...\n");
  randomStimulus<double>(dnrm2_, dnrm2);
  printf("Pass!\n");
  printf("Testing SNRM2...\n");
  randomStimulus<float>(snrm2_, snrm2);
  printf("Pass!\n");
  printf("Testing DZNRM2...\n");
  randomStimulus<std::complex<double>>(dznrm2_, dznrm2);
  printf("Pass!\n");
  printf("Testing SCNRM2...\n");
  randomStimulus<std::complex<float>>(scnrm2_, scnrm2);
  printf("Pass!\n");
}
