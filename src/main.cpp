#include "cli.hpp"
#include "logging.hpp"
#include "option.hpp"
#include <print>
#include <string_view>

int main(int argc, const char *const *const argv) {
  // TODO :: Introduce C++ 20 module stuff?
  // TODO :: allow overload of --help and --version?
  // TODO :: Can this App class be made more constexpr?
  // TODO :: Split converters into its own file
  // TODO :: Maybe option builder pattern would be cool? but idk how would that
  // TODO :: Some debug logging mechanism :)
  // work with default arg types. or maybe it'd just be std::any

  int ligma = 90;

  cli::App app("Test App", "v0.1", "Dev app");
  app.RegisterConverter<int>(
      [](std::string_view raw) { return std::stoi(raw.data()); });
  app.AddOption<int>({'x', "XD"},
                     cli::Option::Params<int>{
                         .description = "Hello",
                         .defaultValue = -69,
                         .callback = [&ligma](int v) { ligma = v; },
                     });
  app.Run(argc, argv);

  CPPLI_LOG_DEBUG("Test log!");

  return 0;
}
