#ifndef IDBMANAGER_H
#define IDBMANAGER_H

#include <QSqlDatabase>

//  Інтерфейс менеджера бази даних
class IDBManager
{
public:
    IDBManager();
    virtual ~IDBManager() = 0;

    virtual QSqlDatabase getDB() = 0;
    virtual bool connectToDB(QString dbName) = 0;
    virtual bool runScript(QString script) = 0;
};

#endif // IDBMANAGER_H
