#include "InterMessageBroker.h"
#include <fstream>

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
    analyzer.cleanSession();
    driverSocket=driverCommunication->reloadMatmorifferParameters();
    if(driverSocket){
        driverSocket->attachTarget(this);
    }
}

void InterMessageBroker::absorbMessage(char* msg)
{
    auto message=analyzer.addMessage(msg);
    emit messageFromDriverSocket(analyzer.makeQStringFromMessage(message));
}

void InterMessageBroker::filterIpAddress(QString ipAddress){
    auto filteredMessages=analyzer.filterIpAddress(ipAddress);
    for(auto& it: filteredMessages){
        emit messageFromDriverSocket(analyzer.makeQStringFromMessage(it));
    }
}

void InterMessageBroker::stopTracing()
{
     driverSocket=nullptr;
}

void InterMessageBroker::showAllLogs()
{
    const auto& vec=analyzer.getAllMessages();
    for(const auto&it: vec){
        emit messageFromDriverSocket(analyzer.makeQStringFromMessage(it));
    }
}

void InterMessageBroker::saveLogs(QString filepath){
    const auto& vec=analyzer.getAllMessages();
    std::ofstream file(filepath.toStdString());
    if(file.is_open()){
        for(const auto&it: vec){
            file<<analyzer.makeQStringFromMessage(it).toStdString()<<"\n";
        }
        file.close();
    }
}
