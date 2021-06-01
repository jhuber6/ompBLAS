#include "Offloading/Gemm.h"

#include <benchmark/benchmark.h>
#include <cstdio>

static void BM_StringCreation(benchmark::State& state) {
  for (auto _ : state) {
  }
}
// Register the function as a benchmark
BENCHMARK(BM_StringCreation);

BENCHMARK_MAIN();
