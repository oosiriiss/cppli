#pragma once

#include <logzy/logzy.hpp>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

struct StringViewHash {
  using is_transparent = void;  // NOLINT
  auto operator()(std::string_view str) const noexcept -> std::size_t {
    return std::hash<std::string_view>{}(str);
  }
};

struct StringViewEqual {
  using is_transparent = void;  // NOLINT
  auto operator()(std::string_view lhs, std::string_view rhs) const noexcept
      -> bool {
    return lhs == rhs;
  }
};

namespace cppli {

  struct Command;

  // An option which expects argument
  struct Option {
    bool expectsValue;
  };

  template <typename StoredType>
  class ArgumentStorage {
   public:
    [[nodiscard]] auto get(std::string_view identifier) const noexcept
        -> std::optional<std::weak_ptr<StoredType>> {
      const auto iter = values_.find(identifier);
      if (iter == values_.end()) {
        return std::nullopt;
      }

      return std::optional(std::weak_ptr<StoredType>(iter->second));
    }

    template <std::size_t Size>
    auto put(std::array<std::string, Size>&& aliases, StoredType&& data)
        -> std::optional<std::weak_ptr<StoredType>> {
      for (const auto& alias : aliases) {
        if (values_.contains(alias)) {
          logzy::error(
              "argument with short name '{}' already exists. Ignoring current "
              "argument.",
              aliases);
          return std::nullopt;
        }
      }

      auto ptr = std::make_shared<StoredType>(std::move(data));

      for (const auto& alias : std::move(aliases)) {
        values_.emplace(std::move(alias), ptr);
      }

      return std::weak_ptr<StoredType>(ptr);
    }

    [[nodiscard]] auto contains(std::string_view identifier) const noexcept
        -> bool {
      return values_.contains(identifier);
    }

   private:
    std::unordered_map<std::string, std::shared_ptr<StoredType>, StringViewHash,
                       StringViewEqual>
        values_;
  };

  // Command determining program behaviour
  struct Command {
    std::size_t positionalArgumentCount = 0;
    ArgumentStorage<Command> subcommands;
    ArgumentStorage<Option> options;
  };

}  // namespace cppli
