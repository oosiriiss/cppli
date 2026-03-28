#pragma once
#include <algorithm>
#include <format>
#include <optional>
#include <ranges>
#include <stdexcept>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "argument_container.hpp"
#include "help.hpp"
#include "option.hpp"
#include "parse_internal.hpp"
#include "vendor/debug_utils.hpp"

namespace cppli {
  struct OptionValue;

  // TODO :: Thinks of a better name
  struct OptionValue {
    std::optional<std::string_view> value;

    [[nodiscard]] constexpr auto operator==(const OptionValue& other) const
        -> bool {
      return value == other.value;
    }
  };

  template <class OptionKey>
  struct ParseResult {
    std::vector<std::string_view> positionals;
    std::unordered_map<OptionKey, OptionValue> options;
  };

  namespace internal {
    template <class OptionKey>
    constexpr void handlePositional(
        std::string_view positional, ParseResult<OptionKey>& result,
        std::optional<std::string_view>& lastPositional) noexcept {
      DEBUG_ASSERT(classifyArgument(positional) ==
                   internal::ArgType::Positional);
      // Previous positional argument was not used by option, so it is a real
      // positional argument and not a value for option
      if (lastPositional) {
        result.positionals.emplace_back(*lastPositional);
      }

      // Temporary storage in lastPositional
      lastPositional.emplace(positional);
    }

    // TODO :: Error handling, For now this just throws std::runtime_error and
    // preferrably I would like to make it Error as Value with std::expected
    template <class OptionKey>
    constexpr void handleOption(
        std::string_view argument,
        std::optional<std::string_view>&
            value,  // Value will be set to nullopt if consumed
        const OptionContainer<OptionKey>& definedOptions,
        ParseResult<OptionKey>& result) {
      DEBUG_ASSERT(classifyArgument(argument) == internal::ArgType::Option);
      auto optionDataOpt = definedOptions.matchOptionByName(argument);
      if (!optionDataOpt) {
        throw std::runtime_error(std::format("Unknown option: '{}'", argument));
      }

      std::optional<std::string_view> optionValue;
      auto& [optionKey, option] = *optionDataOpt;

      if (option.needsValue) {
        if (!value) {
          throw std::runtime_error(
              std::format("Option '{}' requires a value.", argument));
        }
        optionValue.emplace(*value);
      }

      // TODO :: Handle Option values
      result.options.emplace(optionKey, optionValue);
    }
  };  // namespace internal

  // TODO :: Error on parse fail
  template <class OptionKey>
  [[nodiscard]] constexpr auto parseArguments(
      int argc, const char* const* const argv,
      const OptionContainer<OptionKey>& definedOptions)
      -> ParseResult<OptionKey> {
    ParseResult<OptionKey> result;

    using internal::ArgType;
    using internal::ArgumentContainer;
    using internal::classifyArgument;
    using internal::getRelevantValue;
    using internal::handleOption;
    using internal::handlePositional;
    using internal::splitNameValue;

    // TODO :: Allow stopping parsing if '--' encountered.

    ArgumentContainer args(argc, argv);

    // Positionals are first inserted here, as they might be a value for option.
    std::optional<std::string_view> lastPositional;

    for (std::string_view argument : args | std::views::reverse) {
      auto [name, valueOpt] = splitNameValue(argument);

      auto& valueRef = getRelevantValue(valueOpt, lastPositional);

      ArgType type = classifyArgument(name);

      switch (type) {
        case ArgType::Positional:
          handlePositional(argument, result, lastPositional);
          break;
        case ArgType::Option:
          handleOption(name, valueRef, definedOptions, result);
          break;
      }
    }

    // Flushing positional
    if (lastPositional) {
      result.positionals.emplace_back(*lastPositional);
    }
    // Positionals in order they were specified and not reversed
    std::ranges::reverse(result.positionals);

    return result;
  };
}  // namespace cppli
