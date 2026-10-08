#ifndef AYUSHDB_DATABASE_H
#define AYUSHDB_DATABASE_H

#include <optional>
#include <string>
#include <unordered_map>

class Database{

private:
    std::unordered_map<std::string,std::string> data;

public:
    std::optional<std::string> get(const std::string& key) const;

    void set(const std::string& key, const std::string& value);
    
    void remove(const std::string& key);

    bool exists(const std::string& key) const;


};

#endif
