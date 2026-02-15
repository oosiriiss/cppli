#pragma once
#include <cstdint>
#include <functional>
#include <string_view>

#include "option.hpp"

namespace internal {
  // Parsed type of argument from i.e. argv
  enum class ArgType : std::uint8_t {
    //
    Positional,
    // TODO :: Add support for commands
    // Command,
    // Flag is the same as option but without a value for now.
    // Flag,
    //
    Option
  };

  [[nodiscard]] constexpr auto hasOptionFormat(
      std::string_view argument) noexcept -> bool {
    return argument.starts_with('-');
  }

  [[nodiscard]] constexpr auto classifyArgument(
      std::string_view argument) noexcept -> ArgType {
    if (hasOptionFormat(argument)) {
      return ArgType::Option;
    }
    return ArgType::Positional;
  }
}  // namespace internal
