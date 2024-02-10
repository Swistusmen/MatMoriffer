#ifndef LIBMATMORIFFER
#define LIBMATMORIFFER

#include "matmorifferctl.h"

#ifdef __cplusplus
extern "C"
{
#endif
    #define MAX_PAYLOAD 1024

    int matmoriffer_turn_tcp();

    int matmoriffer_turn_udp();

    struct NetlinkSocketArguments{
        int * continueWork;
        char** buffer;
        int bufferLength;
        int messageLenght;
        int* writingIndex;
        int* readngIndex;
    };

    void netlink_socket(struct NetlinkSocketArguments*);

#ifdef __cplusplus
}
#endif

#endif
