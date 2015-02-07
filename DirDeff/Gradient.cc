/*
 * Gradient.cc
 */

#include <string>
#include <sstream>
#include "Gradient.h"

using namespace std;

Gradient::Gradient(int dataRate, long expiry, int neighbour, long timestamp) {
    this->dataRate = dataRate;
    this->expiry = expiry;
    this->neighbour = neighbour;
    this->timestamp = timestamp;
}

string Gradient::toString() const {
    stringstream ss;
    ss << "Gradient [dataRate: " << dataRate << " timestamp: " << timestamp
            << " neighbour: " << neighbour << "]";
    return ss.str();
}

int Gradient::getDataRate() const {
    return dataRate;
}

int Gradient::getNeighbour() const {
    return neighbour;
}

long Gradient::getTimestamp() const {
    return timestamp;
}

long Gradient::getExpiry() const {
    return expiry;
}

int cmp(const Gradient &a, const Gradient &b) {
    if (a.getNeighbour() == b.getNeighbour()) {
        return 0;
    } else if (a.getNeighbour() < b.getNeighbour()) {
        return -1;
    } else {
        return 1;
    }
}

bool operator==(const Gradient& a, const Gradient& b) {
    return cmp(a, b) == 0;
}

bool operator<(const Gradient& a, const Gradient& b) {
    return cmp(a, b) == -1;
}
