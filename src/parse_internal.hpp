#pragma once
#include <cstdint>
#include <optional>
#include <string_view>

#if defined(ENABLE_DEBUG_UTILS)
#include "debug_utils.hpp"
#endif

namespace cppli {
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

    [[nodiscard]] constexpr auto splitNameValue(
        std::string_view argument) noexcept
        -> std::pair<std::string_view, std::optional<std::string_view>> {
      size_t eqIndex = argument.find('=');

      std::string_view name = argument.substr(0, eqIndex);
      std::optional<std::string_view> value;
      // Removing name and '='
      argument.remove_prefix(name.size());
      argument.remove_prefix((argument.starts_with('=') ? 1UZ : 0UZ));
      if (!argument.empty()) {
        value.emplace(argument);
      }
      return std::pair{name, value};
    }

    [[nodiscard]] constexpr auto getRelevantValue(
        std::optional<std::string_view>& higherPriorityValue,
        std::optional<std::string_view>& lowerPriorityValue) noexcept
        -> std::optional<std::string_view>& {
      if (higherPriorityValue) {
        return higherPriorityValue;
      }
      return lowerPriorityValue;
    }
  }  // namespace internal
}  // namespace cppli
