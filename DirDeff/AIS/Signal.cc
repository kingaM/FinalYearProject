/*
 * Signal.cc
 *
 *  Created on: Dec 29, 2014
 *      Author: kinga
 */

#include <AIS/Signal.h>
#include <string>
#include <sstream>

using namespace std;

Signal::Signal() {
    this->present = false;
    this->concentration = 0;
}

Signal::Signal(double concentration) {
    this->present = true;
    this->concentration = concentration;
}

Signal::~Signal() {
    // TODO Auto-generated destructor stub
}

double Signal::getConcentration() const {
    return concentration;
}

void Signal::setConcentration(double concentration) {
    this->concentration = concentration;
}

bool Signal::isPresent() const {
    return present;
}

void Signal::setPresent(bool present) {
    this->present = present;
}

string Signal::toString() const {
    stringstream ss;
    ss << "present: " << present << " conc: " << concentration << endl;;
    return ss.str();
}
