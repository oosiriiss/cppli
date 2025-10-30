#include <tasty/runners.hpp>
#include <tasty/tasty.hpp>

#include "cppli/cppli.hpp"

using cppli::App;

App app;  // NOLINT

constexpr static void specifiedIndex() {
  TASTY_EXPECT(app.getPositionalArgument<int>(0).has_value());
  TASTY_EXPECT(app.getPositionalArgument<int>(1).has_value());
  TASTY_EXPECT(app.getPositionalArgument<int>(2).has_value());
  TASTY_EXPECT(app.getPositionalArgument<int>(3).has_value())

  TASTY_EXPECT(!app.getPositionalArgument<int>(12).has_value())
}

constexpr static void integralConversion() {
  tasty::expectEqual(123,  // NOLINT
                     app.getPositionalArgument<char>(0).value());
  tasty::expectEqual(-123,  // NOLINT
                     app.getPositionalArgument<int>(1).value());
  tasty::expectEqual(1231231,  // NOLINT
                     app.getPositionalArgument<int>(3).value());
}

constexpr static void invalidIntegralConversion() {
  TASTY_EXPECT(!app.getPositionalArgument<unsigned char>(1).has_value());
  TASTY_EXPECT(!app.getPositionalArgument<int>(4).has_value());
}

auto main() -> int {
  // Initialized state
  const char *argv[] = {"123", "-123", "192", "1231231", "xx12xas"};  // NOLINT
  int argc = sizeof(argv) / sizeof(argv[0]);
  app.parseArgs(argc, static_cast<const char **>(argv));

  // Tests
  auto conversionTests = tasty::TestRunner("Conversion tests");

  conversionTests.registerTest(specifiedIndex, "Indexing check");
  conversionTests.registerTest(integralConversion, "Converting valid integers");
  conversionTests.registerTest(invalidIntegralConversion, "Converting invalid integers");

  if (!conversionTests.runAll()) {
    return -1;
  }
  return 0;
}
