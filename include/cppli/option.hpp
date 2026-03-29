#pragma once
#include <optional>
#include <stdexcept>
#include <string_view>
#include <unordered_map>
#include <utility>

#include "vendor/debug_utils.hpp"

namespace cppli {

  // Option defined by user
  struct Option {
    std::string firstName;
    std::string secondName;

    // description that will be used during help message generation
    std::string description;
    bool needsValue = false;
  };

  // Container for options defined by user
  template <class OptionKey>
  struct OptionContainer {
    // TODO :: ?Constexpr? Fixed size map for arguments, as they all should be
    // TODO :: Validate the defined options, check if they make sense.
    // known at compile time

    using Container = std::unordered_map<OptionKey, Option>;

    constexpr void addOption(OptionKey key, Option option);

    [[nodiscard]] constexpr auto matchOptionByName(std::string_view name)
        const noexcept -> std::optional<std::pair<OptionKey, Option>>;

    constexpr auto begin() const { return options_.begin(); }
    constexpr auto end() const { return options_.end(); }

   private:
    Container options_;

    std::unordered_map<std::string_view, OptionKey> nameKeyTranslations_;
  };

  template <class OptionKey>
  constexpr void OptionContainer<OptionKey>::addOption(OptionKey key,
                                                       Option option) {
    // TODO :: Make sure users can switch debug/non-debug version of this
    // library.

    // TODO :: I am not sure if these checks should be debug only, but if the
    // future user would use this, these kinds of errors should only occur
    // during development, so no need to bloat the final executable?
    DEBUG_ONLY(
        if (options_.contains(key)) {
          if constexpr (std::formattable<OptionKey, char>) {
            throw std::invalid_argument(std::format(
                "Attempted to define option with key {} twice.", key));
          } else {
            throw std::invalid_argument(
                "Attepted to define option with the same key twice");
          }
        }
        // Names already taken
        if (nameKeyTranslations_.contains(std::string_view{option.firstName}) ||
            nameKeyTranslations_.contains(
                std::string_view{option.secondName})) {
          if constexpr (std::formattable<OptionKey, char>) {
            OptionKey otherOptionKey =
                (nameKeyTranslations_.contains(
                    std::string_view{option.firstName}))
                    ? nameKeyTranslations_.at(
                          std::string_view{option.firstName})
                    : nameKeyTranslations_.at(
                          std::string_view{option.secondName});

            throw std::invalid_argument(std::format(
                "Option with key '{}' has defined a name ('{}' or "
                "'{}') which was already used by option with key '{}'",
                key, option.firstName, option.secondName, otherOptionKey));
          } else {
            throw std::invalid_argument(std::format(
                "Option with given name ('{}' or '{}') was already defined",
                option.firstName, option.secondName));
          }

          // Unreachable option
          if (option.firstName.empty() && option.secondName.empty()) {
            if constexpr (std::formattable<OptionKey, char>) {
              throw std::invalid_argument(
                  std::format("Option with key '{}' has both firstName and "
                              "secondName empty and "
                              "cannot be reached.",
                              key));
            } else {
              throw std::invalid_argument(
                  "Option with empty firstName and secondName is considered "
                  "invalid.");
            }
          }
        });

    // Adding the option

    options_.emplace(key, std::move(option));
    Option& insertedOption = options_[key];

    if (!insertedOption.firstName.empty()) {
      nameKeyTranslations_.emplace(std::string_view{insertedOption.firstName},
                                   key);
    }

    if (!insertedOption.secondName.empty()) {
      nameKeyTranslations_.emplace(std::string_view{insertedOption.secondName},
                                   key);
    }
  }

  template <class OptionKey>
  [[nodiscard]] constexpr auto OptionContainer<OptionKey>::matchOptionByName(
      std::string_view name) const noexcept
      -> std::optional<std::pair<OptionKey, Option>> {
    // TODO :: return Option& if they become relatively rage
    auto translationIterator = nameKeyTranslations_.find(name);

    // No translation found - unknown argument name
    if (translationIterator == nameKeyTranslations_.end()) {
      return std::nullopt;
    }
    DEBUG_ASSERT(options_.contains(translationIterator->second),
                 "Option must exist if translation was found");
    return std::pair{translationIterator->second,
                     options_.at(translationIterator->second)};
  }
}  // namespace cppli
