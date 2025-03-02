cmake_minimum_required(VERSION 3.22)

if(CMAKE_VERSION GREATER_EQUAL 3.29.20240416)
  # https://www.kitware.com/import-std-in-cmake-3-30
  # https://github.com/Kitware/CMake/blob/v3.30.0/Help/dev/experimental.rst
  set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "0e5b6991-d74f-4b3d-a41c-cf096e0b2508")
endif()

project(mtp_fixed_string LANGUAGES CXX)

option(MTP_BUILD_TEST "Build test" ${PROJECT_IS_TOP_LEVEL})
option(MTP_BUILD_MODULE "Build as module" OFF)
option(MTP_USE_STD_MODULE "Use c++23 std module" OFF)

if(MTP_USE_STD_MODULE AND NOT MTP_BUILD_MODULE)
  message(FATAL_ERROR "Must use module build if using c++23 std module.")
endif()

if(MTP_BUILD_MODULE)
  add_library(mtp_fixed_string)
  add_library(mtp::fixed_string ALIAS mtp_fixed_string)
  target_compile_features(
    mtp_fixed_string
    PRIVATE cxx_std_20
    INTERFACE cxx_std_20)

  if(MTP_USE_STD_MODULE)
    target_compile_definitions(mtp_fixed_string PRIVATE MTP_USE_STD_MODULE)
    target_compile_features(
      mtp_fixed_string
      PRIVATE cxx_std_23
      INTERFACE cxx_std_20)
    set_target_properties(mtp_fixed_string PROPERTIES CXX_MODULE_STD ON)
  endif()

  target_sources(
    mtp_fixed_string
    PUBLIC FILE_SET
           HEADERS
           BASE_DIRS
           ${PROJECT_SOURCE_DIR}/include
           FILES
           ${PROJECT_SOURCE_DIR}/include/mtp/fixed_string.hpp
           FILE_SET
           CXX_MODULES
           BASE_DIRS
           ${PROJECT_SOURCE_DIR}/module
           FILES
           ${PROJECT_SOURCE_DIR}/module/fixed_string.cppm)
else()
  add_library(mtp_fixed_string INTERFACE)
  add_library(mtp::fixed_string ALIAS mtp_fixed_string)
  target_compile_features(mtp_fixed_string INTERFACE cxx_std_20)

  target_sources(
    mtp_fixed_string
    INTERFACE FILE_SET
              HEADERS
              BASE_DIRS
              ${PROJECT_SOURCE_DIR}/include
              FILES
              ${PROJECT_SOURCE_DIR}/include/mtp/fixed_string.hpp)
endif()

if(MTP_BUILD_TEST)
  add_subdirectory(${PROJECT_SOURCE_DIR}/test)
endif()
