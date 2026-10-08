#include <iostream>
#include <string>
#include "include/database.h"

int main(){

    Database db;

    std::cout << "Welcome to AyushDB\n";


    std::string command;

    while(true){
        std::cout<<"AyushDB> ";
        std::getline(std::cin, command);

        if(!command.empty()){
            std::cout<<command;
            command.clear();
        }
    }
    return 0;
}