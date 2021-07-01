find_package(benchmark REQUIRED)

macro(add_benchmark_file BENCHNAME)
  add_executable(${BENCHNAME}-benchmark)
  target_compile_features(${BENCHNAME}-benchmark PUBLIC cxx_std_17)
  target_include_directories(${BENCHNAME}-benchmark PRIVATE ${CMAKE_SOURCE_DIR}/include)
  target_link_libraries(${BENCHNAME}-benchmark PRIVATE benchmark::benchmark
    ompBLAS)
  target_compile_options(${BENCHNAME}-benchmark PRIVATE ${COMPILER_WARNINGS})

  target_sources(${BENCHNAME}-benchmark PRIVATE ${ARGN})

  add_custom_target(benchmark-${BENCHNAME}
    COMMAND "${PROJECT_BINARY_DIR}/util/benchmark/${BENCHNAME}-benchmark" 
    "--benchmark_color=true" 
    "--benchmark_counters_tabular=true" 
    "--benchmark_out=${PROJECT_BINARY_DIR}/Benchmarks/${BENCHNAME}.json"
    DEPENDS ${BENCHNAME}-benchmark)
  list(APPEND Benchmarks "benchmark-${BENCHNAME}")
endmacro()

add_benchmark_file(gemm Gemm.cpp)

make_directory("${PROJECT_BINARY_DIR}/Benchmarks")
add_custom_target(benchmark DEPENDS ${Benchmarks})
