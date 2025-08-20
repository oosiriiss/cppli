#include "argparser.hpp"
#include <iostream>
#include <optional>
#include <print>
#include <stdexcept>
#include <variant>

namespace cli {

ArgvParser::ArgvParser(std::span<std::string_view> raw_args)
    : argv(raw_args), arg_index(1) {}

// Nice visitor util
template <class... Ts> struct visitor : Ts... {
  using Ts::operator()...;
};
template <class... Ts> visitor(Ts...) -> visitor<Ts...>;

std::optional<Option> ArgvParser::MatchOptions(OptionStorage &options) {

  auto argOpt = ReadArg();
  if (!argOpt) {
    return std::nullopt;
  }


  auto arg = *argOpt;

  return std::visit(
      visitor{
          [this, &options](const ShortOption &opt) -> std::optional<Option> {
            std::println("Matched short {}", opt.opt);
            return ParseShort(opt, options);
          },
          [this,
           &options](const MultiShortOption &opt) -> std::optional<Option> {
            std::println("Matched multishort {}", opt.opts);
            return ParseMultiShort(opt, options);
          },
          [this, &options](const LongOption &opt) -> std::optional<Option> {
            std::println("Matched long {}", opt.opt);
            return ParseLong(opt, options);
          },
          [this](const Value &opt) -> std::optional<Option> {
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
    if (!expectedValue)
      return std::nullopt;
    Value value = *expectedValue;
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

std::optional<Option> ArgvParser::ParseMultiShort(const MultiShortOption &opts,
                                                  OptionStorage &options) {

  // I will implement some sort of parsing the argv by each char so that i won't
  // have to return std::vector or something from here.
  throw std::runtime_error("NOT IMPLEMENTED YET");

  // Checking if any option expects argument which isn't supported
  // The elements are looked up first here and in the real look to avoid
  // mutating only a portion of the options before encountering invalid one.
  // And as a bonus it immediately detects any invalid args.
  for (char arg : opts.opts) {
    if (!options.Contains(arg) || options[arg].expectsArgument)
      return std::nullopt;
  }

  // Real loop
  for (char arg : opts.opts) {
    Option &opt = options[arg];
  }
}

std::optional<Value> ArgvParser::ReadExpectValue() {
  auto argOpt = ReadArg();
  if (!argOpt)
    return std::nullopt;

  auto arg = *argOpt;

  if (auto value = std::get_if<Value>(&arg)) {
    return *value;
  } else {
    return std::nullopt;
  }
}

std::optional<Argument> ArgvParser::ReadArg() {
  if (arg_index >= argv.size())
    return std::nullopt;

  std::string_view arg = argv[arg_index++];

  if (arg.starts_with("--")) {
    if (arg.size() <= 2)
      return std::nullopt;
    return LongOption{.opt = arg.substr(2)};
  } else if (arg.starts_with("-")) {
    if (arg.size() <= 1)
      return std::nullopt;
    if (arg.size() == 2)
      return ShortOption{.opt = arg[1]};
    else
      return MultiShortOption{.opts = arg.substr(1)};
  } else {
    return Value{.val = arg};
  }
}

} // namespace cli
