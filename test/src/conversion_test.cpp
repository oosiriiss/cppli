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

struct MyStruct {
  int x;
  int y;
};

constexpr static void ownConvertibleTypes() {
  auto value = app.getPositionalArgument<MyStruct>(5);
  TASTY_EXPECT(value.has_value());
  tasty::expectEqual(1, value->x);
  tasty::expectEqual(2, value->y);
}

auto main() -> int {
  // Initialized state
  const char *argv[] = {"123",     "-123",    "192",
                        "1231231", "xx12xas", "12"};  // NOLINT
  int argc = sizeof(argv) / sizeof(argv[0]);
  app.parseArgs(argc, static_cast<const char **>(argv));

  app.registerConverter<MyStruct>([](std::string_view raw) -> MyStruct {
    return MyStruct{
        .x = raw[0] - '0',
        .y = raw[1] - '0',
    };
  });

  // Tests
  auto conversionTests = tasty::TestRunner("Conversion tests");

  conversionTests.registerTest(specifiedIndex, "Indexing check");
  conversionTests.registerTest(integralConversion, "Converting valid integers");
  conversionTests.registerTest(invalidIntegralConversion,
                               "Converting invalid integers");
  conversionTests.registerTest(ownConvertibleTypes, "Own convertibles types");

  if (!conversionTests.runAll()) {
    return -1;
  }
  return 0;
}
