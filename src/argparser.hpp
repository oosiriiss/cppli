#pragma once
#include <optional>
#include <span>
#include <variant>

#include "cppli/option.hpp"

namespace cppli {
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

  using Argument =
      std::variant<ShortOption, MultiShortOption, LongOption, Value>;

  class ArgvParser {
   public:
    explicit ArgvParser(std::span<std::string_view> argv);

    // Tries to match an option from argv
    // Maps (really just the option) are only altered if an option is matched
    // and it's raw_value is set.
    std::optional<Option> MatchOptions(OptionStorage& options);

   private:
    // Parses the option
    // Cannot be const, because it may read next argv if option has argument
    bool Parse(Option& opt);

    // Actual parsing is done by ArgvParser::Parse
    // Maps (really just the option) are only altered if an option is matched
    // successfully. Cannot be const, because it may read next argv if option
    // has argument
    bool ParseShort(const ShortOption& opt, OptionStorage& options);

    // Actual parsing is done by ArgvParser::Parse
    // Maps (really just the option) are only altered if an option is matched
    // successfully. Cannot be const, because it may read next argv if option
    // has argument
    bool ParseLong(const LongOption& opt, OptionStorage& options);

    // Chains of short options e.g. -abcdefg
    // If any option expects a value this method immediately returns nullopt as
    // it is not supported (yet? idk how would that even work and it would be
    // confusing).
    // Actual parsing is done by ArgvParser::Parse Maps (really just
    // the option) are only altered if an option is matched successfully. Cannot
    // be const, because it may read next argv if option has argument
    static std::optional<Option> ParseMultiShort(const MultiShortOption& opts,
                                                 OptionStorage& options);

    // This reads the next argv and returns it if it is a value
    // TODO :: I mean this looks like a perfect usage for std::expected, but i
    // didn't had any meaningful errors returned as i was writing this.
    std::optional<Value> ReadExpectValue();
    std::optional<Argument> ReadArg();
    std::span<std::string_view> argv_;
    // Index of the currently read argument.
    // Starts from 1 to skip the program name.
    size_t argIndex_ = 1;
  };

}  // namespace cppli
