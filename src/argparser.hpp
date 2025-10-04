
#pragma once
#include <algorithm>
#include <cstdint>
#include <functional>
#include <logzy/logzy.hpp>
#include <optional>
#include <span>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

#include "argument.hpp"

namespace cppli {

  struct ParseResult {
    std::vector<std::string_view> commandPath;
    std::vector<std::string_view> positionalArgs;
    std::vector<std::pair<std::string_view, std::optional<std::string_view>>>
        options;
  };

  class ArgParser {
   public:
    ArgParser() = delete;

    // Conestexpr for tests since it cannot wrok with argc and argv
    [[nodiscard]] constexpr static auto argvToVector(
        int argc,
        char const* const* const argv)  // NOLINT C-style array is required
        -> std::vector<std::string_view> {
      return std::vector<std::string_view>(
          argv, std::next(argv, static_cast<std::ptrdiff_t>(argc)));
    }

    [[nodiscard]] constexpr static auto parseProgramArguments(
        std::span<std::string_view> argv, const Command& rootCommand)
        -> std::optional<ParseResult> {
      std::optional<ParseResult> out = ParseResult{};

      // TODO :: ALlow nestes commands / subcommands
      const Command* currentCommand = &rootCommand;

      bool parsingCommands = true;
      bool wasLastOptionWithArg = false;

      // TODO: improve this loop
      // 	- Double lookup of subcommand
      //
      for (const std::string_view arg : argv) {
        if (parsingCommands && currentCommand->subcommands.contains(arg)) {
          if (auto optSubcommand = currentCommand->subcommands.get(arg)) {
            // if an argument exists the pointer is also valid so this could
            // maybe be inlined
            if (auto subcommand = optSubcommand->lock()) {
              currentCommand = subcommand.get();
              out->commandPath.push_back(arg);
            }
          }
        } else {
          parsingCommands = false;
          if (auto optOption = currentCommand->options.get(arg)) {
            // if an argument exists the pointer is also valid so this could
            // maybe be inlined
            if (auto opt = optOption->lock()) {
              wasLastOptionWithArg = opt->expectsValue;
              out->options.emplace_back(arg, std::nullopt);
            }

          } else {
            // If argument is not an option it is treated as positional
            // argument
            if (wasLastOptionWithArg) {
              out->options.back().second = arg;
            } else {
              out->positionalArgs.push_back(arg);
            }
            wasLastOptionWithArg = false;
          }
        }
      }

      return out;
    }
  };
}  // namespace cppli
