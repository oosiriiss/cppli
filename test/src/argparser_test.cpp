#include "argparser.hpp"

#include <string_view>
#include <unordered_set>

#include "argument.hpp"
#include "tasty/runners.hpp"
#include "tasty/tasty.hpp"

using namespace std::literals::string_view_literals;

using cppli::ArgParser;

auto main() -> int {  // NOLINT
  cppli::Command root;
  cppli::Command subcommand;

  subcommand.options.put<2>({"--help", "-h"},
                            cppli::Option{.expectsValue = false});
  subcommand.options.put<2>({"--number", "-n"},
                            cppli::Option{.expectsValue = true});

  root.options.put<1>({"--balls"}, cppli::Option{.expectsValue = false});
  root.subcommands.put<1>({"help"}, std::move(subcommand));

  std::vector<std::string_view> argv = {"help",     "--balls", "--help",   "-n",
                                        "15",       "-h",      "--number", "15",
                                        "--number", "--balls"};

  auto result = ArgParser::parseProgramArguments(argv, root);

  tasty::expectEqual(1UL, result->commandPath.size());
  tasty::expectEqual(std::string_view("help"), result->commandPath[0]);
  tasty::expectEqual(5UL, result->options.size());  // NOLINT
  tasty::expectEqual(std::string_view("--help"), result->options[0].first);
  tasty::expectEqual(std::string_view("-n"), result->options[1].first);
  tasty::expectEqual(std::string_view("15"), *result->options[1].second);
  tasty::expectEqual(std::string_view("-h"), result->options[2].first);
  tasty::expectEqual(std::string_view("--number"), result->options[3].first);
  tasty::expectEqual(std::string_view("15"), *result->options[3].second);

  // commands after determinig command scope are treated as values
  tasty::expectEqual(std::string_view("--number"), result->options[4].first);
  tasty::expectEqual(std::string_view("--balls"), *result->options[4].second);

  // option from invalid scope is treated as value
  tasty::expectEqual(1UL, result->positionalArgs.size());
  tasty::expectEqual(std::string_view("--balls"), result->positionalArgs[0]);
}
