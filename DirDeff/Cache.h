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
#include <AIS/DendricCells.h>
#include <AIS/PacketFilter.h>

#define MAX_SIZE 10

using namespace std;

class Cache {
    public:
        Cache();
        virtual ~Cache();
        Gradient* addEntry(string type, long timestamp, int dataRate, long duration,
                int neighbour);
        string toString();
        vector<int> getPaths(string type, long currTime);
        int getMinInterval(string type);
        void setInactive(set<pair<string, int>> inactive, long currTime);
        void setDcs(DendricCells dcs);
        const static int SIZE = MAX_SIZE;
    private:
        boost::circular_buffer<Entry> entries;
        DendricCells dcs;
        PacketFilter filter;
};

#endif /* CACHE_H_ */
