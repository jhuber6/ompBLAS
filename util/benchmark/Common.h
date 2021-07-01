#include <algorithm>
#include <cmath>
#include <complex>
#include <chrono>
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

template <typename T> std::vector<T> getRandomVector(std::size_t Size) {
  std::vector<T> Arr(Size);
  std::generate(Arr.begin(), Arr.end(),
                []() { return T(2.0) * random<T>() - T(1.0); });

  return Arr;
}
