#include "uyeislemleri.h"
#include "ui_uyeislemleri.h"

UyeIslemleri::UyeIslemleri(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UyeIslemleri)
{
    ui->setupUi(this);
    QSqlDatabase database = DatabaseHelper::getDatabase();
    sorgu=new QSqlQuery(database);
    sorgu->prepare("select * from üye");
    sorgu->exec();
    model1 = new QSqlQueryModel();
    model1->setQuery(*sorgu);
    ui->tableView->setModel(model1);
    connect(ui->tableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &UyeIslemleri::uyeSecim);
    connect(ui->btn_yenikayit, &QPushButton::clicked, this, &UyeIslemleri::yeniKayit);
    connect(ui->btn_guncelle, &QPushButton::clicked, this, &UyeIslemleri::guncelle);
    connect(ui->btn_sil, &QPushButton::clicked, this, &UyeIslemleri::sil);
}

UyeIslemleri::~UyeIslemleri()
{
    delete ui;
    delete sorgu;
    delete model1;
}

void UyeIslemleri::uyeSecim(const QItemSelection &selected)
{
    if (selected.indexes().isEmpty()){
        ui->uye_no->clear();
        ui->uye_ad->clear();
        ui->uye_soyad->clear();
        return;
    }

    QModelIndex index = selected.indexes().first();
    QString uye_no = model1->data(model1->index(index.row(), 0)).toString();
    QString uye_ad = model1->data(model1->index(index.row(), 1)).toString();
    QString uye_soyad = model1->data(model1->index(index.row(), 2)).toString();

    ui->uye_no->setText(uye_no);
    ui->uye_ad->setText(uye_ad);
    ui->uye_soyad->setText(uye_soyad);
}

void UyeIslemleri::yeniKayit()
{
    QString uyeAd = ui->uye_ad->text();
    QString uyeSoyad = ui->uye_soyad->text();

    if (uyeAd.isEmpty() || uyeSoyad.isEmpty()) {
        QMessageBox::warning(this, "Hata", "Gerekli alanları doldurunuz!");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO üye (uye_ad, uye_soyad) VALUES (:uye_ad, :uye_soyad)");
    query.bindValue(":uye_ad", uyeAd);
    query.bindValue(":uye_soyad", uyeSoyad);

    if (!query.exec()) {
        QMessageBox::critical(this, "Hata", "Yeni kayıt eklenemedi!");
    } else {
        QMessageBox::information(this, "Başarılı", "Yeni kayıt eklendi!");
        model1->setQuery("SELECT * FROM üye");
    }
}
void UyeIslemleri::guncelle()
{
    QString uyeNo = ui->uye_no->text();
    QString uyeAd = ui->uye_ad->text();
    QString uyeSoyad = ui->uye_soyad->text();

    if (uyeNo.isEmpty() || uyeAd.isEmpty() || uyeSoyad.isEmpty()) {
        QMessageBox::warning(this, "Hata", "Gerekli alanları doldurunuz!");
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE üye SET uye_ad = :uye_ad, uye_soyad = :uye_soyad WHERE uye_no = :uye_no");
    query.bindValue(":uye_no", uyeNo);
    query.bindValue(":uye_ad", uyeAd);
    query.bindValue(":uye_soyad", uyeSoyad);

    if (!query.exec()) {
        QMessageBox::critical(this, "Hata", "Üye güncellenemedi!");
    } else {
        QMessageBox::information(this, "Başarılı", "Üye bilgileri güncellendi!");
        model1->setQuery("SELECT * FROM üye");
    }
}
void UyeIslemleri::sil()
{
    QString uyeNo = ui->uye_no->text();

    if (uyeNo.isEmpty()) {
        QMessageBox::warning(this, "Hata", "Silinecek üyeyi seçiniz!");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM odunc_alinan WHERE uye_no = :uye_no");
    query.bindValue(":uye_no", uyeNo);
    query.exec();
    query.next();

    int teslimEdilmeyenKitapSayisi = query.value(0).toInt();
    if (teslimEdilmeyenKitapSayisi > 0) {
        QMessageBox::critical(this, "Hata", "Bu üye silinemez. Üyenin henüz teslim etmediği kitaplar vardır.");
        return;
    }

    query.prepare("DELETE FROM üye WHERE uye_no = :uye_no");
    query.bindValue(":uye_no", uyeNo);

    if (!query.exec()) {
        QMessageBox::critical(this, "Hata", "Üye silinemedi!");
    } else {
        QMessageBox::information(this, "Başarılı", "Üye silindi!");
        model1->setQuery("SELECT * FROM üye");
    }
}
