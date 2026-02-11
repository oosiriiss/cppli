#pragma once

#include <string>

constexpr auto someFun(std::string_view sv) {
  return std::string("hello") + sv.data();
}

void someCompFun(int x);
