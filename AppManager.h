#ifndef PROJECT2_APPMANAGER_H
#define PROJECT2_APPMANAGER_H

#include <queue>
#include <array>
#include <algorithm>
#include "InputParser.h"
#include "Counter.h"

struct firstToFly{
    bool operator() (Passenger& lhs, Passenger& rhs)
    {
        if(lhs.getFlightTime()==rhs.getFlightTime()){
            return (lhs.getArrivalTime()>rhs.getArrivalTime());
        }
        return lhs.getFlightTime() > rhs.getFlightTime();
    }
};
struct compara{
    bool operator() (Counter& lhs, Counter& rhs)
    {
        return lhs.getBusyEnd() > rhs.getBusyEnd();
    }
};

class AppManager {
public:
    AppManager(InputParser& givenData, char *argv);
    ~AppManager();
    void run(bool firstToFly, bool vip, bool online);
    void noFF(vector<Counter> luggageCounter, vector<Counter> securityCounter, vector<Passenger> sorted, bool vip, bool online);
    void FF(vector<Counter> luggageCounter, vector<Counter> securityCounter, vector<Passenger> sorted, bool vip, bool online);
private:
    InputParser data;
    list<Passenger> pasList;
    ofstream myfile;
};


#endif //PROJECT2_APPMANAGER_H
