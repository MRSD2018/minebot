#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <QStringRef>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    serial = new QSerialPort(this);
    ui->setupUi(this);
    setWindowTitle("Sensor Data Tools");
    statusBar()->clearMessage();

    setupPlot();
}

MainWindow::~MainWindow()
{
    closeSerialPort();
    delete ui;
}

void MainWindow::openSerialPort(QString port)
{
    serial->setPortName(port);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite)) {
        qDebug() << "Serial Port Opened!" << endl;
    } else {
        qDebug() << "Serial Port Open Failed! :(" << endl;
    }

    connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &MainWindow::handleError);
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);
}

void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
}

void MainWindow::on_serialStart_clicked()
{
    openSerialPort(ui->serialName->text());
}

void MainWindow::on_serialStop_clicked()
{
    closeSerialPort();
}

void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
    qDebug() << "Serial Message Sent: " << data << endl;
}

void MainWindow::readData()
{
    QByteArray read = serial->readLine();
    strCat += read;
    QString search(strCat);
    QStringList searchList = search.split("\r\n",QString::SkipEmptyParts);


    if (searchList.size() > 1)
    {
        serialIn = searchList[searchList.size()-2]; // 2nd last member
        strCat = strCat.right(200);

        QStringList data = serialIn.split("_",QString::SkipEmptyParts);
        qDebug() << "Serial Data: " << data << endl;

        if (data[0] == "X")
        {
            sensorValue = data[1].toFloat();
            logData(sensorValue);
        }
    }
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}

void MainWindow::setupPlot()
{
  ui->plot->addGraph();
  ui->plot->graph(0)->setPen(QPen(QColor(255, 110, 40),4));

  QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
  timeTicker->setTimeFormat("%s");

  ui->plot->xAxis->setTicker(timeTicker);
  ui->plot->axisRect()->setupFullAxesBox();
  ui->plot->yAxis->setLabel("Sensor Data");
  ui->plot->xAxis->setLabel("Time (s)");

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(ui->plot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->plot->xAxis2, SLOT(setRange(QCPRange)));
  connect(ui->plot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->plot->yAxis2, SLOT(setRange(QCPRange)));

  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
  dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}

void MainWindow::realtimeDataSlot()
{
  static QTime time(QTime::currentTime());
  double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
  static double lastPointKey = 0;
  if (key-lastPointKey > 0.002) // at most add point every 2 ms
  {
    // add data to lines:
    ui->plot->graph(0)->addData(key, sensorValue);
    // rescale value (vertical) axis to fit the current data:
    ui->plot->graph(0)->rescaleValueAxis(true);
    lastPointKey = key;
  }
  // make key axis range scroll with the data (at a constant range size of 8):
  ui->plot->xAxis->setRange(key, 8, Qt::AlignRight);
  ui->plot->replot();
}

void MainWindow::logData(float data)
{
    if (fileLogging)
    {
        QFile logFile (ui->logPath->text());
        if ( logFile.open(QIODevice::WriteOnly | QIODevice::Append) )
        {
            QTextStream stream( &logFile );
            stream << data << endl;
        }
        logFile.close();
    }
}

void MainWindow::on_startLog_clicked()
{
    fileLogging = true;
    qDebug() << "Beginning Data Log to " << ui->logPath->text() << endl;
}

void MainWindow::on_stopLog_clicked()
{
     fileLogging = false;
     qDebug() << "Stopping Data Log to " << ui->logPath->text() << endl;
}

void MainWindow::on_logFileSelect_clicked()
{
    QString file1Name = QFileDialog::getSaveFileName(this,
        tr("Log File Destination"), "log.csv", tr("CSV Files (*.csv)"));
    ui->logPath->setText(file1Name);
}
