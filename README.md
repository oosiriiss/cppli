# cppli

A Header-only, modern, type-safe, and flexible command-line interface (CLI) parsing library for C++23.

> [!CAUTION]
> This project is currently under active development and is not considered finished. the API is definitely a subject to change and is not stable. Features are still being implemented, and there may be unresolved bugs. 


## Features

* **Header-Only:** Ready to use out of the box!
* **Flexible Option Identifiers:** Define your options with custom identifiers like ```enum class``` to easily query the parsed arguments without the worry of spelling mistakes
* **Built-in Help Generation:** Automatically format beautiful (?), wrapped help messages with multi-line column support.

## Requirements 

Modern C++ compiler that supports **C++23**.

## Installation

**Manually**

Since cppli is header-only, you can simply copy the header files in ```./include/``` directly into your project and use them!

**Using CMake (FetchContent)**

```cmake
include(FetchContent)
FetchContent_Declare(
    cppli 
    GIT_REPOSITORY https://github.com/oosiriiss/cppli
    GIT_TAG 0.2.0 # Or your preferred branch/tag/hash
)
FetchContent_Declare(cppli)

# Link the target
target_link_libraries(your_target PRIVATE cppli::cppli)
```

## Usage

Usage examples can be found in the [example](./example/)  directory.

## Quickstart

```cpp
#include <cstdlib>
#include <print>

#include "cppli/cppli.hpp"

enum class ArgumentType : std::uint8_t { Help};

auto main(int argc, char** argv) -> int {
  cppli::OptionContainer<ArgumentType> options;

  options.addOption(ArgumentType::Help,
                    cppli::Option{
                        .firstName = "-h",
                        .secondName = "--help",
                        .description = "Displays help message",
                        .needsValue = false,
                    });

  cppli::ParseResult result = cppli::parseArguments(argc, argv, options);

  if (result.options.contains(ArgumentType::Help)) {
    std::println("Help message!");
    return EXIT_SUCCESS;
  }

  return EXIT_SUCCESS;
}
```


## License

cppli is licensed under the MIT License. See the [License](./LICENSE) for details.
