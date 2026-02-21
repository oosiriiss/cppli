#include <cstdlib>
#include <stdexcept>
#include <tasty/runners.hpp>
#include <tasty/tasty.hpp>

#include "debug_utils.hpp"
#include "option.hpp"

using namespace cppli;  // NOLINT

enum class MyKey : std::uint8_t {
  FirstArg,
  SecondArg,
};

static constexpr void duplicateNamesShouldThrowInDebug() {
  if constexpr (debugutils::DEBUG) {
    // Short name duplicated
    tasty::expectException<std::invalid_argument>([]() -> void {
      OptionContainer<MyKey> cont;

      cont.addOption(MyKey::FirstArg,
                     Option{.firstName = "-h", .secondName = "--help"});
      cont.addOption(MyKey::SecondArg,
                     Option{.firstName = "-h", .secondName = "--hello"});
    });
    // Long name duplicated
    tasty::expectException<std::invalid_argument>([]() -> void {
      OptionContainer<MyKey> cont;

      cont.addOption(MyKey::FirstArg,
                     Option{.firstName = "-o", .secondName = "--help"});
      cont.addOption(MyKey::SecondArg,
                     Option{.firstName = "-h", .secondName = "--help"});
    });
  }
}

static constexpr void matchRegisteredOptionsReturnsProperOne() {
  OptionContainer<MyKey> cont;
  cont.addOption(MyKey::FirstArg,
                 Option{.firstName = "-h", .secondName = "--helo"});

  std::optional byFirst = cont.matchOptionByName("-h");
  std::optional bySecond = cont.matchOptionByName("--helo");

  tasty::expectEqual(byFirst.has_value(), true);
  tasty::expectEqual(bySecond.has_value(), true);

  // Matching non-registered
  std::optional byUnknown = cont.matchOptionByName("-x");
  tasty::expectEqual(byUnknown.has_value(), false);
}

auto main() -> int {
  tasty::TestRunner argContainerRunner("ArgumentContainer tests");
  argContainerRunner.registerTest(duplicateNamesShouldThrowInDebug,
                                  "duplicateNamesShouldThrowInDebug");
  argContainerRunner.registerTest(matchRegisteredOptionsReturnsProperOne,
                                  "matchRegisteredOptionsReturnsProperOne");

  if (!argContainerRunner.runAll()) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
