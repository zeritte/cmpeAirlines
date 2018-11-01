#include <iostream>
#include "InputParser.h"
#include "AppManager.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "run the code with the following command: ./project2 [input_file] [output_file]" << endl;
        return 1;
    }
    InputParser input(argv[1]);
    AppManager app(input);
    app.print();

    return 0;
}