#pragma once

#include "multikey_map.hpp"
#include "option.hpp"

namespace cppli {
  struct Command {
    std::size_t positionalArgumentCount = 0;
    MultiKeyMap<Command> subcommands;
    MultiKeyMap<Option> options;
  };
}  // namespace cppli
