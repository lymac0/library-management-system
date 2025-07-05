#include "kitapislemleri.h"
#include "ui_kitapislemleri.h"

KitapIslemleri::KitapIslemleri(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::KitapIslemleri)
{
    ui->setupUi(this);
    QSqlDatabase database = DatabaseHelper::getDatabase();
    sorgu = new QSqlQuery(database);
    sorgu->prepare("SELECT * FROM kitap");
    sorgu->exec();
    model1 = new QSqlQueryModel();
    model1->setQuery(*sorgu);
    ui->tumkitap->setModel(model1);

    modelOdunc1 = new QSqlQueryModel();
    modelOdunc2 = new QSqlQueryModel();
    ui->oduncalinma1->setModel(modelOdunc1);
    ui->oduncalinma2->setModel(modelOdunc2);

    connect(ui->tumkitap->selectionModel(), &QItemSelectionModel::selectionChanged, this, &KitapIslemleri::kitapSecim);
    connect(ui->btn_yenikayit, &QPushButton::clicked, this, &KitapIslemleri::yeniKayit);
    connect(ui->btn_guncelle, &QPushButton::clicked, this, &KitapIslemleri::guncelle);
    connect(ui->btn_sil, &QPushButton::clicked, this, &KitapIslemleri::sil);
}

KitapIslemleri::~KitapIslemleri()
{
    delete ui;
    delete sorgu;
    delete model1;
}

void KitapIslemleri::kitapSecim(const QItemSelection &selected)
{
    if (selected.indexes().isEmpty()){
        ui->kitapAd->clear();
        ui->kitapNo->clear();
        ui->kitapStok->clear();
        return;
    }
    QModelIndex index = selected.indexes().first();
    QString kitap_no = model1->data(model1->index(index.row(), 0)).toString();
    QString kitap_ad = model1->data(model1->index(index.row(), 1)).toString();
    QString kitap_sayisi = model1->data(model1->index(index.row(), 2)).toString();

    ui->kitapNo->setText(kitap_no);
    ui->kitapAd->setText(kitap_ad);
    ui->kitapStok->setText(kitap_sayisi);

    QSqlQuery queryOdunc1;
    queryOdunc1.prepare("SELECT * FROM odunc_alinan WHERE kitap_no = :kitap_no");
    queryOdunc1.bindValue(":kitap_no", kitap_no);
    queryOdunc1.exec();
    modelOdunc1->setQuery(queryOdunc1);

    QSqlQuery queryOdunc2;
    queryOdunc2.prepare("SELECT * FROM odunc_teslim_edilen WHERE kitap_no = :kitap_no");
    queryOdunc2.bindValue(":kitap_no", kitap_no);
    queryOdunc2.exec();
    modelOdunc2->setQuery(queryOdunc2);
}

void KitapIslemleri::yeniKayit()
{
    QString kitapAd = ui->kitapAd->text();
    int kitapSayisi = ui->kitapStok->text().toInt();

    if (kitapAd.isEmpty() || kitapSayisi == 0) {
        QMessageBox::warning(this, "Hata", "Gerekli alanları doldurunuz!");
        return;
    }
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM kitap WHERE kitap_ad = :kitap_ad");
    checkQuery.bindValue(":kitap_ad", kitapAd);

    if (!checkQuery.exec()) {
        QMessageBox::critical(this, "Hata", "Veritabanı hatası: " + checkQuery.lastError().text());
        return;
    }

    if (checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Uyarı", "Aynı isimde kitap girdisi bulunmaktadır");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO kitap (kitap_ad, kitap_sayisi) VALUES (:kitap_ad, :kitap_sayisi)");
    query.bindValue(":kitap_ad", kitapAd);
    query.bindValue(":kitap_sayisi", kitapSayisi);

    if (!query.exec()) {
        QMessageBox::critical(this, "Hata", "Yeni kayıt eklenemedi!");
    } else {
        QMessageBox::information(this, "Başarılı", "Yeni kayıt eklendi!");
        model1->setQuery("SELECT * FROM kitap");
    }
}

void KitapIslemleri::guncelle()
{
    QString kitapNo = ui->kitapNo->text();
    QString kitapAd = ui->kitapAd->text();
    int kitapSayisi = ui->kitapStok->text().toInt();

    if (kitapNo.isEmpty() || kitapAd.isEmpty() || kitapSayisi == 0) {
        QMessageBox::warning(this, "Hata", "Gerekli alanları doldurunuz!");
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE kitap SET kitap_ad = :kitap_ad, kitap_sayisi = :kitap_sayisi WHERE kitap_no = :kitap_no");
    query.bindValue(":kitap_no", kitapNo);
    query.bindValue(":kitap_ad", kitapAd);
    query.bindValue(":kitap_sayisi", kitapSayisi);

    if (!query.exec()) {
        QMessageBox::critical(this, "Hata", "Kitap güncellenemedi!");
    } else {
        QMessageBox::information(this, "Başarılı", "Kitap bilgileri güncellendi!");
        model1->setQuery("SELECT * FROM kitap");
    }
}

void KitapIslemleri::sil()
{
    QString kitapNo = ui->kitapNo->text();

    if (kitapNo.isEmpty()) {
        QMessageBox::warning(this, "Hata", "Silinecek kitabı seçiniz!");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM odunc_alinan WHERE kitap_no = :kitap_no");
    query.bindValue(":kitap_no", kitapNo);
    query.exec();
    query.next();

    int oduncVerilenKitapSayisi = query.value(0).toInt();
    if (oduncVerilenKitapSayisi > 0) {
        QMessageBox::critical(this, "Hata", "Bu kitap silinemez! Bu kitap bir üyeye ödünç verilmiştir.");
        return;
    }

    query.prepare("DELETE FROM kitap WHERE kitap_no = :kitap_no");
    query.bindValue(":kitap_no", kitapNo);

    if (!query.exec()) {
        QMessageBox::critical(this, "Hata", "Kitap silinemedi!");
    } else {
        QMessageBox::information(this, "Başarılı", "Kitap silindi!");
        model1->setQuery("SELECT * FROM kitap");
    }
}
