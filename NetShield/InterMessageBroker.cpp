#include "InterMessageBroker.h"
#include <fstream>
#include <QMap>
#include <QProcess>

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

void InterMessageBroker::loadDataForStatistics()
{
    const auto copyOfMessages=analyzer.getCopyOfMessages();
    std::map<QString,int> map;
    for(const auto&it: copyOfMessages){
        map[QString::fromStdString(it.source)]++;
    }

    std::vector<std::pair<QString, int>> vec(map.begin(), map.end());

    std::sort(vec.begin(), vec.end(), [](const auto &left, const auto &right) {
        return left.second > right.second;
    });

    QVector<QString> ips;
    QVector<int>counts;
    QVector<QString> domains;
    for(const auto &it: vec){
        ips.push_back(it.first);
        counts.push_back(it.second);
    }

    for(const auto&it: ips){
        domains.push_back(getDomain(it));
    }
    emit iPForStatistics(ips);
    emit countForStatistics(counts);
    emit domainForStatistics(domains);
    emit reloadStats();
}

void InterMessageBroker::saveLogs(QString filepath){
    const auto& vec=analyzer.getAllMessages();

    std::string str=filepath.toStdString();
    size_t pos = str.find("file://");

   if (pos != std::string::npos) {
         str.erase(pos, 7);
    }
    std::ofstream file(str);
    if(file.is_open()){
        for(const auto&it: vec){
            file<<analyzer.makeQStringFromMessage(it).toStdString()<<"\n";
        }
        file.close();
    }
}

void InterMessageBroker::loadLogs(QString filepath)
{
    const auto& vec=analyzer.getAllMessages();

    std::string str=filepath.toStdString();
    size_t pos = str.find("file://");

   if (pos != std::string::npos) {
         str.erase(pos, 7);
    }
    std::ifstream file(str);
    if(file.is_open()){
        std::string line;
        std::vector<std::string> lines;
        while (std::getline(file, line)) {
               lines.push_back(line);
        }
        file.close();
        for(auto& it: lines){
            auto msg=analyzer.addMessage(const_cast<char*>(it.c_str()),' ');
            emit messageFromDriverSocket(analyzer.makeQStringFromMessage(msg));
        }
    }
}

QString InterMessageBroker::getDomain(const QString& ipAddress)
{
    QProcess process;
    process.start("nslookup", QStringList() << ipAddress);
    if (!process.waitForFinished()) {
        qDebug() << "Error executing nslookup:" << process.errorString();
        return "";
    }

    QString result = process.readAllStandardOutput();
    QStringList lines = result.split("\n", QString::SkipEmptyParts);
    for (const QString& line : lines) {
        if (line.contains("name =")) {
            int startIndex = line.indexOf("name =") + 6;
            int endIndex = line.indexOf("\n", startIndex);
            return line.mid(startIndex, endIndex - startIndex).trimmed();
        }
    }

    return "";
}
