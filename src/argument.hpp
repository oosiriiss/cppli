#pragma once
#include <cstdint>

enum class ArgType : std::uint8_t {
  //
  Positional,
  //
  Flag,
  //
  Option
};
