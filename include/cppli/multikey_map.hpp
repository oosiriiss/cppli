#pragma once
#include <logzy/logzy.hpp>
#include <memory>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <vector>

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

// TODO :: This class could be totally constexpr since it is know at compile
// time

template <typename StoredType>
class MultiKeyMap {
 public:
  [[nodiscard]] auto get(std::string_view identifier) const noexcept
      -> std::optional<std::weak_ptr<StoredType>> {
    const auto iter = values_.find(identifier);
    if (iter == values_.end()) {
      return std::nullopt;
    }

    return std::optional(std::weak_ptr<StoredType>(iter->second));
  }

  auto put(const std::vector<std::string>& aliases, StoredType data)
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

    auto ptr = std::make_shared<StoredType>(data);

    for (const auto& alias : aliases) {
      values_.emplace(alias, ptr);
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
      values_{};
};
