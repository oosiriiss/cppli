#pragma once
#include <cassert>
#include <iterator>
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

 public:
  struct ArgIterator {
    using iterator_category = std::bidirectional_iterator_tag;  // NOLINT
    using difference_type = std::ptrdiff_t;                     // NOLINT
    using value_type = std::string_view;                        // NOLINT

    ArgIterator() = default;
    ArgIterator(const char* const* data)  // NOLINT
        : data_(data) {}

    [[nodiscard]] constexpr auto operator*() const -> value_type {
      return std::string_view(*data_);
    }
    [[nodiscard]] constexpr auto operator++() -> ArgIterator& {
      ++data_;
      return *this;
    }
    [[nodiscard]] constexpr auto operator++(int) -> ArgIterator {
      ArgIterator temp = *this;
      ++data_;
      return temp;
    }
    [[nodiscard]] constexpr auto operator--() -> ArgIterator& {
      --data_;
      return *this;
    }
    [[nodiscard]] constexpr auto operator--(int) -> ArgIterator {
      ArgIterator temp = *this;
      --data_;
      return temp;
    }

    constexpr auto operator==(const ArgIterator& other) const -> bool {
      return data_ == other.data_;
    }

   private:
    const char* const* data_{nullptr};
  };

  using iterator = ArgIterator;  // NOLINT

  constexpr auto begin() -> iterator { return iterator(argv_); }
  constexpr auto end() -> iterator { return iterator(argv_ + argc_); }

};  // NOLINT
