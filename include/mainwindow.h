#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "measurementdevice.h"
#include "serialconsole.h"
#include "filehandler.h"
#include <QMainWindow>
#include <QMap>
#include <QTimer>

/**
* @mainpage
* @section Class-Diagram
* @image html generalpurposecontrol.png
* @image latex generalpurposecontrol.ps
* 
*/
namespace Ui {
class MainWindow;
}

/**
* @brief Main window of the application.
* Has two rows of widgets, one for devices and one for scan-parameter-selections
* which are used to apply parameter values and loop over them later,
* for example if you want to ramp the voltage over time.
* Also has a menu to get to the special functions like settings, file selection
* or the serial console. Buttons to start or stop a measurement are also here.
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void setOutputFile(QString filename);
    void setAutoOutputFile(bool autoOutput);
    void nextInterval();
    void measure(quint64 count);
    void scanInit();
    void startMeasurement();
    void finishedMeasurement();
    void probeProgress(int progressStart = 0);

public slots:
    void onDeviceSelectionChange(QPointer<MeasurementDevice> device, QString _newDeviceName, QString _newInterfaceName);
    void onSettingsClicked();
    void onProgressReceived(int progress);
    void onMeasureReady(QString deviceName, quint64 number);
    void onScanParamSelReady(QString deviceName, quint64 number);
    void onTimerTimeout();
    void setSerialMask(QStringList masks);

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddAdjustValuesButtonClicked();
    void onAddMeasureValuesButtonClicked();
    void adjustProgressBarAppearance(int value);
    void onStartMeasurementButtonClicked();
    void onFinishedMeasurement();

private:
    Ui::MainWindow *ui;
    QPointer<FileHandler> fileHandler;
    QString highLevel = "QProgressBar {"
                        "background-color: #74c8ff;"
                        "color: #33ff33;"
                        "text-align: center; }"
                        "QProgressBar::chunk {"
                        "background-color: #010327; }";
    QString lowLevel =  "QProgressBar {"
                        "background-color: #74c8ff;"
                        "color: #008000;"
                        "text-align: center; }"
                        "QProgressBar::chunk {"
                        "background-color: #010327; }";
    bool ongoingMeasurement = false;
    void setUiMeasurementState(bool _ongoingMeasurement);
    void connectScanValues(bool doConnect);
    QTimer intervalTimer;
    bool timerOngoing;
    int numberOfScanparameterSelections;
    int scanParameterReadyCounter;
    int measurementReadyCounter = 0;
    quint64 measCount = 0;
    int measCycle = 0;
};

#endif // MAINWINDOW_H
