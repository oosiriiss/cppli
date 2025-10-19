#include "cppli/cppli.hpp"

#include "argparser.hpp"

namespace cppli {

  void App::registerOption(const std::vector<std::string>& aliases,
                           Option option) {
    rootCommand_.options.put(aliases, option);
  }

  void App::parseArgs(int argc, char const* const* const argv) {
    std::vector<std::string_view> argvVector =
        ArgParser::argvToVector(argc, argv);

    auto result =
        ArgParser::parseProgramArguments(argvVector, this->rootCommand_);

    positionalArgs_.append_range(result.positionalArgs);
  }

}  // namespace cppli
