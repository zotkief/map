#include "../lib/sqlite/sqlite3.h"
#include <cstring>

class DatabaseConnector
{
    int result;
    sqlite3* db;
    std::string queryResult;
public:
    DatabaseConnector();
    ~DatabaseConnector();
    std::string getPresets();
    static int callback(void* buffer, int argc, char** argv, char** azColName);
};