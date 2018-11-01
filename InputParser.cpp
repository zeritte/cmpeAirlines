#include "InputParser.h"

InputParser::InputParser(char *argv) {
    ifstream infile(argv);
    string line;
    if (!infile.is_open())
        cout<<"Could not open file\n";
    getline(infile, line);
    vector<string> data;
    splitter(line,data);
    numOfPassenger = stoi(data[0]); // string to int conversion
    numOfLuggage = stoi(data[1]);
    numOfSecutiry = stoi(data[2]);

    for(int i=0; i<numOfPassenger; i++){
        data.clear(); // cleaning up old data
        getline(infile, line);
        splitter(line,data);
        int aT = stoi(data[0]);
        int fT = stoi(data[1]);
        int lT = stoi(data[2]);
        int sT = stoi(data[3]);
        char uT = data[4][0];
        char tT = data[5][0];
        Passenger myPas(aT, fT, lT, sT, uT, tT);
        passengerList.push_back(myPas);
    }
}

InputParser::InputParser() {
    numOfPassenger=0;
    numOfLuggage=0;
    numOfPassenger=0;
    passengerList.clear();
}

InputParser::InputParser(const InputParser &in) {
    numOfPassenger=in.numOfPassenger;
    numOfLuggage=in.numOfLuggage;
    numOfPassenger=in.numOfPassenger;
    passengerList=in.passengerList;
}

InputParser::~InputParser() {}

template <class Container>
void InputParser::splitter(const string& str, Container& cont) {
    istringstream iss(str);
    copy(istream_iterator<string>(iss),istream_iterator<string>(),back_inserter(cont));
}

int InputParser::getNumOfPassenger() {
    return numOfPassenger;
}

int InputParser::getNumOfLuggage() {
    return numOfLuggage;
}

int InputParser::getNumOfSecurity() {
    return numOfSecutiry;
}

list<Passenger> InputParser::getListOfPassenger(){
    return passengerList;
}
