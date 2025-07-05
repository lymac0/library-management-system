#ifndef KITAPISLEMLERI_H
#define KITAPISLEMLERI_H

#include <QDialog>
#include <QSql>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QMessageBox>
#include "DatabaseHelper.h"
#include <QItemSelection>

namespace Ui {
class KitapIslemleri;
}

class KitapIslemleri : public QDialog
{
    Q_OBJECT

public:
    explicit KitapIslemleri(QWidget *parent = nullptr);
    ~KitapIslemleri();

private slots:
    void kitapSecim(const QItemSelection &selected);
    void yeniKayit();
    void guncelle();
    void sil();

private:
    Ui::KitapIslemleri *ui;
    QSqlQuery *sorgu;
    QSqlQueryModel *model1;
    QSqlQueryModel *modelOdunc1;
    QSqlQueryModel *modelOdunc2;
};

#endif // KITAPISLEMLERI_H
