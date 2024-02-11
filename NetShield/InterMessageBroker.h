#ifndef INTERMESSAGEBROKER_H
#define INTERMESSAGEBROKER_H

#include <QQmlContext>
#include <QQmlEngine>
#include <QObject>
#include "DriverCommunication.h"
#include "Analyzer.h"
#include <QMap>

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
    void iPForStatistics(QVector<QString>);
    void countForStatistics(QVector<int>);
    void domainForStatistics(QVector<QString>);
    void reloadStats();

public slots:
    void tcpClicked();
    void udpClicked();
    void reloadParameters();
    void filterIpAddress(QString);
    void saveLogs(QString);
    void loadLogs(QString);
    void stopTracing();
    void showAllLogs();
    void loadDataForStatistics();

    bool tcpStatus(){return driverCommunication->getCurrentTCP();}
    bool udpStatus(){return driverCommunication->getCurrentUDP();}

private:
    DriverCommunication* driverCommunication;
    std::unique_ptr<DriverSocket> driverSocket;
    Analyzer analyzer;

private:
    QString getDomain(const QString&);
};

#endif // INTERMESSAGEBROKER_H
