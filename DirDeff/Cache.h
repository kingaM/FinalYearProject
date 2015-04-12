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
#include <boost/circular_buffer.hpp>
#include <utility>
#include "AIS/DendriticCells.h"
#include "AIS/PacketFilter.h"
#include "AIS/InterestCacheFilter.h"

using namespace std;

/**
 * Class representing interest cache of the directed diffusion protocol.
 */
class Cache {
    public:
        Cache();
        /**
         * Adds an entry to cache.
         * @return Previous gradient with the same specification (type and
         *  neighbour) or NULL if no such gradient exists
         */
        Gradient* addEntry(string type, long timestamp, int source, int dataRate,
                long duration, int neighbour);
        string toString();
        /**
         * Finds paths for the given type, deletes inactive paths and returns
         *  active ones.
         * @return List of gates to forward the data on
         */
        vector<int> getPaths(string type, long currTime);
        /**
         * Returns the minimum interval for the given interest
         */
        int getMinInterval(string type);
        /**
         * Changes inactive interests to have a lower data rate
         * @param inactive The set of inactive gradients (type and gate)
         * @param currTime The current time
         */
        void setInactive(std::set<pair<string, int>> inactive, long currTime);
        void setDcs(DendriticCells* dcs);
        void setFilter(InterestCacheFilter* filter);
        const static int SIZE = 4;
    private:
        boost::circular_buffer<Entry> entries;
        DendriticCells* dcs;
        InterestCacheFilter* filter = NULL;

        boost::circular_buffer<Entry>::iterator getLeastTrustworthy();
};

#endif /* CACHE_H_ */
