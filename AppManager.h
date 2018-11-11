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
struct comparator{
    bool operator() (Passenger& lhs, Passenger& rhs)
    {
        if(lhs.getTotalTime()!=rhs.getTotalTime()){
            return lhs.getTotalTime()>rhs.getTotalTime();
        }
        else if(lhs.stage!=rhs.stage){
            return lhs.stage<rhs.stage;
        }
        else{
            return lhs.getArrivalTime()>rhs.getArrivalTime();
        }
    }
};

class AppManager {
public:
    AppManager(InputParser& givenData, char *argv);
    ~AppManager();
    void run(bool firstToFly, bool vip, bool online);
    void maintenance(priority_queue<Passenger, vector<Passenger>, comparator>& pas_queue, bool firstToFly, bool vip, bool online);
private:
    InputParser data;
    priority_queue<Passenger, vector<Passenger>, comparator> pas_q;
    ofstream myfile;
};


#endif //PROJECT2_APPMANAGER_H
