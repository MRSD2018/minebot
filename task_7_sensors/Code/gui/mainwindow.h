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

    void setupRealtimeDataDemo(QCustomPlot *customPlot);

private slots:
    void openSerialPort();
    void closeSerialPort();
    void writeData(const QByteArray &data);
    void readData();
    void realtimeDataSlot();
    void handleError(QSerialPort::SerialPortError error);

private:
  Ui::MainWindow *ui;
  QTimer dataTimer;
  QCPItemTracer *itemDemoPhaseTracer;
  QSerialPort *serial;
  QByteArray strCat;
  QString serialIn;
};

#endif // MAINWINDOW_H
