#pragma once


//

#ifdef CPPLI_ENABLE_LOG
#include <format>
#include <optional>
#include <utility>
#include <print>
#include <string_view>

// TODO :: Log levels

enum class LogLevel {
  Info,
  Warning,
  Error,
  Debug,
};
namespace logging::internal {

constexpr std::string_view toString(const LogLevel level) {
  switch (level) {
  case LogLevel::Info:
    return "INFO";
  case LogLevel::Warning:
    return "WARNING";
  case LogLevel::Error:
    return "ERROR";
  case LogLevel::Debug:
    return "DEBUG";
  }
  std::unreachable();
}
// Returns a string that will allow the output of a colored text to the
// terminal.
// Ansi Color codes
// https://en.wikipedia.org/wiki/ANSI_escape_code#8-bit
constexpr std::string_view textColor(const LogLevel level) {
  const std::string_view FG_RED_BG_BLACK = "\033[31;40m";
  const std::string_view FG_CYAN_BG_BLACK = "\033[36;40m";
  const std::string_view FG_YELLOW_BG_BLACK = "\033[33;40m";
  const std::string_view FG_GRAY_BG_BLACK = "\033[90;40m";

  switch (level) {
  case LogLevel::Info:
    return FG_CYAN_BG_BLACK;
  case LogLevel::Warning:
    return FG_YELLOW_BG_BLACK;
  case LogLevel::Error:
    return FG_RED_BG_BLACK;
  case LogLevel::Debug:
    return FG_GRAY_BG_BLACK;
  }
  std::unreachable();
}

// Returns a string that will reset all colorings enabled by textColor
// ANSI reset color code.
// https://en.wikipedia.org/wiki/ANSI_escape_code#8-bit
constexpr std::string_view textColorEnd() { return "\033[39;49m"; }

// Returns a already formatted log level with applied terminal colors
constexpr std::string formatLogLevel(const LogLevel level) {
  const std::string_view levelStr = toString(level);
  const std::string_view color = textColor(level);
  const std::string_view colorEnd = textColorEnd();

  return std::format("[{}{}{}]", color, levelStr, colorEnd);
}

} // namespace logging::internal

#define CPPLI_LOG_INFO(fmt, ...)                                               \
  logInternalFormat(LogLevel::Info, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define CPPLI_LOG_WARNING(fmt, ...)                                            \
  logInternalFormat(LogLevel::Warning, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define CPPLI_LOG_ERROR(fmt, ...)                                              \
  logInternalFormat(LogLevel::Error, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define CPPLI_LOG_DEBUG(fmt, ...)                                              \
  logInternalFormat(LogLevel::Debug, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

template <typename... Args>
inline void logInternalFormat(LogLevel level, const std::string_view file,
                              size_t line, const std::string_view fmt,
                              Args &&...args) {
  const std::string formattedLevel = logging::internal::formatLogLevel(level);
  // prepend file:line, then append fmt
  std::println("{}:{}:{} | {}", formattedLevel, file, line,
               std::vformat(fmt, std::make_format_args(args...)));
}

template <typename T> struct std::formatter<std::optional<T>> {
  constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }
  auto format(const std::optional<T> &opt, std::format_context &ctx) const {
    if (opt) {
      return std::format_to(ctx.out(), "{}", *opt);
    } else {
      return std::format_to(ctx.out(), "{}", "std::optional(nullopt)");
    }
  }
};

#else
#define CPPLI_LOG_DEBUG(fmt, ...)

#endif
