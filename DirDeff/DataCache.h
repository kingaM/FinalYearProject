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

class DataCache {
    public:
        bool isInCache(long int id);
        void add(long int id, int prevHop, int msgType, string dataType,
                int time);
        string toString();
        set<int> findBestNeighbour(string dataType);
        set<string> getAllTypes();
        set<pair<string, int>> getInactive(int currTime);
    private:
        vector<DataEntry> entries;
};

#endif /* DATACACHE_H_ */
