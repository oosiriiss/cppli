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

namespace cppli {

  // Command determining program behaviour
  using Command = std::string_view;

  // An option which expects argument
  using Option = std::string_view;

  // An option without an argument that toggles some state. like '--help' or
  // '-h'
  using Flag = std::string_view;

  // Value argument
  using Value = std::string_view;

  using Argument = std::variant<Command, Option, Flag, Value>;

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

    /**
     *
     * @param argv - program's arguments
     * @param isOptionFn - function returning true if passed string_view is a
     * command. Will be invoked for every argument starting with '-' and more
     * than 1 characater.
     * @param isCommandFn - function returning true if passed string_view is an
     * option. Will be invoked for every argument starting that doesnt start
     * with '-'
     * @returns a vector of classified arguments
     */
    [[nodiscard]] static constexpr auto parseProgramArguments(
        std::span<std::string_view> argv,
        const std::function<bool(std::string_view)>& isOptionFn,
        const std::function<bool(std::string_view)>& isCommandFn)
        -> std::vector<Argument> {
      std::vector<Argument> out;
      out.reserve(argv.size() * 2);

      for (const std::string_view argument : argv) {
        if (argument.starts_with('-') && argument.size() > 1) {
          if (isOptionFn(argument)) {
            logzy::debug("'{}' parsed as Option", argument);
            out.emplace_back(std::in_place_index<1>, argument);  // Option
          } else {
            logzy::debug("'{}' parsed as Flag", argument);
            out.emplace_back(std::in_place_index<2>, argument);  // Flag
          }
          continue;
        }

        if (isCommandFn(argument)) {
          logzy::debug("'{}' parsed as Command", argument);
          out.emplace_back(std::in_place_index<0>, argument);  // Command
        } else {
          logzy::debug("'{}' parsed as Value", argument);
          out.emplace_back(std::in_place_index<3>, argument);  // Value
        }
      }

      return out;
    }

   private:
  };
}  // namespace cppli
