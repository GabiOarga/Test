#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QDebug>


class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    static DatabaseManager *instance();
    explicit DatabaseManager(QObject *parent = 0);
};

#endif // DATABASEMANAGER_H
