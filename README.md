# fixed-string (mtp-libs)

A c++20 implementation of `basic_fixed_string` as described by [proposal p3094](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2024/p3094r5.html).


## Example

A common utility of a `fixed_string` is passing string literals as non-type template parameters:

```cpp
#include <iostream>
#include <mtp/fixed_string.hpp>

template <auto... fixstrs>
consteval auto
concat_fixstrs()
{
  // requires compiler supporting c++20 `CTAD for alias templates`
  // see: https://en.cppreference.com/w/cpp/compiler_support/20#cpp_deduction_guides_201907L
  return mtp::fixed_string{ "" } + ... + fixstrs;

  // otherwise use:
  // return mtp::fixed_string<0>{ "" } + ... + fixstrs;
}

auto main() -> int
{
  constexpr auto fs = concat_fixstrs<"Hello, ", "World!">();
  std::cout << fs << '\n'; // "Hello, World!\n"
  return 0;
}
```

See [tests](/test/fixed_string_tests.cpp) for more examples.


## Modules Support

A module interface unit is provided [module](/module/fixed_string.cppm).

The included `CMakeLists.txt` supports building this library as a module using the `MTP_BUILD_MODULE` option.

See [`configure.sh`](/configure.sh) for an example configuration command that builds the library using modules.

If the tests are also built (using the `MTP_BUILD_TEST` option), the tests will consume the library via the module as well.


## Links

1. [`basic_fixed_string` proposal p3094](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2024/p3094r5.html)
