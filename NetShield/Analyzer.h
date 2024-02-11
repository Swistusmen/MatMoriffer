#ifndef ANALIZER_H
#define ANALIZER_H
#include <string>
#include <vector>
#include <algorithm>
#include <QString>

struct Message{
    std::string source;
    std::string destination;
    std::string sPort;
    std::string dPort;
    std::string packet;
};

class Analyzer{
public:
    void cleanSession();
    Message addMessage(char*,char=',');
    QString makeQStringFromMessage(const Message&);
    std::vector<Message> filterIpAddress(QString);
    std::vector<Message>& getAllMessages();
    std::vector<Message>getCopyOfMessages(){return messages;};
private:
    std::vector<Message> messages;
};

#endif // ANALIZER_H
