#ifndef CMPE250PROJECT2_PASSENGER_H
#define CMPE250PROJECT2_PASSENGER_H

using namespace std;

class Passenger {
public:
    Passenger(int aT, int fT, int lT, int sT, char uT, char tT);
    ~Passenger();
    int getArrivalTime();
    int getFlightTime();
    int getLuggageTime();
    int getSecurityTime();
    char getUserType();
    char getTicketType();
    int getTimeSpend();
    void setTimeSpend(int time);

private:
    int arrivalTime;
    int flightTime;
    int luggageTime;
    int securityTime;
    char userType;
    char ticketType;
    int timeSpend;
};


#endif //CMPE250PROJECT2_PASSENGER_H
