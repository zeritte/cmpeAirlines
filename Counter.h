#ifndef PROJECT2_COUNTER_H
#define PROJECT2_COUNTER_H

#include "Passenger.h"
#include <string>

class Counter {
public:
    Counter();
    ~Counter();
    void makeBusy();
    void notBusy();
    bool isBusy();

private:
    bool busy;
};


#endif //PROJECT2_COUNTER_H
