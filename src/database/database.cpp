#include "database/database.h"

void Database::set(const std::string& key, const std::string& value){
    data.insert_or_assign(key,value);
}

std::optional<std::string> Database::get(const std::string& key) const{
    if(data.contains(key)){
        return data.at(key);
    }

    return std::nullopt;
}

void Database::remove(const std::string& key){
    data.erase(key);
}

bool Database::exists(const std::string& key) const{
    return data.contains(key);
}
