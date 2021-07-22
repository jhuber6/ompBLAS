#include "Common.h"
#include "ompBLAS/ompBLAS.h"

extern "C" {

float sdot_(const int *N, const float *X, const int *INCX, const float *Y,
            const int *INCY);

double ddot_(const int *N, const double *X, const int *INCX, const double *Y,
             const int *INCY);

std::complex<float> cdotu_(const int *N, const std::complex<float> *X,
                           const int *INCX, const std::complex<float> *Y,
                           const int *INCY);

std::complex<double> zdotu_(const int *N, const std::complex<double> *X,
                            const int *INCX, const std::complex<double> *Y,
                            const int *INCY);

std::complex<float> cdotc_(const int *N, const std::complex<float> *X,
                           const int *INCX, const std::complex<float> *Y,
                           const int *INCY);

std::complex<double> zdotc_(const int *N, const std::complex<double> *X,
                            const int *INCX, const std::complex<double> *Y,
                            const int *INCY);
}

template <typename T, typename F, typename G>
void randomStimulus(F &&Ref, G &&Test) {
  int N = irand() % 1024 + 1024;
  int M = irand() % 16 + 16;
  int INCX = 1;
  int INCY = 1;
  std::vector<T> X = getRandomVector<T>(N);
  std::vector<T> Y = getRandomVector<T>(N);

  T RefSum = Ref(&N, X.data(), &INCX, Y.data(), &INCY);
  T TestSum = Test(N, X.data(), INCX, Y.data(), INCY);

  checkResult(RefSum, TestSum);

  INCX = M;
  INCY = M;
  X = getRandomVector<T>(N * M);
  Y = getRandomVector<T>(N * M);

  RefSum = Ref(&N, X.data(), &INCX, Y.data(), &INCY);
  TestSum = Test(N, X.data(), INCX, Y.data(), INCY);

  checkResult(RefSum, TestSum);
}

int main() {
  rnd.seed(std::random_device{}());
  printf("Testing DDOT...\n");
  randomStimulus<double>(ddot_, ddot);
  printf("Pass!\n");
  printf("Testing SDOT...\n");
  randomStimulus<float>(sdot_, sdot);
  printf("Pass!\n");
  printf("Testing ZDOTU...\n");
  randomStimulus<std::complex<double>>(zdotu_, zdotu);
  printf("Pass!\n");
  printf("Testing CDOTU...\n");
  randomStimulus<std::complex<float>>(cdotu_, cdotu);
  printf("Pass!\n");
  printf("Testing ZDOTC...\n");
  randomStimulus<std::complex<double>>(zdotc_, zdotc);
  printf("Pass!\n");
  printf("Testing SDOTC...\n");
  randomStimulus<std::complex<float>>(cdotc_, cdotc);
  printf("Pass!\n");
}
