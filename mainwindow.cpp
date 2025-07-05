#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if(!DatabaseHelper::connectToDatabase()) {
        ui->statusbar->showMessage("Veritabanına bağlanılamadı!");
    } else {
        ui->statusbar->showMessage("Veritabanına bağlanıldı!");
    }
    connect(ui->uye_islemleri, &QPushButton::clicked, this, &MainWindow::uye);
    connect(ui->kitap_islemleri, &QPushButton::clicked, this, &MainWindow::kitap);
    connect(ui->oduncalma_islemleri, &QPushButton::clicked, this, &MainWindow::oduncAlma);
    connect(ui->oduncteslim_islemleri, &QPushButton::clicked, this, &MainWindow::oduncTeslim);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::uye()
{
    UyeIslemleri *uyeislemleri = new UyeIslemleri(this);
    uyeislemleri->exec();
}

void MainWindow::kitap()
{
    KitapIslemleri *kitapislemleri = new KitapIslemleri(this);
    kitapislemleri->exec();
}

void MainWindow::oduncAlma()
{
    OduncIslemleri *oduncislemleri = new OduncIslemleri(this);
    oduncislemleri->exec();
}

void MainWindow::oduncTeslim()
{
    OduncTeslim *oduncteslim = new OduncTeslim(this);
    oduncteslim->exec();
}



