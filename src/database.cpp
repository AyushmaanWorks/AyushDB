#include "database.h"
#include <mutex>
#include <shared_mutex>

namespace ayushdb {

void Database::set(std::string_view key, std::string_view value) {
    std::unique_lock lock(mutex_);
    store_[std::string(key)] = std::string(value);
}

[[nodiscard]] std::optional<std::string> Database::get(std::string_view key) const {
    std::shared_lock lock(mutex_);
    const auto it = store_.find(std::string(key));
    if (it != store_.end()) {
        return it->second;
    }
    return std::nullopt;
}

bool Database::del(std::string_view key) {
    std::unique_lock lock(mutex_);
    return store_.erase(std::string(key)) > 0;
}

[[nodiscard]] bool Database::exists(std::string_view key) const {
    std::shared_lock lock(mutex_);
    return store_.contains(std::string(key));
}

[[nodiscard]] std::vector<std::string> Database::keys() const {
    std::shared_lock lock(mutex_);
    std::vector<std::string> result;
    result.reserve(store_.size());
    for (const auto& [k, v] : store_) {
        result.push_back(k);
    }
    return result;
}

[[nodiscard]] std::size_t Database::size() const {
    std::shared_lock lock(mutex_);
    return store_.size();
}

void Database::clear() {
    std::unique_lock lock(mutex_);
    store_.clear();
}

} // namespace ayushdb
