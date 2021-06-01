#[=======================================================================[.rst:
FindOpenMPTarget
----------------

Finds OpenMP Target Offloading Support.

This module can be used to detect OpenMP target offloading support in a
compiler. If the compiler support OpenMP Offloading to a specified target, the
flags required to compile offloading code to that target are output for each
target.

Variables
^^^^^^^^^

The module exposes the components ``NVPTX`` and ``AMDGCN``.  Each of these
controls the various offloading targets to search OpenMP target offloasing
support for.

Depending on the enabled components the following variables will be set:

``OpenMPTarget_FOUND``
  Variable indicating that OpenMP target offloading flags for all requested
  targets have been found.
``OpenMPTarget_VERSION``
  Minimal version of the OpenMP standard detected among the requested languages,
  or all enabled languages if no components were specified.

This module will set the following variables per language in your
project, where ``<target>`` is one of NVPTX or AMDGCN

``OpenMPTarget_<target>_FOUND``
  Variable indicating if OpenMP support for the ``<target>`` was detected.
``OpenMPTarget_<target>_FLAGS``
  OpenMP compiler flags for offloading to ``<target>``, separated by spaces.

For linking with OpenMP code written in ``<target>``, the following
variables are provided:

``OpenMPTarget_<target>_LIBRARIES``
  A list of libraries needed to link with OpenMP code written in ``<lang>``.

Additionally, the module provides :prop_tgt:`IMPORTED` targets:

``OpenMPTarget::OpenMP_<target>``
  Target for using OpenMP offloading to ``<target>``.

The module will also try to provide the OpenMP version variables:

``OpenMPTarget_<target>_VERSION_MAJOR``
  Major version of OpenMP implemented by the ``<target>`` compiler.
``OpenMPTarget_<target>_VERSION_MINOR``
  Minor version of OpenMP implemented by the ``<target>`` compiler.
``OpenMPTarget_<target>_VERSION``
  OpenMP version implemented by the ``<target>`` compiler.

#]=======================================================================]
cmake_policy(PUSH)
cmake_policy(SET CMP0012 NEW) # if() recognizes numbers and booleans
cmake_policy(SET CMP0054 NEW) # if() quoted variables not dereferenced
cmake_policy(SET CMP0057 NEW) # if IN_LIST

if(NOT OpenMP_FOUND)
  if(CMAKE_C_COMPILER_LOADED AND CMAKE_CXX_COMPILER_LOADED)
    find_package(OpenMP ${OpenMPTarget_FIND_VERSION} REQUIRED C CXX)
  elseif(CMAKE_C_COMPILER_LOADED)
    find_package(OpenMP ${OpenMPTarget_FIND_VERSION} REQUIRED C)
  else()
    find_package(OpenMP ${OpenMPTarget_FIND_VERSION} REQUIRED CXX)
  endif()
endif()

function(_OPENMP_TARGET_ARCH_FLAG_CANDIDATES LANG TARGET_ARCH)
  if(NOT OpenMPTarget_${LANG}_FLAG)
    set(OpenMPTarget_FLAG_CANDIDATES "NOTFOUND")

    set(OMPTarget_FLAG_Clang "-fopenmp-targets=${TARGET_ARCH}")
    set(OMPTarget_FLAG_GNU "-foffload=${TARGET_ARCH}=\"-lm -latomic\"")
    set(OMPTarget_FLAG_XL "-qoffload")

    if(DEFINED OMPTarget_FLAG_${CMAKE_${LANG}_COMPILER_ID})
      set(OpenMPTarget_FLAG_CANDIDATES "${OMPTarget_FLAG_${CMAKE_${LANG}_COMPILER_ID}}")
    endif()

    set(OpenMPTarget_${LANG}_FLAG_CANDIDATES "${OpenMPTarget_FLAG_CANDIDATES}" PARENT_SCOPE)
  else()
    set(OpenMPTarget_${LANG}_FLAG_CANDIDATES "${OpenMPTarget_${LANG}_FLAG}" PARENT_SCOPE)
  endif()
endfunction()

function(_OPENMP_TARGET_ARCH_ARCH_CANDIDATES LANG TARGET_ARCH)
  if (NOT OpenMPTarget_${LANG}_ARCH)
    set(OpenMPTarget_ARCH_CANDIDATES "NOTFOUND")

    if("${TARGET_ARCH}" STREQUAL "NVPTX")
      if ("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
        set(OMPTarget_ARCH_Clang "nvptx32-nvidia-cuda")
      else()
        set(OMPTarget_ARCH_Clang "nvptx64-nvidia-cuda")
      endif()
      set(OMPTarget_ARCH_GNU "nvptx-none")
      set(OMPTarget_ARCH_XL "")

      if(DEFINED OMPTarget_ARCH_${CMAKE_${LANG}_COMPILER_ID})
        set(OpenMPTarget_ARCH_CANDIDATES "${OMPTarget_ARCH_${CMAKE_${LANG}_COMPILER_ID}}")
      endif()
    endif()
    set(OpenMPTarget_${LANG}_ARCH_CANDIDATES "${OpenMPTarget_ARCH_CANDIDATES}" PARENT_SCOPE)
  else()
    set(OpenMPTarget_${LANG}_ARCH_CANDIDATES "${OpenMPTarget_${LANG}_ARCH}" PARENT_SCOPE)
  endif()
endfunction()

set(OpenMPTarget_C_CXX_TEST_SOURCE
"#include <omp.h>
int main(void) {
  int isHost;
#pragma omp target map(from: isHost)
  { isHost = omp_is_initial_device(); }
  return isHost;
}")

function(_OPENMP_TARGET_ARCH_WRITE_SOURCE_FILE LANG SRC_FILE_CONTENT_VAR SRC_FILE_NAME SRC_FILE_FULLPATH)
  set(WORK_DIR ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/FindOpenMPTarget)
  if("${LANG}" STREQUAL "C")
    set(SRC_FILE "${WORK_DIR}/${SRC_FILE_NAME}.c")
    file(WRITE "${SRC_FILE}" "${OpenMPTarget_C_CXX_${SRC_FILE_CONTENT_VAR}}")
  elseif("${LANG}" STREQUAL "CXX")
    set(SRC_FILE "${WORK_DIR}/${SRC_FILE_NAME}.cpp")
    file(WRITE "${SRC_FILE}" "${OpenMPTarget_C_CXX_${SRC_FILE_CONTENT_VAR}}")
  endif()
  set(${SRC_FILE_FULLPATH} "${SRC_FILE}" PARENT_SCOPE)
endfunction()

function(_OPENMP_TARGET_ARCH_GET_FLAGS LANG TARGET_ARCH OPENMP_FLAG_VAR OPENMP_LIB_VAR OPENMP_ARCH_VAR)
  _OPENMP_TARGET_ARCH_ARCH_CANDIDATES(${LANG} ${TARGET_ARCH})
  _OPENMP_TARGET_ARCH_FLAG_CANDIDATES(${LANG} "${OpenMPTarget_${LANG}_ARCH_CANDIDATES}")
  _OPENMP_TARGET_ARCH_WRITE_SOURCE_FILE("${LANG}" "TEST_SOURCE" OpenMPTargetTryFlag _OPENMP_TEST_SRC)

  try_compile(OpenMPTarget_COMPILE_RESULT ${CMAKE_BINARY_DIR} ${_OPENMP_TEST_SRC}
    CMAKE_FLAGS "-DCOMPILE_DEFINITIONS:STRING=${OpenMP_${LANG}_FLAGS} ${OpenMPTarget_${LANG}_FLAG_CANDIDATES}"
                "-DINCLUDE_DIRECTORIES:STRING=${OpenMP_${LANG}_INCLUDE_DIR}"
    LINK_LIBRARIES ${CMAKE_${LANG}_VERBOSE_FLAG}
    OUTPUT_VARIABLE OpenMP_TRY_COMPILE_OUTPUT
  )

if (OpenMPTarget_COMPILE_RESULT AND (NOT "${OpenMPTARGET_${LANG}_ARCH_CANDIDATES}" STREQUAL "NOTFOUND"))
    if(CMAKE_${LANG}_COMPILER_ID STREQUAL "Clang")
      find_library(OpenMPTarget_libomptarget_LIBRARY
        NAMES omptarget
        HINTS ${CMAKE_${LANG}_IMPLICIT_LINK_DIRECTORIES}
      )
      set("${OPENMP_LIB_VAR}" "${OpenMPTarget_libomptarget_LIBRARY}" PARENT_SCOPE)
    else()
      unset("${OPENMP_LIB_VAR}" PARENT_SCOPE)
    endif()
    set("${OPENMP_ARCH_VAR}" "${OpenMPTarget_${LANG}_ARCH_CANDIDATES}" PARENT_SCOPE)
    set("${OPENMP_FLAG_VAR}" "${OpenMPTarget_${LANG}_FLAG_CANDIDATES}" PARENT_SCOPE)
  else()
    set("${OPENMP_ARCH_VAR}" "NOTFOUND" PARENT_SCOPE)
    set("${OPENMP_FLAG_VAR}" "NOTFOUND" PARENT_SCOPE)
  endif()
endfunction()

foreach(LANG IN ITEMS C CXX)
  foreach(TARGET_ARCH IN ITEMS NVPTX AMDGCN)
    if(CMAKE_${LANG}_COMPILER_LOADED)
      if(NOT DEFINED OpenMPTarget_${LANG}_FLAGS OR NOT DEFINED OpenMPTarget_${LANG}_Arch)
        _OPENMP_TARGET_ARCH_GET_FLAGS(${LANG} ${TARGET_ARCH}
        OpenMPTarget_${TARGET_ARCH}_FLAGS_WORK
        OpenMPTarget_${TARGET_ARCH}_LIBS_WORK
        OpenMPTarget_${TARGET_ARCH}_ARCH_WORK)
        set(OpenMPTarget_${TARGET_ARCH}_FLAGS ${OpenMPTarget_${TARGET_ARCH}_FLAGS_WORK}
            CACHE STRING "${TARGET_ARCH} target compile flags for OpenMP target offloading" FORCE)
          set(OpenMPTarget_${TARGET_ARCH}_LIBS ${OpenMPTarget_${TARGET_ARCH}_LIBS_WORK}
            CACHE STRING "${TARGET_ARCH} target libraries for OpenMP target offloading" FORCE)
      endif()
    endif()
  endforeach()
endforeach()

if(OpenMPTarget_FIND_COMPONENTS)
  set(OpenMPTarget_FINDLIST ${OpenMPTarget_FIND_COMPONENTS})
else()
  set(OpenMPTarget_FINDLIST NVPTX)
endif()

unset(_OpenMPTarget_MIN_VERSION)

foreach(LANG IN ITEMS C CXX)
  foreach(TARGET_ARCH IN LISTS OpenMPTarget_FINDLIST)
    if(CMAKE_${LANG}_COMPILER_LOADED)
      set(OpenMPTarget_${TARGET_ARCH}_VERSION "${OpenMP_${LANG}_VERSION}")
      set(OpenMPTarget_${TARGET_ARCH}_VERSION_MAJOR "${OpenMP_${LANG}_VERSION}_MAJOR")
      set(OpenMPTarget_${TARGET_ARCH}_VERSION_MINOR "${OpenMP_${LANG}_VERSION}_MINOR")
      set(OpenMPTarget_${TARGET_ARCH}_FIND_QUIETLY ${OpenMPTarget_FIND_QUIETLY})
      set(OpenMPTarget_${TARGET_ARCH}_FIND_REQUIRED ${OpenMPTarget_FIND_REQUIRED})
      set(OpenMPTarget_${TARGET_ARCH}_FIND_VERSION ${OpenMPTarget_FIND_VERSION})
      set(OpenMPTarget_${TARGET_ARCH}_FIND_VERSION_EXACT ${OpenMPTarget_FIND_VERSION_EXACT})

      if(NOT (${OpenMPTarget_${TARGET_ARCH}_VERSION} VERSION_GREATER_EQUAL "4.0"))
        message(SEND_ERROR "FindOpenMPTarget requires at least OpenMP 4.0")
      endif()

      find_package_handle_standard_args(OpenMPTarget_${TARGET_ARCH}
        NAME_MISMATCHED
        REQUIRED_VARS OpenMPTarget_${TARGET_ARCH}_FLAGS
        VERSION_VAR OpenMPTarget_${TARGET_ARCH}_VERSION)

      if(OpenMPTarget_${TARGET_ARCH}_FOUND)
        if(DEFINED OpenMPTarget_${TARGET_ARCH}_VERSION)
          if(NOT _OpenMPTarget_MIN_VERSION OR _OpenMPTarget_MIN_VERSION VERSION_GREATER OpenMPTarget_${LANG}_VERSION)
            set(_OpenMPTarget_MIN_VERSION OpenMPTarget_${TARGET_ARCH}_VERSION)
          endif()
        endif()
        if(NOT TARGET OpenMPTarget::OpenMPTarget_${TARGET_ARCH})
          add_library(OpenMPTarget::OpenMPTarget_${TARGET_ARCH} INTERFACE IMPORTED)
        endif()
        separate_arguments(_OpenMPTarget_${TARGET_ARCH}_OPTIONS NATIVE_COMMAND "${OpenMPTarget_${TARGET_ARCH}_FLAGS}")
        set_property(TARGET OpenMPTarget::OpenMPTarget_${TARGET_ARCH} PROPERTY
          INTERFACE_COMPILE_OPTIONS 
          "$<$<COMPILE_LANGUAGE:${LANG}>:${_OpenMPTarget_${TARGET_ARCH}_OPTIONS}>"
          "$<$<COMPILE_LANGUAGE:${LANG}>:${OpenMP_${LANG}_FLAGS}>")
        set_property(TARGET OpenMP::OpenMP_${LANG} PROPERTY
          INTERFACE_INCLUDE_DIRECTORIES "$<BUILD_INTERFACE:${OpenMP_${LANG}_INCLUDE_DIRS}>")
        set_property(TARGET OpenMPTarget::OpenMPTarget_${TARGET_ARCH} PROPERTY
          INTERFACE_LINK_LIBRARIES 
          "${OpenMPTarget_${TARGET_ARCH}_LIBS}"
          "${OpenMP_${LANG}_LIBRARIES}")
        set_property(TARGET OpenMPTarget::OpenMPTarget_${TARGET_ARCH} PROPERTY
          INTERFACE_LINK_OPTIONS 
          "$<$<COMPILE_LANGUAGE:${LANG}>:${_OpenMPTarget_${TARGET_ARCH}_OPTIONS}>"
          "$<$<COMPILE_LANGUAGE:${LANG}>:${OpenMP_${LANG}_FLAGS}>")
        unset(_OpenMPTarget_${TARGET_ARCH}_OPTIONS)
      endif()
    endif()
  endforeach()
endforeach()

unset(_OpenMPTarget_REQ_VARS)
foreach(TARGET_ARCH IN LISTS OpenMPTarget_FINDLIST)
  list(APPEND _OpenMPTarget_REQ_VARS "OpenMPTarget_${TARGET_ARCH}_FOUND")
endforeach()

find_package_handle_standard_args(OpenMPTarget
    REQUIRED_VARS ${_OpenMPTarget_REQ_VARS}
    VERSION_VAR ${_OpenMPTarget_MIN_VERSION}
    HANDLE_COMPONENTS)

if(NOT (CMAKE_C_COMPILER_LOADED OR CMAKE_CXX_COMPILER_LOADED) OR CMAKE_Fortran_COMPILER_LOADED)
  message(SEND_ERROR "FindOpenMPTarget requires the C or CXX languages to be enabled")
endif()

unset(OpenMPTarget_C_CXX_TEST_SOURCE)
cmake_policy(POP)
