#include "cppli/cppli.hpp"

auto main() -> int {
  cppli::OptionContainer<std::string> opts;

  // Generated help message for this file can be found at the bottom of the
  // file.

  // As of the current state the help message consists of 3 secions: USAGE,
  // EXAMPLE, OPTIONS
  // USAGE contains a simple definition on how to use the program
  // USAGE:
  // <programName> [OPTIONS] [POS_ARGS]
  //
  //
  // Example section is user defined as an argument to the createHelp function.
  // EXAMPLE:
  // <Whatever_user_inputs_as_arg>
  //
  // The most important part - options.
  // OPTIONS:
  //
  // each option's entry looks like this
  //                COLUMN 1                             |   COLUMN 2
  //  <FIRST_NAME>, <SECOND_NAME> ["<ARG>" if needsValue] <DESCRIPTION>
  //
  // Column 1 and Column 2 have fixed widths, and each column's text will be
  // placed only in their corresponding column, and will "wrap" to the next line
  // of the column if it doesn't fit in one line.

  // For each option description field is defined.
  opts.addOption("opt1", cppli::Option{.firstName = "-h",
                                       .secondName = "--help",
                                       .description = "Example description",
                                       .needsValue = false});
  opts.addOption(
      "opt2", cppli::Option{
                  .firstName = "-o",
                  .secondName = "--opt",
                  .description =
                      // Words are not split (as long as their length is shorter
                      // or equal to the length of the description column)
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

// clang-format off
//
// Output of this file when executed.

//Generated help message:
//
//USAGE:
//cool-program [OPTIONS] [POS_ARGS]
//
//
//EXAMPLE:
//cool-program --new-lines=10 -o
//cool-program --new-lines 10 -o hello-im-positional
//
//OPTIONS:
//   -n, --new-lines <ARG>    I
//                            wanna
//                            Do
//                            Multiple
//                            Lines
//                            Because
//                            I
//                            Can
//
//   -o, --opt                This should be a very very long description spanning multiple lines. I
//                            Could talk about how much I appreciate this option. It introduces so
//                            much freshness into this program and allows to do amazing things.
//                            LONGWORDISSPLITAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
//                            AAAAAAAAAAaaaaaaaaaaaaaaaaaaaaaaaaa
//
//   -h, --help               Example description
//
//
//

// clang-format on
