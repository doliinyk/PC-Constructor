#ifndef SQLITEDBMANAGER_H
#define SQLITEDBMANAGER_H

#include "idbmanager.h"

class SQLiteDBManager : public IDBManager
{
public:
    virtual ~SQLiteDBManager() override;
    static SQLiteDBManager *getInstance();

    virtual QSqlDatabase getDB() override;
    virtual bool connectToDB() override;
    virtual bool runScript(QString script) override;

private:
    QSqlDatabase db;
    static SQLiteDBManager *instance;

    SQLiteDBManager();

    bool openDB();
};

#endif // SQLITEDBMANAGER_H
