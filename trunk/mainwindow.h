#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QProcess>
#include <QRegExp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_actionAboutQt_triggered();
    void on_actionAboutVisualGPG_triggered();
    void on_actionImportCertificates_triggered();
    void on_actionRedisplayCertificates_triggered();
    void on_actionAboutGnuPG_triggered();

private:
    Ui::MainWindow *ui;
    void importCertificateFiles();
};

#endif // MAINWINDOW_H
