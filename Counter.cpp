#include "Counter.h"

Counter::Counter() {
    busyStart=0;
    busyEnd=0;
}

int Counter::getBusyStart() {
    return busyStart;
}

void Counter::setBusyStart(int num) {
    busyStart=num;
}

int Counter::getBusyEnd() {
    return busyEnd;
}

void Counter::setBusyEnd(int num) {
    busyEnd=num;
}

Counter::~Counter() {}
