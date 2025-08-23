#pragma once

#include <format>
#ifdef CPPLI_ENABLE_LOG
#include <print>
#include <string_view>

#define CPPLI_LOG_DEBUG(fmt, ...)                                              \
  logInternalFormat(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

template <typename... Args>
inline void logInternalFormat(const std::string_view file, size_t line,
                              const std::string_view fmt, Args &&...args) {
  // prepend file:line, then append fmt
  std::println("{}:{} | {}", file, line,
               std::vformat(fmt, std::make_format_args(args...)));
}

#else
#define CPPLI_LOG_DEBUG(fmt, ...)

#endif
