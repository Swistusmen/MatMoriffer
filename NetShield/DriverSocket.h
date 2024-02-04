#ifndef DRIVERSOCKET_H
#define DRIVERSOCKET_H
#include "Logger.h"
#include "Target.h"
#include <vector>
#include <string>
#include <thread>
#include <functional>
#include "../kernel/lib/libmatmoriffer.h"

#define MESSAGE_LENGTH 100
#define NO_MESSAGES 1000

class DriverSocket{
public:
    DriverSocket();
    ~DriverSocket();

    //My idea: dodac interfers tylko z 1 metoda wirtualna dla obserwatora, po nim ma implementowac InterMessageBroker-> ta metoda ma sprawiac ze DriverSocket
    //bedzie mogl mu wpychac wiadomosci ilekroc je dostanie, przez co InterMessageBroker bedzie mogl je wysylac
    struct ParsingArguments{
        char** rawMessages;
        int* readingIdx;
        int* writingIdx;
        int numberOfBufferedMessages;
        int messageLength;
        Target* target;
    };

    void attachTarget(Target*);

private:
    int continueWork{1};
    int readingIndex {0};
    int writingIndex {0};
    const int maxBufforMessagesCapacity {NO_MESSAGES};
    const int messageLength {MESSAGE_LENGTH};
    char** rawMessages;
    std::vector<std::string> messages;
    std::thread driverCommunicationThread;
    std::thread parsingThread;
    struct NetlinkSocketArguments args;
    struct ParsingArguments parsingArgs;
private:
    void parseMessages(ParsingArguments* );
};

#endif // DRIVERSOCKET_H
