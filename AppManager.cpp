#include "AppManager.h"

AppManager::AppManager(InputParser givenData) {
    data=givenData;
}

AppManager::~AppManager() {}

void AppManager::print() {
    list<Passenger> p = data.getListOfPassenger();
    for(int i=0; i<data.getNumOfPassenger(); i++){
        cout<< p.front().getArrivalTime()<<endl;
        p.pop_front();
    }
}
