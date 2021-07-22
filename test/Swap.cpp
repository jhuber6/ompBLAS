#include "Common.h"
#include "ompBLAS/ompBLAS.h"

extern "C" {

void sswap_(const int *N, float *X, const int *INCX, float *Y, const int *INCY);
void dswap_(const int *N, double *X, const int *INCX, double *Y,
            const int *INCY);
void cswap_(const int *N, std::complex<float> *X, const int *INCX,
            std::complex<float> *Y, const int *INCY);
void zswap_(const int *N, std::complex<double> *X, const int *INCX,
            std::complex<double> *Y, const int *INCY);
}

template <typename T, typename F, typename G>
void randomStimulus(F &&Ref, G &&Test) {
  int N = irand() % 1024 + 1024;
  int M = irand() % 16 + 16;
  int INCX = 1;
  int INCY = 1;
  std::vector<T> X = getRandomVector<T>(N);
  std::vector<T> Y = getRandomVector<T>(N);

  std::vector<T> YRef = Y;
  std::vector<T> YTest = Y;
  std::vector<T> XRef = X;
  std::vector<T> XTest = X;
  Ref(&N, XRef.data(), &INCX, YRef.data(), &INCY);
  Test(N, XTest.data(), INCX, YTest.data(), INCY);

  checkResult(YRef, YTest);
  checkResult(XRef, XTest);

  INCX = M;
  INCY = M;
  X = getRandomVector<T>(N * M);
  Y = getRandomVector<T>(N * M);

  YRef = Y;
  YTest = Y;
  XRef = X;
  XTest = X;
  Ref(&N, XRef.data(), &INCX, YRef.data(), &INCY);
  Test(N, XTest.data(), INCX, YTest.data(), INCY);

  checkResult(YRef, YTest);
  checkResult(XRef, XTest);
}

int main() {
  rnd.seed(std::random_device{}());
  printf("Testing DSWAP...\n");
  randomStimulus<double>(dswap_, dswap);
  printf("Pass!\n");
  printf("Testing SSWAP...\n");
  randomStimulus<float>(sswap_, sswap);
  printf("Pass!\n");
  printf("Testing ZSWAP...\n");
  randomStimulus<std::complex<double>>(zswap_, zswap);
  printf("Pass!\n");
  printf("Testing CSWAP...\n");
  randomStimulus<std::complex<float>>(cswap_, cswap);
  printf("Pass!\n");
}
