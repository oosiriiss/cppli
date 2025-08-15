#include "argparser.hpp"
#include "cli.hpp"
#include <print>
#include <string_view>

int main(int argc, const char *const *const argv) {
  // TODO :: Introduce C++ 20 module stuff?
  // TODO :: allow overload of --help and --version?
  // TODO :: Can this App class be made more constexpr?
  // TODO ::Split converters into its own file
  // TODO :: Maybe option builder pattern would be cool? but idk how would that
  // TODO :: Some debug logging mechanism :)
  // work with default arg types. or maybe it'd just be std::any

  cli::App app("Test App", "v0.1", "Dev app");
  app.RegisterConverter<int>(
      [](std::string_view raw) { return std::stoi(raw.data()); });
  app.AddOption<int>('x', [](int v) { std::println("Value is: {}", v); });

  std::vector<std::string_view> args(
      argv, std::next(argv, static_cast<std::ptrdiff_t>(argc)));

  for (auto x : args) {
    std::println("argv is: {}", x);
  }

  std::unordered_map<std::string_view, cli::Option> longmap;
  cli::ArgvParser parser(std::span{args}.subspan(0));
  auto option = parser.MatchOption(app.options_, longmap);

  if (option) {
    option->Matched(option->raw_value);
  } else {
    std::println("Option not matched");
  }

  return 0;
}
