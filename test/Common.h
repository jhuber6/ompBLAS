#include <algorithm>
#include <cmath>
#include <complex>
#include <cstdio>
#include <random>
#include <vector>

template <typename T> T getError(const std::vector<T> &X, const std::vector<T> &Y) {
  std::vector<T> Z(X.size());
  std::transform(X.begin(), X.end(), Y.begin(), Z.begin(),
                 [](T X, T Y) { return std::abs(X - Y); });
  T MaxError = *std::max_element(Z.begin(), Z.end());

  return MaxError;
}

template <typename T>
T getError(std::vector<std::complex<T>> &X, const std::vector<std::complex<T>> &Y) {
  std::vector<std::complex<T>> Z(X.size());
  std::transform(X.begin(), X.end(), Y.begin(), Z.begin(),
                 [](std::complex<T> X, std::complex<T> Y) {
                   return std::abs(X - Y);
                 });
  std::complex<T> MaxError =
      *std::max_element(Z.begin(), Z.end(), [](auto X, auto Y) {
        return std::abs(X) < std::abs(Y);
      });
  return std::abs(MaxError);
}

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
