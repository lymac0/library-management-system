#ifndef DATABASEHELPER_H
#define DATABASEHELPER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>

class DatabaseHelper
{
public:
    static bool connectToDatabase()
    {
        QString dbPath = QDir(QCoreApplication::applicationDirPath()).filePath("../../../Ktp_Otomasyon.db");

        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(dbPath);
        qDebug() << dbPath;

        if (!database.open()) {
            qDebug() << "Veritabanına bağlanılamadı:" << database.lastError().text();
            return false;
        }
        qDebug() << "Veritabanına başarıyla bağlanıldı!";
        return true;
    }

    static QSqlDatabase getDatabase()
    {
        return QSqlDatabase::database();
    }
};

#endif // DATABASEHELPER_H
