#include "AppManager.h"

AppManager::AppManager(InputParser& givenData, char *argv) {
    data=givenData;
    myfile.open(argv);
    list<Passenger> pL = data.getListOfPassenger();
    for(int i=0; i<data.getNumOfPassenger(); i++){ // pushing passengers into 8 different queue to process their data
        Passenger pas = pL.front();
        pas_q.push(pas);
        pL.pop_front();
    }
}

void AppManager::run(bool firstToFly, bool vip, bool online) {
    maintenance(pas_q, firstToFly, vip, online);
}

void AppManager::maintenance(priority_queue<Passenger, vector<Passenger>, comparator>& pq, bool firstFly, bool vip, bool online) {
    priority_queue<Passenger, vector<Passenger>, comparator> pas_queue=pq;
    int sum_of_time=0, num_of_missed=0;
    priority_queue<Passenger, vector<Passenger>, firstToFly> luggagePriorityQueue;
    priority_queue<Passenger, vector<Passenger>, firstToFly> securityPriorityQueue;
    queue<Passenger> luggageQueue;
    queue<Passenger> securityQueue;
    queue<Passenger> finalOne;
    Counter lC[data.getNumOfLuggage()];
    Counter sC[data.getNumOfSecurity()];

    int time=0;

    while(!pas_queue.empty()){
        Passenger pas = pas_queue.top();
        pas_queue.pop();
        time=pas.getTotalTime();
        if(pas.stage==0){
            if(online && pas.getTicketType() == 'N'){
                pas.stage++;
                pas_queue.push(pas);
            }
            else{
                bool free_counter=false;
                int x=0;
                while(x < sizeof(lC)){
                    if(!lC[x].isBusy()){
                        lC[x].makeBusy();
                        pas.where=x;
                        pas.setTotalTime(time+pas.getLuggageTime());
                        pas.stage++,
                        pas_queue.push(pas);
                        free_counter= true;
                        break;
                    }
                    x++;
                }
                if(!free_counter){
                    if(firstFly){
                        luggagePriorityQueue.push(pas);
                    }else{
                        luggageQueue.push(pas);
                    }

                }
            }
        }

        else if(pas.stage==1){
            if((pas.getTicketType()=='L') || (!online && pas.getTicketType()=='N')){
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
                    temp.stage++;
                    temp.where=pas.where;
                    temp.setTotalTime(time+temp.getLuggageTime());
                    pas_queue.push(temp);
                }
            }

            if(vip && pas.getUserType()=='V'){
                pas.stage++;
                pas_queue.push(pas);
            }
            else{
                bool free_counter=false;
                int x=0;
                while(x < sizeof(sC)){
                    if(!sC[x].isBusy()){
                        sC[x].makeBusy();
                        pas.where=x;
                        pas.setTotalTime(time+pas.getSecurityTime());
                        pas.stage++,
                        pas_queue.push(pas);
                        free_counter= true;
                        break;
                    }
                    x++;
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

        else if(pas.stage==2){
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
                        temp.stage++;
                        temp.where=pas.where;
                        temp.setTotalTime(time+temp.getSecurityTime());
                        pas_queue.push(temp);
                    }
                }
            }
        }
    }


    while(!finalOne.empty()){
        Passenger temp = finalOne.front();
        if(temp.getTotalTime()>temp.getFlightTime()){
            num_of_missed++;
        }
        sum_of_time+=temp.getTotalTime()-temp.getArrivalTime();
        finalOne.pop();
    }

    cout << "output: "<<sum_of_time*(1.0)/data.getNumOfPassenger()<< " "<<num_of_missed<<endl;
    // output operations
    myfile << sum_of_time*(1.0)/data.getNumOfPassenger()<< " "<<num_of_missed<<endl;
}

AppManager::~AppManager() {
    myfile.close();
}

