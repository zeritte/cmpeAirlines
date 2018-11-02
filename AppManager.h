#ifndef PROJECT2_APPMANAGER_H
#define PROJECT2_APPMANAGER_H

#include <queue>
#include "InputParser.h"
#include "Counter.h"

class AppManager {
public:
    AppManager(InputParser givenData);
    ~AppManager();
    void run(bool firstToFly, bool vip, bool online);

private:
    InputParser data;
    list<Passenger> pasList;
};


#endif //PROJECT2_APPMANAGER_H
