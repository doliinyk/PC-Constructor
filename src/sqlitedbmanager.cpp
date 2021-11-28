#include "sqlitedbmanager.h"

SQLiteDBManager::SQLiteDBManager()
    : IDBManager()
    , dbFile("..\\db\\dblist.txt")
{}

SQLiteDBManager::~SQLiteDBManager() {}

bool SQLiteDBManager::connectToDB(QString dbName)
{
    this->dbName = dbName;
    return openDB();
}

bool SQLiteDBManager::createTable(QString script, QString tableName)
{
    db.open();
    QSqlQuery *query = new QSqlQuery(db);
    bool result = query->exec(script);
    db.close();
    delete query;
    tableNames.push_back(tableName);
    return result;
}

bool SQLiteDBManager::deleteTable(QString tableName)
{
    db.open();
    QSqlQuery *query = new QSqlQuery(db);
    bool result = query->exec("DROP TABLE " + tableName);
    db.close();
    for (QVector<QString>::iterator it = tableNames.begin(); it != tableNames.end(); it++)
        if (*it == tableName) {
            tableNames.erase(it);
            break;
        }
    qDebug() << result;
    return result;
}

QVector<QString> SQLiteDBManager::getTableNames()
{
    return tableNames;
}

bool SQLiteDBManager::createDB(QString dbName)
{
    if (existDB(dbName))
        return false;
    this->dbName = dbName;
    return openDB();
}

bool SQLiteDBManager::deleteDB(QString dbName)
{
    if (!existDB(dbName))
        return false;
    db.removeDatabase(dbName);
    dbFile.open(QIODevice::ReadWrite);
    QString saveLine;
    QTextStream str(&dbFile);
    while (!str.atEnd()) {
        QString tempLine = str.readLine();
        if (!tempLine.contains(dbName))
            saveLine.append(tempLine + "\n");
    }
    dbFile.resize(0);
    str << saveLine;
    dbFile.close();
    QFile::remove(dbName);
    this->dbName.clear();
    tableNames.clear();
    return true;
}

bool SQLiteDBManager::openDB()
{
    dbFile.open(QIODevice::Append);
    QTextStream out(&dbFile);
    out << dbName;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    dbFile.close();
    bool res = db.open();
    return res;
}

bool SQLiteDBManager::existDB(QString dbName)
{
    dbFile.open(QIODevice::ReadOnly);
    QTextStream in(&dbFile);
    while (!in.atEnd())
        if (dbName == in.readLine()) {
            dbFile.close();
            return true;
        }
    dbFile.close();
    return false;
}
