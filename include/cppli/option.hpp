#pragma once
#include <functional>
#include <optional>
#include <string_view>

#include "cppli/cppli_export.hpp"

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

namespace cppli {
  struct CPPLI_EXPORT Option {
    // Function used when calling an option's callback
    using OptionCallback = std::function<void(std::optional<std::string_view>)>;

    struct Identifier {
      // Short name for the option like -h (But without the dash '-')
      char shortName;
      // Long name for the option like --help (But without the double dashes
      // dash
      // '-')
      // TODO :: string_view?
      std::string_view longName;
    };

    template <typename T>
    struct Params {
      std::optional<std::string_view> description = std::nullopt;

      std::optional<T> defaultValue = std::nullopt;

      // TODO :: Hm this could be made std::invocable
      std::optional<std::function<void(T)>> callback = std::nullopt;
    };

    // Gets called after and option is discovered from argv.
    // If rawValue is std::nullopt then the default value will be used.
    void Matched() const;

    // Mostly kept here for printing purposes, but since its relative light it
    // should matter too much
    // Identifier identifier;

    // Description of what the option is used for.
    std::string_view description;

    // Decides whether this option has an argument
    bool expectsArgument;

    // Raw string value of the arugment;
    std::optional<std::string_view> rawValue = std::nullopt;

    // Callback invoked when argument is discovered in the argv
    std::optional<OptionCallback> callback = std::nullopt;
  };

  class OptionStorage {
   public:
    void Add(const Option::Identifier& identifier, Option&& opt);

    bool Contains(char shortOption) const;
    bool Contains(std::string_view longOption) const;
    bool Contains(const Option::Identifier& identifier) const;

    Option& operator[](char shortOption);
    Option& operator[](std::string_view longOption);

   private:
    char ConvertLongToShort(std::string_view longOption) const;

    std::vector<Option> options_;
    std::unordered_map<char, size_t> shortIndexMap_;
    std::unordered_map<std::string_view, size_t> longIndexMap_;
  };

}  // namespace cppli
