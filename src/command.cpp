#include "command.h"
#include <algorithm>
#include <cctype>
#include <sstream>

namespace ayushdb {

namespace {
    std::string to_upper(std::string_view sv) {
        std::string upper(sv);
        std::transform(upper.begin(), upper.end(), upper.begin(),
                       [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
        return upper;
    }
}

[[nodiscard]] CommandType Command::parse_type(std::string_view type_str) noexcept {
    const std::string name = to_upper(type_str);
    if (name == "SET")    return CommandType::SET;
    if (name == "GET")    return CommandType::GET;
    if (name == "DEL")    return CommandType::DEL;
    if (name == "EXISTS") return CommandType::EXISTS;
    if (name == "KEYS")   return CommandType::KEYS;
    if (name == "SIZE")   return CommandType::SIZE;
    if (name == "CLEAR")  return CommandType::CLEAR;
    return CommandType::UNKNOWN;
}

[[nodiscard]] std::string_view Command::type_to_string(CommandType type) noexcept {
    switch (type) {
        case CommandType::SET:    return "SET";
        case CommandType::GET:    return "GET";
        case CommandType::DEL:    return "DEL";
        case CommandType::EXISTS: return "EXISTS";
        case CommandType::KEYS:   return "KEYS";
        case CommandType::SIZE:   return "SIZE";
        case CommandType::CLEAR:  return "CLEAR";
        default:                  return "UNKNOWN";
    }
}

[[nodiscard]] Command Command::parse(std::string_view raw_command) {
    Command cmd;
    std::string raw(raw_command);
    std::istringstream iss(raw);
    std::vector<std::string> tokens;
    std::string token;

    while (iss >> token) {
        tokens.push_back(token);
    }

    if (tokens.empty()) {
        return cmd;
    }

    cmd.type = parse_type(tokens[0]);

    if (tokens.size() > 1) {
        cmd.key = tokens[1];
    }
    if (tokens.size() > 2) {
        cmd.value = tokens[2];
    }
    for (std::size_t i = 1; i < tokens.size(); ++i) {
        cmd.args.push_back(tokens[i]);
    }

    return cmd;
}

[[nodiscard]] std::string Command::to_string() const {
    std::string result(type_to_string(type));
    if (!key.empty()) {
        result += " " + key;
    }
    if (!value.empty()) {
        result += " " + value;
    }
    return result;
}

} // namespace ayushdb
