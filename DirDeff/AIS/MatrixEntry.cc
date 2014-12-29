/*
 * MatrixEntry.cc
 *
 *  Created on: Dec 29, 2014
 *      Author: kinga
 */

#include <AIS/MatrixEntry.h>
#include <cmath>

#define CACHE_SIZE 1000

using namespace std;

MatrixEntry::MatrixEntry() {
    ps = Signal();
    ss1 = Signal();
    ss2 = Signal();
    ss3 = Signal();
    ds1 = Signal();
    ds2 = Signal();
}

MatrixEntry::~MatrixEntry() {
    // TODO Auto-generated destructor stub
}

const Signal& MatrixEntry::getDs1() const {
    return ds1;
}

const Signal& MatrixEntry::getDs2() const {
    return ds2;
}

void MatrixEntry::setDs2(double remainingTime, double expirationTime) {
    double conc = remainingTime / expirationTime;
    this->ds2 = Signal(conc);
}

const Signal& MatrixEntry::getPs() const {
    return ps;
}

void MatrixEntry::setPs() {
    // TODO: Add concentration
    this->ps = Signal();
}

const Signal& MatrixEntry::getSs1() const {
    return ss1;
}

void MatrixEntry::setSs1() {
    this->ss1 = Signal(1);
}

const Signal& MatrixEntry::getSs2() const {
    return ss2;
}

void MatrixEntry::setSs2() {
    this->ss2 = Signal(1);
}

const Signal& MatrixEntry::getSs3() const {
    return ss3;
}

void MatrixEntry::setSs3Ds1(double numOfUpdates) {
    if (numOfUpdates > CACHE_SIZE) {
        double conc = pow((numOfUpdates / (double) CACHE_SIZE), (double) 2);
        this->ds1 = Signal(conc);
    } else {
        double conc = 1.0 - ((numOfUpdates - 1.0) / (double) CACHE_SIZE);
        this->ss3 = Signal(conc);
    }
}
