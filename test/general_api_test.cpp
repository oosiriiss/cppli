#include <tasty/tasty.hpp>

auto main() -> int {
  tasty::TestRunner runner("General API Test");

  if (!runner.runAll()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
