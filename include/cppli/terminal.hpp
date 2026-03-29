#pragma once

#include <cstdint>

#if defined(_WIN32)
#define CPPLI_WINDOWS

#elif defined(__linux__) || defined(__APPLE) || defined(__unix__)
#define CPPLI_POSIX
#include <sys/ioctl.h>
#include <unistd.h>
#endif

namespace cppli::internal {

  constexpr uint16_t DEFAULT_TERMINAL_WIDTH = 80;

#if defined(CPPLI_WINDOWS)
  namespace windows {
    // Manual 'import' of Windows functions to avoid bloating with #include
    // "windows.h"

    using HANDLE = void*;
    using DWORD = unsigned long;
    using BOOL = int;
    using WORD = unsigned short;
    using SHORT = short;

    constexpr DWORD STDOUT_HANDLE = -11;

    struct Coord {
      SHORT x;
      SHORT y;
    };
    struct SmallRect {
      SHORT left;
      SHORT top;
      SHORT right;
      SHORT bottom;
    };
    struct ConsoleBufferInfo {
      Coord size;
      Coord cursorPos;
      WORD attrs;
      SmallRect window;
      Coord maxWindowSize;
    };
    extern "C" {
    __declspec(dllimport) HANDLE __stdcall GetStdHandle(              // NOLINT
        DWORD nStdHandle);                                            // NOLINT
    __declspec(dllimport) BOOL __stdcall GetConsoleScreenBufferInfo(  // NOLINT
        HANDLE handle, ConsoleBufferInfo* bufInfo);                   // NOLINT
    }
    inline auto getTerminalWidth() -> uint16_t {
      ConsoleBufferInfo info{};
      if (GetConsoleScreenBufferInfo(GetStdHandle(STDOUT_HANDLE), &info) > 0) {
      } else {
        info.size.x = DEFAULT_TERMINAL_WIDTH;
      }
      return static_cast<uint16_t>(info.size.x);
    }
  }  // namespace windows
#endif

#if defined(CPPLI_POSIX)
  namespace posix {
    inline auto getTerminalWidth() -> uint16_t {
      struct winsize size;
      if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == -1) {
        // Fallback on error

        size.ws_col = DEFAULT_TERMINAL_WIDTH;
      }

      return static_cast<uint16_t>(size.ws_col);
    }
  }  // namespace posix
#endif

  [[nodiscard]] inline auto getTerminalWidth() -> uint16_t {
#if defined(CPPLI_WINDOWS)
    return windows::getTerminalWidth();
#elif defined(CPPLI_POSIX)
    return posix::getTerminalWidth();
#else
#warning "Unknown system. terminal width set to default value"
    return DEFAULT_TERMINAL_WIDTH;
#endif
  }

}  // namespace cppli::internal
