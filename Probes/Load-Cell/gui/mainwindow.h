#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include "qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setupPlot();
    void changeLabels(QCustomPlot *plot, QString title, QString axis);
    void modifyPlots(int state);

private slots:
    // Serial Functions
    void openSerialPort(QString port);
    void closeSerialPort();
    void on_serialStart_clicked();
    void on_serialStop_clicked();
    void writeData(const QByteArray &data);
    void readData();
    void handleError(QSerialPort::SerialPortError error);

    void realtimeDataSlot();

    void logData(float data);
    void on_startLog_clicked();
    void on_stopLog_clicked();
    void on_logFileSelect_clicked();

private:
  Ui::MainWindow *ui;
  QTimer dataTimer;
  QCPItemTracer *itemDemoPhaseTracer;
  QSerialPort *serial;
  QByteArray strCat;
  QString serialIn;
  // Plotting Data
  float sensorValue;
  // Logging Info
  bool fileLogging = false;
};

#endif // MAINWINDOW_H
