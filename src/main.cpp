#include "database.h"
#include "command.h"
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

int main() {
    std::cout << "AyushDB Core Engine Demo\n";
    std::cout << "------------------------\n";

    ayushdb::Database db;

    // Basic Database Operations
    db.set("user:100", "Alice");
    db.set("user:101", "Bob");
    db.set("session:xyz", "active");

    std::cout << "SIZE: " << db.size() << "\n";
    std::cout << "EXISTS('user:100'): " << (db.exists("user:100") ? "true" : "false") << "\n";

    if (auto val = db.get("user:100"); val.has_value()) {
        std::cout << "GET('user:100'): " << val.value() << "\n";
    }

    std::cout << "KEYS: ";
    for (const auto& k : db.keys()) {
        std::cout << k << " ";
    }
    std::cout << "\n";

    db.del("session:xyz");
    std::cout << "DEL('session:xyz') -> New SIZE: " << db.size() << "\n\n";

    // Command Parsing Demo
    std::vector<std::string> test_commands = {
        "SET counter 42",
        "GET counter",
        "EXISTS counter",
        "KEYS",
        "SIZE",
        "DEL counter",
        "CLEAR"
    };

    std::cout << "Command Parser Output:\n";
    for (const auto& raw : test_commands) {
        auto cmd = ayushdb::Command::parse(raw);
        std::cout << "  Raw: \"" << raw << "\" -> Parsed Type: " 
                  << ayushdb::Command::type_to_string(cmd.type) << "\n";
    }

    std::cout << "\nConcurrent Multi-Reader Single-Writer Test:\n";
    std::thread writer([&db]() {
        for (int i = 0; i < 20; ++i) {
            db.set("k_" + std::to_string(i), "v_" + std::to_string(i));
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });

    auto reader = [&db](int id) {
        for (int i = 0; i < 10; ++i) {
            [[maybe_unused]] auto k = db.keys();
            [[maybe_unused]] auto s = db.size();
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
        std::cout << "  Reader " << id << " completed.\n";
    };

    std::thread r1(reader, 1);
    std::thread r2(reader, 2);

    writer.join();
    r1.join();
    r2.join();

    std::cout << "Final Database Size: " << db.size() << "\n";
    db.clear();
    std::cout << "Cleared Database Size: " << db.size() << "\n";

    return 0;
}