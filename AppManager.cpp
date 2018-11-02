#include "AppManager.h"

AppManager::AppManager(InputParser givenData) {
    data=givenData;
    pasList=data.getListOfPassenger();
}

void AppManager::run(bool firstToFly, bool vip, bool online) {
    list<Passenger> temp = pasList;
    queue<Passenger> luggageQueue;
    queue<Passenger> securityQueue;
    vector<Counter> luggageCounter;
    vector<Counter> securityCounter;

    for(int i=0; i<data.getNumOfLuggage(); i++){
        luggageCounter.push_back(Counter());
    }
    for(int i=0; i<data.getNumOfSecurity(); i++){
        securityCounter.push_back(Counter());
    }

    for(int i=0; i<data.getNumOfPassenger(); i++){
        luggageQueue.push(temp.front());
        temp.pop_front();
    }

    float sumOfTime=0;
    int numOfMissed=0;

    while(luggageQueue.size()!=0){ // where the queue operation are executed
        Passenger pas = luggageQueue.front();
        bool isdone=false;
        for(int i=0; i<data.getNumOfLuggage(); i++){
            if(luggageCounter[i].getAvailable()<=pas.getArrivalTime()){
                luggageCounter[i].setAvailable(pas.getArrivalTime()+pas.getLuggageTime());
                pas.setTimeSpend(pas.getLuggageTime());
                isdone=true;
                securityQueue.push(pas);
                break;
            }
        }
        if(!isdone){
            int minAvailable = luggageCounter[0].getAvailable();
            int theOne;
            for(int i=0; i<data.getNumOfLuggage(); i++){
                if(luggageCounter[i].getAvailable()<=minAvailable){
                    theOne=i;
                }
            }
            luggageCounter[theOne].setAvailable(luggageCounter[theOne].getAvailable()+pas.getLuggageTime());
            pas.setTimeSpend(luggageCounter[theOne].getAvailable()-pas.getArrivalTime());
            securityQueue.push(pas);
        }
        luggageQueue.pop();
    }

    while(securityQueue.size()!=0){
        Passenger pas = securityQueue.front();
        bool isdone=false;
        for(int i=0; i<data.getNumOfSecurity(); i++){
            if(securityCounter[i].getAvailable()<=pas.getTimeSpend()){
                securityCounter[i].setAvailable(pas.getTimeSpend()+pas.getSecurityTime());
                pas.setTimeSpend(pas.getSecurityTime());
                temp.push_front(pas);
                isdone=true;
                break;
            }
        }
        if(!isdone){
            int minAvailable = securityCounter[0].getAvailable();
            int theOne;
            for(int i=0; i<data.getNumOfSecurity(); i++){
                if(securityCounter[i].getAvailable()<=minAvailable){
                    theOne=i;
                }
            }
            securityCounter[theOne].setAvailable(securityCounter[theOne].getAvailable()+pas.getSecurityTime());
            pas.setTimeSpend(securityCounter[theOne].getAvailable()-pas.getTimeSpend());
            temp.push_front(pas);
        }
        sumOfTime+=(pas.getTimeSpend()-pas.getArrivalTime());
        securityQueue.pop();
    }

   for(int i=0; i<data.getNumOfPassenger(); i++){
       Passenger pas = temp.front();
       if(pas.getTimeSpend()>pas.getFlightTime()){
           numOfMissed++;
       }
       temp.pop_front();
   }

    cout << sumOfTime/data.getNumOfPassenger()<<" "<<numOfMissed;
}

AppManager::~AppManager() {}

