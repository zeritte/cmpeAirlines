#ifndef CMPE250PROJECT2_INPUTPARSER_H
#define CMPE250PROJECT2_INPUTPARSER_H

#include "Passenger.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
using namespace std;

class InputParser {
public:
    InputParser(char *argv);
    ~InputParser();
    template <class Container> void splitter(const string& str, Container& cont);
    int getNumOfPassenger();
    int getNumOfLuggage();
    int getNumOfSecurity();
    list<Passenger> getListOfPassenger();

private:
    int numOfPassenger;
    int numOfLuggage;
    int numOfSecutiry;
    list<Passenger> passengerList;
};


#endif //CMPE250PROJECT2_INPUTPARSER_H
