#include "Common.h"
#include "ompBLAS/ompBLAS.h"

#include <benchmark/benchmark.h>

template <typename T = double> static void BM_ddot(benchmark::State &state) {
  long N = state.range(0);
  std::vector<T> Xv = getRandomVector<T>(N);
  std::vector<T> Yv = getRandomVector<T>(N);

  T *X = Xv.data();
  T *Y = Yv.data();

#pragma omp target data map(to : X [0:N]) map(to : Y [0:N])
  for (auto _ : state) {
    auto Start = std::chrono::high_resolution_clock::now();
    T Sum = ddot(N, X, 1, Y, 1);
    benchmark::DoNotOptimize(Sum);
    auto End = std::chrono::high_resolution_clock::now();

    auto Time =
        std::chrono::duration_cast<std::chrono::duration<double>>(End - Start);

    state.counters["FLOPS"] = benchmark::Counter(N * 2.0 / Time.count());
  }
}

BENCHMARK(BM_ddot)->RangeMultiplier(10)->Range(100, 10000000)->UseRealTime();

BENCHMARK_MAIN();
