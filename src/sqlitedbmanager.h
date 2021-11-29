#ifndef SQLITEDBMANAGER_H
#define SQLITEDBMANAGER_H

#include <QFile>
#include <QSqlQuery>
#include "idbmanager.h"

class SQLiteDBManager : public IDBManager
{
public:
    SQLiteDBManager();
    virtual ~SQLiteDBManager() override;

    virtual QSqlDatabase getDB(QString) override;
    virtual bool createDB(QString) override;
    virtual bool deleteDB(QString) override;
    virtual bool connectToDB(QString) override;
    virtual bool createTable(QString) override;
    virtual bool deleteTable(QString) override;
    virtual QStringList getTableNames() override;

private:
    virtual QString nameFileDB(QString) override;
};

#endif // SQLITEDBMANAGER_H
