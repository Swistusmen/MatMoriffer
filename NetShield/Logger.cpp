#include "Logger.h"

void Logger::loadLogs(){
    isApplicationInitialized=true;
    for(const auto& it: logMessages){
        emit sentLogMessage(QString::fromStdString(it));
    }
}

void Logger::addMessage(std::string && msg){
    logMessages.emplace_back(std::move(msg));
    if(isApplicationInitialized){
        emit sentLogMessage(QString::fromStdString(logMessages.back()));
    }
}
