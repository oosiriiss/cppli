#include <cstdlib>
#include <print>

#include "cppli/cppli.hpp"

enum class ArgumentType : std::uint8_t { Help, Output };

auto main(int argc, char** argv) -> int {
  // We create option container with any type out want (that support
  // comparisons)
  // In my opion enum classes are perfect fit for this use case.
  cppli::OptionContainer<ArgumentType> options;

  // Define your options
  // firstName and secondName are the names that will be used to "recognize" the
  // option.
  // firstName is considered to be the "short" name and second name to be the
  // "lon " name, but it is not enforced.
  // description is used for automatic generation of help message.
  // needsValue determines whether an option will need a value for example
  // --out main or --out=main
  options.addOption(ArgumentType::Help,
                    cppli::Option{
                        .firstName = "-h",
                        .secondName = "--help",
                        .description = "Displays help message",
                        .needsValue = false,
                    });
  options.addOption(ArgumentType::Output,
                    cppli::Option{.firstName = "",
                                  .secondName = "--out",
                                  .description = "Sets the output file",
                                  .needsValue = true});

  cppli::ParseResult result = cppli::parseArguments(argc, argv, options);

  // Querying the results

  // This API will definitely change.
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
