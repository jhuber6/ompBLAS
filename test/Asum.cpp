#include "Common.h"
#include "ompBLAS/ompBLAS.h"

extern "C" {
float sasum_(const int *N, const float *X, const int *INCX);
double dasum_(const int *N, const double *X, const int *INCX);
float scasum_(const int *N, const std::complex<float> *X, const int *INCX);
double dzasum_(const int *N, const std::complex<double> *X, const int *INCX);
}

template <typename T, typename F, typename G>
void randomStimulus(F &&Ref, G &&Test) {
  int N = irand() % 1024 + 1024;
  int INCX = 1;
  std::vector<T> X = getRandomVector<T>(N, T(0.1));

  T RefSum = Ref(&N, X.data(), &INCX);
  T TestSum = Test(N, X.data(), INCX);

  checkResult(RefSum, TestSum);
}

int main() {
  rnd.seed(std::random_device{}());
  printf("Testing DASUM...\n");
  randomStimulus<double>(dasum_, dasum);
  printf("Pass!\n");
  printf("Testing SASUM...\n");
  randomStimulus<float>(sasum_, sasum);
  printf("Pass!\n");
  printf("Testing DZASUM...\n");
  randomStimulus<std::complex<double>>(dzasum_, dzasum);
  printf("Pass!\n");
  printf("Testing SCASUM...\n");
  randomStimulus<std::complex<float>>(scasum_, scasum);
  printf("Pass!\n");
}
