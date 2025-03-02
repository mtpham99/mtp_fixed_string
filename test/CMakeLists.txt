find_package(doctest QUIET)
if(NOT doctest_FOUND)
  message("\"doctest\" not found. Fetching from Github.")
  include(FetchContent)
  FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.4.11
    GIT_SHALLOW TRUE)
  FetchContent_MakeAvailable(doctest)
  list(APPEND CMAKE_MODULE_PATH ${doctest_SOURCE_DIR}/scripts/cmake/)
endif()

add_executable(fixed_string_tests ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
                                  ${CMAKE_CURRENT_SOURCE_DIR}/fixed_string_tests.cpp)
target_link_libraries(fixed_string_tests PRIVATE mtp::fixed_string doctest::doctest)
target_compile_features(fixed_string_tests PRIVATE cxx_std_20)

if(MTP_BUILD_MODULE)
  target_compile_definitions(fixed_string_tests PRIVATE MTP_AS_MODULE)
  set_target_properties(fixed_string_tests PROPERTIES CXX_SCAN_FOR_MODULES ON)
endif()

# TODO: get rid of cxx_std_23 requirement when library is compiled with std module
# NOTE: using library compile feature "INTERFACE cxx_std_20" not working
if(MTP_USE_STD_MODULE)
  target_compile_features(fixed_string_tests PRIVATE cxx_std_23)
endif()

include(doctest)
doctest_discover_tests(fixed_string_tests)
