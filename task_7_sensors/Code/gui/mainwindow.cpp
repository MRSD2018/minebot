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


    setupPlots(ui->motorPlot, ui->sensorPlot);
    statusBar()->clearMessage();
    ui->motorPlot->replot();
    ui->sensorPlot->replot();

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
    qDebug() << "Serial Message Sent: " << data << endl;
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

void MainWindow::setupPlots(QCustomPlot *motorPlot, QCustomPlot *sensorPlot)
{
  motorPlot->addGraph(); // blue line
  motorPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
  motorPlot->addGraph(); // red line
  motorPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));

  sensorPlot->addGraph(); // blue line
  sensorPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));

  QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
  timeTicker->setTimeFormat("%h:%m:%s");

  motorPlot->xAxis->setTicker(timeTicker);
  motorPlot->axisRect()->setupFullAxesBox();
  motorPlot->yAxis->setRange(-100, 1000);
  motorPlot->plotLayout()->insertRow(0);
  motorPlot->plotLayout()->addElement(0, 0, new QCPTextElement(motorPlot, "DC Motor Position Control", QFont("sans", 12, QFont::Bold)));
  motorPlot->xAxis->setLabel("Time (s)");
  motorPlot->yAxis->setLabel("Angle (Degrees)");

  sensorPlot->xAxis->setTicker(timeTicker);
  sensorPlot->axisRect()->setupFullAxesBox();
  sensorPlot->yAxis->setRange(-100, 1000);
  sensorPlot->plotLayout()->insertRow(0);
  sensorPlot->plotLayout()->addElement(0, 0, new QCPTextElement(sensorPlot, "Pressure Sensor Data", QFont("sans", 12, QFont::Bold)));
  sensorPlot->xAxis->setLabel("Time (s)");
  sensorPlot->yAxis->setLabel("Sensor Data (N)");

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(motorPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), motorPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(motorPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), motorPlot->yAxis2, SLOT(setRange(QCPRange)));

  connect(sensorPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), sensorPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(sensorPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), sensorPlot->yAxis2, SLOT(setRange(QCPRange)));

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
    ui->motorPlot->graph(0)->addData(key, serialIn.toDouble());
    ui->motorPlot->graph(1)->addData(key, qCos(key)+qrand()/(double)RAND_MAX*0.5*qSin(key/0.4364));
    // rescale value (vertical) axis to fit the current data:
    ui->motorPlot->graph(0)->rescaleValueAxis(true);
    ui->motorPlot->graph(1)->rescaleValueAxis(true);
    lastPointKey = key;
  }
  // make key axis range scroll with the data (at a constant range size of 8):
  ui->motorPlot->xAxis->setRange(key, 8, Qt::AlignRight);
  ui->motorPlot->replot();

  // calculate frames per second:
  static double lastFpsKey;
  static int frameCount;
  ++frameCount;
  if (key-lastFpsKey > 2) // average fps over 2 seconds
  {
    ui->statusBar->showMessage(
          QString("%1 FPS, Total Data points: %2")
          .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
          .arg(ui->motorPlot->graph(0)->data()->size()+ui->motorPlot->graph(1)->data()->size())
          , 0);
    lastFpsKey = key;
    frameCount = 0;
  }
}

void MainWindow::on_sensorButton_clicked()
{
    writeData("R_001");
    qDebug() << "Wrote to Serial!" << endl;
}

void MainWindow::on_motorTabs_tabBarClicked(int index)
{
    switch(index)
    {
        case 0: // DC Position
            writeData("A");
            break;
        case 1: // DC Velocity
            writeData("B");
            break;
        case 2: // Stepper Position
            writeData("C");
            break;
        case 3: // Servo Position
            writeData("D");
            break;
        default: break;
    }
}








