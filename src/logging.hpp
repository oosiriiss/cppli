#pragma once

#ifdef CPPLI_ENABLE_LOG
#include <print>
#include <string_view>

#define CPPLI_LOG_DEBUG(msg) logInternal(__FILE__, __LINE__, msg)

void logInternal(std::string_view file, size_t line, std::string msg) {
  std::println("{}:{} | {}", file, line, msg);
}

#else
#define CPPLI_LOG_DEBUG(msg)

#endif
