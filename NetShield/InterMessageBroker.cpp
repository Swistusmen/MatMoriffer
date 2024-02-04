#include "InterMessageBroker.h"

InterMessageBroker::InterMessageBroker(QObject* parent,DriverCommunication* communicator): QObject(parent),driverCommunication(communicator) {
}

void InterMessageBroker::tcpClicked(){
    driverCommunication->turn_tcp();
}

void InterMessageBroker::udpClicked(){
    driverCommunication->turn_udp();
}

void InterMessageBroker::reloadParameters()
{
    driverSocket=driverCommunication->reloadMatmorifferParameters();
    driverSocket->attachTarget(this);
}

void InterMessageBroker::absorbMessage(char* msg)
{
    QString myQString = QString::fromUtf8(msg);
    emit messageFromDriverSocket(msg);
}
