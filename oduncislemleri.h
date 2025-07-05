#ifndef ODUNCISLEMLERI_H
#define ODUNCISLEMLERI_H

#include <QDialog>
#include <QSql>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QMessageBox>
#include "DatabaseHelper.h"
#include <QItemSelection>
#include <QDate>

namespace Ui {
class OduncIslemleri;
}

class OduncIslemleri : public QDialog
{
    Q_OBJECT

public:
    explicit OduncIslemleri(QWidget *parent = nullptr);
    ~OduncIslemleri();
    QDate tarih = QDate::currentDate();

private slots:
    void oduncAl();
    void uyeSecim(const QItemSelection &selected);
    void kitapSecim(const QItemSelection &selected);

private:
    Ui::OduncIslemleri *ui;
    QSqlQuery *sorgu;
    QSqlQueryModel *modelKitap;
    QSqlQueryModel *modelOdunc;
    QSqlQueryModel *modelUye;
};

#endif // ODUNCISLEMLERI_H
