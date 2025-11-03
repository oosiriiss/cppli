#pragma once

#include <any>
#include <expected>
#include <functional>
#include <string_view>
#include <typeindex>
#include <vector>

#include "cppli/command.hpp"
#include "cppli/cppli_export.hpp"
#include "cppli/option.hpp"

namespace cppli {
  CPPLI_EXPORT class App {
   public:
    void parseArgs(int argc, char const* const* argv);
    void registerOption(const std::vector<std::string>& aliases, Option option);

    template <typename T>
    constexpr void registerConverter(
        std::function<T(std::string_view)> converterFn);

    template <typename T>
    [[nodiscard]] constexpr auto getPositionalArgument(
        std::size_t argumentIndex) const -> std::expected<T, std::string>;

   private:
    std::unordered_map<std::type_index,
                       std::function<std::any(std::string_view)>>
        converters_;

    std::vector<Option> matchedOptions_;
    std::vector<std::string_view> positionalArgs_;

    Command rootCommand_;
  };

  //////////////////////////////////////////
  ////////// Template definitions /////////
  //////////////////////////////////////////

  template <typename T>
  constexpr void App::registerConverter(
      std::function<T(std::string_view)> converterFn) {
    converters_[std::type_index(typeid(T))] =
        [converterFn](std::string_view raw) {
          return std::make_any<T>(converterFn(raw));
        };
  }

  template <typename T>
  [[nodiscard]] constexpr auto App::getPositionalArgument(
      std::size_t argumentIndex) const -> std::expected<T, std::string> {
    using Exp = std::expected<T, std::string>;
    // TODO :: Maybe return string view of raw argument in case of error?
    // TODO :: Support custom formatters

    if (argumentIndex >= positionalArgs_.size()) {
      return std::unexpected(
          std::string("Provided positional argument index than the number of "
                      "passed positional arguments"));
    }

    std::string_view rawArgument = positionalArgs_.at(argumentIndex);
    logzy::debug("Parsing argument: {}", rawArgument);

    if constexpr (std::integral<T>) {
      logzy::debug("Argument is integral");
      T integerValue;
      auto result = std::from_chars(rawArgument.data(),
                                    rawArgument.data() + rawArgument.size(),
                                    integerValue);
      if (result.ec == std::errc::invalid_argument) {
        logzy::debug("Invalid argument");
        return std::unexpected(std::string("Couldn't convert integral value"));
      }
      return Exp(integerValue);
    } else if (converters_.contains(std::type_index(typeid(T)))) {
      return std::any_cast<T>(
          converters_.at(std::type_index(typeid(T)))(rawArgument));
    } else {
      return std::unexpected(std::string(
          "Cannot convert positional type. Unsupported type. Cannot convert"));
    }
  }

}  // namespace cppli
