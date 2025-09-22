#include <format>
#include <optional>
#include <print>
#include <string_view>

#include "cppli/cppli.hpp"
#include "cppli/option.hpp"

int main(int argc, const char* const* const argv) {
  cppli::App app("Test App", "v0.1", "Dev app");

  app.AddOption<std::string_view>(
      // Shortname is an argument used like -n
      // Longname is an arugemnt used like --name
      {.shortName = 'n', .longName = "name"},
      cppli::Option::Params<std::string_view>{
          .description = "Species a name printed by the program",
          // Default value used
          .defaultValue = "This is the value that will be used if option is "
                          "passed without a value",
          // Callback invoked right after option is read from argv (After
          // invokin App::Run)
          .callback =
              [](std::string_view value) {
                std::println("Argument's value is {}", value);
              },
      });
  app.Run(argc, argv);

  // Or if you do not want to invoke callback immediately you can query for the
  // value later
  std::optional<std::string_view> value1 =
      app.GetOptionValue<std::string_view>('n');

  std::optional<std::string_view> value2 =
      app.GetOptionValue<std::string_view>("name");

  if (value1) {
    std::println("Argument's (-n) later queried value is: {}", *value1);
  }
  if (value2) {
    std::println("Argument's (--name) later queried value is: {}", *value2);
  }

  return 0;
}
