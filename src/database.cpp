#include "database.h"
#include <mutex>
#include <shared_mutex>

namespace amonkv {

void Database::set(const std::string& key, const std::string& value) {
    std::unique_lock<std::shared_mutex> lock(mutex);
    store[key] = value;
}

std::optional<std::string> Database::get(const std::string& key) const {
    std::shared_lock<std::shared_mutex> lock(mutex);
    auto it = store.find(key);
    if (it != store.end()) {
        return it->second;
    }
    return std::nullopt;
}

bool Database::del(const std::string& key) {
    std::unique_lock<std::shared_mutex> lock(mutex);
    return store.erase(key) > 0;
}

bool Database::exists(const std::string& key) const {
    std::shared_lock<std::shared_mutex> lock(mutex);
    return store.contains(key);
}

std::vector<std::string> Database::keys() const {
    std::shared_lock<std::shared_mutex> lock(mutex);
    std::vector<std::string> result;
    result.reserve(store.size());
    for (const auto& [k, v] : store) {
        result.push_back(k);
    }
    return result;
}

std::size_t Database::size() const {
    std::shared_lock<std::shared_mutex> lock(mutex);
    return store.size();
}

void Database::clear() {
    std::unique_lock<std::shared_mutex> lock(mutex);
    store.clear();
}

} // namespace amonkv
