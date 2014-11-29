/*
 * Entry.cpp
 *
 *  Created on: Nov 23, 2014
 *      Author: kinga
 */

#include "Entry.h"
#include "Gradient.h"
#include <set>
#include <algorithm>
#include <vector>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>

Entry::Entry(string type, int timestamp, int dataRate, long duration,
        int neighbour) {
    this->type = type;
    this->timestamp = timestamp;
    this->addGradient(dataRate, duration, neighbour);
}

Entry::~Entry() {

}

const set<Gradient>& Entry::getGradients() const {
    return gradients;
}

void Entry::addGradient(int dataRate, long duration, int neighbour) {
    Gradient g = Gradient(dataRate, duration, neighbour);
    set<Gradient>::iterator prevG = gradients.find(g);
    if (prevG != gradients.end()) {
        gradients.erase(prevG);
    }
    gradients.insert(g);
}

vector<int> Entry::getPaths(long currTime) {
    vector<int> paths;
    set<Gradient>::iterator it = gradients.begin();
    int max = min_element(gradients.begin(), gradients.end())->getDataRate();
    cout << max << endl;
    while (it != gradients.end()) {
        if (currTime > it->getTimestamp()) {
            gradients.erase(it++);
            continue;
        }
        if (it->getDataRate() == max) {
            paths.insert(paths.begin(), it->getNeighbour());
        }
        it++;
    }
    return paths;
}

int Entry::getMinInterval() {
    return min_element(gradients.begin(), gradients.end())->getDataRate();
}

void Entry::setGradients(const set<Gradient>& gradients) {
    this->gradients = gradients;
}

int Entry::getTimestamp() const {
    return timestamp;
}

void Entry::setTimestamp(int timestamp) {
    this->timestamp = timestamp;
}

const string& Entry::getType() const {
    return type;
}

void Entry::setType(const string& type) {
    this->type = type;
}

string Entry::toString() {
    stringstream ss;
    ss << "Entry [type: " << type << " timestamp: " << timestamp
            << " gradients: ";
    for (set<Gradient>::iterator it = gradients.begin(); it != gradients.end();
            ++it) {
        ss << it->toString() << ", ";
    }
    ss << "]";
    return ss.str();
}

int cmp(const Entry &a, const Entry &b) {
    if (a.getType() == a.getType()) {
        return 1;
    } else {
        return 0;
    }
}

bool operator==(const Entry& a, const Entry& b)
{
  return cmp(a, b) == 1;
}
