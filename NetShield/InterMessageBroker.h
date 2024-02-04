#ifndef INTERMESSAGEBROKER_H
#define INTERMESSAGEBROKER_H

#include <QQmlContext>
#include <QQmlEngine>
#include <QObject>
#include "DriverCommunication.h"

class InterMessageBroker : public QObject, public Target
{
    Q_OBJECT
public:
    InterMessageBroker()=default;
    InterMessageBroker(QObject* parent,DriverCommunication * communicator);
    virtual ~InterMessageBroker() {}

    void absorbMessage(char* ) override;
signals:
    void someMessage(QString);
    void messageFromDriverSocket(QString);

public slots:
    void tcpClicked();
    void udpClicked();
    void reloadParameters();

    bool tcpStatus(){return driverCommunication->getCurrentTCP();}
    bool udpStatus(){return driverCommunication->getCurrentUDP();}

private:
    DriverCommunication* driverCommunication;
    std::unique_ptr<DriverSocket> driverSocket;
};

#endif // INTERMESSAGEBROKER_H