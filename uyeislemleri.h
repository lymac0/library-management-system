#ifndef UYEISLEMLERI_H
#define UYEISLEMLERI_H

#include <QDialog>
#include <QSql>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QMessageBox>
#include "DatabaseHelper.h"
#include <QItemSelection>


namespace Ui {
class UyeIslemleri;
}

class UyeIslemleri : public QDialog
{
    Q_OBJECT


public:
    explicit UyeIslemleri(QWidget *parent = nullptr);
    ~UyeIslemleri();

private slots:
    void uyeSecim(const QItemSelection &selected);
    void yeniKayit();
    void guncelle();
    void sil();

private:
    Ui::UyeIslemleri *ui;
    QSqlQuery *sorgu;
    QSqlQueryModel *model1;
};

#endif // UYEISLEMLERI_H
