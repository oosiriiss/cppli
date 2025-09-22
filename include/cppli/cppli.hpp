#pragma once

#include <any>
#include <logzy/logzy.hpp>
#include <optional>
#include <print>
#include <stdexcept>
#include <type_traits>
#include <typeindex>
#include <unordered_map>

#include "cppli/cppli_export.hpp"
#include "option.hpp"

/**
 * A note about the MSVC warning C4251:
 * This warning should be suppressed for private data members of the project's
 * exported classes, because there are too many ways to work around it and all
 * involve some kind of trade-off (increased code complexity requiring more
 * developer time, writing boilerplate code, longer compile times), but those
 * solutions are very situational and solve things in slightly different ways,
 * depending on the requirements of the project.
 * That is to say, there is no general solution.
 *
 * What can be done instead is understand where issues could arise where this
 * warning is spotting a legitimate bug. I will give the general description of
 * this warning's cause and break it down to make it trivial to understand.
 *
 * C4251 is emitted when an exported class has a non-static data member of a
 * non-exported class type.
 *
 * The exported class in our case is the class below (exported_class), which
 * has a non-static data member (m_name) of a non-exported class type
 * (std::string).
 *
 * The rationale here is that the user of the exported class could attempt to
 * access (directly, or via an inline member function) a static data member or
 * a non-inline member function of the data member, resulting in a linker
 * error.
 * Inline member function above means member functions that are defined (not
 * declared) in the class definition.
 *
 * Since this exported class never makes these non-exported types available to
 * the user, we can safely ignore this warning. It's fine if there are
 * non-exported class types as private member variables, because they are only
 * accessed by the members of the exported class itself.
 *
 * The name() method below returns a pointer to the stored null-terminated
 * string as a fundamental type (char const), so this is safe to use anywhere.
 * The only downside is that you can have dangling pointers if the pointer
 * outlives the class instance which stored the string.
 *
 * Shared libraries are not easy, they need some discipline to get right, but
 * they also solve some other problems that make them worth the time invested.
 */

// Function for parsing the argument from string_view
using ConverterFn = std::function<std::any(std::string_view)>;

namespace cppli {

  // The app comes with 2 default arguments (-h == --help and -v == --version)
  class CPPLI_EXPORT App {
   public:
    // Title -> the name of the app.
    // description -> description of the app
    // version -> string version representation
    App(std::string_view title, std::string_view description,
        std::string_view version);
    void Run(int argc, const char *const *argv);

    // Tries to add the specified option to the list of options.
    // Throws std::invalid_argument if it encounters option with the same short
    // Throws std::invalid_argument if it encounters option with long name
    // beggining from '-' or '--'
    // name.
    template <typename T>
    void AddOption(Option::Identifier identifier,
                   const Option::Params<T> &params);
    // Registering custom type convreter from std::string_view
    template <typename T>
    void RegisterConverter(std::function<T(std::string_view)> &&converter);

    template <typename T>
    std::optional<T> GetOptionValue(char shortName) const;

    template <typename T>
    std::optional<T> GetOptionValue(std::string_view longName) const;

    // Automatically generates the help message from the options
    void PrintHelp() const;

    // Automatically generates the option version message from title description
    // and version
    void PrintVersion() const;

   private:
    template <typename T>
    T Convert(std::string_view rawValue) const;

   private:
    // Name of the application
    std::string_view title_;

    // Short (not necessarily :) ) description of the application
    std::string_view description_;

    // String encoded version of the application
    std::string_view version_;

    // Registered options
    OptionStorage options_;

    // Custom parsers for options
    std::unordered_map<std::type_index, ConverterFn> customParsers_;
  };

  template <typename T>
  void App::AddOption(Option::Identifier identifier,
                      const Option::Params<T> &params) {
    Option opt{
        .description = params.description.value_or(""),
        .expectsArgument = !std::is_void<T>(),
        .rawValue = std::nullopt,
        .defaultValue =
            (params.defaultValue) ? *params.defaultValue : std::any(),
        .callback = (params.callback) ? std::optional([this,
                                                       def = std::move(
                                                           params.defaultValue),
                                                       callback = std::move(
                                                           *params.callback)](
                                                          std::optional<
                                                              std::string_view>
                                                              rawValue) {
          if (rawValue) {
            logzy::debug("Raw Value: {}", *rawValue);
            T val = Convert<T>(*rawValue);
            std::invoke(callback, std::move(val));
          } else if (def) {
            logzy::debug("Default opotion invoked");
            std::invoke(callback, std::move(*def));
          } else {
            throw std::logic_error(
                "No value and no default value whn invoking a callback");
          }
        })
                                      : std::nullopt,
    };

    options_.Add(identifier, std::move(opt));
  }

  template <typename T>
  void App::RegisterConverter(std::function<T(std::string_view)> &&converter) {
    customParsers_[std::type_index(typeid(T))] =
        [convert = std::move(converter)](std::string_view rawValue) {
          return std::any(convert(rawValue));
        };
  }

  template <typename T>
  T App::Convert(std::string_view rawValue) const {
    // TODO :: Default converters for basic types
    if constexpr (std::is_same_v<T, std::string_view>) {
      return rawValue;
    }

    auto iter = customParsers_.find(std::type_index(typeid(T)));
    if (iter == customParsers_.end()) {
      throw std::logic_error("Default converters not yet implemented");
    }
    const auto &converter = iter->second;
    return std::any_cast<T>(converter(rawValue));
  }

  template <typename T>
  std::optional<T> App::GetOptionValue(char shortName) const {
    // TODO :: Refactor these duplication
    //  TODO :: Introduce ?variant? class for identifie rpart so i dont have to
    //  duplicat estuff
    if (!options_.Contains(shortName)) {
      return std::nullopt;
    }

    const Option &option = options_.Get(shortName);

    if (!option.rawValue) {
      if (option.defaultValue.has_value()) {
        return std::any_cast<T>(option.defaultValue);
      }
      return std::nullopt;
    }

    const std::string_view rawValue = *option.rawValue;
    return std::any_cast<T>(Convert<T>(rawValue));
  }

  template <typename T>
  std::optional<T> App::GetOptionValue(std::string_view longName) const {
    if (!options_.Contains(longName)) {
      return std::nullopt;
    }

    const Option &option = options_.Get(longName);

    if (!option.rawValue) {
      if (option.defaultValue.has_value()) {
        return std::any_cast<T>(option.defaultValue);
      }
      return std::nullopt;
    }

    std::println("ligma");

    const std::string_view rawValue = *option.rawValue;
    return std::any_cast<T>(Convert<T>(rawValue));
  }

}  // namespace cppli
