/*
 * DataCache.h
 *
 *  Created on: Nov 25, 2014
 *      Author: kinga
 */

#ifndef DATACACHE_H_
#define DATACACHE_H_

#include <vector>
#include "DataEntry.h"
#include <string>
#include <set>
#include <utility>

using namespace std;

/**
 * Class representing the data cache in the directed diffusion protocol.
 */
class DataCache {
    public:
        bool isInCache(long int id);
        void add(long int id, int prevHop, int msgType, string dataType,
                int time);
        string toString();
        /**
         * Finds best neighbours to forward the reinforced interest to, based on
         *  where the previous data came from.
         * @return
         */
        set<int> findBestNeighbour(string dataType);
        /**
         * Returns the data paths that did not send data in the last 30 seconds.
         * Deletes them from the cache. This is used to negatively reinforce
         * inactive paths in
         * @see Cache::setInactive(set<pair<string, int>> inactive,
         *  long currTime)
         * @return Set of type, gate pairs that are inactive.
         */
        set<pair<string, int>> getInactive(int currTime);
    private:
        vector<DataEntry> entries;
};

#endif /* DATACACHE_H_ */
