#ifndef KEITHLEY_2000_H
#define KEITHLEY_2000_H

#include "keithley_2xxx.h"


class Keithley_2000 : public Keithley_2xxx
{
    Q_OBJECT

public slots:
    void onReceivedMessage(QString message);

public:
    explicit Keithley_2000(QString _interfaceName);
    const QString getInterfaceName();
    const QString getDeviceName();

private:
    void init();
    void connectRS232();
    const QString deviceName = "MODULE 2000";
};

#endif // KEITHLEY_2000_H
