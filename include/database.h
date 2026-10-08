#ifndef MAANCACHE_DATABASE_H
#define MAANCACHE_DATABASE_H

#include <string>
#include <unordered_map>
#include <vector>
#include <optional>
#include <shared_mutex>
#include <cstddef>

namespace maancache {

class Database {
public:
    Database() = default;
    ~Database() = default;

    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    void set(const std::string& key, const std::string& value);
    std::optional<std::string> get(const std::string& key) const;
    bool del(const std::string& key);
    bool exists(const std::string& key) const;
    std::vector<std::string> keys() const;
    std::size_t size() const;
    void clear();

private:
    std::unordered_map<std::string, std::string> store;
    mutable std::shared_mutex mutex;
};

} // namespace maancache

#endif // MAANCACHE_DATABASE_H
