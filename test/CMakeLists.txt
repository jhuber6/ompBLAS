find_package(BLAS REQUIRED)

macro(add_test_file TESTNAME)
  add_executable(${TESTNAME})
  target_sources(${TESTNAME} PRIVATE ${ARGN})
  target_link_libraries(${TESTNAME} PRIVATE ${BLAS_LIBRARIES} ompBLAS)
  add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
endmacro()

add_test_file(gemm-test Gemm.cpp)
add_test_file(iamax-test Iamax.cpp)
add_test_file(dot-test Dot.cpp)
add_test_file(axpy-test Axpy.cpp)
