#include <doctest/doctest.h>

#ifdef MTP_AS_MODULE
import mtp.fixed_string;
#else
#  include <mtp/fixed_string.hpp>
#endif
using namespace mtp;

// -------------------------------------------------------------------------------------------------

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
#  if defined(__cpp_impl_three_way_comparison) && defined(__cpp_lib_three_way_comparison)
#    define MTP_HAS_THREE_WAY_COMPARE
#  endif
#  if defined(__cpp_char8_t) && defined(__cpp_lib_char8_t)
#    define MTP_HAS_CHAR8_TYPE
#  endif
#endif

#if !defined(MTP_NO_EXCEPTIONS) and !defined(__EXCEPTIONS)
#  define MTP_NO_EXCEPTIONS
#elif defined(MTP_NO_EXCEPTIONS) && defined(__EXCEPTIONS)
#  undef MTP_NO_EXCEPTIONS
#endif

// -------------------------------------------------------------------------------------------------

#include <algorithm>
#include <array>
#ifdef MTP_HAS_FORMAT
#  include <format>
#endif
#include <functional>
#include <iostream>
#ifdef MTP_HAS_FROM_RANGE
#  include <ranges>
#endif
#ifdef MTP_NO_EXCEPTIONS
#  include <stdexcept>
#endif
#include <string_view>
using namespace std::string_view_literals;

// -------------------------------------------------------------------------------------------------

template <basic_fixed_string... fixstrs>
consteval auto
concat_all() noexcept
{
  return (fixed_string<0>{ "" } + ... + fixstrs);
}

// -------------------------------------------------------------------------------------------------

TEST_CASE("constructors")
{
  { // run time

    { // chars
      auto const fs = fixed_string<3>{ 'H', 'i', '!' };
      CHECK(fs.view() == "Hi!"sv);
    }

    { // literal
      auto const fs = fixed_u8string<3>{ u8"Hi!" };
      CHECK(fs.view() == u8"Hi!"sv);
    }

    { // range
      auto const fs = fixed_u16string<3>{ std::from_range, std::array{ u'H', u'i', u'!' } };
      CHECK(fs.view() == u"Hi!"sv);
    }

    { // iterators
      auto const sv = L"Hi!"sv;
      auto const fs = fixed_wstring<3>{ sv.begin(), sv.end() };
      CHECK(fs.view() == L"Hi!"sv);
    }
  }

  { // compile time

    { // chars
      constexpr auto fs = fixed_string<3>{ 'H', 'i', '!' };
      static_assert(fs.view() == "Hi!"sv);
    }

    { // literal
      constexpr auto fs = fixed_u8string<3>{ u8"Hi!" };
      static_assert(fs.view() == u8"Hi!"sv);
    }

    { // range
      constexpr auto fs = fixed_u16string<3>{ std::from_range, std::array{ u'H', u'i', u'!' } };
      static_assert(fs.view() == u"Hi!"sv);
    }

    { // iterators
      constexpr auto sv = L"Hi!"sv;
      constexpr auto fs = fixed_wstring<3>{ sv.begin(), sv.end() };
      static_assert(fs.view() == L"Hi!"sv);
    }
  }
}

TEST_CASE("empty")
{
  { // run time
    auto const arr = std::array<char, 0>{};
    auto const sv = ""sv;

    auto const fs_0 = fixed_string<0>{ std::from_range, arr };
    auto const fs_1 = fixed_string<0>{ arr.begin(), arr.end() };
    auto const fs_2 = fixed_string<0>{ "" };
    auto const fs_3 = fixed_string<0>{};
    CHECK(fs_0 == fs_1);
    CHECK(fs_1 == fs_2);
    CHECK(fs_2 == fs_3);
    CHECK(fs_3.view() == sv);

    CHECK(fs_3.size() == 0);
    CHECK(fs_3.length() == 0);
    CHECK(fs_3.max_size() == 0);
    CHECK(fs_3.empty());
#ifndef MTP_NO_EXCEPTIONS
    CHECK_THROWS_WITH_AS(std::ignore = fs_3.at(0), "mtp::basic_fixed_string::at",
                         std::out_of_range);
#endif
  }

  { // compile time
    constexpr auto arr = std::array<char, 0>{};
    constexpr auto sv = ""sv;

    constexpr auto fs_0 = fixed_string<0>{ std::from_range, arr };
    constexpr auto fs_1 = fixed_string<0>{ arr.begin(), arr.end() };
    constexpr auto fs_2 = fixed_string<0>{ "" };
    constexpr auto fs_3 = fixed_string<0>{};
    static_assert(fs_0 == fs_1);
    static_assert(fs_1 == fs_2);
    static_assert(fs_2 == fs_3);
    static_assert(fs_3.view() == sv);

    static_assert(fs_3.size() == 0);
    static_assert(fs_3.length() == 0);
    static_assert(fs_3.max_size() == 0);
    static_assert(fs_3.empty());
#ifndef MTP_NO_EXCEPTIONS
    CHECK_THROWS_WITH_AS(std::ignore = fs_3.at(0), "mtp::basic_fixed_string::at",
                         std::out_of_range);
#endif
  }
}

TEST_CASE("size")
{
  { // run time
    auto const fs_0 = fixed_string<0>{ "" };
    CHECK(fs_0.size() == 0);
    CHECK(fs_0.length() == 0);
    CHECK(fs_0.max_size() == 0);
    CHECK(fs_0.empty());

    auto const fs_1 = fixed_string<1>{ "1" };
    CHECK(fs_1.size() == 1);
    CHECK(fs_1.length() == 1);
    CHECK(fs_1.max_size() == 1);
    CHECK(not fs_1.empty());

    auto const fs_2 = fixed_string<2>{ '1', '2' };
    CHECK(fs_2.size() == 2);
    CHECK(fs_2.length() == 2);
    CHECK(fs_2.max_size() == 2);
    CHECK(not fs_2.empty());
  }

  { // compile time
    constexpr auto fs_0 = fixed_string<0>{ "" };
    static_assert(fs_0.size() == 0);
    static_assert(fs_0.length() == 0);
    static_assert(fs_0.max_size() == 0);
    static_assert(fs_0.empty());

    constexpr auto fs_1 = fixed_string<1>{ "1" };
    static_assert(fs_1.size() == 1);
    static_assert(fs_1.length() == 1);
    static_assert(fs_1.max_size() == 1);
    static_assert(not fs_1.empty());

    constexpr auto fs_2 = fixed_string<2>{ '1', '2' };
    static_assert(fs_2.size() == 2);
    static_assert(fs_2.length() == 2);
    static_assert(fs_2.max_size() == 2);
    static_assert(not fs_2.empty());
  }
}

TEST_CASE("access")
{
  { // run time
    auto const fs = fixed_string<3>{ "abc" };

    CHECK(fs[0] == 'a');
    CHECK(fs[1] == 'b');
    CHECK(fs[2] == 'c');

    CHECK(fs.at(0) == 'a');
    CHECK(fs.at(1) == 'b');
    CHECK(fs.at(2) == 'c');
#ifndef MTP_NO_EXCEPTIONS
    CHECK_THROWS_WITH_AS(std::ignore = fs.at(3), "mtp::basic_fixed_string::at", std::out_of_range);
#endif

    CHECK(fs.front() == 'a');
    CHECK(fs.back() == 'c');

    CHECK(std::string_view{ fs.c_str() } == "abc"sv);
    CHECK(std::string_view{ fs.data() } == "abc"sv);
  }

  { // compile time
    constexpr auto fs = fixed_string<3>{ "abc" };

    static_assert(fs[0] == 'a');
    static_assert(fs[1] == 'b');
    static_assert(fs[2] == 'c');

    static_assert(fs.at(0) == 'a');
    static_assert(fs.at(1) == 'b');
    static_assert(fs.at(2) == 'c');
#ifndef MTP_NO_EXCEPTIONS
    CHECK_THROWS_WITH_AS(std::ignore = fs.at(3), "mtp::basic_fixed_string::at", std::out_of_range);
#endif

    static_assert(fs.front() == 'a');
    static_assert(fs.back() == 'c');

    static_assert(std::string_view{ fs.c_str() } == "abc"sv);
    static_assert(std::string_view{ fs.data() } == "abc"sv);
  }
}

TEST_CASE("swap")
{
  auto fs_0 = fixed_string<3>{ "123" };
  auto fs_1 = fixed_string<3>{ "abc" };

  CHECK(std::string_view{ fs_0 } == "123"sv);
  CHECK(std::string_view{ fs_1 } == "abc"sv);

  fs_0.swap(fs_1);
  CHECK(std::string_view{ fs_0 } == "abc"sv);
  CHECK(std::string_view{ fs_1 } == "123"sv);

  std::swap(fs_0, fs_1);
  CHECK(std::string_view{ fs_0 } == "123"sv);
  CHECK(std::string_view{ fs_1 } == "abc"sv);
}

TEST_CASE("concat")
{
  { // run time
    auto fs_0 = (fixed_string<1>{ "1" } + '2') + "34";
    CHECK(fs_0.view() == "1234"sv);

    auto fs_1 = "12" + ('3' + fixed_string<1>{ '4' });
    CHECK(fs_1.view() == "1234"sv);
  }

  { // compile time
    constexpr auto fs_0 = (fixed_string<1>{ "1" } + '2') + "34";
    static_assert(fs_0.view() == "1234"sv);

    constexpr auto fs_1 = "12" + ('3' + fixed_string<1>{ '4' });
    static_assert(fs_1.view() == "1234"sv);
  }
}

TEST_CASE("nttp")
{
  static_assert(concat_all<"01", "23">() == "0123");
}

TEST_CASE("comparisons")
{
  { // run time
    auto const fs_0 = fixed_string<3>{ "abc" };
    auto const fs_1 = fixed_string<3>{ "abc" };
    auto const fs_2 = fixed_string<3>{ "abd" };

    CHECK(fs_0 == "abc"sv);
    CHECK(fs_0 == fs_1);
    CHECK(fs_0 != fs_2);
  }

  { // compile time
    constexpr auto fs_0 = fixed_string<3>{ "abc" };
    constexpr auto fs_1 = fixed_string<3>{ "abc" };
    constexpr auto fs_2 = fixed_string<3>{ "abd" };

    static_assert(fs_0 == "abc");
    static_assert(fs_0 == "abc"sv);
    static_assert(fs_0 == fs_1);
    static_assert(fs_0 != fs_2);
  }
}

#ifdef MTP_HAS_THREE_WAY_COMPARE
TEST_CASE("sort")
{
  auto fixstrs = std::array{
    fixed_string<1>{ '3' },
    fixed_string<1>{ '1' },
    fixed_string<1>{ '4' },
    fixed_string<1>{ '2' },
  };
  auto const sorted = std::array{
    fixed_string<1>{ '1' },
    fixed_string<1>{ '2' },
    fixed_string<1>{ '3' },
    fixed_string<1>{ '4' },
  };
  CHECK(fixstrs != sorted);

  std::sort(fixstrs.begin(), fixstrs.end());
  CHECK(fixstrs == sorted);
}
#endif

TEST_CASE("hash")
{
  auto const fs_0 = fixed_string<3>{ "123" };
  auto const fs_1 = fixed_string<3>{ "124" };
  auto const sv = "123"sv;

  CHECK(std::hash<fixed_string<3>>{}(fs_0) == std::hash<std::string_view>{}(sv));
  CHECK(std::hash<fixed_string<3>>{}(fs_1) != std::hash<std::string_view>{}(sv));
}

#ifdef MTP_HAS_FORMAT
TEST_CASE("format")
{
  auto const fs = fixed_string<8>{ ", World!" };
  CHECK(std::format("Hello{}", fs) == "Hello, World!");
}
#endif

TEST_CASE("stream")
{
  auto const fs = fixed_string<14>{ "Hello, World!\n" };
  std::cout << fs;
}
