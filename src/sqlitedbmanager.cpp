#include "sqlitedbmanager.h"
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>

SQLiteDBManager *SQLiteDBManager::instance = nullptr;

SQLiteDBManager::SQLiteDBManager() {}

SQLiteDBManager::~SQLiteDBManager() {}

SQLiteDBManager *SQLiteDBManager::getInstance()
{
    return (instance ?: instance = new SQLiteDBManager);
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
    QSqlQuery tempQuery(db);
    bool tempResult = tempQuery.exec(script);
    if (!tempResult) {
        qDebug() << tempQuery.lastError();
        qDebug() << tempQuery.lastQuery();
    }
    return tempResult;
}

bool SQLiteDBManager::openDB()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    if (!QDir("db").exists())
        QDir().mkdir("db");

    db.setDatabaseName("db//pc_constructor_db.sqlite");
    return db.open();
}
