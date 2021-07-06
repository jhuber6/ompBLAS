#include "Common.h"
#include "ompBLAS/ompBLAS.h"

#include <benchmark/benchmark.h>

template <typename T = double> static void BM_dgemv(benchmark::State &state) {
  long M = state.range(0);
  long N = M;
  T Alpha = T(2.0) * random<T>() - T(1.0);
  T Beta = T(2.0) * random<T>() - T(1.0);

  std::vector<T> Av = getRandomVector<T>(M * N);
  std::vector<T> Xv = getRandomVector<T>(N);
  std::vector<T> Yv = getRandomVector<T>(M);

  T *A = Av.data();
  T *X = Xv.data();
  T *Y = Yv.data();

  double M_dbl = M;
  double N_dbl = N;
  const double FLOPS = M_dbl * N_dbl * 2.0 + M_dbl * 2.0;

#pragma omp target data map(to                                                 \
                            : A [0:M * N]) map(to                              \
                                               : X [0:N]) map(tofrom           \
                                                              : Y [0:M])
  for (auto _ : state) {
    auto Start = std::chrono::high_resolution_clock::now();
    dgemv(RowMajor, NoTrans, M, N, Alpha, A, M, X, 1, Beta, Y, 1);
    auto End = std::chrono::high_resolution_clock::now();

    auto Time =
        std::chrono::duration_cast<std::chrono::duration<double>>(End - Start);

    state.counters["FLOPS"] = benchmark::Counter(FLOPS / Time.count());
  }
}

BENCHMARK(BM_dgemv)->RangeMultiplier(2)->Range(64, 8192)->UseRealTime();

BENCHMARK_MAIN();
