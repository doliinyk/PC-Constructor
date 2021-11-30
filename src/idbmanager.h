#ifndef IDBMANAGER_H
#define IDBMANAGER_H

#include <QSqlDatabase>

class IDBManager
{
public:
    IDBManager();
    virtual ~IDBManager();

    virtual QSqlDatabase getDB() = 0;
    virtual bool connectToDB(QString dbName) = 0;
    virtual bool runScript(QString script) = 0;
};

#endif // IDBMANAGER_H
