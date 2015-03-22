/*
 * Cache.cc
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
#include <stdio.h>
#include "AIS/DendricCells.h"
#include "AIS/InterestCacheFilter.h"
#include "debug.h"

using namespace std;

Cache::Cache() {
    entries.set_capacity(SIZE);
}

void Cache::setDcs(DendricCells* dcs) {
    this->dcs = dcs;
}

boost::circular_buffer<Entry>::iterator Cache::getLeastTrustworthy() {
    double min = 1;
    boost::circular_buffer<Entry>::iterator pos = entries.end() - 1;
    for (auto it = entries.begin(); it != entries.end(); it++) {
        if (filter->getTrustworthiness(it->getSource()) < min) {
            min = filter->getTrustworthiness(it->getSource());
            pos = it;
        }
    }
    return pos;
}

Gradient* Cache::addEntry(string type, long timestamp, int source, int dataRate,
        long expiresAt, int neighbour) {
    Entry e = Entry(type, timestamp, source, dataRate, expiresAt, neighbour);
    boost::circular_buffer<Entry>::iterator entry = find(entries.begin(),
            entries.end(), e);
    // add only when it's better
    if (entry != entries.end() && entry->getMinInterval() > dataRate) {
        return entry->addGradient(dataRate, expiresAt, neighbour, timestamp);
    } else if (entry != entries.end()) {
        return NULL;
    } else {
        if (filter != NULL && entries.full()) {
            boost::circular_buffer<Entry>::iterator pos = getLeastTrustworthy();
            entries.erase(pos);
        }
        Maturity m = dcs->mature(type);
        DEBUG_MSG(
                "MATURITY: " << DendricCell::maturity(m) << " type " <<
                type);
        e.addGradient(dataRate, expiresAt, neighbour, timestamp);
        entries.push_back(e);
        return NULL;
    }
}

vector<int> Cache::getPaths(string type, long currTime) {
    Entry wrapper = Entry(type, -1, -1, -1, -1, -1);
    boost::circular_buffer<Entry>::iterator entry = find(entries.begin(),
            entries.end(), wrapper);
    if (entry == entries.end()) {
        vector<int> empty;
        return empty;
    }
    vector<int> paths = entry->getPaths(currTime);
    if (paths.empty()) {
        Maturity m = dcs->mature(entry->getType());
        DEBUG_MSG(
                "D MATURITY: " << DendricCell::maturity(m) << " type " <<
                entry->getType());
        entries.erase(entry);
    }
    return paths;
}

int Cache::getMinInterval(string type) {
    Entry wrapper = Entry(type, -1, -1, -1, -1, -1);
    boost::circular_buffer<Entry>::iterator entry = find(entries.begin(),
            entries.end(), wrapper);
    if (entry == entries.end()) {
        DEBUG_MSG("no entries");
        return 0;
    }
    return entry->getMinInterval();
}

string Cache::toString() {
    stringstream ss;
    ss << "Cache [entries: ";
    for (auto it = entries.begin(); it != entries.end(); ++it) {
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

void Cache::setFilter(InterestCacheFilter* filter) {
    this->filter = filter;
}
