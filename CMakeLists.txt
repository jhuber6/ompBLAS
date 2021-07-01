cmake_minimum_required(VERSION 3.13.4)
project(ompBLAS VERSION 1.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(OpenMPTarget REQUIRED NVPTX)

option(OMPBLAS_BUILD_TESTS "Build the ompBLAS tests" ON)
option(OMPBLAS_BUILD_BENCHMARK "Build the ompBLAS benchmarks" ON)

# Compiler flags
set(COMPILER_WARNINGS
    -Wall
    -Wextra
    -Wsign-compare
    -Wpedantic
    -Wunused
    -Wnon-virtual-dtor
    -Woverloaded-virtual
    -Wcast-align
    -Wnull-dereference)

# Compilation targets
add_library(ompBLAS SHARED)
target_compile_features(ompBLAS PUBLIC cxx_std_17)
target_include_directories(
  ompBLAS PUBLIC 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(ompBLAS PUBLIC OpenMPTarget::OpenMPTarget_NVPTX)
target_compile_options(ompBLAS PRIVATE 
  $<$<CXX_COMPILER_ID:Clang>:${COMPILER_WARNINGS}>
  $<$<CXX_COMPILER_ID:GNU>:${COMPILER_WARNINGS}>)

# Configure Installation 
install(TARGETS ompBLAS
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION
  "lib/cmake/${PROJECT_NAME}")

install(EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION "lib/cmake/${PROJECT_NAME}")

install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
              "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION "lib/cmake/${PROJECT_NAME}")

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ompBLAS DESTINATION include
        FILES_MATCHING PATTERN "*.h")

add_subdirectory(lib)

if (OMPBLAS_BUILD_BENCHMARK)
  add_subdirectory(util)
endif()

if (OMPBLAS_BUILD_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()
