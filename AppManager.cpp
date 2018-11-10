#include "AppManager.h"

AppManager::AppManager(InputParser& givenData, char *argv) {
    data=givenData;
    pasList=data.getListOfPassenger();
    myfile.open(argv);
}

void AppManager::run(bool firstToFly, bool vip, bool online) {
    vector<Counter> luggageCounter;
    vector<Counter> securityCounter;
    vector<Passenger> sortedList;

    list<Passenger> temp = pasList; // to keep original list safely

    for (int i = 0; i < data.getNumOfPassenger(); i++) { // pushing passengers into sortedList
        sortedList.push_back(temp.front());
        temp.pop_front();
    }

    sort(sortedList.begin(), sortedList.end()); // sorting passengers according to their arrival time

    for (int i = 0; i < data.getNumOfLuggage(); i++) { // creating luggage counters
        luggageCounter.emplace_back(Counter());
    }
    for (int i = 0; i < data.getNumOfSecurity(); i++) { // creating security counters
        securityCounter.emplace_back(Counter());
    }

    if(!firstToFly && !vip && !online) { // case 1
        case1(luggageCounter,securityCounter,sortedList);
    }
    else if(firstToFly && !vip && !online) {
        case2(luggageCounter,securityCounter,sortedList);
    }
    else if(!firstToFly && vip && !online) {
        case3(luggageCounter,securityCounter,sortedList);
    }
    else if(firstToFly && vip && !online) {
        case4(luggageCounter,securityCounter,sortedList);
    }
    else if(!firstToFly && !vip && online) {
        case5(luggageCounter,securityCounter,sortedList);
    }
    else if(firstToFly && !vip && online) {
        case6(luggageCounter,securityCounter,sortedList);
    }
    else if(!firstToFly && vip && online) {
        case7(luggageCounter,securityCounter,sortedList);
    }
    else if(firstToFly && vip && online) {
        case8(luggageCounter,securityCounter,sortedList);
    }
}

void AppManager::case1(vector<Counter> luggageCounter, vector<Counter> securityCounter, vector<Passenger> sorted) {
    vector<Passenger> securityQueue;
    vector<Passenger> finalList;
    int sumOfTime=0, numOfMissed=0;

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
                    minAvailable=luggageCounter[i].getBusyEnd();
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
    cout << "case 1: "<<sumOfTime*(1.0)/data.getNumOfPassenger()<< " "<<numOfMissed<<endl;
    // output operations
    myfile << sumOfTime*(1.0)/data.getNumOfPassenger()<< " "<<numOfMissed<<endl;
}

void AppManager::case2(vector<Counter> luggageCounter, vector<Counter> securityCounter,vector<Passenger> sorted) {
    vector<Passenger> securityQueue;
    vector<Passenger> finalList;
    int sumOfTime=0, numOfMissed=0;
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
            priority_queue<Counter, vector<Counter>, compara> sikem;

            for (int i = 0; i < data.getNumOfLuggage(); i++) { // find nearest available counter
                sikem.push(luggageCounter[i]);
            }
            int theOne;
            Counter anan = sikem.top();
            for (int i = 0; i < data.getNumOfLuggage(); i++) { // find nearest available counter
                if(luggageCounter[i].getBusyEnd()==anan.getBusyEnd() && luggageCounter[i].getBusyStart()==anan.getBusyStart()){
                    theOne=i;
                    break;
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

    cout << "case 2: "<<sumOfTime*(1.0)/data.getNumOfPassenger()<< " "<<numOfMissed<<endl;
    // output operations
    myfile << sumOfTime*(1.0)/data.getNumOfPassenger()<< " "<<numOfMissed<<endl;
}

void AppManager::case3(vector<Counter> luggageCounter, vector<Counter> securityCounter, vector<Passenger> sorted) {
    vector<Passenger> securityQueue;
    vector<Passenger> finalList;
    int sumOfTime=0, numOfMissed=0;

    while (!sorted.empty()) { // where the luggage counter operation is executed
        Passenger pas = sorted.front(); // takes a passenger
        bool isdone = false;
        for (int i = 0; i < data.getNumOfLuggage(); i++) { // tries to put him into an available counter
            if (luggageCounter[i].getBusyEnd() <= pas.getArrivalTime()) { // if there is available counter
                luggageCounter[i].setBusyStart(pas.getArrivalTime());
                luggageCounter[i].setBusyEnd(pas.getArrivalTime() + pas.getLuggageTime());
                pas.setTimeSpend(pas.getLuggageTime());
                isdone = true;

                if(pas.getUserType()!='V'){ // not a vip
                    securityQueue.push_back(pas); // after puts him into security queue
                }else{
                    finalList.push_back(pas);
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
                    minAvailable=luggageCounter[i].getBusyEnd();
                }
            }
            // let the time pass
            luggageCounter[theOne].setBusyStart(luggageCounter[theOne].getBusyEnd());
            luggageCounter[theOne].setBusyEnd(luggageCounter[theOne].getBusyStart()+pas.getLuggageTime());
            // puts passenger into counter
            pas.setTimeSpend(luggageCounter[theOne].getBusyEnd() - pas.getArrivalTime());

            if(pas.getUserType()!='V'){ // not a vip
                securityQueue.push_back(pas); // after puts him into security queue
            }else{
                finalList.push_back(pas);
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
    cout << "case 3: "<<sumOfTime*(1.0)/data.getNumOfPassenger()<< " "<<numOfMissed<<endl;
    // output operations
    myfile << sumOfTime*(1.0)/data.getNumOfPassenger()<< " "<<numOfMissed<<endl;
}

void AppManager::case4(vector<Counter> luggageCounter, vector<Counter> securityCounter, vector<Passenger> sorted) {
    vector<Passenger> securityQueue;
    vector<Passenger> finalList;
    int sumOfTime=0, numOfMissed=0;
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

                if(pas.getUserType()!='V'){ // not a vip
                    securityQueue.push_back(pas); // after puts him into security queue
                }else{
                    finalList.push_back(pas);
                }

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
                    minAvailable=luggageCounter[i].getBusyEnd();
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

            if(pas.getUserType()!='V'){ // not a vip
                securityQueue.push_back(thePas); // after puts him into security queue
            }else{
                finalList.push_back(thePas);
            }

            sorted.erase(remove(sorted.begin(), sorted.end(), thePas), sorted.end());
        }
    }

    sort(securityQueue.begin(), securityQueue.begin()+data.getNumOfSecurity());

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
            int minAvailable = securityCounter[0].getBusyEnd();
            int theOne;
            for (int i = 0; i < data.getNumOfSecurity(); i++) { // find nearest available counter
                if (securityCounter[i].getBusyEnd() <= minAvailable) {
                    theOne = i;
                    minAvailable=securityCounter[i].getBusyEnd();
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

    cout << "case 4: "<<sumOfTime*(1.0)/data.getNumOfPassenger()<< " "<<numOfMissed<<endl;
    // output operations
    myfile << sumOfTime*(1.0)/data.getNumOfPassenger()<< " "<<numOfMissed<<endl;
}

void AppManager::case5(vector<Counter> luggageCounter, vector<Counter> securityCounter, vector<Passenger> sorted) {
    vector<Passenger> securityQueue;
    vector<Passenger> finalList;
    int sumOfTime=0, numOfMissed=0;

    while (!sorted.empty()) { // where the luggage counter operation is executed
        Passenger pas = sorted.front(); // takes a passenger
        if(pas.getTicketType()=='L'){
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
                        minAvailable=luggageCounter[i].getBusyEnd();
                    }
                }
                // let the time pass
                luggageCounter[theOne].setBusyStart(luggageCounter[theOne].getBusyEnd());
                luggageCounter[theOne].setBusyEnd(luggageCounter[theOne].getBusyStart()+pas.getLuggageTime());
                // puts passenger into counter
                pas.setTimeSpend(luggageCounter[theOne].getBusyEnd() - pas.getArrivalTime());
                securityQueue.push_back(pas); // after puts passenger into security queue
            }
        }else{
            securityQueue.push_back(pas);
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
    cout << "case 5: "<<sumOfTime*(1.0)/data.getNumOfPassenger()<< " "<<numOfMissed<<endl;
    // output operations
    myfile << sumOfTime*(1.0)/data.getNumOfPassenger()<< " "<<numOfMissed<<endl;
}

void AppManager::case6(vector<Counter> luggageCounter, vector<Counter> securityCounter, vector<Passenger> sorted) {
    vector<Passenger> securityQueue;
    vector<Passenger> finalList;
    priority_queue<Passenger, vector<Passenger>, firstToFly> luggagePriorityQueue;
    priority_queue<Passenger, vector<Passenger>, firstToFly> securityPriorityQueue;
    int sumOfTime=0, numOfMissed=0;

    while (!sorted.empty()) { // where the luggage counter operation is executed
        Passenger pas = sorted.front(); // takes a passenger
        if(pas.getTicketType()=='L'){
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
                        minAvailable=luggageCounter[i].getBusyEnd();
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
        }else{
            securityQueue.push_back(pas);
            sorted.erase(sorted.begin());
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
            int minAvailable = securityCounter[0].getBusyEnd();
            int theOne;
            for (int i = 0; i < data.getNumOfSecurity(); i++) { // find nearest available counter
                if (securityCounter[i].getBusyEnd() <= minAvailable) {
                    theOne = i;
                    minAvailable=securityCounter[i].getBusyEnd();
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

    cout << "case 6: "<<sumOfTime*(1.0)/data.getNumOfPassenger()<< " "<<numOfMissed<<endl;
    // output operations
    myfile << sumOfTime*(1.0)/data.getNumOfPassenger()<< " "<<numOfMissed<<endl;
}

void AppManager::case7(vector<Counter> luggageCounter, vector<Counter> securityCounter, vector<Passenger> sorted) {
    vector<Passenger> securityQueue;
    vector<Passenger> finalList;
    int sumOfTime=0, numOfMissed=0;

    while (!sorted.empty()) { // where the luggage counter operation is executed
        Passenger pas = sorted.front(); // takes a passenger
        if(pas.getTicketType()=='L'){
            bool isdone = false;
            for (int i = 0; i < data.getNumOfLuggage(); i++) { // tries to put him into an available counter
                if (luggageCounter[i].getBusyEnd() <= pas.getArrivalTime()) { // if there is available counter
                    luggageCounter[i].setBusyStart(pas.getArrivalTime());
                    luggageCounter[i].setBusyEnd(pas.getArrivalTime() + pas.getLuggageTime());
                    pas.setTimeSpend(pas.getLuggageTime());
                    isdone = true;

                    if(pas.getUserType()!='V'){ // not a vip
                        securityQueue.push_back(pas); // after puts him into security queue
                    }else{
                        finalList.push_back(pas);
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
                        minAvailable=luggageCounter[i].getBusyEnd();
                    }
                }
                // let the time pass
                luggageCounter[theOne].setBusyStart(luggageCounter[theOne].getBusyEnd());
                luggageCounter[theOne].setBusyEnd(luggageCounter[theOne].getBusyStart()+pas.getLuggageTime());
                // puts passenger into counter
                pas.setTimeSpend(luggageCounter[theOne].getBusyEnd() - pas.getArrivalTime());

                if(pas.getUserType()!='V'){ // not a vip
                    securityQueue.push_back(pas); // after puts him into security queue
                }else{
                    finalList.push_back(pas);
                }

            }
        }else{
            if(pas.getUserType()!='V'){ // not a vip
                securityQueue.push_back(pas); // after puts him into security queue
            }else{
                finalList.push_back(pas);
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
    cout << "case 7: "<<sumOfTime*(1.0)/data.getNumOfPassenger()<< " "<<numOfMissed<<endl;
    // output operations
    myfile << sumOfTime*(1.0)/data.getNumOfPassenger()<< " "<<numOfMissed<<endl;
}

void AppManager::case8(vector<Counter> luggageCounter, vector<Counter> securityCounter, vector<Passenger> sorted) {
    vector<Passenger> securityQueue;
    vector<Passenger> finalList;
    int sumOfTime=0, numOfMissed=0;
    priority_queue<Passenger, vector<Passenger>, firstToFly> luggagePriorityQueue;
    priority_queue<Passenger, vector<Passenger>, firstToFly> securityPriorityQueue;

    while (!sorted.empty()) { // where the luggage counter operation is executed
        Passenger pas = sorted.front(); // takes a passenger
        if(pas.getTicketType()=='L'){
            bool isdone = false;
            for (int i = 0; i < data.getNumOfLuggage(); i++) { // tries to put him into an available counter
                if (luggageCounter[i].getBusyEnd() <= pas.getArrivalTime()) { // if there is available counter
                    luggageCounter[i].setBusyStart(pas.getArrivalTime());
                    luggageCounter[i].setBusyEnd(pas.getArrivalTime() + pas.getLuggageTime());
                    pas.setTimeSpend(pas.getLuggageTime());
                    isdone = true;
                    if(pas.getUserType()!='V'){ // not a vip
                        securityQueue.push_back(pas); // after puts him into security queue
                    }else{
                        finalList.push_back(pas);
                    }
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
                        minAvailable=luggageCounter[i].getBusyEnd();
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

                if(pas.getUserType()!='V'){ // not a vip
                    securityQueue.push_back(thePas); // after puts him into security queue
                }else{
                    finalList.push_back(thePas);
                }

                sorted.erase(remove(sorted.begin(), sorted.end(), thePas), sorted.end());
            }
        }else{
            if(pas.getUserType()!='V'){ // not a vip
                securityQueue.push_back(pas); // after puts him into security queue
            }else{
                finalList.push_back(pas);
            }
            sorted.erase(sorted.begin());
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
            int minAvailable = securityCounter[0].getBusyEnd();
            int theOne;
            for (int i = 0; i < data.getNumOfSecurity(); i++) { // find nearest available counter
                if (securityCounter[i].getBusyEnd() <= minAvailable) {
                    theOne = i;
                    minAvailable=securityCounter[i].getBusyEnd();
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

    cout << "case 8: "<<sumOfTime*(1.0)/data.getNumOfPassenger()<< " "<<numOfMissed<<endl;
    // output operations
    myfile << sumOfTime*(1.0)/data.getNumOfPassenger()<< " "<<numOfMissed<<endl;
}

AppManager::~AppManager() {
    myfile.close();
}

