#include "UtilDb.hpp"
#include <iostream>
#include <memory>
#include <sqlite3.h>

// Initialisation des membres statiques
std::unique_ptr<sqlite3, decltype(&sqlite3_close)> UtilDb::conn_(nullptr, sqlite3_close);
std::string UtilDb::dbPath_;

void UtilDb::init(const std::string &dbPath)
{
    dbPath_ = dbPath;

    sqlite3 *rawConn = nullptr;
    if (sqlite3_open(dbPath_.c_str(), &rawConn) != SQLITE_OK)
    {
        throw std::runtime_error("Échec de l'initialisation SQLite: " + std::string(sqlite3_errmsg(rawConn)));
    }
    conn_.reset(rawConn);

    std::cout << "SQLite initialisé (" << dbPath_ << ")" << std::endl;
}

sqlite3 *UtilDb::getCon()
{
    if (!conn_)
    {
        throw std::runtime_error("Connexion non initialisée. Appelez UtilDb::init() d'abord.");
    }
    return conn_.get();
}

void UtilDb::closeCon()
{
    if (conn_)
    {
        std::cout << "Fermeture de la connexion SQLite" << std::endl;
        conn_.reset();
    }
}
