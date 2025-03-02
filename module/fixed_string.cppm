module;

// -------------------------------------------------------------------------------------------------

#if !defined(MTP_NO_EXCEPTIONS) and !defined(__EXCEPTIONS)
#  define MTP_NO_EXCEPTIONS
#elif defined(MTP_NO_EXCEPTIONS) && defined(__EXCEPTIONS)
#  undef MTP_NO_EXCEPTIONS
#endif

#if !defined(MTP_HAS_INCLUDE)
#  ifdef __has_include
#    define MTP_HAS_INCLUDE(incl) __has_include(incl)
#  else
#    define MTP_HAS_INCLUDE(incl) 0
#  endif
#endif

#if MTP_HAS_INCLUDE(<version>)
#  include <version>
#  if MTP_HAS_INCLUDE(<format>) && defined(__cpp_lib_format)
#    define MTP_HAS_FORMAT
#  endif
#  if MTP_HAS_INCLUDE(<ranges>) && defined(__cpp_lib_containers_ranges)
#    define MTP_HAS_FROM_RANGE
#  endif
#  if defined(__cpp_char8_t) && defined(__cpp_lib_char8_t)
#    define MTP_HAS_CHAR8_TYPE
#  endif
#  if defined(__cpp_impl_three_way_comparison) && defined(__cpp_lib_three_way_comparison)
#    define MTP_HAS_THREE_WAY_COMPARE
#  endif
#endif

#if !defined(MTP_USE_STD_MODULE) && defined(__cpp_lib_modules)
#  define MTP_USE_STD_MODULE
#endif

// -------------------------------------------------------------------------------------------------

#ifndef MTP_USE_STD_MODULE
#  include <algorithm>
#  include <array>
#  include <compare>
#  include <concepts>
#  include <cstddef>
#  ifdef MTP_HAS_FORMAT
#    include <format>
#  endif
#  include <iterator>
#  include <ostream>
#  ifdef MTP_HAS_FROM_RANGE
#    include <ranges>
#  endif
#  ifndef MTP_NO_EXCEPTIONS
#    include <stdexcept>
#  endif
#  include <string_view>
#  include <type_traits>
#endif

// -------------------------------------------------------------------------------------------------

export module mtp.fixed_string;

// -------------------------------------------------------------------------------------------------

#ifdef MTP_USE_STD_MODULE
import std;
#endif

// -------------------------------------------------------------------------------------------------

#ifdef __clang__
#  pragma clang diagnostic ignored "-Wunknown-warning-option"
#  pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

// -------------------------------------------------------------------------------------------------

#define MTP_EXPORT export
#define MTP_AS_MODULE
#include <mtp/fixed_string.hpp>
