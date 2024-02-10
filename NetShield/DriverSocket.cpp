#include "DriverSocket.h"
#include "../kernel/lib/libmatmoriffer.h"
#include "../kernel/lib/libmatmoriffer.c"

DriverSocket::DriverSocket(){
    rawMessages=new char*[maxBufforMessagesCapacity];
    for(int i=0;i<maxBufforMessagesCapacity;i++){
        rawMessages[i]=new char[messageLength];
    }

    args={&continueWork,rawMessages,maxBufforMessagesCapacity,messageLength,&writingIndex,&readingIndex};
    driverCommunicationThread=std::thread(netlink_socket,&args);
}

DriverSocket::~DriverSocket(){
    continueWork=0;
    driverCommunicationThread.join();
    parsingThread.join();
}

void DriverSocket::attachTarget(Target* toObserve){
    parsingArgs.readingIdx=&readingIndex;
    parsingArgs.messageLength=messageLength;
    parsingArgs.target=toObserve;
    parsingArgs.numberOfBufferedMessages=maxBufforMessagesCapacity;
    parsingArgs.rawMessages=rawMessages;
    parsingArgs.writingIdx=&writingIndex;
    parsingThread=std::thread(std::bind(&DriverSocket::parseMessages,this,&parsingArgs));
}

void DriverSocket::parseMessages(ParsingArguments* p){

    while(continueWork){
        int end=*p->writingIdx;
        if(end==*p->readingIdx){
            continue;
        }
        end = end< *p->readingIdx ? p->numberOfBufferedMessages :end;
        for(;*p->readingIdx<end; (*p->readingIdx)++){
            p->target->absorbMessage(p->rawMessages[*p->readingIdx]);
        }
        if(*p->readingIdx==p->numberOfBufferedMessages){
            *p->readingIdx=0;
        }
    }
}
