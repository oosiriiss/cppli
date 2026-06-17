#pragma once

#if defined(CPPLI_ENABLE_DEBUG_UTILS)

#include <cstdlib>
#include <iostream>
#include <print>  // TODO :: The print header can be heavy on comp time, fix this if it becomes a problem
#include <source_location>
#include <stacktrace>

namespace cppli {
  namespace debugutils {
    constexpr bool DEBUG = false;  // NOLINT
  }  // namespace debugutils

#define CPPLI_DEBUG_ONLY(...) __VA_ARGS__

  [[noreturn]] inline void handleAssertFail(
      std::string_view expr, std::string_view message,
      const std::source_location& sourceLoc = std::source_location::current(),
      const std::stacktrace& stacktrace = std::stacktrace::current()) {
    std::println(std::cerr, "\nAssertion failed");
    std::println(std::cerr, "Expression: {}", expr);
    if (!message.empty()) {
      std::println(std::cerr, "Message: {}", message);
    }
    std::println("File: {}", sourceLoc.file_name());
    std::println("Line: {}", sourceLoc.line());
    std::println("Stacktrace:\n{}", std::to_string(stacktrace));
    std::abort();
  }

#define CPPLI_GET_FIRST(First, ...) First  // NOLINT

#define CPPLI_DEBUG_ASSERT(expr, ...) \
  (static_cast<bool>(expr)            \
       ? void(0)                      \
       : handleAssertFail(#expr, CPPLI_GET_FIRST(__VA_ARGS__ __VA_OPT__(, ) "")));

}  // namespace cppli
#else

namespace cppli {
  namespace debugutils {
    constexpr bool DEBUG = true;  // NOLINT
  }  // namespace debugutils

#define CPPLI_DEBUG_ONLY(...)
#define CPPLI_DEBUG_ASSERT(...)
}  // namespace cppli
#endif
