#include <iostream>
#include "InputParser.h"
#include "Passenger.h"
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "run the code with the following command: ./project2 [input_file] [output_file]" << endl;
        return 1;
    }
    InputParser input(argv[1]);
    list<Passenger> liste = input.getListOfPassenger();
    for(int i=0; i<input.getNumOfPassenger(); i++){
        cout << liste.front().getSecurityTime()<<endl;
        liste.pop_front();
    }
    input.getListOfPassenger();
    input.getListOfPassenger();

    return 0;
}