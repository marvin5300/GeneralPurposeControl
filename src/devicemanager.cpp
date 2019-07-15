#include "devicemanager.h"

#include "devices/keithley_2000.h"
#include "devices/keithley_2410.h"
#include "devices/dummy.h"
#include <QDebug>

QList<QPointer<MeasurementDevice> > DeviceManager::activeDevicesList;
QStringList DeviceManager::ports;
QPointer<QStandardItemModel> DeviceManager::activeDeviceNameModel;
QPointer<QStandardItemModel> DeviceManager::allDeviceNameModel;

const QStringList DeviceManager::deviceNameList({
                                           "MODULE 2000",
                                           "MODULE 2410"
                                        });

QPointer<MeasurementDevice> DeviceManager::getDevice(QString deviceName, QString portName){
    static QPointer<MeasurementDevice> device;
    if (deviceName == "not selected"){
        device = new Dummy();
    }
    if (deviceName == deviceNameList.at(0)){
        device = new Keithley_2000(portName);
    }
    if (deviceName == deviceNameList.at(1)){
        device = new Keithley_2410(portName);
    }
    activeDevicesList.append(device);
    return device;
}

void DeviceManager::cleanupActiveDevicesList(){
    for (int i = activeDevicesList.size()-1; i >= 0; i--){
        if (activeDevicesList.at(i).isNull()){
            activeDevicesList.removeAt(i);
        }
    }
}

void DeviceManager::removeDevice(QPointer<MeasurementDevice> device){
    for (int i = activeDevicesList.size()-1; i >= 0; i--){
        if (!activeDevicesList.at(i).isNull()){
            /*qDebug() << activeDevicesList.at(i)->getDeviceName() << " == " << device->getDeviceName();
            qDebug() << activeDevicesList.at(i)->getInterfaceName() << " == " << device->getInterfaceName();
            qDebug() << activeDevicesList.at(i)->getLocalId() << " == " << device->getLocalId();*/
            if (activeDevicesList.at(i)->getDeviceName() == device->getDeviceName()
                    && activeDevicesList.at(i)->getInterfaceName() == device->getInterfaceName()
                    && activeDevicesList.at(i)->getLocalId() == device->getLocalId()){
                activeDevicesList.removeAt(i);
                actualizeDeviceNameModel();
                return;
            }
        }else{
            activeDevicesList.removeAt(i);
        }
    }
}

QPointer<QStandardItemModel> DeviceManager::getActiveDeviceNameModel(){
    actualizeDeviceNameModel();
    return activeDeviceNameModel;
}

void DeviceManager::actualizeDeviceNameModel(){
    cleanupActiveDevicesList();
    if (activeDeviceNameModel.isNull()){
        activeDeviceNameModel = new QStandardItemModel;
    }
    activeDeviceNameModel->clear();
    for (auto device : activeDevicesList){
        activeDeviceNameModel->appendRow(new QStandardItem(QString(device->getDeviceName() + "  on port: " + device->getInterfaceName())));
    }
}

QPointer<QStandardItemModel> DeviceManager::getAllDeviceNameModel(){
    if (allDeviceNameModel.isNull()){
        allDeviceNameModel = new QStandardItemModel;
        allDeviceNameModel->appendRow(new QStandardItem(QString("not selected")));
        for (auto deviceName : deviceNameList){
            allDeviceNameModel->appendRow(new QStandardItem(deviceName));
        }
    }
    return allDeviceNameModel;
}

void DeviceManager::generatePortList(){    // this is done once at start
    if (!ports.empty()){
        return;
    }
#if defined(Q_OS_WIN)
    for (quint8 i = 1; i < 100; i++){
        ports.append(QString("COM%1").arg(i));
    }
#else
    for (quint8 i = 0; i < 100; i++){
        ports.append(QString("/dev/ttyACM%1").arg(i));
    }
#endif
}