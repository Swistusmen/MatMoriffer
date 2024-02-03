#ifndef INTERMESSAGEBROKER_H
#define INTERMESSAGEBROKER_H

#include <QQmlContext>
#include <QQmlEngine>
#include <QObject>
#include "DriverCommunication.h"

class InterMessageBroker : public QObject
{
    Q_OBJECT
public:
    InterMessageBroker()=default;
    InterMessageBroker(QObject* parent,DriverCommunication * communicator);
    virtual ~InterMessageBroker() {}

    void makeConnections();
signals:
    void someMessage(QString);

public slots:
    void tcpClicked();
    void udpClicked();
    bool reloadParameters();

    bool tcpStatus(){return driverCommunication->getCurrentTCP();}
    bool udpStatus(){return driverCommunication->getCurrentUDP();}

private:
    DriverCommunication* driverCommunication;
};

#endif // INTERMESSAGEBROKER_H
