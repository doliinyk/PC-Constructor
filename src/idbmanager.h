#ifndef IDBMANAGER_H
#define IDBMANAGER_H

#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>

class IDBManager
{
public:
    IDBManager();
    virtual ~IDBManager();

    virtual bool createDB(QString) = 0;
    virtual bool deleteDB(QString) = 0;
    virtual bool connectToDB(QString) = 0;
    virtual bool createTable(QString, QString) = 0;
    virtual bool deleteTable(QString) = 0;
    virtual QVector<QString> getTableNames() = 0;

protected:
    QSqlDatabase db;
};

#endif // IDBMANAGER_H
