/*
 * DataCache.h
 *
 *  Created on: Nov 25, 2014
 *      Author: kinga
 */
#include <vector>
#include "DataEntry.h"
#include <string>
#include <set>

#ifndef DATACACHE_H_
#define DATACACHE_H_

using namespace std;

class DataCache {
public:
    DataCache();
    virtual ~DataCache();
    bool isInCache(long int id);
    void add(long int id, int prevHop, int msgType);
    string toString();
    set<int> findBestNeighbour();
private:
    vector<DataEntry> entries;
};

#endif /* DATACACHE_H_ */
