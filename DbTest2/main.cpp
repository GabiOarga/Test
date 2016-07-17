#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "databasemanager.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    DatabaseManager *dbManager1 = DatabaseManager::instance();
    DatabaseManager *dbManager2 = DatabaseManager::instance();
    DatabaseManager *dbManager3 = DatabaseManager::instance();
    DatabaseManager *dbManager4 = DatabaseManager::instance();
    DatabaseManager *dbManager5 = DatabaseManager::instance();
    DatabaseManager *dbManager6 = DatabaseManager::instance();

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
