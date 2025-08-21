#pragma once
#include <any>
#include <functional>
#include <optional>
#include <string>

namespace cli {
struct Option {

  // Function used when calling an option's callback
  using OptionCallback = std::function<void(std::optional<std::string_view>)>;

  struct Identifier {
    // Short name for the option like -h (But without the dash '-')
    char shortName;
    // Long name for the option like --help (But without the double dashes dash
    // '-')
    // TODO :: string_view?
    std::string_view longName;
  };

  template <typename T> struct Params {

    std::optional<std::string_view> description = std::nullopt;

    std::optional<T> defaultValue = std::nullopt;

    // TODO :: Hm this could be made std::invocable
    std::optional<std::function<void(T)>> callback = std::nullopt;
  };

  // Gets called after and option is discovered from argv.
  // If rawValue is std::nullopt then the default value will be used.
  void Matched() const;

  // Mostly kept here for printing purposes, but since its relative light it
  // should matter too much
  // Identifier identifier;

  // Description of what the option is used for.
  std::string_view description;

  // Decides whether this option has an argument
  bool expectsArgument;

  // Raw string value of the arugment;
  std::optional<std::string_view> rawValue = std::nullopt;

  // Callback invoked when argument is discovered in the argv
  std::optional<OptionCallback> callback = std::nullopt;
};

class OptionStorage {

public:
  void Add(Option::Identifier &&identifier, Option &&oopt);

  bool Contains(char shortOption) const;
  bool Contains(std::string_view longOption) const;
  bool Contains(const Option::Identifier &identifier) const;

  Option &operator[](char shortOption);
  Option &operator[](std::string_view longOption);

private:
  char ConvertLongToShort(std::string_view longOption) const;

  std::vector<Option> options_;
  std::unordered_map<char, size_t> shortIndexMap_;
  std::unordered_map<std::string_view, size_t> longIndexMap_;
};

}; // namespace cli
