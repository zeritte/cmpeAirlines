#ifndef PROJECT2_APPMANAGER_H
#define PROJECT2_APPMANAGER_H

#include <queue>
#include <array>
#include <algorithm>
#include "InputParser.h"
#include "Counter.h"

struct firstToFly{ // sorts according to passengers' flight time, if equal sorts according to arrival time
    bool operator() (Passenger& lhs, Passenger& rhs)
    {
        if(lhs.getFlightTime()==rhs.getFlightTime()){
            return lhs.getArrivalTime()>rhs.getArrivalTime();
        }
        return lhs.getFlightTime() > rhs.getFlightTime();
    }
};
struct comparator{ // sorts according to passengers' total time, if equal according to their stage(which operation they are in), if equal according to their arrival time.
    bool operator() (Passenger& lhs, Passenger& rhs)
    {
        if(lhs.getTotalTime()==rhs.getTotalTime()){
            if(lhs.step==rhs.step){
                return lhs.getArrivalTime()>rhs.getArrivalTime();
            }
            return lhs.step<rhs.step;
        }
        return lhs.getTotalTime()>rhs.getTotalTime();
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
    priority_queue<Passenger, vector<Passenger>, comparator> pas_q; // base priority queue
    ofstream myfile;
};


#endif //PROJECT2_APPMANAGER_H
