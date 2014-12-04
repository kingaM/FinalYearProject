/*
 * Cache.h
 *
 *  Created on: Nov 24, 2014
 *      Author: kinga
 */

#ifndef CACHE_H_
#define CACHE_H_

#include <string>
#include <set>
#include "Entry.h"
#include <vector>
#include <array>
#include <boost/circular_buffer.hpp>
#include <utility>

#define MAX_SIZE 10000

using namespace std;

class Cache {
public:
    Cache();
    virtual ~Cache();
    void addEntry(string type, int timestamp, int dataRate, long duration,
            int neighbour);
    string toString();
    vector<int> getPaths(string type, long currTime);
    int getMinInterval(string type);
    void setInactive(set<pair<string, int>> inactive, long currTime);
private:
    boost::circular_buffer<Entry> entries;
};

#endif /* CACHE_H_ */
