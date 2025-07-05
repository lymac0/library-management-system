#include "oduncislemleri.h"
#include "ui_oduncislemleri.h"

OduncIslemleri::OduncIslemleri(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OduncIslemleri)
{
    ui->setupUi(this);

    QSqlDatabase database = DatabaseHelper::getDatabase();

    modelKitap = new QSqlQueryModel();
    modelKitap->setQuery("SELECT * FROM kitap", database);
    ui->tableViewKitaplar->setModel(modelKitap);

    modelOdunc = new QSqlQueryModel();
    modelOdunc->setQuery("SELECT * FROM odunc_alinan", database);
    ui->tableViewOdunc->setModel(modelOdunc);

    modelUye = new QSqlQueryModel();
    modelUye->setQuery("SELECT * FROM üye", database);
    ui->tableViewUye->setModel(modelUye);

    connect(ui->tableViewUye->selectionModel(), &QItemSelectionModel::selectionChanged, this, &OduncIslemleri::uyeSecim);
    connect(ui->tableViewKitaplar->selectionModel(), &QItemSelectionModel::selectionChanged, this, &OduncIslemleri::kitapSecim);
    connect(ui->btn_oduncAl, &QPushButton::clicked, this, &OduncIslemleri::oduncAl);
}

OduncIslemleri::~OduncIslemleri()
{
    delete ui;
    delete modelKitap;
    delete modelOdunc;
    delete modelUye;
}

void OduncIslemleri::oduncAl()
{
    QString kitapNo = ui->kitapNo->text();
    QString uyeNo = ui->uyeNo->text();
    QDate oduncTarih = ui->oduncTarih->date();
    if (oduncTarih > tarih) {
        QMessageBox::warning(this, "Hata", "İleri tarih için işlem yapılamaz!");
        return;
    }
    QString oduncTarihStr = ui->oduncTarih->date().toString("dd/MM/yyyy");



    if (kitapNo.isEmpty() || uyeNo.isEmpty()) {
        QMessageBox::warning(this, "Hata", "Gerekli alanları doldurunuz!");
        return;
    }

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT * FROM odunc_alinan WHERE uye_no = :uye_no AND kitap_no = :kitap_no");
    checkQuery.bindValue(":uye_no", uyeNo);
    checkQuery.bindValue(":kitap_no", kitapNo);
    if (checkQuery.exec() && checkQuery.next()) {
        QMessageBox::information(this, "Hata", "Bu üye bu kitabın bir tanesini şu an ödünç almış tekrar ödünç verilemez!");
        return;
    }

    QSqlQuery stokSorgu;
    stokSorgu.prepare("SELECT COUNT(*) FROM odunc_alinan WHERE kitap_no = :kitap_no");
    stokSorgu.bindValue(":kitap_no", kitapNo);
    if (!stokSorgu.exec()) {
        QMessageBox::critical(this, "Hata", "Veritabanı hatası!");
        return;
    }

    stokSorgu.next();
    int oduncSayisiKitap = stokSorgu.value(0).toInt();

    QSqlQuery stokSorguKitap;
    stokSorguKitap.prepare("SELECT kitap_sayisi FROM kitap WHERE kitap_no = :kitap_no");
    stokSorguKitap.bindValue(":kitap_no", kitapNo);
    if (!stokSorguKitap.exec()) {
        QMessageBox::critical(this, "Hata", "Veritabanı hatası!");
        return;
    }

    stokSorguKitap.next();
    int kitapStok = stokSorguKitap.value(0).toInt();
    if (oduncSayisiKitap >= kitapStok) {
        QMessageBox::warning(this, "Hata", "Seçilen kitap stokta bulunmamaktadır!");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO odunc_alinan (uye_no, kitap_no, odunc_alma_tarihi) VALUES (:uye_no, :kitap_no, :odunc_alma_tarihi)");
    query.bindValue(":uye_no", uyeNo);
    query.bindValue(":kitap_no", kitapNo);
    query.bindValue(":odunc_alma_tarihi",oduncTarihStr);
    if (!query.exec()) {
        QMessageBox::critical(this, "Hata", "Yeni kayıt eklenemedi!");
    } else {
        QMessageBox::information(this, "Başarılı", "Yeni kayıt eklendi!");
        modelOdunc->setQuery("SELECT * FROM odunc_alinan");
    }
}

void OduncIslemleri::uyeSecim(const QItemSelection &selected)
{
    if (selected.indexes().isEmpty())
        return;

    QModelIndex index = selected.indexes().first();
    QString uye_no = modelUye->data(modelUye->index(index.row(), 0)).toString();

    ui->uyeNo->setText(uye_no);
}

void OduncIslemleri::kitapSecim(const QItemSelection &selected)
{
    if (selected.indexes().isEmpty())
        return;

    QModelIndex index = selected.indexes().first();
    QString kitap_no = modelKitap->data(modelKitap->index(index.row(), 0)).toString();

    ui->kitapNo->setText(kitap_no);
}
