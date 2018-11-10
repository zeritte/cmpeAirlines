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
            return lhs.getArrivalTime()>rhs.getArrivalTime();
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
    void case1(vector<Counter> luggageCounter, vector<Counter> securityCounter, vector<Passenger> sorted);
    void case2(vector<Counter> luggageCounter, vector<Counter> securityCounter, vector<Passenger> sorted);
    void case3(vector<Counter> luggageCounter, vector<Counter> securityCounter, vector<Passenger> sorted);
    void case4(vector<Counter> luggageCounter, vector<Counter> securityCounter, vector<Passenger> sorted);
    void case5(vector<Counter> luggageCounter, vector<Counter> securityCounter, vector<Passenger> sorted);
    void case6(vector<Counter> luggageCounter, vector<Counter> securityCounter, vector<Passenger> sorted);
    void case7(vector<Counter> luggageCounter, vector<Counter> securityCounter, vector<Passenger> sorted);
    void case8(vector<Counter> luggageCounter, vector<Counter> securityCounter, vector<Passenger> sorted);

private:
    InputParser data;
    list<Passenger> pasList;
    ofstream myfile;
};


#endif //PROJECT2_APPMANAGER_H
