#ifndef TARGET_H
#define TARGET_H
#include <iostream>

class Target{
public:
    Target()=default;
    virtual ~Target(){};
    virtual void absorbMessage(char*) =0;
};

#endif // TARGET_H
