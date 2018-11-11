#include "AppManager.h"

AppManager::AppManager(InputParser& givenData, char *argv) {
    data=givenData;
    myfile.open(argv);
    list<Passenger> pL = data.getListOfPassenger();
    for(int i=0; i<data.getNumOfPassenger(); i++){ // pushes passengers into base priority queue
        Passenger pas = pL.front();
        pas_q.push(pas);
        pL.pop_front();
    }
}

void AppManager::run(bool firstToFly, bool vip, bool online) {
    maintenance(pas_q, firstToFly, vip, online); // calls helper function
}

void AppManager::maintenance(priority_queue<Passenger, vector<Passenger>, comparator>& pq, bool firstFly, bool vip, bool online) {
    priority_queue<Passenger, vector<Passenger>, comparator> pas_queue=pq; // keeps original queue safely
    int sum_of_time=0, num_of_missed=0;
    priority_queue<Passenger, vector<Passenger>, firstToFly> luggagePriorityQueue; // created for first to fly first serve cases
    priority_queue<Passenger, vector<Passenger>, firstToFly> securityPriorityQueue; // created for first to fly first serve cases
    queue<Passenger> luggageQueue; // created for first come first serve cases
    queue<Passenger> securityQueue; // created for first come first serve cases
    queue<Passenger> finalOne; // final queue for calculations
    Counter lC[data.getNumOfLuggage()]; // creates luggage counters
    Counter sC[data.getNumOfSecurity()]; // creates security counters

    int time=0;

    /* 
    if passenger just came, step=0. if on the luggage counter, step=1. if on the security counter, step=2. 
    necessary arrangements are made such as online ticketing case or vip case.
    */

    while(!pas_queue.empty()){ // while there is passenger in list
        Passenger pas = pas_queue.top();
        pas_queue.pop();
        time=pas.getTotalTime();
        if(pas.step==0){

            if(online && pas.getTicketType() == 'N'){ // if online ticketing is implemented
                pas.step++;
                pas_queue.push(pas);
            }
            else{
                bool free_counter=false;
                int i=0;
                while(i < sizeof(lC)){ // searchs for available counter
                    if(!lC[i].isBusy()){ // if found
                        lC[i].makeBusy();
                        pas.where=i;
                        pas.setTotalTime(time+pas.getLuggageTime());
                        pas.step++,
                        pas_queue.push(pas);
                        free_counter= true;
                        break;
                    }
                    i++;
                }
                if(!free_counter){ // if not found push into queue
                    if(firstFly){ // if first to fly is implemented
                        luggagePriorityQueue.push(pas);
                    }else{
                        luggageQueue.push(pas);
                    }

                }
            }
        }

        else if(pas.step==1){

            if((pas.getTicketType()=='L') || (!online && pas.getTicketType()=='N')){ // if online ticketing is implemented
                lC[pas.where].notBusy();
                if(!luggageQueue.empty() || !luggagePriorityQueue.empty()){
                    Passenger temp;
                    if(firstFly){
                        temp = luggagePriorityQueue.top();
                        luggagePriorityQueue.pop();
                    }else{
                        temp = luggageQueue.front();
                        luggageQueue.pop();
                    }
                    lC[pas.where].makeBusy();
                    temp.step++;
                    temp.where=pas.where;
                    temp.setTotalTime(time+temp.getLuggageTime());
                    pas_queue.push(temp);
                }
            }

            if(vip && pas.getUserType()=='V'){ // if vip is implemented
                pas.step++;
                pas_queue.push(pas);
            }
            else{
                bool free_counter=false;
                int i=0;
                while(i < sizeof(sC)){
                    if(!sC[i].isBusy()){
                        sC[i].makeBusy();
                        pas.where=i;
                        pas.setTotalTime(time+pas.getSecurityTime());
                        pas.step++,
                        pas_queue.push(pas);
                        free_counter= true;
                        break;
                    }
                    i++;
                }
                if(!free_counter){
                    if(firstFly){
                        securityPriorityQueue.push(pas);
                    }else{
                        securityQueue.push(pas);
                    }
                }
            }
        }

        else if(pas.step==2){

            if(vip && pas.getUserType()=='V'){
                finalOne.push(pas);
            }
            else{
                if(sC[pas.where].isBusy()){
                    sC[pas.where].notBusy();
                    finalOne.push(pas);
                    if(!securityQueue.empty() || !securityPriorityQueue.empty()){
                        Passenger temp;
                        if(firstFly){
                            temp = securityPriorityQueue.top();
                            securityPriorityQueue.pop();
                        }else{
                            temp = securityQueue.front();
                            securityQueue.pop();
                        }
                        sC[pas.where].makeBusy();
                        temp.step++;
                        temp.where=pas.where;
                        temp.setTotalTime(time+temp.getSecurityTime());
                        pas_queue.push(temp);
                    }
                }
            }
        }
    }

    
    while(!finalOne.empty()){ // where calculations are made
        Passenger temp = finalOne.front();
        if(temp.getTotalTime()>temp.getFlightTime()){
            num_of_missed++;
        }
        sum_of_time+=temp.getTotalTime()-temp.getArrivalTime();
        finalOne.pop();
    }

    // output operations
    cout << "output: "<<sum_of_time*(1.0)/data.getNumOfPassenger()<< " "<<num_of_missed<<endl;
    myfile << sum_of_time*(1.0)/data.getNumOfPassenger()<< " "<<num_of_missed<<endl;
}

AppManager::~AppManager() {
    myfile.close();
}

