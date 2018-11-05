#include "Passenger.h"

Passenger::Passenger(int aT, int fT, int lT, int sT, char uT, char tT) {
    arrivalTime=aT;
    flightTime=fT;
    luggageTime=lT;
    securityTime=sT;
    userType=uT;
    ticketType=tT;
    timeSpend=arrivalTime;
}

bool Passenger::operator==(const Passenger &rhs) {
    return(arrivalTime==rhs.arrivalTime && flightTime==rhs.flightTime && luggageTime==rhs.luggageTime && securityTime==rhs.securityTime && userType==rhs.userType && ticketType==rhs.ticketType);
}

bool Passenger::operator<(const Passenger& rhs) const
{
    return (timeSpend < rhs.timeSpend);
}

int Passenger::getTimeSpend(){
    return timeSpend;
}

void Passenger::setTimeSpend(int time){
    timeSpend+=time;
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
    timeSpend=0;
}
