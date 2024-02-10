#include "Analyzer.h"

void Analyzer::cleanSession(){
    messages.clear();
}

Message Analyzer::addMessage(char* msg){
    std::string msgAsString{msg};
    Message properMessage;
    if(msgAsString.at(0)=='U'){
            properMessage.packet="UDP";
        }else{
            properMessage.packet="TCP";
        }

        int i=0;
        for(int dots=0;dots<=3&& msgAsString[i]!=',';i++){
            if(isdigit(msgAsString[i])){
                properMessage.source+=msgAsString[i];
            }
            if( msgAsString[i]=='.'){
                dots++;
                properMessage.source+=msgAsString[i];
            }
        }
        i++;

        while(msgAsString[i]!=','){
            if(isdigit(msgAsString[i])){
            properMessage.sPort+=msgAsString[i];
            }
            i++;
        }
        i++;

        for(int dots=0;dots<=3 && msgAsString[i]!=',';i++){
            if(isdigit(msgAsString[i])){
                properMessage.destination+=msgAsString[i];
            }
            if( msgAsString[i]=='.'){
                dots++;
                properMessage.destination+=msgAsString[i];
            }
        }
        i++;

        while(msgAsString[i]!=',' && i<msgAsString.size()){
            if(isdigit(msgAsString[i])){
            properMessage.dPort+=msgAsString[i];
            }
            i++;
        }

        messages.push_back(properMessage);
    return properMessage;
}

 QString Analyzer::makeQStringFromMessage(const Message& msg)
 {
     return QString::fromStdString(msg.packet+" source IP: "+msg.source+" source port: "+msg.sPort+" destination IP: "+msg.destination+" destination port: "+msg.dPort);
 }

 std::vector<Message> Analyzer::filterIpAddress(QString IPAddress){
     std::string ip=IPAddress.toStdString();
     std::vector<Message> filteredMessages;
     std::copy_if(messages.begin(),messages.end(),std::back_inserter(filteredMessages),[&ip](const auto& i){
         return i.source==ip || i.destination==ip;
     });
     return filteredMessages;
 }

 std::vector<Message>& Analyzer::getAllMessages()
 {
     return messages;
 }
