#ifndef PROJECT2_APPMANAGER_H
#define PROJECT2_APPMANAGER_H

#include <queue>
#include "InputParser.h"
#include "Counter.h"

class AppManager {
public:
    AppManager(InputParser givenData, char *argv);
    ~AppManager();
    void run(bool firstToFly, bool vip, bool online);
    void case1(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed);
    void case2(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed);
    void case3(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed);
    void case4(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed);
    void case5(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed);
    void case6(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed);
    void case7(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed);
    void case8(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed);

private:
    InputParser data;
    list<Passenger> pasList;
    ofstream myfile;
};


#endif //PROJECT2_APPMANAGER_H
