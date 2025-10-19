#include "argparser.hpp"

#include <string_view>
#include <unordered_set>

#include "cppli/cppli.hpp"
#include "tasty/runners.hpp"
#include "tasty/tasty.hpp"

using namespace std::literals::string_view_literals;

using cppli::ArgParser;

auto main() -> int {  // NOLINT
  cppli::Command root;
  cppli::Command subcommand;

  subcommand.options.put(  //
      {"--help", "-h"},    //
      cppli::Option{.expectsValue = false});
  subcommand.options.put(  //
      {"--number", "-n"},  //
      cppli::Option{.expectsValue = true});

  root.options.put({"--balls"}, cppli::Option{.expectsValue = false});
  root.subcommands.put({"help"}, std::move(subcommand));

  std::vector<std::string_view> argv = {"help",     "--balls", "--help",   "-n",
                                        "15",       "-h",      "--number", "15",
                                        "--number", "--balls"};

  auto result = ArgParser::parseProgramArguments(argv, root);

  tasty::expectEqual(1, result.commandPath.size());
  tasty::expectEqual("help", result.commandPath[0]);
  tasty::expectEqual(5, result.options.size());  // NOLINT
  tasty::expectEqual("--help", result.options[0].name);
  tasty::expectEqual("-n", result.options[1].name);
  tasty::expectEqual("15", result.options[1].rawValue);
  tasty::expectEqual("-h", result.options[2].name);
  tasty::expectEqual("--number", result.options[3].name);
  tasty::expectEqual("15", result.options[3].rawValue);

  // commands after determinig command scope are treated as values
  tasty::expectEqual("--number", result.options[4].name);
  tasty::expectEqual("--balls", result.options[4].rawValue);

  // option from invalid scope is treated as value
  tasty::expectEqual(1, result.positionalArgs.size());
  tasty::expectEqual("--balls", result.positionalArgs[0]);
}
