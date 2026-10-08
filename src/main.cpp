#include "database.h"
#include "command.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

namespace {
    std::string trim(std::string_view s) {
        auto start = s.find_first_not_of(" \t\r\n");
        if (start == std::string_view::npos) return "";
        auto end = s.find_last_not_of(" \t\r\n");
        return std::string(s.substr(start, end - start + 1));
    }
}

void print_help() {
    std::cout << "Available Commands:\n";
    std::cout << "  SET <key> <value>   Set key to hold string value\n";
    std::cout << "  GET <key>           Get value of key\n";
    std::cout << "  DEL <key>           Delete key\n";
    std::cout << "  EXISTS <key>        Determine if a key exists\n";
    std::cout << "  KEYS                Get all keys in the database\n";
    std::cout << "  SIZE                Return number of keys\n";
    std::cout << "  CLEAR               Delete all keys\n";
    std::cout << "  HELP                Show this help message\n";
    std::cout << "  EXIT / QUIT         Exit the database shell\n";
}

int main(int argc, char* argv[]) {
    ayushdb::Database db;

    if (argc > 1 && std::string_view(argv[1]) == "--demo") {
        std::cout << "Running AyushDB Demo Mode...\n";
        db.set("user:100", "Alice");
        db.set("user:101", "Bob");
        std::cout << "SIZE: " << db.size() << "\n";
        if (auto val = db.get("user:100"); val) {
            std::cout << "GET user:100 -> " << *val << "\n";
        }
        return 0;
    }

    std::cout << "====================================================\n";
    std::cout << "  AyushDB v1.0.0 (C++20 In-Memory Key-Value Store)  \n";
    std::cout << "  Type 'HELP' for commands, 'EXIT' to quit.         \n";
    std::cout << "====================================================\n\n";

    std::string line;
    while (true) {
        std::cout << "ayushdb> ";
        if (!std::getline(std::cin, line)) {
            break;
        }

        std::string trimmed_line = trim(line);
        if (trimmed_line.empty()) {
            continue;
        }

        auto cmd = ayushdb::Command::parse(trimmed_line);

        if (cmd.type == ayushdb::CommandType::UNKNOWN) {
            std::string upper_line = trimmed_line;
            std::transform(upper_line.begin(), upper_line.end(), upper_line.begin(),
                           [](unsigned char c) { return static_cast<char>(std::toupper(c)); });

            if (upper_line == "EXIT" || upper_line == "QUIT") {
                std::cout << "Goodbye!\n";
                break;
            }
            if (upper_line == "HELP") {
                print_help();
                continue;
            }
            std::cout << "(error) ERR unknown command '" << trimmed_line << "'\n";
            continue;
        }

        switch (cmd.type) {
            case ayushdb::CommandType::SET: {
                if (cmd.key.empty() || cmd.value.empty()) {
                    std::cout << "(error) ERR wrong number of arguments for 'SET' command\n";
                } else {
                    db.set(cmd.key, cmd.value);
                    std::cout << "OK\n";
                }
                break;
            }
            case ayushdb::CommandType::GET: {
                if (cmd.key.empty()) {
                    std::cout << "(error) ERR wrong number of arguments for 'GET' command\n";
                } else {
                    auto res = db.get(cmd.key);
                    if (res.has_value()) {
                        std::cout << "\"" << res.value() << "\"\n";
                    } else {
                        std::cout << "(nil)\n";
                    }
                }
                break;
            }
            case ayushdb::CommandType::DEL: {
                if (cmd.key.empty()) {
                    std::cout << "(error) ERR wrong number of arguments for 'DEL' command\n";
                } else {
                    bool removed = db.del(cmd.key);
                    std::cout << (removed ? "(integer) 1\n" : "(integer) 0\n");
                }
                break;
            }
            case ayushdb::CommandType::EXISTS: {
                if (cmd.key.empty()) {
                    std::cout << "(error) ERR wrong number of arguments for 'EXISTS' command\n";
                } else {
                    bool found = db.exists(cmd.key);
                    std::cout << (found ? "(integer) 1\n" : "(integer) 0\n");
                }
                break;
            }
            case ayushdb::CommandType::KEYS: {
                auto all_keys = db.keys();
                if (all_keys.empty()) {
                    std::cout << "(empty list or set)\n";
                } else {
                    for (std::size_t i = 0; i < all_keys.size(); ++i) {
                        std::cout << (i + 1) << ") \"" << all_keys[i] << "\"\n";
                    }
                }
                break;
            }
            case ayushdb::CommandType::SIZE: {
                std::cout << "(integer) " << db.size() << "\n";
                break;
            }
            case ayushdb::CommandType::CLEAR: {
                db.clear();
                std::cout << "OK\n";
                break;
            }
            default: {
                std::cout << "(error) ERR unknown command '" << trimmed_line << "'\n";
                break;
            }
        }
    }

    return 0;
}