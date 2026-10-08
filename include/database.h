#ifndef AYUSHDB_DATABASE_H
#define AYUSHDB_DATABASE_H

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <optional>
#include <shared_mutex>
#include <cstddef>

namespace ayushdb {

/**
 * @brief Thread-safe in-memory key-value database engine.
 * Supports concurrent readers and single writer access model using std::shared_mutex.
 */
class Database {
public:
    Database() = default;
    ~Database() = default;

    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
    Database(Database&&) noexcept = default;
    Database& operator=(Database&&) noexcept = default;

    /**
     * @brief Inserts or updates a key-value pair.
     */
    void set(std::string_view key, std::string_view value);

    /**
     * @brief Retrieves value for a key if present.
     */
    [[nodiscard]] std::optional<std::string> get(std::string_view key) const;

    /**
     * @brief Deletes a key if present.
     */
    bool del(std::string_view key);

    /**
     * @brief Checks if a key exists in the database.
     */
    [[nodiscard]] bool exists(std::string_view key) const;

    /**
     * @brief Returns vector of all stored keys.
     */
    [[nodiscard]] std::vector<std::string> keys() const;

    /**
     * @brief Returns current total entry count.
     */
    [[nodiscard]] std::size_t size() const;

    /**
     * @brief Removes all key-value entries.
     */
    void clear();

private:
    std::unordered_map<std::string, std::string> store_;
    mutable std::shared_mutex mutex_;
};

} // namespace ayushdb

#endif // AYUSHDB_DATABASE_H
