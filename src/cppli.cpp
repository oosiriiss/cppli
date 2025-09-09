#include "cppli/cppli.hpp"

#include <format>
#include <iterator>
#include <print>
#include <span>
#include <string_view>
#include <vector>

#include "argparser.hpp"

namespace cppli {
  App::App(std::string_view title, std::string_view description,
           std::string_view version)
      : title_(title),
        description_(description),
        version_(version) {}

  void App::Run(const int argc, const char* const* const argv) {
    std::vector<std::string_view> args(argv, std::next(argv, argc));

    ArgvParser parser(std::span{args});

    while (auto optional = parser.MatchOptions(options_)) {
      (*optional).Matched();
    }
  }

  void App::PrintHelp() const {
    std::print("Usage:\n");
    std::print("{} [options] [arguments]\n", title_);
    std::print("\n\n Options:\n");

    // for (const auto &[_, o] : options_) {
    //   std::print("-{}, --{:<20}  {}", o.shortName, o.longName,
    //   o.description);
    // }
  }

  void App::PrintVersion() const {
    std::print("{} {}\n{}\n", title_, version_, description_);
  }

}  // namespace cppli
