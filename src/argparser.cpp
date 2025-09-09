#include "argparser.hpp"

#include <optional>
#include <print>
#include <span>
#include <stdexcept>
#include <string_view>
#include <variant>

#include "cppli/option.hpp"

namespace cppli {

  ArgvParser::ArgvParser(std::span<std::string_view> argv)
      : argv_(argv) {}

  // Nice visitor util
  template <typename... Callable>
  struct Visitor : Callable... {  // NOLINT
    using Callable::operator()...;
  };
  template <class... Ts>
  Visitor(Ts...) -> Visitor<Ts...>;

  std::optional<Option> ArgvParser::MatchOptions(OptionStorage &options) {
    auto argOpt = ReadArg();
    if (!argOpt) {
      return std::nullopt;
    }

    auto arg = *argOpt;

    return std::visit(
        Visitor{
            [this, &options](const ShortOption &opt) -> std::optional<Option> {
              std::println("Matched short {}", opt.opt);
              return ParseShort(opt, options);
            },
            [this, &options](const LongOption &opt) -> std::optional<Option> {
              std::println("Matched long {}", opt.opt);
              return ParseLong(opt, options);
            },
            [&options](const MultiShortOption &opt) -> std::optional<Option> {
              std::println("Matched multishort {}", opt.opts);
              return ParseMultiShort(opt, options);
            },
            [](const Value &opt) -> std::optional<Option> {
              std::println("V {}", opt.val);
              // Value read without an argument
              return std::nullopt;
            },
        },
        arg);
  }

  std::optional<Option> ArgvParser::Parse(Option &opt) {
    if (opt.expectsArgument) {
      auto expectedValue = ReadExpectValue();
      if (!expectedValue) {
        return std::nullopt;
      }
      const Value value = *expectedValue;
      opt.rawValue = value.val;
    }
    return opt;
  }

  std::optional<Option> ArgvParser::ParseShort(const ShortOption &opt,
                                               OptionStorage &options) {
    if (options.Contains(opt.opt)) {
      return Parse(options[opt.opt]);
    }
    return std::nullopt;
  }
  std::optional<Option> ArgvParser::ParseLong(const LongOption &opt,
                                              OptionStorage &options) {
    if (options.Contains(opt.opt)) {
      return Parse(options[opt.opt]);
    }
    return std::nullopt;
  }

  std::optional<Option> ArgvParser::ParseMultiShort(
      const MultiShortOption &opts, OptionStorage &options) {
    // I will implement some sort of parsing the argv by each char so that i
    // won't have to return std::vector or something from here.
    throw std::runtime_error("NOT IMPLEMENTED YET");

    // Checking if any option expects argument which isn't supported
    // The elements are looked up first here and in the real look to avoid
    // mutating only a portion of the options before encountering invalid one.
    // And as a bonus it immediately detects any invalid args.
    for (const char arg : opts.opts) {
      if (!options.Contains(arg) || options[arg].expectsArgument) {
        return std::nullopt;
      }
    }

    // Real loop
    // for (char arg : opts.opts) {
    //  Option &opt = options[arg]; // NOLINT
    //}
  }

  std::optional<Value> ArgvParser::ReadExpectValue() {
    auto argOpt = ReadArg();
    if (!argOpt) {
      return std::nullopt;
    }

    auto arg = *argOpt;

    if (auto *value = std::get_if<Value>(&arg)) {
      return *value;
    }
    // Another value enocuntered - reverting arg index
    --argIndex_;
    return std::nullopt;
  }

  std::optional<Argument> ArgvParser::ReadArg() {
    if (argIndex_ >= argv_.size()) {
      return std::nullopt;
    }

    const std::string_view arg = argv_[argIndex_++];

    if (arg.starts_with("--")) {
      if (arg.size() <= 2) {
        return std::nullopt;
      }
      return LongOption{.opt = arg.substr(2)};
    }

    if (arg.starts_with("-")) {
      if (arg.size() <= 1) {
        return std::nullopt;
      }
      if (arg.size() == 2) {
        return ShortOption{.opt = arg[1]};
      }
      return MultiShortOption{.opts = arg.substr(1)};
    }
    return Value{.val = arg};
  }

}  // namespace cppli
