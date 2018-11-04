#include "AppManager.h"

AppManager::AppManager(InputParser givenData, char *argv) {
    data=givenData;
    pasList=data.getListOfPassenger();
    myfile.open(argv);
}

bool timeSpendSorted(Passenger& lhs, Passenger& rhs) {
    return lhs.getTimeSpend() < rhs.getTimeSpend();
}

void AppManager::run(bool firstToFly, bool vip, bool online) {
    vector<Counter> luggageCounter;
    vector<Counter> securityCounter;
    vector<Passenger> sortedList;

    list<Passenger> temp = pasList; // to keep original list safely
    priority_queue<Passenger, vector<Passenger>, firstComeFirstServe> arrivalTimeSorter;

    for (int i = 0; i < data.getNumOfPassenger(); i++) { // sorting passengers in the order of their arrival
        arrivalTimeSorter.push(temp.front());
        temp.pop_front();
    }
    while(!arrivalTimeSorter.empty()){ // creating
        sortedList.push_back(arrivalTimeSorter.top());
        arrivalTimeSorter.pop();
    }
    for (int i = 0; i < data.getNumOfLuggage(); i++) { // creating luggage counters
        luggageCounter.emplace_back(Counter());
    }
    for (int i = 0; i < data.getNumOfSecurity(); i++) { // creating security counters
        securityCounter.emplace_back(Counter());
    }
    float sumOfTime = 0;
    int numOfMissed = 0;

    if(!firstToFly && !vip && !online) { // case 1
        case1(luggageCounter,securityCounter,sumOfTime,numOfMissed,sortedList);
    }
    else if(firstToFly && !vip && !online) {
        case2(luggageCounter,securityCounter,sumOfTime,numOfMissed,sortedList);
    }
    else if(!firstToFly && vip && !online) {
        case3(luggageCounter,securityCounter,sumOfTime,numOfMissed,sortedList);
    }
    else if(firstToFly && vip && !online) {
        case4(luggageCounter,securityCounter,sumOfTime,numOfMissed,sortedList);
    }
    else if(!firstToFly && !vip && online) {
        case5(luggageCounter,securityCounter,sumOfTime,numOfMissed,sortedList);
    }
    else if(firstToFly && !vip && online) {
        case6(luggageCounter,securityCounter,sumOfTime,numOfMissed,sortedList);
    }
    else if(!firstToFly && vip && online) {
        case7(luggageCounter,securityCounter,sumOfTime,numOfMissed,sortedList);
    }
    else if(firstToFly && vip && online) {
        case8(luggageCounter,securityCounter,sumOfTime,numOfMissed,sortedList);
    }
}



void AppManager::case1(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed, vector<Passenger> sorted) {
    vector<Passenger> securityQueue;
    vector<Passenger> finalList;

    while (!sorted.empty()) { // where the luggage counter operation is executed
        Passenger pas = sorted.front(); // takes a passenger
        bool isdone = false;
        for (int i = 0; i < data.getNumOfLuggage(); i++) { // tries to put him into an available counter
            if (luggageCounter[i].getBusyEnd() <= pas.getArrivalTime()) { // if there is available counter
                luggageCounter[i].setBusyStart(pas.getArrivalTime());
                luggageCounter[i].setBusyEnd(pas.getArrivalTime() + pas.getLuggageTime());
                pas.setTimeSpend(pas.getLuggageTime());
                isdone = true;
                securityQueue.push_back(pas); // after puts him into security queue
                break;
            }
        }
        if (!isdone) { // if there is not available counter
            int minAvailable = luggageCounter[0].getBusyEnd();
            int theOne;
            for (int i = 0; i < data.getNumOfLuggage(); i++) { // find nearest available counter
                if (luggageCounter[i].getBusyEnd() <= minAvailable) {
                    theOne = i;
                }
            }
            // let the time pass
            luggageCounter[theOne].setBusyStart(luggageCounter[theOne].getBusyEnd());
            luggageCounter[theOne].setBusyEnd(luggageCounter[theOne].getBusyStart()+pas.getLuggageTime());
            // puts passenger into counter
            pas.setTimeSpend(luggageCounter[theOne].getBusyEnd() - pas.getArrivalTime());
            securityQueue.push_back(pas); // after puts passenger into security queue
        }
        sorted.erase(sorted.begin()); // passes to next passenger
    }

    sort(securityQueue.begin(), securityQueue.end(), timeSpendSorted);

    while (!securityQueue.empty()) { // security counter operations, same with luggage counter
        Passenger pas = securityQueue.front();
        bool isdone = false;
        for (int i = 0; i < data.getNumOfSecurity(); i++) {
            if (securityCounter[i].getBusyEnd() <= pas.getTimeSpend()) { // if there is available counter
                securityCounter[i].setBusyStart(pas.getTimeSpend());
                securityCounter[i].setBusyEnd(pas.getTimeSpend() + pas.getSecurityTime());
                pas.setTimeSpend(pas.getSecurityTime());
                isdone = true;
                finalList.push_back(pas); // puts to final list
                break;
            }
        }
        if (!isdone) { // if there is not available counter
            int minAvailable = securityCounter[0].getBusyEnd();
            int theOne;
            for (int i = 0; i < data.getNumOfSecurity(); i++) {
                if (securityCounter[i].getBusyEnd() <= minAvailable) {
                    theOne = i;
                }
            }
            securityCounter[theOne].setBusyStart(securityCounter[theOne].getBusyEnd());
            securityCounter[theOne].setBusyEnd(securityCounter[theOne].getBusyStart()+pas.getSecurityTime());
            pas.setTimeSpend(securityCounter[theOne].getBusyEnd() - pas.getTimeSpend());
            finalList.push_back(pas); // puts back to temp
        }
        sumOfTime += (pas.getTimeSpend() - pas.getArrivalTime());
        securityQueue.erase(securityQueue.begin());;
    }

    for (int i = 0; i < data.getNumOfPassenger(); i++) { // searches for missed flights
        Passenger pas = finalList.front();
        if (pas.getTimeSpend() > pas.getFlightTime()) {
            numOfMissed++;
        }
        finalList.erase(finalList.begin());
    }
    cout << "case 1: "<<sumOfTime/data.getNumOfPassenger()<< " "<<numOfMissed<<endl;
    // output operations
    myfile << sumOfTime/data.getNumOfPassenger()<< " "<<numOfMissed<<endl;
}


void AppManager::case2(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed,vector<Passenger> sorted) {
    vector<Passenger> securityQueue;
    vector<Passenger> finalList;
    priority_queue<Passenger, vector<Passenger>, firstToFly> luggagePriorityQueue;
    priority_queue<Passenger, vector<Passenger>, firstToFly> securityPriorityQueue;

    while (!sorted.empty()) { // where the luggage counter operation is executed
        Passenger pas = sorted.front(); // takes a passenger
        bool isdone = false;
        for (int i = 0; i < data.getNumOfLuggage(); i++) { // tries to put him into an available counter
            if (luggageCounter[i].getBusyEnd() <= pas.getArrivalTime()) { // if there is available counter
                luggageCounter[i].setBusyStart(pas.getArrivalTime());
                luggageCounter[i].setBusyEnd(pas.getArrivalTime() + pas.getLuggageTime());
                pas.setTimeSpend(pas.getLuggageTime());
                isdone = true;
                securityQueue.push_back(pas); // after puts him into security queue
                sorted.erase(sorted.begin()); // passes to next passenger
                break;
            }
        }
        if (!isdone) { // if there is not available counter
            int minAvailable = luggageCounter[0].getBusyEnd();
            int theOne;
            for (int i = 0; i < data.getNumOfLuggage(); i++) { // find nearest available counter
                if (luggageCounter[i].getBusyEnd() <= minAvailable) {
                    theOne = i;
                }
            }

            vector<Passenger> temp=sorted;
            while(!temp.empty()){
                Passenger tempPas = temp.front();
                if(tempPas.getArrivalTime()<=luggageCounter[theOne].getBusyEnd()){
                    luggagePriorityQueue.push(tempPas);
                }
                temp.erase(temp.begin());
            }

            Passenger thePas = luggagePriorityQueue.top();
            luggagePriorityQueue = priority_queue<Passenger, vector<Passenger>, firstToFly>();

            // let the time pass
            luggageCounter[theOne].setBusyStart(luggageCounter[theOne].getBusyEnd());
            luggageCounter[theOne].setBusyEnd(luggageCounter[theOne].getBusyStart()+thePas.getLuggageTime());

            // puts passenger into counter
            thePas.setTimeSpend(luggageCounter[theOne].getBusyEnd() - thePas.getArrivalTime());
            securityQueue.push_back(thePas); // after puts passenger into security queue

            sorted.erase(remove(sorted.begin(), sorted.end(), thePas), sorted.end());
        }
    }

    sort(securityQueue.begin(), securityQueue.begin()+data.getNumOfSecurity(), timeSpendSorted);

    while (!securityQueue.empty()) { // security counter operations, same with luggage counter
        Passenger pas = securityQueue.front();
        bool isdone = false;
        for (int i = 0; i < data.getNumOfSecurity(); i++) {
            if (securityCounter[i].getBusyEnd() <= pas.getTimeSpend()) { // if there is available counter
                securityCounter[i].setBusyStart(pas.getTimeSpend());
                securityCounter[i].setBusyEnd(pas.getTimeSpend() + pas.getSecurityTime());
                pas.setTimeSpend(pas.getSecurityTime());
                isdone = true;
                sumOfTime += (pas.getTimeSpend() - pas.getArrivalTime());
                finalList.push_back(pas); // puts to final list
                securityQueue.erase(securityQueue.begin());
                break;
            }
        }
        if (!isdone) { // if there is not available counter
            int minAvailable = securityCounter[0].getBusyEnd();
            int theOne;
            for (int i = 0; i < data.getNumOfSecurity(); i++) { // find nearest available counter
                if (securityCounter[i].getBusyEnd() <= minAvailable) {
                    theOne = i;
                }
            }

            vector<Passenger> temp=securityQueue;
            while(!temp.empty()){
                Passenger tempPas = temp.front();
                if(tempPas.getArrivalTime()<=securityCounter[theOne].getBusyEnd()){
                    securityPriorityQueue.push(tempPas);
                }
                temp.erase(temp.begin());
            }

            Passenger thePas = securityPriorityQueue.top();
            securityPriorityQueue = priority_queue<Passenger, vector<Passenger>, firstToFly>();

            // let the time pass
            securityCounter[theOne].setBusyStart(securityCounter[theOne].getBusyEnd());
            securityCounter[theOne].setBusyEnd(securityCounter[theOne].getBusyStart()+thePas.getSecurityTime());

            // puts passenger into counter
            thePas.setTimeSpend(securityCounter[theOne].getBusyEnd() - thePas.getTimeSpend());
            finalList.push_back(thePas); // puts to final list

            // sum of time operations
            sumOfTime += (thePas.getTimeSpend() - thePas.getArrivalTime());

            securityQueue.erase(remove(securityQueue.begin(), securityQueue.end(), thePas), securityQueue.end());

        }
    }

    while(!finalList.empty()){ // searches for missed flights
        Passenger pas = finalList.front();
        if (pas.getTimeSpend() > pas.getFlightTime()) {
            numOfMissed++;
        }
        finalList.erase(finalList.begin());
    }

    cout << "case 2: "<<sumOfTime/data.getNumOfPassenger()<< " "<<numOfMissed<<endl;
    // output operations
    myfile << sumOfTime/data.getNumOfPassenger()<< " "<<numOfMissed<<endl;
}


void AppManager::case3(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed, vector<Passenger> sorted) {

}



void AppManager::case4(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed, vector<Passenger> sorted) {

}



void AppManager::case5(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed, vector<Passenger> sorted) {

}



void AppManager::case6(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed, vector<Passenger> sorted) {

}



void AppManager::case7(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed, vector<Passenger> sorted) {

}



void AppManager::case8(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed, vector<Passenger> sorted) {

}





AppManager::~AppManager() {
    myfile.close();
}

