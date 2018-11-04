#ifndef PROJECT2_APPMANAGER_H
#define PROJECT2_APPMANAGER_H

#include <queue>
#include <algorithm>
#include "InputParser.h"
#include "Counter.h"

struct firstComeFirstServe{
    bool operator() (Passenger& lhs, Passenger& rhs)
    {
        return lhs.getArrivalTime() > rhs.getArrivalTime();
    }
};
struct firstToFly{
    bool operator() (Passenger& lhs, Passenger& rhs)
    {
        return lhs.getFlightTime() > rhs.getFlightTime();
    }
};

class AppManager {
public:
    AppManager(InputParser givenData, char *argv);
    ~AppManager();
    void run(bool firstToFly, bool vip, bool online);
    void case1(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed, vector<Passenger> sorted);
    void case2(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed, vector<Passenger> sorted);
    void case3(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed, vector<Passenger> sorted);
    void case4(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed, vector<Passenger> sorted);
    void case5(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed, vector<Passenger> sorted);
    void case6(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed, vector<Passenger> sorted);
    void case7(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed, vector<Passenger> sorted);
    void case8(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed, vector<Passenger> sorted);

private:
    InputParser data;
    list<Passenger> pasList;
    ofstream myfile;
};


#endif //PROJECT2_APPMANAGER_H
