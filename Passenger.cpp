#include "Passenger.h"

Passenger::Passenger(int aT, int fT, int lT, int sT, char uT, char tT) {
    arrivalTime=aT;
    flightTime=fT;
    luggageTime=lT;
    securityTime=sT;
    userType=uT;
    ticketType=tT;
    totalTime=arrivalTime;
    step=0;
}

Passenger::Passenger(){}

int Passenger::getTotalTime(){
    return totalTime;
}

void Passenger::setTotalTime(int time){
    totalTime=time;
}

int Passenger::getArrivalTime() {
    return arrivalTime;
}

int Passenger::getFlightTime() {
    return flightTime;
}

int Passenger::getLuggageTime() {
    return luggageTime;
}

int Passenger::getSecurityTime() {
    return securityTime;
}

char Passenger::getUserType() {
    return userType;
}

char Passenger::getTicketType() {
    return ticketType;
}

Passenger::~Passenger() {
    arrivalTime=0;
    flightTime=0;
    luggageTime=0;
    securityTime=0;
    userType=0;
    ticketType=0;
    totalTime=0;
    step=0;
}
