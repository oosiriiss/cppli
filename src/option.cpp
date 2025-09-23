#include "cppli/option.hpp"

#include <cstddef>
#include <format>
#include <functional>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <utility>

namespace cppli {

  void Option::matched() const {
    if (callback) {
      (*callback)(rawValue);
    }
  }

  void OptionStorage::add(const Option::Identifier& identifier, Option&& opt) {
    if (longIndexMap_.contains(identifier.longName)) {
      throw std::logic_error(std::format("Duplicated long option \"{}\" found.",
                                         identifier.longName));
    }
    if (shortIndexMap_.contains(identifier.shortName)) {
      throw std::logic_error(std::format(
          "Duplicated short option \"{}\" found.", identifier.shortName));
    }

    const size_t index = options_.size();

    options_.emplace_back(std::move(opt));

    shortIndexMap_.emplace(identifier.shortName, index);
    longIndexMap_.emplace(identifier.longName, index);
  }

  bool OptionStorage::contains(char shortOption) const {
    return shortIndexMap_.contains(shortOption);
  }

  bool OptionStorage::contains(std::string_view longOption) const {
    return longIndexMap_.contains(longOption);
  }

  bool OptionStorage::contains(const Option::Identifier& identifier) const {
    return contains(identifier.shortName) || contains(identifier.longName);
  }

  const Option& OptionStorage::get(char shortOption) const {
    const size_t index = shortIndexMap_.at(shortOption);
    return options_.at(index);
  }

  const Option& OptionStorage::get(std::string_view longOption) const {
    const size_t index = longIndexMap_.at(longOption);
    return options_.at(index);
  }
  Option& OptionStorage::get(char shortOption) {
    const size_t index = shortIndexMap_.at(shortOption);
    return options_.at(index);
  }

  Option& OptionStorage::get(std::string_view longOption) {
    const size_t index = longIndexMap_.at(longOption);
    return options_.at(index);
  }
}  // namespace cppli
