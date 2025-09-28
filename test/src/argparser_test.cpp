#include "argparser.hpp"

#include <print>
#include <stdexcept>
#include <string_view>
#include <typeinfo>

#include "tasty/runners.hpp"
#include "tasty/tasty.hpp"

using namespace std::literals::string_view_literals;

static constexpr auto ok(bool x) -> void {
  static int TestNum = 1;
  if (!x) {
    throw std::invalid_argument("Test Failed");
  }
  std::println("Test {} passed", TestNum++);
}

constexpr static auto convertingToVector() -> bool {
  const char* argv[] = {"Hello", "World!"};  // NOLINT
  int argc = sizeof(argv) / sizeof(argv[0]);
  auto args = cppli::ArgParser::argvToVector(
      argc,
      static_cast<char const* const* const>(argv)  // NOLINT
  );

  tasty::expectEqual(args.size(), 2UL);
  tasty::expectEqual(args[0], "Hello"sv);
  tasty::expectEqual(args[1], "World!"sv);

  return true;
}

constexpr static auto convertingToVectorNoARGV() -> bool {
  auto args = cppli::ArgParser::argvToVector(0, nullptr);
  tasty::expectEqual(args.size(), 0UL);
  return true;
}

constexpr static auto parseCommand() {
  std::vector<std::string_view> argv = {"command"sv};
  std::span commands = argv;

  std::vector<cppli::Argument> args =
      cppli::ArgParser::parseProgramArguments(argv, commands, {});

  tasty::expectEqual(argv.size(), args.size());
  tasty::expectEqual(0UL, args[0].index());
}
constexpr static auto parseValue() {
  std::vector<std::string_view> argv = {"SomeValue"sv};

  std::vector<cppli::Argument> args =
      cppli::ArgParser::parseProgramArguments(argv, {}, {});

  tasty::expectEqual(argv.size(), args.size());
  tasty::expectEqual(3UL, args[0].index());
}

constexpr static auto parseOption() {
  std::vector<std::string_view> argv = {"--Hello"sv};
  std::span options = argv;

  std::vector<cppli::Argument> args =
      cppli::ArgParser::parseProgramArguments(argv, {}, options);

  tasty::expectEqual(argv.size(), args.size());
  tasty::expectEqual(1UL, args[0].index());
}

constexpr static auto parseFlag() {
  std::vector<std::string_view> argv = {"--Hello"sv};

  std::vector<cppli::Argument> args =
      cppli::ArgParser::parseProgramArguments(argv, {}, {});

  tasty::expectEqual(argv.size(), args.size());
  tasty::expectEqual(2UL, args[0].index());
}

constexpr static auto parseCombinations() {
  std::vector<std::string_view> argv = {"join"sv, "-n", "4",     "--strict",
                                        "Hello",  " ",  "World!"};

  std::vector<std::string_view> commands = {"join"sv};
  std::vector<std::string_view> options = {"-n"sv};

  std::vector<cppli::Argument> args =
      cppli::ArgParser::parseProgramArguments(argv, commands, options);

  tasty::expectEqual(argv.size(), args.size());
  std::vector<std::size_t> expectedVariantIndices = {0, 1, 3, 2, 3, 3, 3};

  for (std::size_t i = 0; i < args.size(); ++i) {
    tasty::expectEqual(expectedVariantIndices[i], args[i].index());
  }
}

auto main() -> int {  // NOLINT

  auto conversion = tasty::TestRunner("Converting To Vector");

  conversion.registerTest(convertingToVector);
  conversion.registerTest(convertingToVectorNoARGV);
  if (!conversion.runAll()) {
    return -1;
  }

  auto parse = tasty::TestRunner("Parsing argv");
  parse.registerTest(parseCommand);
  parse.registerTest(parseValue);
  parse.registerTest(parseOption);
  parse.registerTest(parseFlag);
  parse.registerTest(parseCombinations);

  if (!parse.runAll()) {
    return -1;
  }
}
