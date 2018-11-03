#ifndef PROJECT2_COUNTER_H
#define PROJECT2_COUNTER_H

#include "Passenger.h"
#include <string>

class Counter {
public:
    Counter();
    ~Counter();
    int getBusyStart();
    void setBusyStart(int num);
    int getBusyEnd();
    void setBusyEnd(int num);

private:
    int busyStart;
    int busyEnd;
};


#endif //PROJECT2_COUNTER_H
