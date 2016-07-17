#include "databasemanager.h"

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent)
{
    qDebug() << "thing created";
}

DatabaseManager* DatabaseManager::instance() {
    static DatabaseManager *s_instance;

    if (!s_instance)
    {
        s_instance = new DatabaseManager;
        qDebug() << "New DB Created";
    }
    else
    {
        qDebug() << "Old DB returned";
    }
    return s_instance;
}
