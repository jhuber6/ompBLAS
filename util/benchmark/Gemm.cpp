#include "ompBLAS/ompBLAS.h"
#include "Common.h"

#include <benchmark/benchmark.h>

template <typename T = double>
static void BM_dgemm(benchmark::State& state) {
  long M = state.range(0);
  long N = M;
  long K = M;
  T Alpha = T(2.0) * random<T>() - T(1.0);
  T Beta = T(2.0) * random<T>() - T(1.0);

  std::vector<T> Av = getRandomVector<T>(M * K);
  std::vector<T> Bv = getRandomVector<T>(K * N);
  std::vector<T> Cv = getRandomVector<T>(M * N);

  T *A = Av.data();
  T *B = Bv.data();
  T *C = Cv.data();

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
