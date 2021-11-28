#ifndef SQLITEDBMANAGER_H
#define SQLITEDBMANAGER_H

#include "idbmanager.h"

class SQLiteDBManager : public IDBManager
{
public:
    SQLiteDBManager();
    virtual ~SQLiteDBManager() override;

    virtual bool createDB(QString) override;
    virtual bool deleteDB(QString) override;
    virtual bool connectToDB(QString) override;
    virtual bool createTable(QString, QString) override;
    virtual bool deleteTable(QString) override;
    virtual QVector<QString> getTableNames() override;

private:
    QString dbName;
    QVector<QString> tableNames;
    QFile dbFile;

    bool openDB();
    bool existDB(QString);
    bool closeDB();
};

#endif // SQLITEDBMANAGER_H
