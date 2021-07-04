#include "Common.h"
#include "ompBLAS/ompBLAS.h"

extern "C" {

void scopy_(const int *N, const float *X, const int *INCX, float *Y,
            const int *INCY);
void dcopy_(const int *N, const double *X, const int *INCX, double *Y,
            const int *INCY);
void ccopy_(const int *N, const std::complex<float> *X, const int *INCX,
            std::complex<float> *Y, const int *INCY);
void zcopy_(const int *N, const std::complex<double> *X, const int *INCX,
            std::complex<double> *Y, const int *INCY);
}

template <typename T, typename F, typename G>
void randomStimulus(F &&Ref, G &&Test) {
  int N = irand() % 1024 + 1024;
  int INCX = 1;
  int INCY = 1;
  std::vector<T> X = getRandomVector<T>(N);
  std::vector<T> Y = getRandomVector<T>(N);

  std::vector<T> YRef = Y;
  std::vector<T> YTest = Y;
  Ref(&N, X.data(), &INCX, Y.data(), &INCY);
  Test(N, X.data(), INCX, Y.data(), INCY);

  checkResult(YRef, YTest);
}

int main() {
  rnd.seed(std::random_device{}());
  printf("Testing DCOPY...\n");
  randomStimulus<double>(dcopy_, dcopy);
  printf("Pass!\n");
  printf("Testing SCOPY...\n");
  randomStimulus<float>(scopy_, scopy);
  printf("Pass!\n");
  printf("Testing ZCOPY...\n");
  randomStimulus<std::complex<double>>(zcopy_, zcopy);
  printf("Pass!\n");
  printf("Testing CCOPY...\n");
  randomStimulus<std::complex<float>>(ccopy_, ccopy);
  printf("Pass!\n");
}
