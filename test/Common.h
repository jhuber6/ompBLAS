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
  static constexpr double threshold = 1.0E-2;
};

template <typename T>
T getError(const std::vector<T> &X, const std::vector<T> &Y) {
  std::vector<T> Z(X.size());
  std::transform(X.begin(), X.end(), Y.begin(), Z.begin(),
                 [](T X, T Y) { return (X - Y) * (X - Y); });
  T Sum = std::accumulate(Z.begin(), Z.end(), T(0.0));

  return std::sqrt(Sum / static_cast<T>(Z.size()));
}

template <typename T>
T getError(const std::vector<std::complex<T>> &X,
           const std::vector<std::complex<T>> &Y) {
  std::vector<std::complex<T>> Z(X.size());
  std::transform(X.begin(), X.end(), Y.begin(), Z.begin(),
                 [](auto X, auto Y) { return (X - Y) * (X - Y); });
  std::complex<T> Sum = std::accumulate(Z.begin(), Z.end(), std::complex<T>(0.0));

  return std::sqrt(std::abs(Sum) / static_cast<T>(Z.size()));
}

template <typename T>
void checkResult(const std::complex<T> &X, const std::complex<T> &Y) {
  double Error =
      static_cast<double>(std::sqrt(std::abs((X - Y) * (X - Y))));
  if (std::abs(Error) > error_traits<std::complex<T>>::threshold) {
    printf("*** FAILURE ***\nError: %.12e\n", Error);
    exit(1);
  }
}

template <typename T> void checkResult(const T &X, const T &Y) {
  double Error = static_cast<double>(X - Y);
  if (std::abs(Error) > error_traits<T>::threshold) {
    printf("*** FAILURE ***\nError: %.12e\n", Error);
    exit(1);
  }
}

template <typename T>
void checkResult(const std::vector<T> &X, const std::vector<T> &Y) {
  double Error = static_cast<double>(getError(X, Y));
  if (std::abs(Error) > error_traits<T>::threshold) {
    printf("*** FAILURE ***\nError: %.12e\n", Error);
    exit(1);
  }
}

template <typename T>
std::vector<T> getRandomVector(std::size_t Size, T Range = 2.0) {
  std::vector<T> Arr(Size);
  std::generate(Arr.begin(), Arr.end(),
                [&]() { return Range * random<T>() - (Range / T(2.0)); });

  return Arr;
}
