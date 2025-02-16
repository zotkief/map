#include "../lib/sqlite/sqlite3.h"

class DatabaseConnector
{
    int result;
    sqlite3* db;
public:
    DatabaseConnector()
    {
        result=sqlite3_open("../data/database.sqlite", &db);

        if (result) {
            std::cout<<"error"<<std::endl;
            sqlite3_close(db);
            db = nullptr;
        }
        else
            std::cout<<"spoko"<<std::endl;
    }
    ~DatabaseConnector()
    {
        sqlite3_close(db);
        db=nullptr;
    }
    std::string getPresents()
    {
        
    }
};