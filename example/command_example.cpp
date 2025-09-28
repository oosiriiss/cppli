#include <print>

#include "cppli/command.hpp"
#include "cppli/option.hpp"

using cppli::Command;
using cppli::CommandContext;
using cppli::OptionInfo;
auto main() -> int {
  constexpr auto comm =
      Command(CommandContext({{{"hello", "bols"}, OptionInfo("Hello!")}}));

  std::println("{}", comm.context.infos_[0].name);

  return 0;
}
