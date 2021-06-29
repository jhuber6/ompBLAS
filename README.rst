==================================================
OpenMP Offloading Implementations of BLAS Routines
==================================================

This repository contains implementations of standard BLAS Routines intended for
Offloading to a GPU device. This is intended for testing compiler optimizations
and is written using idiomatic OpenMP.

.. contents::
  :local:

Building and Running
--------------------

To build, simply run CMake and select your offloading compiler. The script will
attempt to detect offloading support. Running the tests requires a reference
BLAS implementation installed and benchmarking is done using Google Benchmark.

.. code-block:: shell
  
  $ cmake ../ -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release
  $ make -j

To run tests against a reference BLAS implementation, run ``ctest``.

.. code-block:: shell

  $ ctest ./

To run a benchmark, use the custom ``benchmark`` target.

.. code-block:: shell
  
  $ make benchmark-gemm
