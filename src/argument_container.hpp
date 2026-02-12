#pragma once
#include <cassert>
#include <stdexcept>
#include <string_view>

struct ArgumentContainer {
 public:
  constexpr ArgumentContainer(int argc, char const* const* const argv)
      : argc_(argc),
        argv_(argv) {}

  [[nodiscard]] constexpr auto operator[](size_t index) const
      -> std::string_view {
    // TODO :: debug only exception?
    if (index >= argc_ || index < 0) {
      throw std::invalid_argument("Argument index outside of bounds");
    }
    assert(index >= 0 && index < argc_);
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
