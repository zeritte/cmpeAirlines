#include <iostream>
#include "InputParser.h"
#include "AppManager.h"

int main(int argc, char* argv[]) {

    //auto start = std::chrono::high_resolution_clock::now();


    if (argc != 3) {
        cout << "run the code with the following command: ./project2 [input_file] [output_file]" << endl;
        return 1;
    }
    InputParser input(argv[1]);
    AppManager app(input, argv[2]);
    app.run(false,false,false);




    /*
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    cout << "time needed: " << duration.count() << " microseconds" <<endl;
    */

    return 0;
}