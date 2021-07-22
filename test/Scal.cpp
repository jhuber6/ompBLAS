#include "Common.h"
#include "ompBLAS/ompBLAS.h"

extern "C" {

void sscal_(const int *N, const float *Alpha, const float *X, const int *INCX);
void dscal_(const int *N, const double *Alpha, const double *X,
            const int *INCX);
void cscal_(const int *N, const std::complex<float> *Alpha,
            const std::complex<float> *X, const int *INCX);
void zscal_(const int *N, const std::complex<double> *Alpha,
            const std::complex<double> *X, const int *INCX);
}

template <typename T, typename F, typename G>
void randomStimulus(F &&Ref, G &&Test) {
  int N = irand() % 1024 + 1024;
  int M = irand() % 16 + 16;
  T Alpha = T(2.0) * random<T>() - T(1.0);
  int INCX = 1;
  std::vector<T> X = getRandomVector<T>(N);

  std::vector<T> XRef = X;
  std::vector<T> XTest = X;
  Ref(&N, &Alpha, XRef.data(), &INCX);
  Test(N, Alpha, XTest.data(), INCX);

  checkResult(XRef, XTest);

  INCX = M;
  X = getRandomVector<T>(N * M);

  XRef = X;
  XTest = X;
  Ref(&N, &Alpha, XRef.data(), &INCX);
  Test(N, Alpha, XTest.data(), INCX);

  checkResult(XRef, XTest);
}

int main() {
  rnd.seed(std::random_device{}());
  printf("Testing DSCAL...\n");
  randomStimulus<double>(dscal_, dscal);
  printf("Pass!\n");
  printf("Testing SSCAL...\n");
  randomStimulus<float>(sscal_, sscal);
  printf("Pass!\n");
  printf("Testing ZSCAL...\n");
  randomStimulus<std::complex<double>>(zscal_, zscal);
  printf("Pass!\n");
  printf("Testing CSCAL...\n");
  randomStimulus<std::complex<float>>(cscal_, cscal);
  printf("Pass!\n");
}
