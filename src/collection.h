#ifndef COLLECTION_H
#define COLLECTION_H

#include <QString>
#include "sqlitedbmanager.h"

class Collection
{
public:
    explicit Collection();
    ~Collection();

    QString getName();

    QSqlDatabase getDB(QString);
    bool createCollection(QString);
    bool createBuild(QString);
    QStringList getBuildNames();
    bool deleteBuild(QString);
    bool connectToCollection(QString);
    bool deleteCollection(QString);

private:
    IDBManager *db;
    QString name;
};

#endif // COLLECTION_H
