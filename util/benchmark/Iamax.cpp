#include "Common.h"
#include "ompBLAS/ompBLAS.h"

#include <benchmark/benchmark.h>

template <typename T = double> static void BM_idamax(benchmark::State &state) {
  long M = state.range(0);
  std::vector<T> Xv = getRandomVector<T>(M);

  T *X = Xv.data();

#pragma omp target data map(to : X[0:M])
  for (auto _ : state) {
    auto Start = std::chrono::high_resolution_clock::now();
    int Idx = idamax(M, X, 1);
    benchmark::DoNotOptimize(Idx);
    auto End = std::chrono::high_resolution_clock::now();

    auto Time =
        std::chrono::duration_cast<std::chrono::duration<double>>(End - Start);

    state.counters["Bandwidth"] =
        benchmark::Counter(M * sizeof(T) / Time.count());
  }
}

BENCHMARK(BM_idamax)->RangeMultiplier(10)->Range(100, 10000000)->UseRealTime();

BENCHMARK_MAIN();
