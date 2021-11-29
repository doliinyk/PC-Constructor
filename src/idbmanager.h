#ifndef IDBMANAGER_H
#define IDBMANAGER_H

#include <QSqlDatabase>

class IDBManager
{
public:
    IDBManager();
    virtual ~IDBManager();

    virtual QSqlDatabase getDB(QString) = 0;
    virtual bool createDB(QString) = 0;
    virtual bool deleteDB(QString) = 0;
    virtual bool connectToDB(QString) = 0;
    virtual bool createTable(QString) = 0;
    virtual bool deleteTable(QString) = 0;
    virtual QStringList getTableNames() = 0;

protected:
    QSqlDatabase db;

    virtual QString nameFileDB(QString) = 0;
};

#endif // IDBMANAGER_H
