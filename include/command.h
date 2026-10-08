#ifndef AYUSHDB_COMMAND_H
#define AYUSHDB_COMMAND_H

#include <string>
#include <string_view>
#include <vector>
#include <cstdint>

namespace ayushdb {

/**
 * @brief Supported commands in AyushDB.
 */
enum class CommandType : std::uint8_t {
    SET,
    GET,
    DEL,
    EXISTS,
    KEYS,
    SIZE,
    CLEAR,
    UNKNOWN
};

/**
 * @brief Representation of a parsed database command.
 */
struct Command {
    CommandType type{CommandType::UNKNOWN};
    std::string key;
    std::string value;
    std::vector<std::string> args;

    [[nodiscard]] static CommandType parse_type(std::string_view type_str) noexcept;
    [[nodiscard]] static std::string_view type_to_string(CommandType type) noexcept;
    [[nodiscard]] static Command parse(std::string_view raw_command);
    [[nodiscard]] std::string to_string() const;
};

} // namespace ayushdb

#endif // AYUSHDB_COMMAND_H
