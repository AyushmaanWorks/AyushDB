# AyushDB 🚀

A Redis-inspired, thread-safe, in-memory key-value database written in modern **C++20**. 

AyushDB is a portfolio project built to explore low-level systems programming, modern C++ idioms, multithreaded synchronization, and performance benchmarking.

---

## 🌟 Key Features

* **Thread-Safe Core Engine**: Uses a **Multiple Readers / Single Writer (MRSW)** concurrency model via `std::shared_mutex`.
* **$O(1)$ Hash Map Storage**: Backed by `std::unordered_map` for fast key-value lookups.
* **Core Key-Value Operations**:
  * `SET(key, value)` — Store or update a string value.
  * `GET(key)` — Retrieve value as `std::optional<std::string>`.
  * `DEL(key)` — Remove key-value entry.
  * `EXISTS(key)` — Check key existence (`store.contains()`).
  * `KEYS()` — Retrieve a vector of all active keys.
  * `SIZE()` — Return total entry count.
  * `CLEAR()` — Purge all database entries.
* **Interactive CLI Shell (REPL)**: An interactive command-line interface to execute commands live.
* **Multithreaded Benchmark Suite**: Automated performance testing measuring throughput (Operations Per Second - OPS) across concurrent worker threads.

---

## 📐 Architecture & Design Choices

### 1. Reader-Writer Lock Synchronization (`std::shared_mutex`)
In typical key-value workloads, read requests (`GET`, `EXISTS`, `KEYS`, `SIZE`) far outnumber write requests (`SET`, `DEL`, `CLEAR`). 
- **Read Operations**: Acquire `std::shared_lock<std::shared_mutex>`, allowing multiple threads to read concurrently without blocking one another.
- **Write Operations**: Acquire `std::unique_lock<std::shared_mutex>`, ensuring exclusive write access during updates to prevent data races.

### 2. Expressive Return Types (`std::optional`)
`GET(key)` returns `std::optional<std::string>`. If a key exists, it returns `std::string`; if missing, it returns `std::nullopt`. This avoids returning sentinel error strings or throwing exceptions.

---

## ⚡ Performance Benchmarks

Tested on local machine across 100,000 operations per thread:

| Workload | Threads | Total Operations | Elapsed Time | Throughput (OPS) |
| :--- | :--- | :--- | :--- | :--- |
| **100% READ (`GET`)** | 1 | 100,000 | ~102 ms | **~973,600 OPS** |
| **80% READ / 20% WRITE** | 1 | 100,000 | ~153 ms | **~651,700 OPS** |
| **80% READ / 20% WRITE** | 4 | 400,000 | ~2,065 ms | **~193,600 OPS** |
| **80% READ / 20% WRITE** | 8 | 800,000 | ~4,267 ms | **~187,400 OPS** |

---

## 🛠️ Building & Running

### Prerequisites
* C++20 compatible compiler (`g++` 10+, `clang++` 11+, or MSVC)
* CMake 3.20+ (Optional)

### Option 1: Direct Build with GCC
```bash
g++ -O3 -std=c++20 -Iinclude src/main.cpp src/database.cpp src/command.cpp -o AyushDB.exe
./AyushDB.exe
```

### Option 2: Build with CMake
```bash
cmake -B build -S .
cmake --build build --config Release
./build/AyushDB.exe
```

---

## 💻 Example Terminal Usage

Launch `AyushDB.exe` to enter the interactive console:

```text
====================================================
  AyushDB v1.0.0 (C++20 In-Memory Key-Value Store)  
  Type 'HELP' for commands, 'EXIT' to quit.         
====================================================

ayushdb> SET user:100 Alice
OK
ayushdb> GET user:100
"Alice"
ayushdb> EXISTS user:100
(integer) 1
ayushdb> KEYS
1) "user:100"
ayushdb> DEL user:100
(integer) 1
ayushdb> GET user:100
(nil)
ayushdb> EXIT
Goodbye!
```

To run the automated multithreaded benchmark suite:
```bash
./AyushDB.exe --benchmark
```

---

## 🛣️ Future Roadmap

- [ ] **Networking**: Add a multi-threaded TCP socket server to accept remote connections.
- [ ] **RESP Protocol**: Parse Redis Wire Protocol (`*3\r\n$3\r\nSET...`) for `redis-cli` compatibility.
- [ ] **Disk Persistence**: Implement Append-Only File (AOF) logging and snapshotting.
- [ ] **TTL & Expiration**: Background thread for key expiration (`EXPIRE` command) and LRU cache eviction.

---

## 📝 License
This project is open-source under the MIT License.
