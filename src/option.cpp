#include "option.hpp"
#include <format>
#include <stdexcept>

namespace cli {
Option::Option(const std::string description, bool expectsArgument,
               std::optional<OptionCallback> callback)
    : description(description), expectsArgument(expectsArgument), rawValue(""),
      callback_(callback) {}

void Option::Matched() const {
  if (callback_ && rawValue) {
    (*callback_)(*rawValue);
  }
}

void OptionStorage::Add(Option::Identifier &&identifier, Option &&opt) {

  if (longIndexMap_.contains(identifier.longName)) {
    throw std::logic_error(std::format("Duplicated long option \"{}\" found.",
                                       identifier.longName));
  }
  if (shortIndexMap_.contains(identifier.shortName)) {
    throw std::logic_error(std::format("Duplicated short option \"{}\" found.",
                                       identifier.shortName));
  }

  const size_t index = options_.size();

  options_.emplace_back(std::move(opt));

  shortIndexMap_.emplace(std::move(identifier.shortName), index);
  longIndexMap_.emplace(std::move(identifier.longName), index);
}

bool OptionStorage::Contains(char shortOption) const {
  return shortIndexMap_.contains(shortOption);
}

bool OptionStorage::Contains(std::string_view longOption) const {
  return longIndexMap_.contains(longOption);
}

bool OptionStorage::Contains(const Option::Identifier &identifier) const {
  return Contains(identifier.shortName) || Contains(identifier.longName);
}

Option &OptionStorage::operator[](char shortOption) {

  int index = shortIndexMap_[shortOption];
  return options_[index];
}
Option &OptionStorage::operator[](std::string_view longOption) {

  int index = longIndexMap_[longOption];
  return options_[index];
}
} // namespace cli
