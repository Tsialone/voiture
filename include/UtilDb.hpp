#pragma once
#include <sqlite3.h>
#include <memory>
#include <string>


class UtilDb {
public:
    static void init(const std::string& dbPath = "database/voiture.sqlite3");
    
    static sqlite3* getCon();
    
    static void closeCon();

private:
    static std::unique_ptr<sqlite3, decltype(&sqlite3_close)> conn_;
    static std::string dbPath_;
};
