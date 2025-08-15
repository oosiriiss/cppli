#include "argparser.hpp"
#include "cli.hpp"
#include <optional>
#include <print>
#include <stdexcept>
#include <variant>

namespace cli {

ArgvParser::ArgvParser(std::span<std::string_view> raw_args)
    : argv(raw_args), arg_index(0) {}

// Nice visitor util
template <class... Ts> struct visitor : Ts... {
  using Ts::operator()...;
};
template <class... Ts> visitor(Ts...) -> visitor<Ts...>;

std::optional<Option> ArgvParser::MatchOption(ShortOptions &shortOptions,
                                              LongOptions &longOptions) {

  auto argOpt = ReadArg();
  if (!argOpt) {
    std::println("Readarg failed");
    return std::nullopt;
  }

  std::println("an option was matched");

  auto arg = *argOpt;

  return std::visit(
      visitor{
          [this,
           &shortOptions](const ShortOption &opt) -> std::optional<Option> {
            std::println("Matched short {}", opt.opt);
            return ParseShort(opt, shortOptions);
          },
          [this, &shortOptions](
              const MultiShortOption &opt) -> std::optional<Option> {
            std::println("Matched multishort {}", opt.opts);
            return ParseMultiShort(opt, shortOptions);
          },
          [this, &longOptions](const LongOption &opt) -> std::optional<Option> {
            std::println("Matched long {}", opt.opt);
            return ParseLong(opt, longOptions);
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
    opt.raw_value = value.val;
  }
  return opt;
}

std::optional<Option> ArgvParser::ParseShort(const ShortOption &opt,
                                             ShortOptions &shortOptions) {

  auto it = shortOptions.find(opt.opt);
  if (it == shortOptions.end())
    return std::nullopt;

  Option &option = it->second;
  return Parse(option);
}
std::optional<Option> ArgvParser::ParseLong(const LongOption &opt,
                                            LongOptions &longOptions) {

  auto it = longOptions.find(opt.opt);
  if (it == longOptions.end())
    return std::nullopt;

  Option &option = it->second;
  return Parse(option);
}

std::optional<Option> ArgvParser::ParseMultiShort(const MultiShortOption &opts,
                                                  ShortOptions &shortOptions) {

  // I will implement some sort of parsing the argv by each char so that i won't
  // have to return std::vector or something from here.
  throw std::runtime_error("NOT IMPLEMENTED YET");

  // Checking if any option expects argument which isn't supported
  // The elements are looked up first here and in the real look to avoid
  // mutating only a portion of the options before encountering invalid one.
  // And as a bonus it immediately detects any invalid args.
  for (char arg : opts.opts) {
    auto it = shortOptions.find(arg);
    if (it == shortOptions.end() || it->second.expectsArgument)
      return std::nullopt;
  }

  // Real loop
  for (char arg : opts.opts) {
    auto it = shortOptions.find(arg);
    if (it == shortOptions.end() || it->second.expectsArgument)
      return std::nullopt;
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

  ++arg_index;
  std::string_view arg = argv[arg_index];

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
