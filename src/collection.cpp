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

QSqlDatabase Collection::getDB(QString dbName)
{
    return db->getDB(dbName);
}

bool Collection::createCollection(QString collectionName)
{
    name = collectionName;
    return db->createDB(collectionName);
}

bool Collection::createBuild(QString buildName)
{
    return db->createTable("CREATE TABLE " + buildName
                           + "("
                             "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                             "motherboard INTEGER NOT NULL, "
                             "cpu INTEGER NOT NULL, "
                             "drive INTEGER NOT NULL, "
                             "ram INTEGER NOT NULL, "
                             "supply INTEGER NOT NULL, "
                             "gpu INTEGER, "
                             "gpu2 INTEGER"
                             ")");
}

QStringList Collection::getBuildNames()
{
    return db->getTableNames();
}

bool Collection::deleteBuild(QString buildName)
{
    return db->deleteTable(buildName);
}

bool Collection::connectToCollection(QString dbName)
{
    name = dbName;
    return db->connectToDB(dbName);
}

bool Collection::deleteCollection(QString dbName)
{
    return db->deleteDB(dbName);
}
