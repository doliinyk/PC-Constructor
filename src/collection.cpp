#include "collection.h"

Collection::Collection()
    : db(new SQLiteDBManager)
{}

Collection::~Collection()
{
    delete db;
}

QString Collection::getName()
{
    return name;
}

bool Collection::createCollection(QString collectionName)
{
    name = collectionName;
    return db->createDB(collectionName);
}

bool Collection::createBuild(QString buildName)
{
    return db->createTable("CREATE TABLE " + buildName
                               + " ("
                                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                 "motherboard INTEGER NOT NULL,"
                                 "cpu INTEGER NOT NULL,"
                                 "drive INTEGER NOT NULL,"
                                 "ram INTEGER NOT NULL,"
                                 "gpu INTEGER,"
                                 "gpu2 INTEGER,"
                                 "supply INTEGER NOT NULL)",
                           buildName);
}

QVector<QString> Collection::getBuildNames()
{
    return db->getTableNames();
}

bool Collection::deleteBuild(QString buildName)
{
    return db->deleteTable(buildName);
}

bool Collection::connectToDB()
{
    return db->connectToDB(name);
}

bool Collection::deleteDB()
{
    return db->deleteDB(name);
}
