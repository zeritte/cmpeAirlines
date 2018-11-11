#include "Counter.h"

Counter::Counter() {
    busy=false;
}
void Counter::makeBusy() {
    busy=true;
}
void Counter::notBusy() {
    busy=false;
}
bool Counter::isBusy() {
    return busy;
}

Counter::~Counter() {}
