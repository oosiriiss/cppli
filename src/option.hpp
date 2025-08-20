#pragma once
#include <functional>
#include <optional>
#include <string>

namespace cli {
class Option {

  // Function used when calling an option's callback
  using OptionCallback = std::function<void(std::string_view)>;

public:
  struct Identifier {
    // Short name for the option like -h (But without the dash '-')
    char shortName;
    // Long name for the option like --help (But without the double dashes dash
    // '-')
    // TODO :: string_view?
    std::string_view longName;
  };
  // Gets called when option is used in App args.
  void Matched() const;

  // Short name should  be a letter like h. It will react -h option.
  // Long name should  be a String like help. It will react to --help option.
  // Description should be a string describing what does this option do.
  Option(const std::string description, bool expectsArgument,
         std::optional<OptionCallback> callback = std::nullopt);

public:
  // Mostly kept here for printing purposes, but since its relative light it
  // should matter too much
  Identifier identifier;

  // Description of what the option is used for.
  const std::string_view description;

  // Decides whether this option has an argument
  const bool expectsArgument;

  // Raw string value of the arugment;
  std::optional<std::string_view> rawValue = std::nullopt;

private:
  // Callback invoked when argument is discovered in the argv
  const std::optional<OptionCallback> callback_ = std::nullopt;
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
