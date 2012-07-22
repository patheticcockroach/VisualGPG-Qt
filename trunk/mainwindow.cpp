#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //ui->tableWidget->setColumnCount(5);
    ui->tableWidget->insertRow(0);
    ui->tableWidget->insertRow(1);
    //QStringList certifTableLabels;
    //certifTableLabels << "Name" << "E-mail" << "Valid from" << "Valid until" << "Key-ID";
    //ui->tableWidget->setHorizontalHeaderLabels(certifTableLabels);
    QTableWidgetItem *item0 = new QTableWidgetItem("Item2");
    //item0->setFlags(Qt::ItemIsEnabled);
    //item0->setWhatsThis("You can change this task's comment, start time and end time.");
    QTableWidgetItem *item1 = new QTableWidgetItem("Item3");
    QTableWidgetItem *item2 = new QTableWidgetItem("A");
    QTableWidgetItem *item3 = new QTableWidgetItem("B");
    //item0->setCheckState(Qt::Checked);
    ui->tableWidget->setItem(0,0,item0);
    ui->tableWidget->setItem(1,0,item1);
    ui->tableWidget->setItem(0,1,item3);
    ui->tableWidget->setItem(1,1,item2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAboutQt_triggered()
{
    qApp->aboutQt();
}

void MainWindow::on_actionAboutVisualGPG_triggered()
{
    QMessageBox::information(this,"About VisualGPG","VisualGPG 2.0.0-dev1");
}

void MainWindow::on_actionImportCertificates_triggered()
{
    this->importCertificateFiles();
}

void MainWindow::importCertificateFiles()
{
    QStringList certfFiles = QFileDialog::getOpenFileNames(this, "Choose ce", QString(), "Certificates (*.asc *.cer *.cert *.crt *.der *.pem *.gpg *.p7c *.p12 *.pfx *.pgp);;Any files (*.*)");
}

void MainWindow::on_actionRedisplayCertificates_triggered()
{
    QProcess gnuPG;
    QStringList gpgArguments;
    QString keyLineStart;
    QTableWidget *activeKeyTable;
    if(ui->tabWidget->currentIndex()==0) {
        gpgArguments << "--list-secret-keys";
        keyLineStart="sec";
        activeKeyTable=ui->tableWidget;
    }
    else {
        gpgArguments << "--list-keys";
        keyLineStart="pub";
        activeKeyTable=ui->tableWidgetAllCerts;
    }
    gnuPG.start("gpg2.exe",gpgArguments);
    QByteArray gpgout;
    while (gnuPG.waitForReadyRead()) {
      gpgout += gnuPG.readAll();
    }
    QMessageBox::information(this,"About GnuPG",QString::fromLocal8Bit(gpgout));

    QStringList entries = QString::fromLocal8Bit(gpgout).split('\n');
    entries.removeLast();
    entries.removeFirst();entries.removeFirst();

    // split list of keys per key
    QString entry,keyEntry;
    QStringList keyList;
    foreach(entry, entries) {
        if(entry.length()>1) {
            keyEntry+=entry;
        }
        else {
            keyList << keyEntry;
            keyEntry = "";
        }
    }

    // and now process that list
    activeKeyTable->clearContents();
    while(activeKeyTable->rowCount()>0) activeKeyTable->removeRow(0);
    foreach(keyEntry, keyList) {
        //QRegExp reCap("^\\+(\\d{1,2})\\(([089])\\)(\\d{2,5})\\-(\\d+)$"); // http://www.java2s.com/Code/Cpp/Qt/UseQRegExptomatchphonenumbers.htm
        //QRegExp reCap("sec +([0-9]{3,5})(?:R|D)/([0-9a-z]{8}) ([0-9]{4}-[0-9]{2}[0-9]{2})(?:\\[expires\\: ([0-9]{4}-[0-9]{2}-[0-9]{2}))");
        QRegExp reCap(keyLineStart+" +([0-9]{3,5})(?:R|D)/([0-9a-z]{8}) ([0-9]{4}-[0-9]{2}-[0-9]{2})");
        QRegExp reCapExpi("\\[expire(s|d)\\: ([0-9]{4}-[0-9]{2}-[0-9]{2})\\]");
        reCap.setMinimal(true);reCapExpi.setMinimal(true);
        reCap.setCaseSensitivity(Qt::CaseInsensitive);reCapExpi.setCaseSensitivity(Qt::CaseInsensitive);
        reCap.indexIn(keyEntry);reCapExpi.indexIn(keyEntry);
        //QMessageBox::information(this,"About GnuPG",reCap.cap(1)+" "+reCap.cap(2)+" "+reCap.cap(3)+" "+reCap.cap(4)+" "+reCap.cap(5)+" "+reCap.cap(6));
        //QMessageBox::information(this,"About GnuPG",reCapExpi.cap(1)+" "+reCapExpi.cap(2));
        activeKeyTable->insertRow(activeKeyTable->rowCount());
        QTableWidgetItem *itemValidFrom = new QTableWidgetItem(reCap.cap(3));
        QTableWidgetItem *itemValidUntil = new QTableWidgetItem(reCapExpi.cap(2));
        QTableWidgetItem *itemKeyID = new QTableWidgetItem(reCap.cap(2));
        activeKeyTable->setItem(activeKeyTable->rowCount()-1,2,itemValidFrom);
        activeKeyTable->setItem(activeKeyTable->rowCount()-1,3,itemValidUntil);
        activeKeyTable->setItem(activeKeyTable->rowCount()-1,4,itemKeyID);
    }
}




void MainWindow::on_actionAboutGnuPG_triggered()
{
    // super-méga génial pour l'utilisation synchronisée de QProcess: http://www.java2s.com/Code/Cpp/Qt/SynchronousUseofQProcess.htm
    QProcess gnuPG;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    gnuPG.setProcessEnvironment(env);
    QStringList gpgArguments;
    gpgArguments << "--version";
    gnuPG.start("gpg2.exe",gpgArguments);

    QByteArray gpgout;
    while (gnuPG.waitForReadyRead()) {
      gpgout += gnuPG.readAll();
    }
    //QStringList entries = QString::fromLocal8Bit(gpgout).split('\n');
    //entries.removeLast();

    QMessageBox::information(this,"About GnuPG",QString::fromLocal8Bit(gpgout));
}


/* // http://lists.qt.nokia.com/pipermail/qt-interest/2009-August/011515.html
QFile outFile;
QTextStream outStream;
outFile.open(gnuPG.StandardOutput, QIODevice::ReadOnly);
outStream.setDevice(&outFile);

QMessageBox::information(this,"output",outStream);

//connect(gnuPG, SIGNAL(readyReadStandardOutput()), this, SLOT(gpgOutReady(gnuPG)));*/
/*void MainWindow::gpgOutReady(QProcess gnuPG)
{
    QByteArray gpgoutBinary = gnuPG.readAllStandardOutput();
    QString gpgout;
    gpgout = QString(gpgoutBinary);
    QMessageBox::information(this,"output",gpgout);
}*/
