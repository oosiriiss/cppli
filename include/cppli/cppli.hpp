#pragma once

#include <string_view>
#include <vector>

#include "cppli/command.hpp"
#include "cppli/cppli_export.hpp"
#include "cppli/option.hpp"
#include "cppli/multikey_map.hpp"

namespace cppli {

  CPPLI_EXPORT class App {
   public:
    void parseArgs(int argc, char const* const* argv);
    void registerOption(const std::vector<std::string>& aliases, Option option);

   private:
    std::vector<Option> matchedOptions_;
    std::vector<std::string_view> positionalArgs_;

    Command rootCommand_;
  };

}  // namespace cppli
