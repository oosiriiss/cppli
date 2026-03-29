#pragma once
#include "cppli/option.hpp"
#include "cppli/terminal.hpp"

namespace cppli {

  namespace internal {
    // All indents are in spaces.

    struct OutputBuffer {
      std::string data;
      std::uint16_t lineWidth{80};

      static constexpr auto INDENT = ' ';
      static constexpr size_t OPT_INDENT = 3;
      static constexpr size_t OPT_COL_WIDTH = 25;
      static constexpr size_t DESC_START_IDX = OPT_INDENT + OPT_COL_WIDTH;

      constexpr void writeHeader(const std::string& header) {
        data.append(header);
        data.push_back('\n');
      }
      constexpr void endSection() {
        data.push_back('\n');
        data.push_back('\n');
      }

      constexpr void writeOption(const std::string& firstName,
                                 const std::string& secondName,
                                 const std::string& description,
                                 const bool needsValue) {
        std::string nameColumn = formatName(firstName, secondName, needsValue);

        size_t nameOffset = 0;
        size_t descriptionOffset = 0;

        const size_t descriptionColumnWidth =
            lineWidth - OPT_INDENT - OPT_COL_WIDTH;

        while (nameOffset < nameColumn.size() ||
               descriptionOffset < description.size()) {
          data.append(OPT_INDENT, INDENT);
          writeColumnPart(nameColumn, nameOffset, OPT_COL_WIDTH);

          writeColumnPart(description, descriptionOffset,
                          descriptionColumnWidth);
          data.push_back('\n');
        }
      }

      static constexpr auto removeLeadingSpaces(std::string_view& str) noexcept
          -> size_t {
        size_t count = 0;
        while (str.starts_with(' ')) {
          str.remove_prefix(1);
          ++count;
        }
        return count;
      }

      static constexpr auto getRawColumnLine(
          const std::string& data, size_t offset,
          const std::uint16_t maxWidth) noexcept -> std::string_view {
        const auto maximumToWrite =
            std::min<size_t>(data.size() - offset, maxWidth);
        return std::string_view{data.data() + offset, maximumToWrite};
      }

      static constexpr void truncateToWholeWords(std::string_view& fragment,
                                                 const std::string& data,
                                                 size_t dataOffset) noexcept

      {
        const bool dataDoesntEndWithFragment =
            data.size() > (dataOffset + fragment.size());
        if (fragment.size() > 0 && dataDoesntEndWithFragment) {
          const bool endsWithWhitespace = std::isspace(fragment.back()) != 0;
          const bool isEndOfWord =
              std::isspace(data[dataOffset + fragment.size()]) != 0;

          if (!endsWithWhitespace && !isEndOfWord) {
            const size_t lastSpaceIndex = fragment.find_last_of(' ');
            if (lastSpaceIndex != std::string_view::npos) {
              fragment = fragment.substr(0, lastSpaceIndex + 1);
            }
          }
        }
      }

      /**
       * Truncates the given string to the first \n character
       *
       * Returns true if the character was present.
       */
      static constexpr auto truncateToNewline(std::string_view& part) -> bool {
        if (auto newLineIndex = part.find('\n');
            newLineIndex != std::string_view::npos) {
          part = part.substr(0, newLineIndex);
          return true;
        }
        return false;
      }

     private:
      constexpr void writeColumnPart(const std::string& data, size_t& offset,
                                     const std::uint16_t width) {
        // Nothing to write, filling with padding
        if (offset >= data.size()) {
          this->data.append(width, ' ');
          return;
        }

        auto part = getRawColumnLine(data, offset, width);

        // On Each row's leading spaces are ignored
        offset += removeLeadingSpaces(part);

        // Words are preserved (or cut if longer than {width})
        truncateToWholeWords(part, data, offset);

        // Newlines also denote end of column part.
        size_t toSkipAfter = truncateToNewline(part) ? 1 : 0;

        const std::uint16_t padding = width - part.size();
        this->data.append(part);
        this->data.append(padding, ' ');

        offset += part.size() + toSkipAfter;
      }

      static constexpr auto formatName(const std::string& firstName,
                                       const std::string& secondName,
                                       const bool needsValue) -> std::string {
        std::string nameColumn;
        nameColumn.reserve(OPT_COL_WIDTH);
        if (!firstName.empty()) {
          nameColumn.append(firstName);
        }
        if (!firstName.empty() && !secondName.empty()) {
          nameColumn.push_back(',');
          nameColumn.push_back(' ');
        }
        if (!secondName.empty()) {
          nameColumn.append(secondName);
        }

        if (needsValue) {
          nameColumn.append(" <ARG>");
        }

        return nameColumn;
      }
    };

  }  // namespace internal

  template <class OptionKey>
  inline auto createHelp(const OptionContainer<OptionKey>& options,
                         std::string_view programName,
                         std::string_view exampleSection = "") -> std::string {
    auto termWidth = internal::getTerminalWidth();

    internal::OutputBuffer buf{.lineWidth = termWidth};

    buf.writeHeader("USAGE:");
    buf.data += std::format("{} [OPTIONS] [POS_ARGS]\n", programName);
    buf.endSection();

    if (!exampleSection.empty()) {
      buf.writeHeader("EXAMPLE:");
      buf.data += exampleSection;
      buf.endSection();
    }

    buf.writeHeader("OPTIONS:");

    for (const auto& [key, opt] : options) {
      buf.writeOption(opt.firstName, opt.secondName, opt.description,
                      opt.needsValue);
      buf.data.push_back('\n');
    }
    buf.endSection();

    return buf.data;
  }

}  // namespace cppli
