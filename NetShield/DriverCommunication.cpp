#include "DriverCommunication.h"
#include "Common.h"
#include "../kernel/lib/libmatmoriffer.h"
#include "../kernel/lib/libmatmoriffer.c"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <QProcess>

DriverCommunication::DriverCommunication(bool moduleLoaded){
    if(!moduleLoaded){
        return;
    }

    if(!checkIfModuleIsLoaded()){
        loadModule();
        checkIfModuleIsLoaded();
    }
}

bool DriverCommunication::checkIfModuleIsLoaded(){
    QString shellResponse;
    if(!executeShellCommand(CHECK_IF_MODULE_IS_LOADED,shellResponse)){
        internalMessages.emplace_back("Driver not initialized: could not execute shell command");
        return false;
    }
    if(shellResponse==""){
        internalMessages.emplace_back("Driver not initialized: module not found");
        return false;
    }
    return true;
}

void DriverCommunication::loadModule(){
    QString shellResponse;
    internalMessages.emplace_back("Attempting to load module");
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

bool DriverCommunication::reloadMatmorifferParameters()
{
    bool wasTransactionSuccesfull=true;
    wasTransactionSuccesfull=tryToExecuteShellCommand(matmoriffer_turn_tcp(),"Parameter: tcp failed to change",currentStatus.tcp);
    wasTransactionSuccesfull=tryToExecuteShellCommand(matmoriffer_turn_udp(),"Parameter: udp failed to change",currentStatus.udp);

    return wasTransactionSuccesfull;
}

bool DriverCommunication::tryToExecuteShellCommand(int success, const std::string& message, bool& field){
    if(!success){
        internalMessages.emplace_back(message);
        return false;
    }
    field=!field;
    return true;
}
