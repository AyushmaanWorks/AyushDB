#include "database.h"
#include "command.h"
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>
#include <cctype>
#include <iomanip>

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
    std::cout << "  BENCHMARK           Run multithreaded performance benchmark\n";
    std::cout << "  HELP                Show this help message\n";
    std::cout << "  EXIT / QUIT         Exit the database shell\n";
}

void run_benchmark(amonkv::Database& db) {
    std::cout << "\n====================================================\n";
    std::cout << "       AmonKV Multithreaded Benchmark Suite         \n";
    std::cout << "====================================================\n";

    const int total_keys_prefill = 10000;
    std::cout << "Prefilling database with " << total_keys_prefill << " key-value pairs...\n";
    db.clear();
    for (int i = 0; i < total_keys_prefill; ++i) {
        db.set("bench_key_" + std::to_string(i), "bench_val_" + std::to_string(i));
    }

    const std::vector<int> thread_counts = {1, 2, 4, 8, 16};
    const int ops_per_thread = 100000;

    std::cout << "\nRunning Benchmark Tests (" << ops_per_thread << " ops/thread):\n";
    std::cout << "-------------------------------------------------------------------\n";
    std::cout << std::left << std::setw(10) << "Workload" 
              << std::setw(10) << "Threads" 
              << std::setw(14) << "Total Ops" 
              << std::setw(14) << "Time (ms)" 
              << std::setw(18) << "Throughput (OPS)" << "\n";
    std::cout << "-------------------------------------------------------------------\n";

    for (int num_threads : thread_counts) {
        std::vector<std::thread> threads;
        threads.reserve(num_threads);

        auto start = std::chrono::high_resolution_clock::now();

        for (int t = 0; t < num_threads; ++t) {
            threads.emplace_back([&db, ops_per_thread, t]() {
                for (int i = 0; i < ops_per_thread; ++i) {
                    int key_idx = (t * ops_per_thread + i) % 10000;
                    [[maybe_unused]] auto res = db.get("bench_key_" + std::to_string(key_idx));
                }
            });
        }

        for (auto& th : threads) {
            th.join();
        }

        auto end = std::chrono::high_resolution_clock::now();
        double elapsed_ms = std::chrono::duration<double, std::milli>(end - start).count();
        long long total_ops = static_cast<long long>(num_threads) * ops_per_thread;
        double ops_per_sec = (total_ops / (elapsed_ms / 1000.0));

        std::cout << std::left << std::setw(10) << "100% READ" 
                  << std::setw(10) << num_threads 
                  << std::setw(14) << total_ops 
                  << std::setw(14) << std::fixed << std::setprecision(2) << elapsed_ms 
                  << std::setw(18) << std::fixed << std::setprecision(0) << ops_per_sec << "\n";
    }

    std::cout << "-------------------------------------------------------------------\n";

    for (int num_threads : thread_counts) {
        std::vector<std::thread> threads;
        threads.reserve(num_threads);

        auto start = std::chrono::high_resolution_clock::now();

        for (int t = 0; t < num_threads; ++t) {
            threads.emplace_back([&db, ops_per_thread, t]() {
                for (int i = 0; i < ops_per_thread; ++i) {
                    int key_idx = (t * ops_per_thread + i) % 10000;
                    std::string key = "bench_key_" + std::to_string(key_idx);
                    if (i % 5 == 0) {
                        db.set(key, "updated_val");
                    } else {
                        [[maybe_unused]] auto res = db.get(key);
                    }
                }
            });
        }

        for (auto& th : threads) {
            th.join();
        }

        auto end = std::chrono::high_resolution_clock::now();
        double elapsed_ms = std::chrono::duration<double, std::milli>(end - start).count();
        long long total_ops = static_cast<long long>(num_threads) * ops_per_thread;
        double ops_per_sec = (total_ops / (elapsed_ms / 1000.0));

        std::cout << std::left << std::setw(10) << "80/20 MIXED" 
                  << std::setw(10) << num_threads 
                  << std::setw(14) << total_ops 
                  << std::setw(14) << std::fixed << std::setprecision(2) << elapsed_ms 
                  << std::setw(18) << std::fixed << std::setprecision(0) << ops_per_sec << "\n";
    }

    std::cout << "-------------------------------------------------------------------\n";
    std::cout << "Benchmark Complete! Final DB Size: " << db.size() << "\n";
    std::cout << "====================================================\n\n";
}

int main(int argc, char* argv[]) {
    amonkv::Database db;

    if (argc > 1 && (std::string_view(argv[1]) == "--benchmark" || std::string_view(argv[1]) == "-b")) {
        run_benchmark(db);
        return 0;
    }

    std::cout << "====================================================\n";
    std::cout << "  AmonKV v1.0.0 (C++20 In-Memory Key-Value Store)   \n";
    std::cout << "  Type 'BENCHMARK' to test throughput, 'EXIT' to quit.\n";
    std::cout << "====================================================\n\n";

    std::string line;
    while (true) {
        std::cout << "amonkv> ";
        if (!std::getline(std::cin, line)) {
            break;
        }

        std::string trimmed_line = trim(line);
        if (trimmed_line.empty()) {
            continue;
        }

        auto cmd = amonkv::Command::parse(trimmed_line);

        if (cmd.type == amonkv::CommandType::UNKNOWN) {
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
            if (upper_line == "BENCHMARK") {
                run_benchmark(db);
                continue;
            }
            std::cout << "(error) ERR unknown command '" << trimmed_line << "'\n";
            continue;
        }

        switch (cmd.type) {
            case amonkv::CommandType::SET: {
                if (cmd.key.empty() || cmd.value.empty()) {
                    std::cout << "(error) ERR wrong number of arguments for 'SET' command\n";
                } else {
                    db.set(cmd.key, cmd.value);
                    std::cout << "OK\n";
                }
                break;
            }
            case amonkv::CommandType::GET: {
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
            case amonkv::CommandType::DEL: {
                if (cmd.key.empty()) {
                    std::cout << "(error) ERR wrong number of arguments for 'DEL' command\n";
                } else {
                    bool removed = db.del(cmd.key);
                    std::cout << (removed ? "(integer) 1\n" : "(integer) 0\n");
                }
                break;
            }
            case amonkv::CommandType::EXISTS: {
                if (cmd.key.empty()) {
                    std::cout << "(error) ERR wrong number of arguments for 'EXISTS' command\n";
                } else {
                    bool found = db.exists(cmd.key);
                    std::cout << (found ? "(integer) 1\n" : "(integer) 0\n");
                }
                break;
            }
            case amonkv::CommandType::KEYS: {
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
            case amonkv::CommandType::SIZE: {
                std::cout << "(integer) " << db.size() << "\n";
                break;
            }
            case amonkv::CommandType::CLEAR: {
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