#ifndef SQLITEDBMANAGER_H
#define SQLITEDBMANAGER_H

#include "idbmanager.h"

//  Реалізація інтерфейсу менеджера бази даних
//  Менеджер бази даних SQLite
class SQLiteDBManager : public IDBManager
{
public:
    virtual ~SQLiteDBManager() override;
    static SQLiteDBManager *getInstance();

    virtual QSqlDatabase getDB() override;
    virtual bool connectToDB(QString dbName) override;
    virtual bool runScript(QString script) override;

private:
    SQLiteDBManager();

    QSqlDatabase db;
    static SQLiteDBManager *instance;

    bool openDB(QString dbName);
};

#endif // SQLITEDBMANAGER_H
