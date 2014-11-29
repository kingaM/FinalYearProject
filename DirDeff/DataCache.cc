/*
 * DataCache.cc
 *
 *  Created on: Nov 25, 2014
 *      Author: kinga
 */

#include <DataCache.h>
#include <algorithm>
#include <sstream>
#include <string>
#include <set>

using namespace std;

DataCache::DataCache() {
    // TODO Auto-generated constructor stub

}

DataCache::~DataCache() {
    // TODO Auto-generated destructor stub
}

bool DataCache::isInCache(long int id) {
    DataEntry wrapper = DataEntry(id, -1, -1);
    return find(entries.begin(), entries.end(), wrapper) != entries.end();
}

void DataCache::add(long int id, int prevHop, int msgType) {
    DataEntry e = DataEntry(id, prevHop, msgType);
    entries.push_back(e);
}

string DataCache::toString() {
    stringstream ss;
    ss << "DataCache [entries: ";
    for (vector<DataEntry>::iterator it = entries.begin(); it != entries.end();
            ++it) {
        ss << "[id: " << it->getId() << " prevHop: " << it->getPrevHop()
                << " type: " << it->getMsgType() << "]\n";
    }
    ss << "]";
    return ss.str();
}

set<int> DataCache::findBestNeighbour() {
    set<int> neighbours;
    for (vector<DataEntry>::iterator it = entries.begin(); it != entries.end();
            ++it) {
        if (it->getMsgType() != 1) {
            neighbours.insert(it->getPrevHop());
        }
    }
    return neighbours;
}

