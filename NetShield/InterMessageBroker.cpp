#include "InterMessageBroker.h"

InterMessageBroker::InterMessageBroker(QObject* parent,DriverCommunication* communicator): QObject(parent),driverCommunication(communicator) {
}

void InterMessageBroker::tcpClicked(){
    driverCommunication->turn_tcp();
}

void InterMessageBroker::udpClicked(){
    driverCommunication->turn_udp();
}

bool InterMessageBroker::reloadParameters()
{
    //tutaj gdzie musi byc mechanizmktory stopuje obecny odczyt
    return driverCommunication->reloadMatmorifferParameters();
}
