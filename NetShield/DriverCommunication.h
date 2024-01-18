#ifndef DRIVERCOMMUNICATION_H
#define DRIVERCOMMUNICATION_H

#include <QDebug>
#include <vector>
#include <string>

#define CHECK_IF_MODULE_IS_LOADED "lsmod | grep matmoriffer"
#define LOAD_MODULE "sudo insmod matmoriffer.ko"

class DriverCommunication
{
public:
    DriverCommunication(bool moduleLoaded=true);

    void turn_tcp();
    void turn_udp();
    bool reloadMatmorifferParameters();
private:
    struct DriverStatus{
        bool tcp;
        bool udp;
        bool leavingPackets;
        bool incomingPackets;
    };

    bool checkIfModuleIsLoaded();
    void loadModule();
    bool executeShellCommand(const std::string& ,QString& );

    inline bool tryToExecuteShellCommand(int, const std::string&, bool&);
private:
    bool workingWithModule {true};
    std::vector<std::string> internalMessages;
    DriverStatus futureStatus;
    DriverStatus currentStatus;
};

#endif // DRIVERCOMMUNICATION_H
