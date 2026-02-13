#pragma once

#if defined(ENABLE_DEBUG_UTILS)

namespace debugutils {
  constexpr bool DEBUG = false;  // NOLINT
}  // namespace debugutils

#define DEBUG_ONLY(...)

#else

namespace debugutils {
  constexpr bool DEBUG = true;  // NOLINT
}  // namespace debugutils

#define DEBUG_ONLY(...) __VA_ARGS__

#endif
