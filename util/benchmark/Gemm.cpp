#include "ompBLAS/Gemm.h"

#include <benchmark/benchmark.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <random>
#include <chrono>

static std::mt19937 rnd{std::random_device{}()};
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

static void BM_dgemm(benchmark::State& state) {
  long M = state.range(0);
  long N = M;
  long K = M;
  double Alpha = 2.0 * drand() - 1.0;
  double Beta = 2.0 * drand() - 1.0;

  std::vector<double> Av(M * K);
  std::vector<double> Bv(K * N);
  std::vector<double> Cv(M * N);

  std::generate(Av.begin(), Av.end(), []() { return 2.0 * drand() - 1.0; });
  std::generate(Bv.begin(), Bv.end(), []() { return 2.0 * drand() - 1.0; });
  std::generate(Cv.begin(), Cv.end(), []() { return 2.0 * drand() - 1.0; });

  double *A = Av.data();
  double *B = Bv.data();
  double *C = Cv.data();

  double M_dbl = M;
  double N_dbl = N;
  double K_dbl = K;
  const double FLOPS = M_dbl * N_dbl * K_dbl * 2.0 + M_dbl * N_dbl * 2.0;

#pragma omp target data                                                        \
  map(to: A[0:M * K])                                                          \
  map(to: B[0:K * N])                                                          \
  map(tofrom: C[0:M * N])
  for (auto _ : state) {
    auto Start = std::chrono::high_resolution_clock::now();
    dgemm(RowMajor, NoTrans, NoTrans, M, N, K, Alpha, A, K, B, N, Beta, C, N);
    auto End = std::chrono::high_resolution_clock::now();

    auto Time =
      std::chrono::duration_cast<std::chrono::duration<double>>(
        End - Start);

    state.counters["FLOPS"] = benchmark::Counter(FLOPS / Time.count());
  }
}

BENCHMARK(BM_dgemm)->RangeMultiplier(2)->Range(64, 4096)->UseRealTime();

BENCHMARK_MAIN();
