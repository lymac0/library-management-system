#include "oduncteslim.h"
#include "ui_oduncteslim.h"

OduncTeslim::OduncTeslim(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OduncTeslim)
{
    ui->setupUi(this);

    QSqlDatabase database = DatabaseHelper::getDatabase();

    modelOduncAlinan = new QSqlQueryModel();
    modelOduncAlinan->setQuery("SELECT * FROM odunc_alinan",database);
    ui->tableViewOduncKitap->setModel(modelOduncAlinan);

    modelOduncTeslim = new QSqlQueryModel();
    modelOduncTeslim->setQuery("SELECT * FROM odunc_teslim_edilen",database);
    ui->tableViewTeslimKitap->setModel(modelOduncTeslim);

    connect(ui->tableViewOduncKitap->selectionModel(), &QItemSelectionModel::selectionChanged, this, &OduncTeslim::oduncSecim);
    connect(ui->btn_oduncVer, &QPushButton::clicked, this, &OduncTeslim::oduncuVer);

    ui->tarihEdit->setDate(QDate::currentDate());
}

OduncTeslim::~OduncTeslim()
{
    delete ui;
    delete modelOduncAlinan;
    delete modelOduncTeslim;
}
void OduncTeslim::oduncSecim(const QItemSelection &selected)
{
    if (selected.indexes().isEmpty()){
        ui->kitapNo->clear();
        ui->uyeNo->clear();
        return;
    }
    index = selected.indexes().first();
    QString uye_no = modelOduncAlinan->data(modelOduncAlinan->index(index.row(), 0)).toString();
    QString kitap_no = modelOduncAlinan->data(modelOduncAlinan->index(index.row(), 1)).toString();

    ui->uyeNo->setText(uye_no);
    ui->kitapNo->setText(kitap_no);
}

void OduncTeslim::oduncuVer()
{
    QString uyeNo = ui->uyeNo->text();
    QString kitapNo = ui->kitapNo->text();
    QString oduncAlma = modelOduncAlinan->data(modelOduncAlinan->index(index.row(), 2)).toString();
    QString oduncVerme = ui->tarihEdit->date().toString("dd/MM/yyyy");


    if (uyeNo.isEmpty()) {
        QMessageBox::warning(this, "Hata", "Lütfen seçim yapınız!");
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM odunc_alinan WHERE uye_no = :uye_no");
    query.bindValue(":uye_no", uyeNo);

    if (query.exec()) {
        QMessageBox::information(this, "Başarılı", "Ödünç alma işlemi başarıyla gerçekleştirildi!");
        modelOduncAlinan->setQuery("SELECT * FROM odunc_alinan");

        QSqlQuery query2;
        query2.prepare("INSERT INTO odunc_teslim_edilen (uye_no, kitap_no,alma_tarihi,verme_tarihi) VALUES (:uye_no, :kitap_no, :alma_tarihi, :verme_tarihi)");
        query2.bindValue(":uye_no", uyeNo);
        query2.bindValue(":kitap_no", kitapNo);
        query2.bindValue(":alma_tarihi", oduncAlma);
        query2.bindValue(":verme_tarihi",oduncVerme);


        if (!query2.exec()) {
            QMessageBox::warning(this, "Hata", "Ödünç alma işlemi gerçekleştirilemedi: " + query2.lastError().text());
        }

        QDate almaTarihi = QDate::fromString(oduncAlma, "dd/MM/yyyy");
        QDate vermeTarihiDate = QDate::fromString(oduncVerme, "dd/MM/yyyy");
        int gunFarki = almaTarihi.daysTo(vermeTarihiDate);
        int borc = 0;

        if (gunFarki > 15) {
            borc = (gunFarki - 15) * 2;
        }

        QSqlQuery query3;
        query3.prepare("UPDATE odunc_teslim_edilen SET borc = :borc WHERE uye_no = :uye_no AND kitap_no = :kitap_no");
        query3.bindValue(":borc", borc);
        query3.bindValue(":uye_no", uyeNo);
        query3.bindValue(":kitap_no", kitapNo);
        query3.exec();
        modelOduncTeslim->setQuery("SELECT * FROM odunc_teslim_edilen");
    } else {
        QMessageBox::warning(this, "Hata", "Ödünç alma işlemi gerçekleştirilemedi: " + query.lastError().text());
    }
}



