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
        int time) {
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
                << it->getDataType() << "]\n";
    }
    ss << "]";
    return ss.str();
}

set<int> DataCache::findBestNeighbour(string dataType) {
    set<int> neighbours;
    for (vector<DataEntry>::iterator it = entries.begin(); it != entries.end();
            ++it) {
        if (it->getMsgType() != 1 && it->getDataType() == dataType) {
            neighbours.insert(it->getPrevHop());
        }
    }
    return neighbours;
}

set<pair<string, int>> DataCache::getInactive(int currTime) {
    set<pair<string, int>> inactive;
    for (vector<DataEntry>::iterator it = entries.begin(); it != entries.end();
            ) {
        if (currTime - it->getTime() > 30 * 100000000000
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
    return inactive;
}
