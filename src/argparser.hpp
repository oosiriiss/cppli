#pragma once
#include <algorithm>
#include <cstdint>
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
    constexpr explicit ArgParser(int argc, char const* const* const argv)
        : argv_(argvToVector(argc, argv)) {}

    // Conestexpr for tests since it cannot wrok with argc and argv
    [[nodiscard]] constexpr static auto argvToVector(
        int argc,
        char const* const* const argv)  // NOLINT C-style array is required
        -> std::vector<std::string_view> {
      return std::vector<std::string_view>(
          argv, std::next(argv, static_cast<std::ptrdiff_t>(argc)));
    }

    [[nodiscard]] constexpr auto parseProgramArguments(
        std::span<std::string_view> commands,
        std::span<std::string_view> options) -> std::vector<Argument> {
      std::vector<Argument> out;
      out.reserve(argv_.size() * 2);

      for (const std::string_view argument : argv_) {
        if (argument.starts_with('-') && argument.size() > 1) {
          if (std::ranges::find(options, argument) != options.end()) {
            logzy::debug("'{}' parsed as Option", argument);
            out.emplace_back(std::in_place_index<1>, argument);  // Option
          } else {
            logzy::debug("'{}' parsed as Flag", argument);
            out.emplace_back(std::in_place_index<2>, argument);  // Flag
          }
          continue;
        }

        if (std::ranges::find(commands, argument) != commands.end()) {
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
    enum class ParseState : std::uint8_t { Value, ShortFlag, LongFlag, End };

   private:
    /**
     * View of the program's argv
     */
    std::vector<std::string_view> argv_;

    /**
     * Index pointing to the currently procesed char of current argument
     * Used only when parsing Flag argument like '-xvzf' (state is
     * ParserState::FlagRead)
     */
    std::size_t shortFlagIndex_ = 0;

    /*
     * Current state of the parser
     */
    ParseState state_ = ParserState::NewArgument;
  };

}  // namespace cppli
