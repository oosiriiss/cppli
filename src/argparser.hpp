#pragma once
#include "option.hpp"
#include <expected>
#include <optional>
#include <span>
#include <string_view>
#include <unordered_map>
#include <variant>

namespace cli {
struct Option;

struct ShortOption {
  char opt;
};
struct MultiShortOption {
  std::string_view opts;
};

struct LongOption {
  std::string_view opt;
};

struct Value {
  std::string_view val;
};

using Argument = std::variant<ShortOption, MultiShortOption, LongOption, Value>;
class ArgvParser {

public:
  ArgvParser(std::span<std::string_view> argv);

  // Tries to match an option from argv
  // Maps (really just the option) are only altered if an option is matched and
  // it's raw_value is set.
  std::optional<Option> MatchOptions(OptionStorage &options);

private:
  // Parses the option
  // Cannot be const, because it may read next argv if option has argument
  std::optional<Option> Parse(Option &opt);

  // Actual parsing is done by ArgvParser::Parse
  // Maps (really just the option) are only altered if an option is matched
  // successfully. Cannot be const, because it may read next argv if option has
  // argument
  std::optional<Option> ParseShort(const ShortOption &opt,
                                   OptionStorage &shortOptions);

  // Actual parsing is done by ArgvParser::Parse
  // Maps (really just the option) are only altered if an option is matched
  // successfully. Cannot be const, because it may read next argv if option has
  // argument
  std::optional<Option> ParseLong(const LongOption &opt,
                                  OptionStorage &longOptions);

  // Chains of short options e.g. -abcdefg
  // If any option expects a value this method immediately returns nullopt as it
  // is not supported (yet? idk how would that even work and it would be
  // confusing).
  // Actual parsing is done by ArgvParser::Parse Maps (really just
  // the option) are only altered if an option is matched successfully. Cannot
  // be const, because it may read next argv if option has argument
  std::optional<Option> ParseMultiShort(const MultiShortOption &opts,
                                        OptionStorage &shortOptions);

  // This reads the next argv and returns it if it is a value
  // TODO :: I mean this looks like a perfect usage for std::expected, but i
  // didn't had any meaningful errors returned as i was writing this.
  std::optional<Value> ReadExpectValue();
  std::optional<Argument> ReadArg();
  std::span<std::string_view> argv;
  size_t arg_index;
};

} // namespace cli
