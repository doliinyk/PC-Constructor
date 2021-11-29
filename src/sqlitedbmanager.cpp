#include "sqlitedbmanager.h"

SQLiteDBManager::SQLiteDBManager()
    : IDBManager()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
}

SQLiteDBManager::~SQLiteDBManager() {}

QSqlDatabase SQLiteDBManager::getDB(QString dbName)
{
    if (connectToDB(dbName))
        return db;
}

bool SQLiteDBManager::createDB(QString dbName)
{
    if (QFile::exists(nameFileDB(dbName)))
        return false;

    db.setDatabaseName(nameFileDB(dbName));

    return db.open();
}

bool SQLiteDBManager::deleteDB(QString dbName)
{
    if (!QFile::exists(nameFileDB(dbName)))
        return false;

    db.close();
    db.removeDatabase(nameFileDB(dbName));

    return QFile::remove(nameFileDB(dbName));
}

bool SQLiteDBManager::connectToDB(QString dbName)
{
    db.setDatabaseName(nameFileDB(dbName));

    return db.open();
}

bool SQLiteDBManager::createTable(QString script)
{
    return QSqlQuery(db).exec(script);
}

bool SQLiteDBManager::deleteTable(QString tableName)
{
    return QSqlQuery(db).exec("DROP TABLE " + tableName);
}

QStringList SQLiteDBManager::getTableNames()
{
    QStringList temp = db.tables();
    for (int i = 0; i < temp.size(); i++)
        if (temp[i].startsWith("sqlite_"))
            temp.erase(temp.begin() + i);

    return temp;
}

QString SQLiteDBManager::nameFileDB(QString dbName)
{
    return "..\\db\\" + dbName + ".sqlite";
}
