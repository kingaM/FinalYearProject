/*
 * DataCache.cc
 *
 *  Created on: Nov 25, 2014
 *      Author: kinga
 */

#include "DataCache.h"
#include <algorithm>
#include <sstream>
#include <string>
#include <set>
#include <utility>

using namespace std;

bool DataCache::isInCache(long int id) {
    DataEntry wrapper = DataEntry(id, -1, -1, "", 0);
    return find(entries.begin(), entries.end(), wrapper) != entries.end();
}

void DataCache::add(long int id, int prevHop, int msgType, string dataType,
        long time) {
    DataEntry e = DataEntry(id, prevHop, msgType, dataType, time);
    entries.push_back(e);
}

string DataCache::toString() {
    stringstream ss;
    ss << "DataCache [entries: ";
    for (vector<DataEntry>::iterator it = entries.begin(); it != entries.end();
            ++it) {
        ss << "[id: " << it->getId() << " prevHop: " << it->getPrevHop()
                << " type: " << it->getMsgType() << " data type: "
                << it->getDataType() << " timestamp: " << it->getTime() << "]\n";
    }
    ss << "]";
    return ss.str();
}

set<int> DataCache::findBestNeighbour(string dataType) {
    set<int> neighbours;
    for (auto it = entries.begin(); it != entries.end(); ++it) {
        if (it->getMsgType() != 1 && it->getDataType().compare(dataType) == 0) {
            neighbours.insert(it->getPrevHop());
        }
    }
    return neighbours;
}

set<pair<string, int>> DataCache::getInactive(long currTime) {
    set<pair<string, int>> inactive;
    set<pair<string, int>> active;
    for (auto it = entries.begin(); it != entries.end();) {
        if (currTime - it->getTime() <= 30 * 100000000000
                && it->getMsgType() == 2) {
            active.insert(
                    pair<string, int>(it->getDataType(), it->getPrevHop()));
            it++;
        } else if (currTime - it->getTime() > 30 * 100000000000
                && it->getMsgType() == 2 && it->getPrevHop() != -1) {
            inactive.insert(
                    pair<string, int>(it->getDataType(), it->getPrevHop()));
            it = entries.erase(it);
        } else if (currTime - it->getTime() > 30 * 100000000000) {
            it = entries.erase(it);
        } else {
            it++;
        }
    }
    bool found = false;
    for (auto it = inactive.begin(); it != inactive.end();) {
        for (auto act = active.begin(); act != active.end(); act++) {
            if (act->first.compare(it->first) == 0
                    && act->second == it->second) {
                it = inactive.erase(it);
                found = true;
                break;
            }
        }
        if (!found) {
            it++;
        }
        found = false;
    }
    return inactive;
}
