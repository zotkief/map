#include "../lib/sqlite3/include/sqlite3.h"
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
    std::string getData(std::string presetName);
    static int callback(void* buffer, int argc, char** argv, char** azColName);
};