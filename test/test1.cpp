#include <iostream>
#include <tasty/tasty.hpp>

#include "lib1.hpp"

int main() {
  std::cout << "Test 1";
  tasty::expectEqual(true, false);
}
