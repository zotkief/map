#include <gdal/gdal_priv.h>
#include <iostream>
#include <string>
#include <sstream>

#include "include/mapDisplay.hpp"
#include "include/map.hpp"
#include "include/geography.hpp"
#include "include/databaseConnector.hpp"
#include "include/interpreter.hpp"

int main() {
    GDALAllRegister();

    std::string command;
    DatabaseConnector data();
    std::string message="enter help to see manual";

    while(1){
        std::cout<<message<<std::endl;
        std::cin>>command;
        std::istringstream stream(command);

        std::string word;
        stream>>word;

        if (word == "exit") {
            std::cout<<"exiting"<<std::endl;
            break;
        } 
        else if (word == "help") {
            std::cout<<processManual()<<std::endl;
        } 
        else if (word == "gen") {
            processGen(command);
        } 
        else if (word == "showPresets") {
            std::cout<<processShow()<<std::endl;
        }        
    }
    return 0;
}
