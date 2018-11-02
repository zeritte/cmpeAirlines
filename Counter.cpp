#include "Counter.h"

Counter::Counter() {
    whenAvailable=0;
}

int Counter::getAvailable() {
    return whenAvailable;
}

void Counter::setAvailable(int num) {
    whenAvailable=num;
}

Counter::~Counter() {}
