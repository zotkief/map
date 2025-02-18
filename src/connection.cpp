#include <string>
#include <iostream>

#include "databaseConnector.hpp"

DatabaseConnector::DatabaseConnector()
{
    result=sqlite3_open("../data/database.sqlite", &db);

    if (result) {
        std::cout<<"error"<<std::endl;
        sqlite3_close(db);
        db = nullptr;
    }
}
DatabaseConnector::~DatabaseConnector()
{
    sqlite3_close(db);
    db=nullptr;
}
std::string DatabaseConnector::getPresets()
{
    std::string query="SELECT presetName FROM presets;";
    char* message;
    queryResult="";
    sqlite3_exec(db,query.c_str(),callback,&queryResult,&message);
    std::string s=queryResult;
    return s;
}
int DatabaseConnector::callback(void* buffer, int argc, char** argv, char** azColName) {
    std::string* resultBuffer=(std::string*)buffer;

    for (int i = 0; i < argc; i++) {
        std::string s(argv[i]);
        (*resultBuffer)+=s;
        (*resultBuffer)+=" ";
    }
    (*resultBuffer)+="\n";
    return 0;
}