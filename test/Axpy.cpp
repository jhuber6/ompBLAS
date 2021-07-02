#include "Common.h"
#include "ompBLAS/ompBLAS.h"

extern "C" {

void saxpy_(const int *N, const float *Alpha, const float *X, const int *INCX,
            float *Y, const int *INCY);
void daxpy_(const int *N, const double *Alpha, const double *X, const int *INCX,
            double *Y, const int *INCY);
void caxpy_(const int *N, const std::complex<float> *Alpha,
            const std::complex<float> *X, const int *INCX,
            std::complex<float> *Y, const int *INCY);
void zaxpy_(const int *N, const std::complex<double> *Alpha,
            const std::complex<double> *X, const int *INCX,
            std::complex<double> *Y, const int *INCY);
}

template <typename T, typename F, typename G>
void randomStimulus(F &&Ref, G &&Test) {
  int N = irand() % 1024 + 1024;
  T Alpha = T(2.0) * random<T>() - T(1.0);
  int INCX = 1;
  int INCY = 1;
  std::vector<T> X = getRandomVector<T>(N);
  std::vector<T> Y = getRandomVector<T>(N);

  std::vector<T> YRef = Y;
  std::vector<T> YTest = Y;
  Ref(&N, &Alpha, X.data(), &INCX, YRef.data(), &INCY);
  Test(N, Alpha, X.data(), INCX, YTest.data(), INCY);

  checkResult(YRef, YTest);
}

int main() {
  rnd.seed(std::random_device{}());
  printf("Testing DAXPY...\n");
  randomStimulus<double>(daxpy_, daxpy);
  printf("Pass!\n");
  printf("Testing SAXPY...\n");
  randomStimulus<float>(saxpy_, saxpy);
  printf("Pass!\n");
  printf("Testing ZAXPY...\n");
  randomStimulus<std::complex<double>>(zaxpy_, zaxpy);
  printf("Pass!\n");
  printf("Testing CAXPY...\n");
  randomStimulus<std::complex<float>>(caxpy_, caxpy);
  printf("Pass!\n");
}
