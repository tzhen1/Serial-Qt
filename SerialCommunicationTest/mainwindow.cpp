#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Serial");

    mSerial = new QSerialPort(this);
    updateSerialPorts();

    mSerialScanTimer = new QTimer(this);
    mSerialScanTimer->setInterval(5000);
    mSerialScanTimer->start();

    connect(mSerialScanTimer, &QTimer::timeout,
            this, &MainWindow::updateSerialPorts);

    connect(ui->lineEdit, &QLineEdit::returnPressed,
            this, &MainWindow::on_pushButton_2_clicked);

    connect(mSerial, &QSerialPort::readyRead,
            this, &MainWindow::serialReadyRead);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateSerialPorts()
{
    mSerialPorts = QSerialPortInfo::availablePorts();

    ui->comboBox->clear();
    for (QSerialPortInfo port : mSerialPorts) {
        ui->comboBox->addItem(port.portName(), port.systemLocation());
    }
}



//void MainWindow::on_connectPushButton_clicked()
//{
//    ui->pushButton->setEnabled(false);
//    QString serialName =  ui->comboBox->currentText();
//    QString serialLoc  =  ui->comboBox->currentData().toString();

//    if (mSerial->isOpen()) {
//        qDebug() << "Serial already connected, disconnecting!";
//        mSerial->close();
//    }

//    mSerial->setPortName(serialLoc);
//    mSerial->setBaudRate(QSerialPort::Baud115200);
//    mSerial->setDataBits(QSerialPort::Data8);
//    mSerial->setParity(QSerialPort::NoParity);
//    mSerial->setStopBits(QSerialPort::OneStop);
//    mSerial->setFlowControl(QSerialPort::NoFlowControl);

//    if(mSerial->open(QIODevice::ReadWrite)) {
//        qDebug() << "SERIAL: OK!";
//    } else {
//        qDebug() << "SERIAL: ERROR!";
//    }
//    ui->pushButton->setEnabled(true);
//}

//void MainWindow::on_sendPushButton_clicked()
//{
//    if (mSerial->isOpen()) {

//        QString str= ui->lineEdit->text();
//        ui->lineEdit->clear();
//        str.append("\r\n");
//        mSerial->write(str.toLocal8Bit());
//    } else {
//        qDebug() << "Serial port not connected!";
//    }
//}

void MainWindow::serialReadyRead()
{
    QByteArray data = mSerial   ->readAll();
    QString str = QString(data);
    ui->textBrowser->insertPlainText(str);
    QScrollBar *sb = ui->textBrowser->verticalScrollBar();
    sb->setValue(sb->maximum());

}


void MainWindow::on_pushButton_clicked()
{
    ui->pushButton->setEnabled(false);
    QString serialName =  ui->comboBox->currentText();
    QString serialLoc  =  ui->comboBox->currentData().toString();

    if (mSerial->isOpen()) {
        qDebug() << "Serial already connected, disconnecting!";
        mSerial->close();
    }

    mSerial->setPortName(serialLoc);
    mSerial->setBaudRate(QSerialPort::Baud115200);
    mSerial->setDataBits(QSerialPort::Data8);
    mSerial->setParity(QSerialPort::NoParity);
    mSerial->setStopBits(QSerialPort::OneStop);
    mSerial->setFlowControl(QSerialPort::NoFlowControl);

    if(mSerial->open(QIODevice::ReadWrite)) {
        qDebug() << "SERIAL: OK!";
    } else {
        qDebug() << "SERIAL: ERROR!";
    }
    ui->pushButton->setEnabled(true);
}


void MainWindow::on_pushButton_2_clicked()
{
    if (mSerial->isOpen()) {

        QString str= ui->lineEdit->text();
        ui->lineEdit->clear();
        str.append("\r\n");
        mSerial->write(str.toLocal8Bit());
    } else {
        qDebug() << "Serial port not connected!";
    }
}

