/*
 * Cache.cpp
 *
 *  Created on: Nov 24, 2014
 *      Author: kinga
 */

#include "Cache.h"
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

Cache::Cache() {
    // TODO Auto-generated constructor stub

}

Cache::~Cache() {
    // TODO Auto-generated destructor stub
}

void Cache::addEntry(string type, int timestamp, int dataRate, long expiresAt,
        int neighbour) {
    Entry e = Entry(type, timestamp, dataRate, expiresAt, neighbour);
    vector<Entry>::iterator entry = find(entries.begin(), entries.end(), e);
    if (entry != entries.end()) {
        entry->addGradient(dataRate, expiresAt, neighbour);
    } else {
        entries.push_back(e);
    }
}

vector<int> Cache::getPaths(string type, long currTime) {
    Entry wrapper = Entry(type, -1, -1, -1, -1);
    vector<Entry>::iterator entry = find(entries.begin(), entries.end(),
            wrapper);
    if (entry == entries.end()) {
        vector<int> empty;
        return empty;
    }
    return entry->getPaths(currTime);
}

int Cache::getMinInterval(string type) {
    Entry wrapper = Entry(type, -1, -1, -1, -1);
    vector<Entry>::iterator entry = find(entries.begin(), entries.end(),
            wrapper);
    if (entry == entries.end()) {
        return -1;
    }
    return entry->getMinInterval();
}

string Cache::toString() {
    stringstream ss;
    ss << "Cache [entries: ";
    for (vector<Entry>::iterator it = entries.begin(); it != entries.end();
            ++it) {
        ss << it->toString() << "\n";
    }
    ss << "]";
    return ss.str();
}

