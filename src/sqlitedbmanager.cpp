#include "sqlitedbmanager.h"
#include <QDir>
#include <QSqlQuery>

SQLiteDBManager *SQLiteDBManager::instance = nullptr;

SQLiteDBManager::SQLiteDBManager() {}

SQLiteDBManager::~SQLiteDBManager() {}

SQLiteDBManager *SQLiteDBManager::getInstance()
{
    return instance ?: instance = new SQLiteDBManager;
}

QSqlDatabase SQLiteDBManager::getDB()
{
    return db;
}

bool SQLiteDBManager::connectToDB()
{
    return openDB();
}

bool SQLiteDBManager::runScript(QString script)
{
    return QSqlQuery(db).exec(script);
}

//QStringList SQLiteDBManager::getTableNames()
//{
//    QStringList tempTablesNames = db.tables();
//    for (int i = tempTablesNames.size() - 1; i >= 0; i--)
//        if (tempTablesNames[i].startsWith("sqlite_") || tempTablesNames[i].startsWith("__"))
//            tempTablesNames.erase(tempTablesNames.constBegin() + i);

//    return tempTablesNames;
//}

bool SQLiteDBManager::openDB()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    if (!QDir("db").exists())
        QDir().mkdir("db");

    db.setDatabaseName("db//pc_constructor_db.sqlite");
    return db.open();
}
