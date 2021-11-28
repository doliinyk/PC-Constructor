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

    bool createCollection(QString);
    bool createBuild(QString);
    QVector<QString> getBuildNames();
    bool deleteBuild(QString);
    bool connectToDB();
    bool deleteDB();

private:
    IDBManager *db;
    QString name;
};

#endif // COLLECTION_H
