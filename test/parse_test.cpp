#include "parse.hpp"

#include <cstdlib>
#include <tasty/runners.hpp>
#include <tasty/tasty.hpp>

#include "option.hpp"

enum class OptionKey : std::uint8_t {
  First,
  Second,
  Third,
};

static constexpr void properParsingOfPositionals() {
  std::array argv{"./program", "positional", "positional2", "--option",
                  "value",     "--option2",  "positional3"};
  ArgumentContainer args(argv.size(), argv.data());
  OptionContainer<OptionKey> container;

  container.addOption(
      OptionKey::First,
      Option{.firstName = "-o", .secondName = "--option", .needsValue = false});
  container.addOption(OptionKey::Second, Option{.firstName = "-s",
                                                .secondName = "--option2",
                                                .needsValue = false});
  auto result = parseArguments(args, container);
  tasty::expectEqual(
      "./program", result.positionals[0]);  // TODO :: What about program name?
  tasty::expectEqual("positional", result.positionals[1]);
  tasty::expectEqual("positional2", result.positionals[2]);
  tasty::expectEqual("value", result.positionals[3]);
  tasty::expectEqual("positional3", result.positionals[4]);
}
static constexpr void properParsingOfOptionsWithoutValues() {
  std::array argv{"./program", "positional", "positional2", "--option",
                  "value",     "--option2",  "positional3"};
  ArgumentContainer args(argv.size(), argv.data());
  OptionContainer<OptionKey> container;

  container.addOption(
      OptionKey::First,
      Option{.firstName = "-o", .secondName = "--option", .needsValue = false});
  container.addOption(OptionKey::Second, Option{.firstName = "-s",
                                                .secondName = "--option2",
                                                .needsValue = false});
  auto result = parseArguments(args, container);
  tasty::expectEqual(OptionValue::empty(), result.options[OptionKey::First]);
  tasty::expectEqual(OptionValue::empty(), result.options[OptionKey::Second]);
}

static constexpr void properParsingOptionsWithValuesAsSeparateArguments() {
  std::array argv{"./program", "positional", "positional2", "--option",
                  "value",     "--option2",  "option2value"};
  ArgumentContainer args(argv.size(), argv.data());
  OptionContainer<OptionKey> container;

  container.addOption(
      OptionKey::First,
      Option{.firstName = "-o", .secondName = "--option", .needsValue = false});
  container.addOption(
      OptionKey::Second,
      Option{.firstName = "-s", .secondName = "--option2", .needsValue = true});
  container.addOption(
      OptionKey::Third,
      Option{.firstName = "-g", .secondName = "--option3", .needsValue = true});
  auto result = parseArguments(args, container);
  tasty::expectEqual(OptionValue::empty(), result.options[OptionKey::First]);
  tasty::expectEqual("option2value", *result.options[OptionKey::Second].value);
}

static constexpr void parsingOptionWithValueButNoValueWasGivenShouldError() {
  std::array argv{"./program", "--option"};
  ArgumentContainer args(argv.size(), argv.data());
  OptionContainer<OptionKey> container;

  container.addOption(
      OptionKey::First,
      Option{.firstName = "-o", .secondName = "--option", .needsValue = true});

  tasty::expectException<std::runtime_error>(
      [&]() -> void { auto result = parseArguments(args, container); });
}

auto main() -> int {
  tasty::TestRunner runner("Parsing Arguments Test");
  runner.registerTest(properParsingOfPositionals, "properParsingOfPositionals");
  runner.registerTest(properParsingOfOptionsWithoutValues,
                      "properParsingOfOptionsWithoutValues");
  runner.registerTest(properParsingOptionsWithValuesAsSeparateArguments,
                      "properParsingOptionsWithValuesAsSeparateArguments");
  runner.registerTest(parsingOptionWithValueButNoValueWasGivenShouldError,
                      "parsingOptionWithValueButNoValueWasGivenShouldError");

  if (!runner.runAll()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
