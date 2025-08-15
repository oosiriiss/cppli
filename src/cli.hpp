#include <any>
#include <format>
#include <functional>
#include <optional>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <unordered_map>

// Function used when calling an option's callback
using OptionCallback = std::function<void(std::string_view)>;

// Function for parsing the argument from string_view
using ConverterFn = std::function<std::any(std::string_view)>;

namespace cli {
class Option {

public:
  // Gets called when option is used in App args.
  void Matched(std::optional<std::string_view> arg) const;

  // Short name should  be a letter like h. It will react -h option.
  // Long name should  be a String like help. It will react to --help option.
  // Description should be a string describing what does this option do.
  Option(const char shortName, const std::string longName,
         const std::string description, bool expectsArgument,
         std::optional<OptionCallback> callback = std::nullopt);

public:
  // Short name for the option like -h (But without the dash '-')
  const char shortName;
  // Long name for the option like --help (But without the double dashes dash
  // '-')
  const std::string longName;

  // Description of what the option is used for.
  const std::string description;

  // Decides whether this option has an argument
  const bool expectsArgument;

  // Raw string value of the arugment;
  std::string_view raw_value;

private:
  // Callback invoked when argument is discovered in the argv
  const std::optional<OptionCallback> callback_ = std::nullopt;
};

// The app comes with 2 default arguments (-h == --help and -v == --version)
class App {
public:
  // Title -> the name of the app.
  // description -> description of the app
  // version -> string version representation
  App(std::string title, std::string description, std::string version);
  void Initialize(int argc, char **argv);

  // Tries to add the specified option to the list of options.
  // Throws std::invalid_argument if it encounters option with the same short
  // Throws std::invalid_argument if it encounters option with long name
  // beggining from '-' or '--'
  // name.
  template <typename T, typename F>
    requires std::invocable<F, T>
  void AddOption(char shortName, F &&callback) {
    if (options_.contains(shortName))
      throw std::logic_error(
          std::format("duplicate short option '{}'", shortName));

    Option o = Option(
        shortName, "LONG_NAME", "DESC", true,
        [this, callback = std::move(callback)](std::string_view raw_value) {
          T val = convert<T>(raw_value);
          std::invoke(callback, std::move(val));
        });
    options_.emplace(shortName, std::move(o));
  }

  // Registering custom type convreter from std::string_view
  template <typename T> void RegisterConverter(ConverterFn &&converter) {
    custom_parsers_[std::type_index(typeid(T))] =
        [this, convert = std::move(converter)](
            std::string_view raw_value) -> std::any {
      return std::any(convert(raw_value));
    };
  }

  // For now only short name querying is avaialable since it is used as a
  // key in the hashmap
  std::optional<const Option &> GetOption(std::string_view shortName) const;

  // Automatically generates the help message from the options
  void PrintHelp() const;

  // Automatically generates the option version message from title description
  // and version
  void PrintVersion() const;

  // This was done only public for testing of argvparser
  std::unordered_map<char, Option> options_;
  // This was done only public for testing of argvparser
  std::unordered_map<std::type_index, ConverterFn> custom_parsers_;

private:
  void ParseOptions(const int argc, const char *const *const argv) const;
  bool TryParseShortOption(std::string_view arg) const;
  bool TryParseLongOption(std::string_view arg) const;

private:
  // Name of the application
  const std::string title_;
  // Short (not necessarily :) ) description of the application
  const std::string description_;
  // String encoded version of the application
  const std::string version_;

  template <typename T> T convert(std::string_view sv) {
    auto it = custom_parsers_.find(std::type_index(typeid(T)));
    if (it != custom_parsers_.end()) {
      return std::any_cast<T>(it->second(sv));
    }
    throw std::logic_error("Default converters not yet implemented");
  }
};
} // namespace cli
