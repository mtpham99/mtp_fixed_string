#ifndef MTP_FIXED_STRING_HPP
#define MTP_FIXED_STRING_HPP

// -------------------------------------------------------------------------------------------------

#if __cplusplus <= 201703L
#  error "Requires c++20 or later."
#endif

// -------------------------------------------------------------------------------------------------

#ifndef MTP_EXPORT
#  define MTP_EXPORT
#endif

#ifndef MTP_EXPECTS
#  if defined(_MSC_VER) && !defined(__clang__)
#    define MTP_EXPECTS(cond) __assume(cond)
#  elif defined(__GNUC__) || defined(__clang__)
#    define MTP_EXPECTS(cond) ((cond) ? static_cast<void>(0) : __builtin_unreachable())
#  else
#    define MTP_EXPECTS(cond) static_cast<void>(0)
#  endif
#endif

#if !defined(MTP_NO_EXCEPTIONS) and !defined(__EXCEPTIONS)
#  define MTP_NO_EXCEPTIONS
#elif defined(MTP_NO_EXCEPTIONS) && defined(__EXCEPTIONS)
#  undef MTP_NO_EXCEPTIONS
#endif

#ifdef MTP_NO_EXCEPTIONS
#  define MTP_NOEXCEPT noexcept(true)
#else
#  define MTP_NOEXCEPT noexcept(false)
#endif

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
#  if defined(__cpp_char8_t) && defined(__cpp_lib_char8_t)
#    define MTP_HAS_CHAR8_TYPE
#  endif
#  if defined(__cpp_impl_three_way_comparison) && defined(__cpp_lib_three_way_comparison)
#    define MTP_HAS_THREE_WAY_COMPARE
#  endif
#endif

// -------------------------------------------------------------------------------------------------

#ifndef MTP_AS_MODULE
#  ifdef MTP_USE_STD_MODULE
import std;
#  else
#    include <algorithm>
#    include <array>
#    include <compare>
#    include <concepts>
#    include <cstddef>
#    ifdef MTP_HAS_FORMAT
#      include <format>
#    endif
#    include <iterator>
#    include <ostream>
#    ifdef MTP_HAS_FROM_RANGE
#      include <ranges>
#    endif
#    ifndef MTP_NO_EXCEPTIONS
#      include <stdexcept>
#    endif
#    include <string_view>
#    include <type_traits>
#  endif
#endif

// -------------------------------------------------------------------------------------------------

namespace std {

#ifndef MTP_HAS_FROM_RANGE
MTP_EXPORT inline constexpr struct from_range_t
{
  explicit from_range_t() = default;
} from_range{};
#endif

} // namespace std

// -------------------------------------------------------------------------------------------------

namespace mtp {

MTP_EXPORT template <typename CharT, std::size_t N>
struct basic_fixed_string;

MTP_EXPORT template <std::size_t N>
using fixed_string = basic_fixed_string<char, N>;

MTP_EXPORT template <std::size_t N>
using fixed_wstring = basic_fixed_string<wchar_t, N>;

#ifdef MTP_HAS_CHAR8_TYPE
MTP_EXPORT template <std::size_t N>
using fixed_u8string = basic_fixed_string<char8_t, N>;
#endif

MTP_EXPORT template <std::size_t N>
using fixed_u16string = basic_fixed_string<char16_t, N>;

MTP_EXPORT template <std::size_t N>
using fixed_u32string = basic_fixed_string<char32_t, N>;

template <typename CharT, std::size_t N>
struct basic_fixed_string
{
  CharT _data[N + 1] = {};

  using value_type = CharT;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using const_pointer = value_type const*;
  using pointer = value_type*;
  using const_reference = value_type const&;
  using reference = value_type&;
  using const_iterator = const_pointer;
  using iterator = const_iterator;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using reverse_iterator = const_reverse_iterator;

  static constexpr std::integral_constant<size_type, N> size{};
  static constexpr std::integral_constant<size_type, N> length{};
  static constexpr std::integral_constant<size_type, N> max_size{};
  static constexpr std::bool_constant<N == 0> empty{};

  template <std::convertible_to<CharT>... CharTs>
    requires(sizeof...(CharTs) == N && (... && !std::is_pointer_v<CharTs>))
  [[nodiscard]] explicit constexpr basic_fixed_string(CharTs... chars) noexcept
      : _data{ chars..., CharT{} }
  {}

  [[nodiscard]] consteval basic_fixed_string(CharT const (&str)[N + 1]) noexcept
  {
    MTP_EXPECTS(str[N] == CharT{});
    std::ranges::copy(str, str + N, _data);
  }

  template <std::input_iterator I, std::sentinel_for<I> S>
    requires(std::convertible_to<std::iter_value_t<I>, CharT>)
  [[nodiscard]] constexpr basic_fixed_string(I begin, S end) noexcept
  {
    MTP_EXPECTS(std::ranges::distance(begin, end) == N);
    std::ranges::copy(begin, end, _data);
  }

  template <std::ranges::range R>
    requires(std::convertible_to<std::ranges::range_value_t<R>, CharT>)
  [[nodiscard]] explicit constexpr basic_fixed_string(std::from_range_t, R&& container) noexcept
  {
    MTP_EXPECTS(std::ranges::size(container) == N);
    std::ranges::move(container, _data);
  }

  [[nodiscard]] constexpr basic_fixed_string(basic_fixed_string const&) noexcept = default;

  [[nodiscard]] constexpr auto operator=(basic_fixed_string const&) noexcept
      -> basic_fixed_string& = default;

  [[nodiscard]] constexpr auto
  view() const noexcept -> std::basic_string_view<CharT>
  {
    return std::basic_string_view<CharT>{ data(), size() };
  }

  [[nodiscard]] constexpr
  operator std::basic_string_view<CharT>() const noexcept
  {
    return view();
  }

  [[nodiscard]] constexpr auto
  data() const noexcept -> const_pointer
  {
    return static_cast<const_pointer>(_data);
  }

  [[nodiscard]] constexpr auto
  c_str() const noexcept -> const_pointer
  {
    return data();
  }

  [[nodiscard]] constexpr auto
  cbegin() const noexcept -> const_iterator
  {
    return data();
  }

  [[nodiscard]] constexpr auto
  cend() const noexcept -> const_iterator
  {
    return data() + size();
  }

  [[nodiscard]] constexpr auto
  begin() const noexcept -> const_iterator
  {
    return cbegin();
  }

  [[nodiscard]] constexpr auto
  end() const noexcept -> const_iterator
  {
    return cend();
  }

  [[nodiscard]] constexpr auto
  crbegin() const noexcept -> const_reverse_iterator
  {
    return const_reverse_iterator{ end() };
  }

  [[nodiscard]] constexpr auto
  crend() const noexcept -> const_reverse_iterator
  {
    return const_reverse_iterator{ begin() };
  }

  [[nodiscard]] constexpr auto
  rbegin() const noexcept -> const_reverse_iterator
  {
    return crbegin();
  }

  [[nodiscard]] constexpr auto
  rend() const noexcept -> const_reverse_iterator
  {
    return crend();
  }

  [[nodiscard]] constexpr auto
  operator[](size_type pos) const noexcept -> const_reference
  {
    MTP_EXPECTS(pos < size());
    return _data[pos];
  }

  [[nodiscard]] constexpr auto
  at(size_type pos) const MTP_NOEXCEPT -> const_reference
  {
#ifdef MTP_NO_EXCEPTIONS
    MTP_EXPECTS(pos < size());
#else
    if (pos >= size()) {
      throw std::out_of_range("mtp::basic_fixed_string::at");
    }
#endif
    return (*this)[pos];
  }

  [[nodiscard]] constexpr auto
  front() const noexcept -> const_reference
  {
    MTP_EXPECTS(!empty());
    return (*this)[0];
  }

  [[nodiscard]] constexpr auto
  back() const noexcept -> const_reference
  {
    MTP_EXPECTS(!empty());
    return (*this)[size() - 1];
  }

  constexpr auto
  swap(basic_fixed_string& fs) noexcept -> void
  {
    std::ranges::swap_ranges(_data, _data + size(), fs._data, fs._data + fs.size());
  }

  template <std::size_t N2>
  [[nodiscard]] friend constexpr auto
  operator+(basic_fixed_string const& lhs, basic_fixed_string<CharT, N2> const& rhs) noexcept
      -> basic_fixed_string<CharT, N + N2>
  {
    CharT str[N + N2] = {};
    auto it = std::ranges::copy(lhs.begin(), lhs.end(), str).out;
    it = std::ranges::copy(rhs.begin(), rhs.end(), it).out;
    return basic_fixed_string<CharT, N + N2>{ str, it };
  }

  [[nodiscard]] friend constexpr auto
  operator+(basic_fixed_string const& lhs, CharT rhs) noexcept -> basic_fixed_string<CharT, N + 1>
  {
    CharT str[N + 1] = {};
    auto it = std::ranges::copy(lhs.begin(), lhs.end(), str).out;
    *it++ = rhs;
    return basic_fixed_string<CharT, N + 1>{ str, it };
  }

  [[nodiscard]] friend constexpr auto
  operator+(CharT lhs, basic_fixed_string const& rhs) noexcept -> basic_fixed_string<CharT, 1 + N>
  {
    CharT str[1 + N] = {};
    auto it = std::ranges::begin(str);
    *it++ = lhs;
    it = std::ranges::copy(rhs.begin(), rhs.end(), it).out;
    return basic_fixed_string<CharT, 1 + N>{ str, it };
  }

  template <std::size_t N2>
  [[nodiscard]] friend consteval auto
  operator+(basic_fixed_string const& lhs, CharT const (&rhs)[N2]) noexcept
      -> basic_fixed_string<CharT, N + (N2 - 1)>
  {
    MTP_EXPECTS(rhs[N2 - 1] == CharT{});

    CharT str[N + (N2 - 1)] = {};
    auto it = std::ranges::copy(lhs.begin(), lhs.end(), str).out;
    it = std::ranges::copy(rhs, rhs + N2 - 1, it).out;
    return basic_fixed_string<CharT, N + (N2 - 1)>{ str, it };
  }

  template <std::size_t N2>
  [[nodiscard]] friend consteval auto
  operator+(CharT const (&lhs)[N2], basic_fixed_string const& rhs) noexcept
      -> basic_fixed_string<CharT, (N2 - 1) + N>
  {
    MTP_EXPECTS(lhs[N2 - 1] == CharT{});

    CharT str[(N2 - 1) + N] = {};
    auto it = std::ranges::copy(lhs, lhs + N2 - 1, str).out;
    it = std::ranges::copy(rhs.begin(), rhs.end(), it).out;
    return basic_fixed_string<CharT, (N2 - 1) + N>{ str, it };
  }

  template <std::size_t N2>
  [[nodiscard]] friend constexpr auto
  operator==(basic_fixed_string const& lhs, basic_fixed_string<CharT, N2> const& rhs) noexcept
      -> bool
  {
    return lhs.view() == rhs.view();
  }

#ifdef MTP_HAS_THREE_WAY_COMPARE
  template <std::size_t N2>
  [[nodiscard]] friend constexpr auto
  operator<=>(basic_fixed_string const& lhs, basic_fixed_string<CharT, N2> const& rhs) noexcept
      -> std::strong_ordering
  {
    return lhs.view() <=> rhs.view();
  }
#endif

  template <std::size_t N2>
  [[nodiscard]] friend consteval auto
  operator==(basic_fixed_string const& lhs, CharT const (&rhs)[N2]) noexcept -> bool
  {
    MTP_EXPECTS(rhs[N2 - 1] == CharT{});
    return lhs.view() == std::basic_string_view<CharT>{ rhs, N2 - 1 };
  }

#ifdef MTP_HAS_THREE_WAY_COMPARE
  template <std::size_t N2>
  [[nodiscard]] friend consteval auto
  operator<=>(basic_fixed_string const& lhs, CharT const (&rhs)[N2]) noexcept
      -> std::strong_ordering
  {
    MTP_EXPECTS(rhs[N2 - 1] == CharT{});
    return lhs.view() <=> std::basic_string_view<CharT>{ rhs, N2 - 1 };
  }
#endif

  friend auto
  operator<<(std::basic_ostream<CharT>& os, basic_fixed_string const& fs) noexcept
      -> std::basic_ostream<CharT>&
  {
    return os << fs.view();
  }
};

namespace concepts {

template <typename T, typename... Ts>
concept any_of = (... || std::same_as<T, Ts>);

template <typename T>
concept char_type =
#ifdef MTP_HAS_CHAR8_TYPE
    any_of<T, char, wchar_t, char8_t, char16_t, char32_t>;
#else
    any_of<T, char, wchar_t, char16_t, char32_t>;
#endif

} // namespace concepts

MTP_EXPORT template <concepts::char_type CharT, std::convertible_to<CharT>... CharTs>
basic_fixed_string(CharT, CharTs...) -> basic_fixed_string<CharT, 1 + sizeof...(CharTs)>;

MTP_EXPORT template <concepts::char_type CharT, std::size_t N>
basic_fixed_string(CharT const (&)[N]) -> basic_fixed_string<CharT, N - 1>;

MTP_EXPORT template <concepts::char_type CharT, std::size_t N>
basic_fixed_string(std::from_range_t, std::array<CharT, N>) -> basic_fixed_string<CharT, N>;

} // namespace mtp

namespace std {

MTP_EXPORT template <typename CharT, size_t N>
constexpr auto
swap(::mtp::basic_fixed_string<CharT, N>& a, ::mtp::basic_fixed_string<CharT, N>& b) noexcept
    -> void
{
  a.swap(b);
}

MTP_EXPORT template <size_t N>
struct hash<::mtp::fixed_string<N>> : hash<string_view>
{};

MTP_EXPORT template <size_t N>
struct hash<::mtp::fixed_wstring<N>> : hash<wstring_view>
{};

#ifdef MTP_HAS_CHAR8_TYPE
MTP_EXPORT template <size_t N>
struct hash<::mtp::fixed_u8string<N>> : hash<u8string_view>
{};
#endif

MTP_EXPORT template <size_t N>
struct hash<::mtp::fixed_u16string<N>> : hash<u16string_view>
{};

MTP_EXPORT template <size_t N>
struct hash<::mtp::fixed_u32string<N>> : hash<u32string_view>
{};

#ifdef MTP_HAS_FORMAT
MTP_EXPORT template <typename CharT, size_t N>
struct formatter<::mtp::basic_fixed_string<CharT, N>> : formatter<basic_string_view<CharT>>
{
  template <typename format_context>
  auto
  format(::mtp::basic_fixed_string<CharT, N> const& fs, format_context& ctx) const
      -> decltype(ctx.out())
  {
    return formatter<basic_string_view<CharT>>::format(basic_string_view<CharT>(fs), ctx);
  }
};
#endif

} // namespace std

// -------------------------------------------------------------------------------------------------

#undef MTP_HAS_THREE_WAY_COMPARE
#undef MTP_HAS_CHAR8_TYPE
#undef MTP_HAS_FROM_RANGE
#undef MTP_HAS_FORMAT
#undef MTP_HAS_INCLUDE
#undef MTP_NOEXCEPT
#undef MTP_EXPECTS
#undef MTP_EXPORT

// -------------------------------------------------------------------------------------------------

#endif // MTP_FIXED_STRING_HPP
