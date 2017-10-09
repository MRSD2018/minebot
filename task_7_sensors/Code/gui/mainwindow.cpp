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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("MRSD Sensors Lab");

    setupRealtimeDataDemo(ui->customPlot);
    statusBar()->clearMessage();
    ui->customPlot->replot();

    serial = new QSerialPort(this);
    openSerialPort();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openSerialPort()
{
    serial->setPortName("COM7");
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

void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}

void MainWindow::readData()
{
    QByteArray data = serial->readAll();

    //qDebug() << data << endl;

    strCat += data;
    QString search(strCat);
    QStringList searchList = search.split("\r\n",QString::SkipEmptyParts);

    //qDebug() << searchList << endl;

    if (searchList.size() > 1)
    {
        serialIn = searchList[searchList.size()-2]; // 2nd last member
        strCat = strCat.right(50);

        qDebug() << "Serial In: " << serialIn << endl;
    }
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}

void MainWindow::setupRealtimeDataDemo(QCustomPlot *customPlot)
{
  // include this section to fully disable antialiasing for higher performance:
  /*
  customPlot->setNotAntialiasedElements(QCP::aeAll);
  QFont font;
  font.setStyleStrategy(QFont::NoAntialias);
  customPlot->xAxis->setTickLabelFont(font);
  customPlot->yAxis->setTickLabelFont(font);
  customPlot->legend->setFont(font);
  */
  customPlot->addGraph(); // blue line
  customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
  customPlot->addGraph(); // red line
  customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));

  QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
  timeTicker->setTimeFormat("%h:%m:%s");
  customPlot->xAxis->setTicker(timeTicker);
  customPlot->axisRect()->setupFullAxesBox();
  customPlot->yAxis->setRange(-100, 1000);

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
  dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}

void MainWindow::realtimeDataSlot()
{
  static QTime time(QTime::currentTime());
  // calculate two new data points:
  double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
  static double lastPointKey = 0;
  if (key-lastPointKey > 0.002) // at most add point every 2 ms
  {
    // add data to lines:
    ui->customPlot->graph(0)->addData(key, serialIn.toDouble());
    ui->customPlot->graph(1)->addData(key, qCos(key)+qrand()/(double)RAND_MAX*0.5*qSin(key/0.4364));
    // rescale value (vertical) axis to fit the current data:
    ui->customPlot->graph(0)->rescaleValueAxis(true);
    ui->customPlot->graph(1)->rescaleValueAxis(true);
    lastPointKey = key;
  }
  // make key axis range scroll with the data (at a constant range size of 8):
  ui->customPlot->xAxis->setRange(key, 8, Qt::AlignRight);
  ui->customPlot->replot();

  // calculate frames per second:
  static double lastFpsKey;
  static int frameCount;
  ++frameCount;
  if (key-lastFpsKey > 2) // average fps over 2 seconds
  {
    ui->statusBar->showMessage(
          QString("%1 FPS, Total Data points: %2")
          .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
          .arg(ui->customPlot->graph(0)->data()->size()+ui->customPlot->graph(1)->data()->size())
          , 0);
    lastFpsKey = key;
    frameCount = 0;
  }
}
