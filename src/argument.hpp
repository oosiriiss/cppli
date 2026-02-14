#pragma once
#include <cstdint>
#include <format>
#include <optional>
#include <stdexcept>
#include <unordered_map>

#include "debug_utils.hpp"

// Parsed type of argument from i.e. argv
enum class ArgType : std::uint8_t {
  //
  Positional,
  // TODO :: Add support for commands
  Command,
  //
  Flag,
  //
  Option
};

// Option defined by user
struct Option {
  std::string_view firstName;
  std::string_view secondName;
};

// Container for options defined by user
template <class OptionKey>
struct ArgumentContainer {
  // TODO :: ?Constexpr? Fixed size map for arguments, as they all should be
  // known at compile time

  using Container = std::unordered_map<OptionKey, Option>;

  constexpr void addOption(OptionKey key, Option option);

  [[nodiscard]] constexpr auto matchOptionByName(
      std::string_view name) const noexcept -> std::optional<Option>;

 private:
  Container options_;

  std::unordered_map<std::string_view, OptionKey> nameKeyTranslations_;
};

template <class OptionKey>
constexpr void ArgumentContainer<OptionKey>::addOption(OptionKey key,
                                                       Option option) {
  // TODO :: Make sure users can switch debug/non-debug version of this
  // library.

  // TODO :: I am not sure if these checks should be debug only, but if the
  // future user would use this, these kinds of errors should only occur during
  // development, so no need to bloat the final executable?
  DEBUG_ONLY(if (options_.contains(key)) {
    if constexpr (std::formattable<OptionKey, char>) {
      throw std::invalid_argument(
          std::format("Attempted to define option with key {} twice.", key));
    } else {
      throw std::invalid_argument(
          "Attepted to define option with the same key twice");
    }
  });
  // Names already taken
  DEBUG_ONLY(if (nameKeyTranslations_.contains(option.firstName) ||
                 nameKeyTranslations_.contains(option.secondName)) {
    if constexpr (std::formattable<OptionKey, char>) {
      OptionKey otherOptionKey =
          (nameKeyTranslations_.contains(option.firstName))
              ? nameKeyTranslations_.at(option.firstName)
              : nameKeyTranslations_.at(option.secondName);
      throw std::invalid_argument(std::format(
          "Option with key '{}' has defined a name ('{}' or "
          "'{}') which was already used by option with key '{}'",
          key, option.firstName, option.secondName, otherOptionKey));
    } else {
      throw std::invalid_argument(std::format(
          "Option with given name ('{}' or '{}') was already defined",
          option.firstName, option.secondName));
    }
  });

  options_.emplace(key, option);
  nameKeyTranslations_.emplace(option.firstName, key);
  nameKeyTranslations_.emplace(option.secondName, key);

  // TODO :: Insert translations
}

template <class OptionKey>
[[nodiscard]] constexpr auto ArgumentContainer<OptionKey>::matchOptionByName(
    std::string_view name) const noexcept -> std::optional<Option> {
  // TODO :: return Option& if they become relatively rage
  auto translationIterator = nameKeyTranslations_.find(name);

  // No translation found - unknown argument name
  if (translationIterator == nameKeyTranslations_.end()) {
    return std::nullopt;
  }
  DEBUG_ASSERT(options_.contains(translationIterator->second),
               "Option must exist if translation was found");
  return options_.at(translationIterator->second);
}
