#ifndef DRIVERCOMMUNICATION_H
#define DRIVERCOMMUNICATION_H

#include <QDebug>
#include <vector>
#include <string>
#include <map>
#include "DriverSocket.h"

#define CHECK_IF_MODULE_IS_LOADED "lsmod | grep matmoriffer"
#define LOAD_MODULE "sudo insmod matmoriffer.ko"

class DriverCommunication
{
public:
    DriverCommunication(Logger& logger, bool moduleLoaded=true);

    void turn_tcp();
    void turn_udp();
    std::unique_ptr<DriverSocket> reloadMatmorifferParameters();
    bool getCurrentTCP(){return currentStatus.tcp;};
    bool getCurrentUDP(){return currentStatus.udp;};
private:
    struct DriverStatus{
        bool tcp{false};
        bool udp {false};
        bool leavingPackets {false};
        bool incomingPackets {false};
    };

    void loadDriverState();
    void parseDriverData(const std::string& driverData);
    bool checkIfModuleIsLoaded();
    void loadModule();
    bool executeShellCommand(const std::string& ,QString& );

    inline bool tryToExecuteShellCommand(std::function<int()> f, std::string&&, bool&, const bool);
private:
    bool workingWithModule {false};
    std::vector<std::string> internalMessages;
    DriverStatus futureStatus;
    DriverStatus currentStatus;
    Logger& logger;
};

#endif // DRIVERCOMMUNICATION_H
