#include "sqlitedbmanager.h"
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>

SQLiteDBManager *SQLiteDBManager::instance = nullptr;

SQLiteDBManager::SQLiteDBManager() {}

SQLiteDBManager::~SQLiteDBManager() {}

SQLiteDBManager *SQLiteDBManager::getInstance()
{
    if (!instance)
        instance = new SQLiteDBManager();
    return instance;
}

QSqlDatabase SQLiteDBManager::getDB()
{
    return db;
}

bool SQLiteDBManager::connectToDB(QString dbName)
{
    return openDB(dbName);
}

bool SQLiteDBManager::runScript(QString script)
{
    QSqlQuery tempQuery;
    bool result = tempQuery.exec(script);
    if (!result)
        qDebug() << tempQuery.lastError() << Qt::endl << tempQuery.lastQuery() << Qt::endl;

    return result;
}

bool SQLiteDBManager::openDB(QString dbName)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    if (!QDir("db").exists())
        QDir().mkdir("db");

    db.setDatabaseName(QString("db//%1.sqlite").arg(dbName));
    return db.open();
}
