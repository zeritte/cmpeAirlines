#include "Passenger.h"

Passenger::Passenger(int aT, int fT, int lT, int sT, char uT, char tT) {
    arrivalTime=aT;
    flightTime=fT;
    luggageTime=lT;
    securityTime=sT;
    userType=uT;
    ticketType=tT;
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

Passenger::~Passenger() {}