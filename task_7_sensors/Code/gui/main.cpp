#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QTextStream>
#include <QFile>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

void serialRead()
{
    if (serial.isOpen() && serial.isWritable())
    {
        QByteArray ba("R");
        serial.
        qDebug() << "data has been send" << endl;
        serial.close();
    }
}


int main(int argc, char *argv[])
{

    QSerialPort serial;
    serial.setPortName("COM7");
    serial.open(QIODevice::ReadWrite);
    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    if (serial.isOpen() && serial.isWritable())
    {
        QByteArray ba("R");
        serial.write(ba);
        serial.flush();
        qDebug() << "data has been send" << endl;
        serial.close();
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

