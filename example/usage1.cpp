#include <charconv>
#include <format>
#include <print>
#include <stdexcept>
#include <string_view>
#include <system_error>

#include "cppli/cppli.hpp"
#include "cppli/option.hpp"

int main(int argc, const char* const* const argv) {
  // TODO :: Introduce C++ 20 module stuff?
  // TODO :: allow overload of --help and --version?
  // TODO :: Can this App class be made more constexpr?
  // TODO :: Split converters into its own file
  // TODO :: Maybe option builder pattern would be cool? but idk how would that
  // TODO :: Some debug logging mechanism :)
  // work with default arg types. or maybe it'd just be std::any

  cppli::App app("Test App", "v0.1", "Dev app");

  app.RegisterConverter<int>([](std::string_view raw) {
    int out = 0;
    const std::from_chars_result result =
        std::from_chars(raw.data(), raw.data() + raw.length(), out);
    if (result.ec == std::errc::invalid_argument) {
      throw std::invalid_argument(std::format("Couldn't parse {} to int", raw));
    }
    return out;
  });

  app.AddOption<int>(
      {.shortName = 'x', .longName = "XD"},
      cppli::Option::Params<int>{
          .description = "Hello",
          .defaultValue = -69,  // NOLINT
          .callback =
              [](int value) { std::println("Argument's value is {}", value); },
      });
  app.Run(argc, argv);

  return 0;
}
