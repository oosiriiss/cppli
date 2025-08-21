#pragma once

#include <any>
#include <stdexcept>
#include <type_traits>
#include <typeindex>
#include <unordered_map>

#include "option.hpp"

// Function for parsing the argument from string_view
using ConverterFn = std::function<std::any(std::string_view)>;

namespace cli {

// The app comes with 2 default arguments (-h == --help and -v == --version)
class App {
public:
  // Title -> the name of the app.
  // description -> description of the app
  // version -> string version representation
  App(std::string_view title, std::string_view description,
      std::string_view version);
  void Run(const int argc, const char *const *const argv);

  // Tries to add the specified option to the list of options.
  // Throws std::invalid_argument if it encounters option with the same short
  // Throws std::invalid_argument if it encounters option with long name
  // beggining from '-' or '--'
  // name.
  template <typename T>
  void AddOption(Option::Identifier &&id, Option::Params<T> &&params);
  // Registering custom type convreter from std::string_view
  template <typename T> void RegisterConverter(ConverterFn &&converter);
  // For now only short name querying is avaialable since it is used as a
  // key in the hashmap
  std::optional<const Option &> GetOption(std::string_view shortName) const;

  // Automatically generates the help message from the options
  void PrintHelp() const;

  // Automatically generates the option version message from title description
  // and version
  void PrintVersion() const;

private:
  template <typename T> T convert(std::string_view sv);

private:
  // Name of the application
  const std::string_view title_;
  // Short (not necessarily :) ) description of the application
  const std::string_view description_;
  // String encoded version of the application
  const std::string_view version_;

  // Registered options
  OptionStorage options_;
  // Custom parsers for options
  std::unordered_map<std::type_index, ConverterFn> custom_parsers_;
};

template <typename T>
void App::AddOption(Option::Identifier &&id, Option::Params<T> &&params) {
  Option o{
      .description = params.description.value_or(""),
      .expectsArgument = !std::is_void<T>(),
      .rawValue = std::nullopt,
      .callback =
          (params.callback) ? std::optional([this,
                                             def =
                                                 std::move(params.defaultValue),
                                             callback =
                                                 std::move(*params.callback)](
                                                std::optional<std::string_view>
                                                    raw_value) {
            if (raw_value) {
              T val = convert<T>(*raw_value);
              std::invoke(callback, std::move(val));
            } else if (def) {
              std::invoke(callback, std::move(*def));
            } else {
              throw std::logic_error(
                  "No value and no default value whn invoking a callback");
            }
          })
                            : std::nullopt,
  };

  options_.Add(std::move(id), std::move(o));
}

template <typename T> void App::RegisterConverter(ConverterFn &&converter) {
  custom_parsers_[std::type_index(typeid(T))] =
      [this,
       convert = std::move(converter)](std::string_view raw_value) -> std::any {
    return std::any(convert(raw_value));
  };
}
template <typename T> T App::convert(std::string_view sv) {
  auto it = custom_parsers_.find(std::type_index(typeid(T)));
  if (it != custom_parsers_.end()) {
    return std::any_cast<T>(it->second(sv));
  }
  throw std::logic_error("Default converters not yet implemented");
}

} // namespace cli
