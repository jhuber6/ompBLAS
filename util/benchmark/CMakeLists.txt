find_package(benchmark REQUIRED)

add_executable(gemm)
target_compile_features(gemm PUBLIC cxx_std_17)
target_include_directories(gemm PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(gemm PRIVATE offloading benchmark::benchmark benchmark::benchmark_main)
target_compile_options(gemm PRIVATE ${COMPILER_WARNINGS})

target_sources(gemm PRIVATE
  Gemm.cpp)
