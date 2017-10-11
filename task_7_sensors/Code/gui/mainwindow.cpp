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

    setupPlots();

    statusBar()->clearMessage();
    ui->motorPlot->replot();
    ui->sensorPlot->replot();

    serial = new QSerialPort(this);
    openSerialPort();
}

MainWindow::~MainWindow()
{
    closeSerialPort();
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
    QByteArray read = serial->readAll();
    strCat += read;
    QString search(strCat);
    QStringList searchList = search.split("\r\n",QString::SkipEmptyParts);

    if (searchList.size() > 1)
    {
        serialIn = searchList[searchList.size()-2]; // 2nd last member
        strCat = strCat.right(200);

        QStringList data = serialIn.split("_",QString::SkipEmptyParts);

        if (data[0] == "X" && data.length() == 6)
        {
            motorValue = data[1].toFloat();
            setpointValue = data[2].toFloat();
            sensorValue = data[3].toFloat();
            directionValue = data[4].toInt();
            state = data[5].toInt();
            modifyPlots(state);
//            qDebug() << "Serial Data: " << motorValue << endl;
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

void MainWindow::setupPlots()
{
  ui->motorPlot->addGraph(); // blue line
  ui->motorPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
  ui->motorPlot->addGraph(); // red line
  ui->motorPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));
  ui->sensorPlot->addGraph(); // blue line
  ui->sensorPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));

  QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
  timeTicker->setTimeFormat("%s");

  ui->motorPlot->xAxis->setTicker(timeTicker);
  ui->motorPlot->axisRect()->setupFullAxesBox();
  ui->motorPlot->yAxis->setRange(0, 100);
  ui->motorPlot->plotLayout()->insertRow(0);

  ui->sensorPlot->xAxis->setTicker(timeTicker);
  ui->sensorPlot->axisRect()->setupFullAxesBox();
  ui->sensorPlot->yAxis->setRange(0, 100);
  ui->sensorPlot->plotLayout()->insertRow(0);

  modifyPlots(0);

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(ui->motorPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->motorPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(ui->motorPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->motorPlot->yAxis2, SLOT(setRange(QCPRange)));

  connect(ui->sensorPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->sensorPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(ui->sensorPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->sensorPlot->yAxis2, SLOT(setRange(QCPRange)));

  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
  dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}

void MainWindow::changeLabels(QCustomPlot *plot, QString title, QString axis)
{
    plot->plotLayout()->removeAt(0);
    plot->plotLayout()->addElement(0, 0, new QCPTextElement(plot, title, QFont("sans", 12, QFont::Bold)));
    plot->yAxis->setLabel(axis);
    plot->xAxis->setLabel("Time (s)");
}

void MainWindow::modifyPlots(int state)
{
    switch(state)
    {
        case 0:
            changeLabels(ui->motorPlot, "DC Motor Position", "Angle (Degrees)");
            changeLabels(ui->sensorPlot, "Pressure Sensor", "Sensor Data (N)");
            ui->dcPos->setChecked(true);
            break;
        case 1:
            changeLabels(ui->motorPlot, "DC Motor Velocity", "Angle (Degrees/Second)");
            changeLabels(ui->sensorPlot, "Pressure Sensor", "Sensor Data (N)");
            ui->dcVel->setChecked(true);
            break;
        case 2:
            changeLabels(ui->motorPlot, "Stepper Motor Position", "Angle (Degrees)");
            changeLabels(ui->sensorPlot, "Infrared Sensor", "Sensor Data (m)");
            ui->stepPos->setChecked(true);
            break;
        case 3:
            changeLabels(ui->motorPlot, "Servo Motor Position", "Angle (Degrees)");
            changeLabels(ui->sensorPlot, "Potentiometer Sensor", "Sensor Data (Degrees)");
            ui->servPos->setChecked(true);
            break;
        default: break;
    }
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
    ui->motorPlot->graph(0)->addData(key, motorValue);
    ui->motorPlot->graph(1)->addData(key, setpointValue);
    ui->sensorPlot->graph(0)->addData(key, sensorValue);
    // rescale value (vertical) axis to fit the current data:
//    ui->motorPlot->graph(0)->rescaleValueAxis(false);
//    ui->motorPlot->graph(1)->rescaleValueAxis(false);
    ui->sensorPlot->graph(0)->rescaleValueAxis(true);
    lastPointKey = key;
  }
  // make key axis range scroll with the data (at a constant range size of 8):
  ui->motorPlot->xAxis->setRange(key, 8, Qt::AlignRight);
  ui->sensorPlot->xAxis->setRange(key, 8, Qt::AlignRight);
  ui->motorPlot->replot();
  ui->sensorPlot->replot();

  // calculate frames per second:
  static double lastFpsKey;
  static int frameCount;
  ++frameCount;
  if (key-lastFpsKey > 2) // average fps over 2 seconds
  {
    if (directionValue == 0)
        ui->statusBar->showMessage(QString("Motor Direction: Clockwise"), 0);
    else
        ui->statusBar->showMessage(QString("Motor Direction: Anti-Clockwise"), 0);
    lastFpsKey = key;
    frameCount = 0;
  }
}


void MainWindow::on_dcPos_clicked()
{
    writeData("A");
    modifyPlots(0);
}

void MainWindow::on_dcVel_clicked()
{
    writeData("B");
    modifyPlots(1);
}

void MainWindow::on_stepPos_clicked()
{
    writeData("C");
    modifyPlots(2);
}

void MainWindow::on_servPos_clicked()
{
    writeData("D");
    modifyPlots(3);
}

void MainWindow::on_manualButton_clicked()
{
    double value(ui->manualInput->value());

    QString valueAsString = QString::number(value);
    valueAsString = "M_" + valueAsString;
    QByteArray serialSend(valueAsString.toUtf8());

    writeData(serialSend);
}

void MainWindow::on_sensorButton_clicked()
{
    writeData("S");
}

