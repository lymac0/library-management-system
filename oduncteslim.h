#ifndef ODUNCTESLIM_H
#define ODUNCTESLIM_H

#include <QDialog>
#include "DatabaseHelper.h"
#include <QSql>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QItemSelection>
#include <QDate>


namespace Ui {
class OduncTeslim;
}

class OduncTeslim : public QDialog
{
    Q_OBJECT

public:
    explicit OduncTeslim(QWidget *parent = nullptr);
    ~OduncTeslim();

private slots:
    void oduncuVer();
    void oduncSecim(const QItemSelection &selected);

private:
    Ui::OduncTeslim *ui;
    QSqlQuery *sorgu;
    QSqlQueryModel *modelOduncAlinan;
    QSqlQueryModel *modelOduncTeslim;
    QModelIndex index;
};

#endif // ODUNCTESLIM_H
