#include "cppli/option.hpp"

#include <format>
#include <functional>
#include <optional>
#include <stdexcept>

namespace cppli {

  void Option::Matched() const {
    if (callback) {
      (*callback)(rawValue);
    }
  }

  void OptionStorage::Add(const Option::Identifier& identifier, Option&& opt) {
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

  bool OptionStorage::Contains(char shortOption) const {
    return shortIndexMap_.contains(shortOption);
  }

  bool OptionStorage::Contains(std::string_view longOption) const {
    return longIndexMap_.contains(longOption);
  }

  bool OptionStorage::Contains(const Option::Identifier& identifier) const {
    return Contains(identifier.shortName) || Contains(identifier.longName);
  }

  Option& OptionStorage::operator[](char shortOption) {
    size_t index = shortIndexMap_[shortOption];
    return options_[index];
  }

  Option& OptionStorage::operator[](std::string_view longOption) {
    size_t index = longIndexMap_[longOption];
    return options_[index];
  }
}  // namespace cppli
