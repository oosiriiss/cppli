#include "cppli/cppli.hpp"

auto main() -> int {
  cppli::OptionContainer<std::string> opts;

  opts.addOption("opt1", cppli::Option{.firstName = "-h",
                                       .secondName = "--help",
                                       .description = "Example description",
                                       .needsValue = false});
  opts.addOption(
      "opt2", cppli::Option{
                  .firstName = "-o",
                  .secondName = "--opt",
                  .description =
                      "This should be a very very long description spanning "
                      "multiple lines. I Could talk about how much I "
                      "appreciate this option. It introduces so much freshness "
                      "into this program and allows to do amazing things. "
                      "LONGWORDISSPLITAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
                      "AAAAAAAAAAAAAAAAAAAAAAAAAAA"
                      "aaaaaaaaaaaaaaaaaaaaaaaaa",
                  .needsValue = false});
  opts.addOption(
      "opt3",
      cppli::Option{
          .firstName = "-n",
          .secondName = "--new-lines",
          .description = "I\n wanna\nDo\nMultiple\nLines\nBecause\nI\nCan",
          .needsValue = true});

  std::string help =
      cppli::createHelp(opts, "cool-program",
                        "cool-program --new-lines=10 -o\ncool-program "
                        "--new-lines 10 -o hello-im-positional");

  std::println("Generated help message:\n\n{}", help);
}
