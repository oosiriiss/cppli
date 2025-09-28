#include "argparser.hpp"

#include <print>
#include <stdexcept>

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
      static_cast<const char* const* const>(argv)  //
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

auto main() -> int {  // NOLINT


  ok(convertingToVector());
  ok(convertingToVectorNoARGV());
}
