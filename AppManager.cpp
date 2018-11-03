#include "AppManager.h"

AppManager::AppManager(InputParser givenData, char *argv) {
    data=givenData;
    pasList=data.getListOfPassenger();
    myfile.open(argv);
}

void AppManager::run(bool firstToFly, bool vip, bool online) {
    vector<Counter> luggageCounter;
    vector<Counter> securityCounter;
    for (int i = 0; i < data.getNumOfLuggage(); i++) { // creating luggage counters
        luggageCounter.emplace_back(Counter());
    }
    for (int i = 0; i < data.getNumOfSecurity(); i++) { // creating security counters
        securityCounter.emplace_back(Counter());
    }
    float sumOfTime = 0;
    int numOfMissed = 0;

    if(!firstToFly && !vip && !online) { // case 1
        case1(luggageCounter,securityCounter,sumOfTime,numOfMissed);
    }
    else if(firstToFly && !vip && !online) {
        case2(luggageCounter,securityCounter,sumOfTime,numOfMissed);
    }
    else if(!firstToFly && vip && !online) {
        case3(luggageCounter,securityCounter,sumOfTime,numOfMissed);
    }
    else if(firstToFly && vip && !online) {
        case4(luggageCounter,securityCounter,sumOfTime,numOfMissed);
    }
    else if(!firstToFly && !vip && online) {
        case5(luggageCounter,securityCounter,sumOfTime,numOfMissed);
    }
    else if(firstToFly && !vip && online) {
        case6(luggageCounter,securityCounter,sumOfTime,numOfMissed);
    }
    else if(!firstToFly && vip && online) {
        case7(luggageCounter,securityCounter,sumOfTime,numOfMissed);
    }
    else if(firstToFly && vip && online) {
        case8(luggageCounter,securityCounter,sumOfTime,numOfMissed);
    }


}

void AppManager::case1(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed) {
    queue<Passenger> luggageQueue;
    queue<Passenger> securityQueue;
    list<Passenger> temp = pasList; // to keep original list safely

    for (int i = 0; i < data.getNumOfPassenger(); i++) { // putting passengers into luggage queue
        luggageQueue.push(temp.front());
        temp.pop_front();
    }
    while (!luggageQueue.empty()) { // where the luggage counter operation is executed
        Passenger pas = luggageQueue.front(); // takes a passenger
        bool isdone = false;

        for (int i = 0; i < data.getNumOfLuggage(); i++) { // tries to put him into an available counter
            if (luggageCounter[i].getBusyEnd() <= pas.getArrivalTime()) { // if there is available counter
                luggageCounter[i].setBusyStart(pas.getArrivalTime());
                luggageCounter[i].setBusyEnd(pas.getArrivalTime() + pas.getLuggageTime());
                pas.setTimeSpend(pas.getLuggageTime());
                isdone = true;
                securityQueue.push(pas); // after puts him into security queue
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
            securityQueue.push(pas); // after puts passenger into security queue
        }
        luggageQueue.pop(); // passes to next passenger
    }

    while (!securityQueue.empty()) { // security counter operations, same with luggage counter
        Passenger pas = securityQueue.front();
        bool isdone = false;
        for (int i = 0; i < data.getNumOfSecurity(); i++) {
            if (securityCounter[i].getBusyEnd() <= pas.getTimeSpend()) { // if there is available counter
                securityCounter[i].setBusyStart(pas.getTimeSpend());
                securityCounter[i].setBusyEnd(pas.getTimeSpend() + pas.getSecurityTime());
                pas.setTimeSpend(pas.getSecurityTime());
                isdone = true;
                temp.push_front(pas); // puts back to temp
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
            temp.push_front(pas); // puts back to temp
        }
        sumOfTime += (pas.getTimeSpend() - pas.getArrivalTime());
        securityQueue.pop();
    }

    for (int i = 0; i < data.getNumOfPassenger(); i++) { // searches for missed flights
        Passenger pas = temp.front();
        if (pas.getTimeSpend() > pas.getFlightTime()) {
            numOfMissed++;
        }
        temp.pop_front();
    }
    cout << "case 1: "<<sumOfTime/data.getNumOfPassenger()<< " "<<numOfMissed<<endl;
    // output operations
    myfile << sumOfTime/data.getNumOfPassenger()<< " "<<numOfMissed<<endl;
}

void AppManager::case2(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed) {
    struct firstToFly{
        bool operator() (Passenger& lhs, Passenger& rhs)
        {
            return lhs.getFlightTime() > rhs.getFlightTime();
        }
    };
    /*
    priority_queue<Passenger, vector<Passenger>, firstToFly> luggageQueue;
    priority_queue<Passenger, vector<Passenger>, firstToFly> securityQueue;
    list<Passenger> temp = pasList; // to keep original list safely



    for (int i = 0; i < data.getNumOfPassenger(); i++) { // putting passengers into luggage queue
        luggageQueue.push(temp.front());
        temp.pop_front();
    }

    while (!luggageQueue.empty()) { // where the luggage counter operation is executed
        Passenger pas = luggageQueue.top(); // takes a passenger
        bool isdone = false;
        for (int i = 0; i < data.getNumOfLuggage(); i++) { // tries to put him into an available counter
            if (luggageCounter[i].getAvailable() <= pas.getArrivalTime()) { // if there is available counter
                cout << "passenger arrival time:" << pas.getArrivalTime() << " counter available time:"<<luggageCounter[i].getAvailable()<<endl;
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
            cout << "passenger arrival time:" << pas.getArrivalTime() << " counter available time:"<<luggageCounter[theOne].getAvailable()<<endl;
            securityQueue.push(pas); // after puts passenger into security queue
        }
        luggageQueue.pop(); // passes to next passenger
    }

    priority_queue <Passenger, vector<Passenger>, firstToFly> g = securityQueue;
    while (!g.empty())
    {
        Passenger a = g.top();
        cout << a.getArrivalTime();
        g.pop();cout << '\n';
    }cout<< "ended"<<endl;

    while (!securityQueue.empty()) { // security counter operations, same with luggage counter
        Passenger pas = securityQueue.top();
        bool isdone = false;
        for (int i = 0; i < data.getNumOfSecurity(); i++) {
            if (securityCounter[i].getAvailable() <= pas.getTimeSpend()) {
                securityCounter[i].setAvailable(pas.getTimeSpend() + pas.getSecurityTime());
                pas.setTimeSpend(pas.getSecurityTime());
                temp.push_front(pas); // puts back to temp
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
            temp.push_front(pas); // puts back to temp
        }
        sumOfTime += (pas.getTimeSpend() - pas.getArrivalTime());
        securityQueue.pop();
    }

    for (int i = 0; i < data.getNumOfPassenger(); i++) { // searches for missed flights
        Passenger pas = temp.front();
        cout << "flight: "<<pas.getFlightTime()<<" time spend: "<<pas.getTimeSpend()<<endl;
        if (pas.getTimeSpend() > pas.getFlightTime()) {
            numOfMissed++;
        }
        temp.pop_front();
    }
    cout << "case 2: "<<sumOfTime/data.getNumOfPassenger()<< " "<<numOfMissed<<endl;
    // output operations
    myfile << sumOfTime/data.getNumOfPassenger()<< " "<<numOfMissed<<endl; */
}


void AppManager::case3(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed) {

}

void AppManager::case4(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed) {

}

void AppManager::case5(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed) {

}

void AppManager::case6(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed) {

}

void AppManager::case7(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed) {

}

void AppManager::case8(vector<Counter> luggageCounter, vector<Counter> securityCounter, float sumOfTime, int numOfMissed) {

}



AppManager::~AppManager() {
    myfile.close();
}

