#include <cstdlib>
#include <stdexcept>
#include <tasty/runners.hpp>
#include <tasty/tasty.hpp>

#include "argument.hpp"
#include "debug_utils.hpp"

enum class MyKey : std::uint8_t {
  FirstArg,
  SecondArg,
};

static constexpr void duplicateNamesShouldThrowInDebug() {
  if constexpr (debugutils::DEBUG) {
    // Short name duplicated
    tasty::expectException<std::invalid_argument>([]() -> void {
      ArgumentContainer<MyKey> cont;

      cont.addOption(MyKey::FirstArg,
                     Option{.firstName = "-h", .secondName = "--help"});
      cont.addOption(MyKey::SecondArg,
                     Option{.firstName = "-h", .secondName = "--hello"});
    });
    // Long name duplicated
    tasty::expectException<std::invalid_argument>([]() -> void {
      ArgumentContainer<MyKey> cont;

      cont.addOption(MyKey::FirstArg,
                     Option{.firstName = "-o", .secondName = "--help"});
      cont.addOption(MyKey::SecondArg,
                     Option{.firstName = "-h", .secondName = "--help"});
    });
  }
}

static constexpr void matchRegisteredOptionsReturnsProperOne() {
  ArgumentContainer<MyKey> cont;
  cont.addOption(MyKey::FirstArg,
                 Option{.firstName = "-h", .secondName = "--helo"});

  std::optional<Option> byFirst = cont.matchOptionByName("-h");
  std::optional<Option> bySecond = cont.matchOptionByName("--helo");

  tasty::expectEqual(byFirst.has_value(), true);
  tasty::expectEqual(bySecond.has_value(), true);

  // Matching non-registered
  std::optional<Option> byUnknown = cont.matchOptionByName("-x");
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
