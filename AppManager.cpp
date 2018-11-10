#include "AppManager.h"

AppManager::AppManager(InputParser& givenData, char *argv) {
    data=givenData;
    myfile.open(argv);
}

void AppManager::run(bool firstToFly, bool vip, bool online) {
    vector<Counter> luggageCounter(data.getNumOfLuggage());
    vector<Counter> securityCounter(data.getNumOfSecurity());

    list<Passenger> temp = data.getListOfPassenger(); // to keep original list safely

    for (int i = 0; i < data.getNumOfPassenger(); i++) { // pushing passengers into sortedList
        temp.push_back(temp.front());
        temp.pop_front();
    }

    temp.sort(); // sorting passengers according to their arrival time

    if(!firstToFly) {
        noFF(luggageCounter,securityCounter,temp, vip, online); // if there is no first-to-fly application
    }
    else{
        FF(luggageCounter,securityCounter,temp, vip, online); // if there is f
    }
}

void AppManager::noFF(vector<Counter> luggageCounter, vector<Counter> securityCounter, list<Passenger> sorted, bool vip, bool online) {
    vector<Passenger> securityQueue;
    vector<Passenger> finalList;
    int sumOfTime=0, numOfMissed=0;

    while (!sorted.empty()) { // where the luggage counter operation is executed
        Passenger pas = sorted.front(); // takes a passenger
        if(pas.stage!=0){
            continue;
        }
        if(online && pas.getTicketType()=='N'){
            if(vip && pas.getUserType()=='V'){
                finalList.push_back(pas);
            }else{
                securityQueue.push_back(pas);
            }
        }
        else{
            bool isdone = false;
            for (int i = 0; i < data.getNumOfLuggage(); i++) { // tries to put him into an available counter
                if (luggageCounter[i].getBusyEnd() <= pas.getArrivalTime()) { // if there is available counter
                    luggageCounter[i].setBusyStart(pas.getArrivalTime());
                    luggageCounter[i].setBusyEnd(pas.getArrivalTime() + pas.getLuggageTime());
                    pas.setTimeSpend(pas.getLuggageTime());
                    isdone = true;
                    if(pas.getUserType() == 'V' && vip) {
                        finalList.push_back(pas);
                    }else{
                        securityQueue.push_back(pas);
                    }
                    break;
                }
            }
            if (!isdone) { // if there is not available counter
                int minAvailable = luggageCounter[0].getBusyEnd();
                int theOne;
                for (int i = 0; i < data.getNumOfLuggage(); i++) { // find nearest available counter
                    if (luggageCounter[i].getBusyEnd() <= minAvailable) {
                        theOne = i;
                        minAvailable = luggageCounter[i].getBusyEnd();
                    }
                }
                // let the time pass
                luggageCounter[theOne].setBusyStart(luggageCounter[theOne].getBusyEnd());
                luggageCounter[theOne].setBusyEnd(luggageCounter[theOne].getBusyStart() + pas.getLuggageTime());
                // puts passenger into counter
                pas.setTimeSpend(luggageCounter[theOne].getBusyEnd() - pas.getArrivalTime());
                if(pas.getUserType() == 'V' && vip) {
                    finalList.push_back(pas);
                }else{
                    securityQueue.push_back(pas);
                }
            }
        }
        sorted.erase(sorted.begin()); // passes to next passenger
    }

    sort(securityQueue.begin(), securityQueue.end());

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
                    minAvailable=securityCounter[i].getBusyEnd();
                }
            }
            securityCounter[theOne].setBusyStart(securityCounter[theOne].getBusyEnd());
            securityCounter[theOne].setBusyEnd(securityCounter[theOne].getBusyStart()+pas.getSecurityTime());
            pas.setTimeSpend(securityCounter[theOne].getBusyEnd() - pas.getTimeSpend());
            finalList.push_back(pas); // puts back to temp
        }
        securityQueue.erase(securityQueue.begin());;
    }

    while(!finalList.empty()){ // searches for missed flights
        Passenger pas = finalList.front();
        sumOfTime += (pas.getTimeSpend() - pas.getArrivalTime());
        if (pas.getTimeSpend() > pas.getFlightTime()) {
            numOfMissed++;
        }
        finalList.erase(finalList.begin());
    }
    cout << "output: "<<sumOfTime*(1.0)/data.getNumOfPassenger()<< " "<<numOfMissed<<endl;
    // output operations
    myfile << sumOfTime*(1.0)/data.getNumOfPassenger()<< " "<<numOfMissed<<endl;
}

void AppManager::FF(vector<Counter> luggageCounter, vector<Counter> securityCounter,list<Passenger> sorted, bool vip, bool online) {
    vector<Passenger> securityQueue;
    vector<Passenger> finalList;
    int sumOfTime=0, numOfMissed=0;
    priority_queue<Passenger, vector<Passenger>, firstToFly> luggagePriorityQueue;
    priority_queue<Passenger, vector<Passenger>, firstToFly> securityPriorityQueue;

    while (!sorted.empty()) { // where the luggage counter operation is executed
        Passenger pas = sorted.front(); // takes a passenger
        if(online && pas.getTicketType()=='N'){
            if(vip && pas.getUserType()=='V'){
                finalList.push_back(pas);
            }
            else{
                securityQueue.push_back(pas);
            }
            sorted.erase(sorted.begin());
        }else {
            bool isdone = false;
            for (int i = 0; i < data.getNumOfLuggage(); i++) { // tries to put him into an available counter
                if (luggageCounter[i].getBusyEnd() <= pas.getArrivalTime()) { // if there is available counter
                    luggageCounter[i].setBusyStart(pas.getArrivalTime());
                    luggageCounter[i].setBusyEnd(pas.getArrivalTime() + pas.getLuggageTime());
                    pas.setTimeSpend(pas.getLuggageTime());
                    isdone = true;
                    if (pas.getUserType() == 'V' && vip) {
                        finalList.push_back(pas);
                    } else {
                        securityQueue.push_back(pas);
                    }
                    sorted.erase(sorted.begin()); // passes to next passenger
                    break;
                }
            }
            if (!isdone) { // if there is not available counter
                priority_queue<Counter, vector<Counter>, compara> sikem;

                for (int i = 0; i < data.getNumOfLuggage(); i++) { // find nearest available counter
                    sikem.push(luggageCounter[i]);
                }
                int theOne;
                Counter anan = sikem.top();
                for (int i = 0; i < data.getNumOfLuggage(); i++) { // find nearest available counter
                    if (luggageCounter[i].getBusyEnd() == anan.getBusyEnd() &&
                        luggageCounter[i].getBusyStart() == anan.getBusyStart()) {
                        theOne = i;
                        break;
                    }
                }

                list<Passenger> temp = sorted;
                while (!temp.empty()) {
                    Passenger tempPas = temp.front();
                    if (tempPas.getArrivalTime() <= luggageCounter[theOne].getBusyEnd()) {
                        luggagePriorityQueue.push(tempPas);
                    }
                    temp.erase(temp.begin());
                }

                Passenger thePas = luggagePriorityQueue.top();
                luggagePriorityQueue = priority_queue<Passenger, vector<Passenger>, firstToFly>();

                // let the time pass
                luggageCounter[theOne].setBusyStart(luggageCounter[theOne].getBusyEnd());
                luggageCounter[theOne].setBusyEnd(luggageCounter[theOne].getBusyStart() + thePas.getLuggageTime());

                // puts passenger into counter
                thePas.setTimeSpend(luggageCounter[theOne].getBusyEnd() - thePas.getArrivalTime());

                if (thePas.getUserType() == 'V' && vip) {
                    finalList.push_back(thePas);
                } else {
                    securityQueue.push_back(thePas);
                }
                sorted.erase(remove(sorted.begin(), sorted.end(), thePas), sorted.end());
            }
        }
    }

    sort(securityQueue.begin(), securityQueue.end());

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
                securityQueue.erase(securityQueue.begin());
                break;
            }
        }
        if (!isdone) { // if there is not available counter
            priority_queue<Counter, vector<Counter>, compara> sikem;

            for (int i = 0; i < data.getNumOfSecurity(); i++) { // find nearest available counter
                sikem.push(securityCounter[i]);
            }
            int theOne;
            Counter anan = sikem.top();
            for (int i = 0; i < data.getNumOfSecurity(); i++) { // find nearest available counter
                if(securityCounter[i].getBusyEnd()==anan.getBusyEnd() && securityCounter[i].getBusyStart()==anan.getBusyStart()){
                    theOne=i;
                    break;
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

            securityQueue.erase(remove(securityQueue.begin(), securityQueue.end(), thePas), securityQueue.end());

        }
    }

    while(!finalList.empty()){ // searches for missed flights
        Passenger pas = finalList.front();
        sumOfTime += (pas.getTimeSpend() - pas.getArrivalTime());
        if (pas.getTimeSpend() > pas.getFlightTime()) {
            numOfMissed++;
        }
        finalList.erase(finalList.begin());
    }

    cout << "output: "<<sumOfTime*(1.0)/data.getNumOfPassenger()<< " "<<numOfMissed<<endl;
    // output operations
    myfile << sumOfTime*(1.0)/data.getNumOfPassenger()<< " "<<numOfMissed<<endl;
}

AppManager::~AppManager() {
    myfile.close();
}

