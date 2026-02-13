
#include <array>
#include <string>
#include <tasty/errors.hpp>
#include <tasty/runners.hpp>
#include <tasty/tasty.hpp>

#include "argument_container.hpp"

auto main() -> int {
  std::array argv1 = {
      "./program",
      "arg",
      "--flag",
      "--option=value",
  };

  ArgumentContainer args1(argv1.size(), argv1.data());

  try {
    tasty::expectEqual(args1.count(), argv1.size());
    tasty::expectEqual(args1[0], argv1[0]);
    tasty::expectEqual(args1[1], argv1[1]);
    tasty::expectEqual(args1[2], argv1[2]);
    tasty::expectEqual(args1[3], argv1[3]);
    tasty::expectEqual(args1[4], argv1[3]);
  } catch (const tasty::errors::ExpectFailed& err) {
    return -1;
  }

  return 0;
}
