#include "AppManager.h"

AppManager::AppManager(InputParser givenData) {
    data=givenData;
    pasList=data.getListOfPassenger();
}

void AppManager::run(bool firstToFly, bool vip, bool online) {
    // list<Passenger> pasList = pasList;
    vector<Counter> luggageCounter;
    vector<Counter> securityCounter;
    float sumOfTime = 0;
    int numOfMissed = 0;

    if(!firstToFly && !vip && !online) {
        queue<Passenger> luggageQueue;
        queue<Passenger> securityQueue;

        for (int i = 0; i < data.getNumOfLuggage(); i++) { // creating luggage counters
            luggageCounter.emplace_back(Counter());
        }
        for (int i = 0; i < data.getNumOfSecurity(); i++) { // creating security counters
            securityCounter.emplace_back(Counter());
        }

        for (int i = 0; i < data.getNumOfPassenger(); i++) { // putting passengers into luggage queue
            luggageQueue.push(pasList.front());
            pasList.pop_front();
        }

        while (!luggageQueue.empty()) { // where the luggage counter operation is executed
            Passenger pas = luggageQueue.front(); // takes a passenger
            bool isdone = false;
            for (int i = 0; i < data.getNumOfLuggage(); i++) { // tries to put him into an available counter
                if (luggageCounter[i].getAvailable() <= pas.getArrivalTime()) { // if there is available counter
                    luggageCounter[i].setAvailable(pas.getArrivalTime() + pas.getLuggageTime());
                    pas.setTimeSpend(pas.getLuggageTime());
                    isdone = true;
                    securityQueue.push(pas); // after puts him into security queue
                    break;
                }
            }
            if (!isdone) { // if there is not
                int minAvailable = luggageCounter[0].getAvailable();
                int theOne;
                for (int i = 0; i < data.getNumOfLuggage(); i++) { // find nearest available counter
                    if (luggageCounter[i].getAvailable() <= minAvailable) {
                        theOne = i;
                    }
                }
                // let the time pass
                luggageCounter[theOne].setAvailable(luggageCounter[theOne].getAvailable() + pas.getLuggageTime());
                // puts passenger into counter
                pas.setTimeSpend(luggageCounter[theOne].getAvailable() - pas.getArrivalTime());
                securityQueue.push(pas); // after puts passenger into security queue
            }
            luggageQueue.pop(); // passes to next passenger
        }

        while (!securityQueue.empty()) { // security counter operations, same with luggage counter
            Passenger pas = securityQueue.front();
            bool isdone = false;
            for (int i = 0; i < data.getNumOfSecurity(); i++) {
                if (securityCounter[i].getAvailable() <= pas.getTimeSpend()) {
                    securityCounter[i].setAvailable(pas.getTimeSpend() + pas.getSecurityTime());
                    pas.setTimeSpend(pas.getSecurityTime());
                    pasList.push_front(pas); // puts back to pasList
                    isdone = true;
                    break;
                }
            }
            if (!isdone) {
                int minAvailable = securityCounter[0].getAvailable();
                int theOne;
                for (int i = 0; i < data.getNumOfSecurity(); i++) {
                    if (securityCounter[i].getAvailable() <= minAvailable) {
                        theOne = i;
                    }
                }
                securityCounter[theOne].setAvailable(securityCounter[theOne].getAvailable() + pas.getSecurityTime());
                pas.setTimeSpend(securityCounter[theOne].getAvailable() - pas.getTimeSpend());
                pasList.push_front(pas); // puts back to pasList
            }
            sumOfTime += (pas.getTimeSpend() - pas.getArrivalTime());
            securityQueue.pop();
        }

        for (int i = 0; i < data.getNumOfPassenger(); i++) { // searchs for missed flights
            Passenger pas = pasList.front();
            if (pas.getTimeSpend() > pas.getFlightTime()) {
                numOfMissed++;
            }
            pasList.pop_front();
        }

        cout << sumOfTime / data.getNumOfPassenger() << " " << numOfMissed << endl;
    }
}

AppManager::~AppManager() {}

