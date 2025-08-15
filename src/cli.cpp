#include "cli.hpp"
#include <cstring>
#include <format>
#include <print>
#include <stdexcept>

namespace cli {

Option::Option(const char shortName, const std::string longName,
               const std::string description, bool expectsArgument,
               std::optional<OptionCallback> callback)
    : shortName(shortName), longName(longName), description(description),
      expectsArgument(expectsArgument), raw_value(""), callback_(callback) {}

void Option::Matched(std::optional<std::string_view> arg) const {

  if (callback_ && arg) {
    (*callback_)(*arg);
  }
}

App::App(std::string title, std::string description, std::string version)
    : title_(title), description_(description), version_(version) {}

void App::Initialize(int argc, char **argv) {
  // AddOption(Option('h', "help", "Displays help message", false,
  //                  [this] { PrintHelp(); }));
  // AddOption(Option('v', "version", "Displays version info", false,
  //                  [this] { PrintVersion(); }));

  ParseOptions(argc, argv);
}

void App::ParseOptions(const int argc, const char *const *const argv) const {
  if (argc <= 1)
    return;

  for (int i = 1; i < argc; ++i) {
    const char *const argument = argv[i];
    const int argumentLength = strlen(argument);

    if (TryParseShortOption(argument)) {

    } else if (TryParseLongOption(argument)) {

    } else {
      // TODO :: Change this to not  exception
      throw std::runtime_error(std::format("Invalid argument {}", argument));
    }
  }
}

// TODO :: Parsing options with arguments
// TODO :: Parsing multiple short options after single dash
bool App::TryParseShortOption(std::string_view arg) const {
  throw std::runtime_error("NOT IMPLEMENTED");
  if (arg.size() < 2)
    return false;
  if (arg.size() > 2) {
    if (arg[0] == '-' && arg[1] != '-') {
      throw std::runtime_error(
          "Parsing multiple short options not yet implemented");
    }
    // Possible long option
    return false;
  }

  const char shortName = arg[1];

  if (options_.contains(shortName)) {
    // Option matched
    const Option &current = options_.at(shortName);
    current.Matched("");

    return true;
  }

  // Invalid
  return false;
}

bool App::TryParseLongOption(std::string_view arg) const {
  throw std::runtime_error("TryParseLongOption not yet implemented");
}

void App::PrintHelp() const {
  std::print("Usage:\n");
  std::print("{} [options] [arguments]\n", title_);
  std::print("\n\n Options:\n");

  for (const auto &[_, o] : options_) {

    std::print("-{}, --{:<20}  {}", o.shortName, o.longName, o.description);
  }
}

void App::PrintVersion() const {
  std::print("{} {}\n{}\n", title_, version_, description_);
}

} // namespace cli
