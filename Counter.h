#ifndef PROJECT2_COUNTER_H
#define PROJECT2_COUNTER_H

#include "Passenger.h"
#include <string>

class Counter {
public:
    Counter();
    ~Counter();
    int getAvailable();
    void setAvailable(int num);

private:
    int whenAvailable;


};


#endif //PROJECT2_COUNTER_H
