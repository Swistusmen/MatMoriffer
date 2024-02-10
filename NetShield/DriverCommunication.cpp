#include "DriverCommunication.h"
#include "Common.h"
#include "../kernel/lib/libmatmoriffer.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <QProcess>

DriverCommunication::DriverCommunication(Logger& l, bool moduleLoaded):logger(l){
    if(!moduleLoaded){
        return;
    }

    if(!checkIfModuleIsLoaded()){
        loadModule();
        if(!checkIfModuleIsLoaded()){
            return;
        }
    }
    workingWithModule=true;
    loadDriverState();
}

void DriverCommunication::loadDriverState(){
    char buffer[128];
    std::string result = "";

    FILE* pipe = popen("cat /proc/matmoriffer", "r");

    if (!pipe) {
        logger.addMessage( "Error opening pipe.");
        return;
    }

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    pclose(pipe);
    parseDriverData(result);
}

void DriverCommunication::parseDriverData(const std::string& driverData){
    currentStatus.tcp=driverData.find("TCP")!=std::string::npos;
    currentStatus.udp=driverData.find("UDP")!=std::string::npos;
    futureStatus=currentStatus;
}


bool DriverCommunication::checkIfModuleIsLoaded(){
    QString shellResponse;
    if(!executeShellCommand(CHECK_IF_MODULE_IS_LOADED,shellResponse)){
        logger.addMessage("Driver not initialized: could not execute shell command");
        return false;
    }
    if(shellResponse==""){
        logger.addMessage("Driver not initialized: module not found");
        return false;
    }
    return true;
}

void DriverCommunication::loadModule(){
    QString shellResponse;
    logger.addMessage("Attempting to load module");
    executeShellCommand(LOAD_MODULE,shellResponse);
}

bool DriverCommunication::executeShellCommand(const std::string & command, QString& output){
    std::string shellExecute="pkexec"
                             " "+ command;
    QString program = "/bin/bash";
        QStringList arguments;
        arguments << "-c" << QString::fromStdString(shellExecute);
        QProcess process;
        process.start(program, arguments);
        process.waitForFinished(-1);

        output=process.readAllStandardOutput();

        if (process.exitCode() != 0 || output=="") {
            return false;
        }

        return true;
}

void DriverCommunication::turn_tcp()
{
    futureStatus.tcp=!futureStatus.tcp;
}

void DriverCommunication::turn_udp()
{
    futureStatus.udp=!futureStatus.udp;
}

std::unique_ptr<DriverSocket> DriverCommunication::reloadMatmorifferParameters()
{
    if(workingWithModule){
    bool wasTransactionSuccesfull=true;
    wasTransactionSuccesfull=tryToExecuteShellCommand(matmoriffer_turn_tcp,"Parameter: tcp failed to change",currentStatus.tcp,futureStatus.tcp);
    wasTransactionSuccesfull=tryToExecuteShellCommand(matmoriffer_turn_udp,"Parameter: udp failed to change",currentStatus.udp,futureStatus.udp);

    return std::make_unique<DriverSocket>();
    }else{
        logger.addMessage("Module not present in kernel space, insert it to enable tracking");
        return nullptr;
    }

}

bool DriverCommunication::tryToExecuteShellCommand(std::function<int()> f, std::string&& message, bool& currentField, const bool futureField){
    if(futureField==currentField){
        return true;
    }
    if(!f()){
        logger.addMessage(std::move(message));
        return false;
    }
    currentField=futureField;
    return true;
}
