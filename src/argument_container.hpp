#pragma once
#include <cassert>
#include <stdexcept>
#include <string_view>

#include "debug_utils.hpp"

struct ArgumentContainer {
 public:
  constexpr ArgumentContainer(int argc, char const* const* const argv)
      : argc_(argc),
        argv_(argv) {}

  [[nodiscard]] constexpr auto operator[](size_t index) const
      -> std::string_view {
    // TODO :: debug only exception?
    DEBUG_ONLY(if (index >= argc_ || index < 0) {
      throw std::invalid_argument("Argument index outside of bounds");
    });
    return argv_[index];
  }

  /**
   * Returns the number of program arguments
   */
  [[nodiscard]] constexpr auto count() const noexcept -> int { return argc_; }

 private:
  int argc_{0};
  char const* const* argv_{nullptr};
};
