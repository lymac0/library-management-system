#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "uyeislemleri.h"
#include "kitapislemleri.h"
#include "oduncislemleri.h"
#include "oduncteslim.h"
#include "DatabaseHelper.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void uye();
    void kitap();
    void oduncAlma();
    void oduncTeslim();



private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
