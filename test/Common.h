#include <algorithm>
#include <cmath>
#include <complex>
#include <cstdio>
#include <random>
#include <vector>

static std::mt19937 rnd;
static std::uniform_real_distribution<double> uniformDouble(0.0, 1.0);
static std::uniform_real_distribution<float> uniformSingle(0.0, 1.0);
static std::uniform_int_distribution<int> uniformInt(0);

int irand() { return uniformInt(rnd); }
double srand() { return uniformSingle(rnd); }
double drand() { return uniformDouble(rnd); }
std::complex<float> crand() {
  return std::complex<float>(uniformSingle(rnd), uniformSingle(rnd));
}
std::complex<double> zrand() {
  return std::complex<double>(uniformDouble(rnd), uniformDouble(rnd));
}

template <typename T> T random();
template <> double random() { return drand(); }
template <> float random() { return srand(); }
template <> std::complex<double> random() { return zrand(); }
template <> std::complex<float> random() { return crand(); }

template <typename T> struct error_traits {};

template <> struct error_traits<double> {
  static constexpr double threshold = 1.0E-13;
};
template <> struct error_traits<float> {
  static constexpr double threshold = 1.0E-4;
};
template <> struct error_traits<std::complex<double>> {
  static constexpr double threshold = 1.0E-11;
};
template <> struct error_traits<std::complex<float>> {
  static constexpr double threshold = 1.0E-3;
};

template <typename T>
T getError(const std::vector<T> &X, const std::vector<T> &Y) {
  std::vector<T> Z(X.size());
  std::transform(X.begin(), X.end(), Y.begin(), Z.begin(),
                 [](T X, T Y) { return std::abs(X - Y); });
  T MaxError = *std::max_element(Z.begin(), Z.end());

  return MaxError;
}

template <typename T>
T getError(const std::vector<std::complex<T>> &X,
           const std::vector<std::complex<T>> &Y) {
  std::vector<std::complex<T>> Z(X.size());
  std::transform(X.begin(), X.end(), Y.begin(), Z.begin(),
                 [](auto X, auto Y) { return X - Y; });
  T MaxRealError = std::max_element(Z.begin(), Z.end(), [](auto X, auto Y) {
                     return X.real() < Y.real();
                   })->real();
  T MaxImagError = std::max_element(Z.begin(), Z.end(), [](auto X, auto Y) {
                     return X.imag() < Y.imag();
                   })->imag();
  return std::max(MaxRealError, MaxImagError);
}

template <typename T>
void checkResult(const std::vector<T> &X, const std::vector<T> &Y) {
  double Error = static_cast<double>(getError(X, Y));
  printf("*** FAILURE ***\nError: %.12e\n", Error);
  if (Error > error_traits<T>::threshold) {
    exit(1);
  }
}

template <typename T> std::vector<T> getRandomVector(std::size_t Size) {
  std::vector<T> Arr(Size);
  std::generate(Arr.begin(), Arr.end(),
                []() { return T(2.0) * random<T>() - T(1.0); });

  return Arr;
}
