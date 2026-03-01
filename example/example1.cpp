#include <cstdlib>
#include <iostream>
#include <print>

#include "argument_container.hpp"
#include "option.hpp"
#include "parse.hpp"

enum class ArgumentType : std::uint8_t { Help, Output };

auto main(int argc, char** argv) -> int {
  cppli::OptionContainer<ArgumentType> options;
  options.addOption(ArgumentType::Help, cppli::Option{.firstName = "-h",
                                                      .secondName = "--help",
                                                      .needsValue = false});
  options.addOption(ArgumentType::Output, cppli::Option{.firstName = "",
                                                        .secondName = "--out",
                                                        .needsValue = true});

  cppli::ParseResult result = cppli::parseArguments(argc, argv, options);

  if (result.options.contains(ArgumentType::Help)) {
    std::println("Help message!");
    return EXIT_SUCCESS;
  }

  if (result.options.contains(ArgumentType::Output)) {
    if (auto value = result.options.at(ArgumentType::Output).value) {
      std::println("Output value is: {}", *value);
    } else {
      std::println("No value provided for --output");
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
