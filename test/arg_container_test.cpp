
#include <array>
#include <cstdlib>
#include <print>
#include <queue>
#include <ranges>
#include <stack>
#include <tasty/errors.hpp>
#include <tasty/runners.hpp>
#include <tasty/tasty.hpp>

#include "argument_container.hpp"

static constexpr void properAccesToArguments() {
  std::array argv1 = {
      "./program",
      "arg",
      "--flag",
      "--option=value",
  };

  ArgumentContainer args1(argv1.size(), argv1.data());
  tasty::expectEqual(args1.count(), argv1.size());
  tasty::expectEqual(args1[0], argv1[0]);
  tasty::expectEqual(args1[1], argv1[1]);
  tasty::expectEqual(args1[2], argv1[2]);
  tasty::expectEqual(args1[3], argv1[3]);

  // This should throw, and for now operator[] only throws in debug build only
  // as this error is purely skill issue related, and related only to the
  // developer and not the users
  // tasty::expectEqual(args1[4], argv1[3]);
}

static constexpr void properForwardIterationOfArguments() {
  std::array argv = {
      "./program",
      "arg",
      "--flag",
      "--option=value",
  };
  std::queue<std::string_view> queue;
  queue.push_range(argv);

  ArgumentContainer args(argv.size(), argv.data());

  for (std::string_view actual : args) {
    std::string_view expected = queue.front();
    queue.pop();
    std::print("{}", expected);
    tasty::expectEqual(expected, actual);
  }
  std::println();
}
static constexpr void properBackwardIterationOfArguments() {
  std::array argv = {
      "./program",
      "arg",
      "--flag",
      "--option=value",
  };
  std::stack<std::string_view> stack;
  stack.push_range(argv);

  ArgumentContainer args(argv.size(), argv.data());

  for (std::string_view actual : args | std::views::reverse) {
    std::string_view expected = stack.top();
    stack.pop();
    std::print("{}", expected);
    tasty::expectEqual(expected, actual);
  }
  std::println();
}

auto main() -> int {
  tasty::TestRunner runner("ArgumentContainer tests");

  runner.registerTest(properAccesToArguments, "properAccesToArguments");
  runner.registerTest(properForwardIterationOfArguments,
                      "properForwardIterationOfArguments");
  runner.registerTest(properBackwardIterationOfArguments,
                      "properBackwardIterationOfArguments");

  if (!runner.runAll()) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
