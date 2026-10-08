#ifndef MAANCACHE_COMMAND_H
#define MAANCACHE_COMMAND_H

#include <string>
#include <string_view>
#include <vector>
#include <cstdint>

namespace maancache {

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

struct Command {
    CommandType type{CommandType::UNKNOWN};
    std::string key;
    std::string value;
    std::vector<std::string> args;

    static CommandType parse_type(std::string_view type_str) noexcept;
    static std::string_view type_to_string(CommandType type) noexcept;
    static Command parse(std::string_view raw_command);
    std::string to_string() const;
};

} // namespace maancache

#endif // MAANCACHE_COMMAND_H
