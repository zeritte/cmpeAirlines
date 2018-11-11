#ifndef CMPE250PROJECT2_PASSENGER_H
#define CMPE250PROJECT2_PASSENGER_H

using namespace std;

class Passenger {
public:
    Passenger(int aT, int fT, int lT, int sT, char uT, char tT);
    Passenger();
    ~Passenger();
    int getArrivalTime();
    int getFlightTime();
    int getLuggageTime();
    int getSecurityTime();
    char getUserType();
    char getTicketType();
    int getTotalTime();
    void setTotalTime(int time);
    int step; // passenger's step
    int where; // passenger's position
private:
    int arrivalTime;
    int flightTime;
    int luggageTime;
    int securityTime;
    char userType;
    char ticketType;
    int totalTime;
};


#endif //CMPE250PROJECT2_PASSENGER_H
