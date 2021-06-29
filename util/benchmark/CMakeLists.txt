find_package(benchmark REQUIRED)

add_executable(gemm-benchmark)
target_compile_features(gemm-benchmark PUBLIC cxx_std_17)
target_include_directories(gemm-benchmark PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(gemm-benchmark PRIVATE benchmark::benchmark
  ompBLAS)
target_compile_options(gemm-benchmark PRIVATE ${COMPILER_WARNINGS})

target_sources(gemm-benchmark PRIVATE Gemm.cpp)

add_custom_target(benchmark-gemm
  COMMAND "${PROJECT_BINARY_DIR}/util/benchmark/gemm-benchmark" 
  "--benchmark_color=true" 
  "--benchmark_counters_tabular=true" 
  DEPENDS gemm-benchmark)
