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
#include <boost/circular_buffer.hpp>
#include <utility>

using namespace std;

Cache::Cache() {
    entries.set_capacity(MAX_SIZE);
}

Cache::~Cache() {
    // TODO Auto-generated destructor stub
}

Gradient* Cache::addEntry(string type, long timestamp, int dataRate, long expiresAt,
        int neighbour) {
    Entry e = Entry(type, timestamp, dataRate, expiresAt, neighbour);
    boost::circular_buffer<Entry>::iterator entry = find(entries.begin(),
            entries.end(), e);
    if (entry != entries.end()) {
        return entry->addGradient(dataRate, expiresAt, neighbour, timestamp);
    } else {
        entries.push_back(e);
        return NULL;
    }
}

vector<int> Cache::getPaths(string type, long currTime) {
    Entry wrapper = Entry(type, -1, -1, -1, -1);
    boost::circular_buffer<Entry>::iterator entry = find(entries.begin(),
            entries.end(), wrapper);
    if (entry == entries.end()) {
        vector<int> empty;
        return empty;
    }
    return entry->getPaths(currTime);
}

int Cache::getMinInterval(string type) {
    Entry wrapper = Entry(type, -1, -1, -1, -1);
    boost::circular_buffer<Entry>::iterator entry = find(entries.begin(),
            entries.end(), wrapper);
    if (entry == entries.end()) {
        return -1;
    }
    return entry->getMinInterval();
}

string Cache::toString() {
    stringstream ss;
    ss << "Cache [entries: ";
    for (boost::circular_buffer<Entry>::iterator it = entries.begin();
            it != entries.end(); ++it) {
        ss << it->toString() << "\n";
    }
    ss << "]";
    return ss.str();
}

void Cache::setInactive(set<pair<string, int>> inactive, long currTime) {
    for (Entry e : entries) {
        for (pair<string, int> p : inactive) {
            if (e.getType() == p.first) {
                e.addGradient(20, currTime + 100 * 1000, p.second, currTime);
            }
        }
    }
}

