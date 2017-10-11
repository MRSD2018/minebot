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

    void setupPlots();
    void changeLabels(QCustomPlot *plot, QString title, QString axis);
    void modifyPlots(int state);

private slots:
    void openSerialPort();
    void closeSerialPort();
    void writeData(const QByteArray &data);
    void readData();
    void realtimeDataSlot();
    void handleError(QSerialPort::SerialPortError error);

    void on_sensorButton_clicked();
    void on_manualButton_clicked();

    void on_dcPos_clicked();
    void on_dcVel_clicked();
    void on_stepPos_clicked();
    void on_servPos_clicked();



private:
  Ui::MainWindow *ui;
  QTimer dataTimer;
  QCPItemTracer *itemDemoPhaseTracer;
  QSerialPort *serial;
  QByteArray strCat;
  QString serialIn;
  // Plotting Data
  float motorValue;
  float setpointValue;
  float sensorValue;
  int directionValue;
  int state;
};

#endif // MAINWINDOW_H
