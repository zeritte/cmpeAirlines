#ifndef PROJECT2_APPMANAGER_H
#define PROJECT2_APPMANAGER_H

#include <queue>
#include "InputParser.h"

class AppManager {
public:
    AppManager(InputParser givenData);
    ~AppManager();
    void print();

private:
    priority_queue<Passenger> luggageQueue;
    priority_queue<Passenger> securityQueue;
    InputParser data;
};


#endif //PROJECT2_APPMANAGER_H
