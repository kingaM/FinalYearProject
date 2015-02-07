/*
 * Entry.cc
 *
 *  Created on: Nov 23, 2014
 *      Author: kinga
 */

#include "DataEntry.h"
#include <set>
#include <algorithm>
#include <vector>
#include <iterator>
#include <string>
#include <sstream>
#include <iostream>

DataEntry::DataEntry(long int id, int prevHop, int msgType, string dataType,
        int time) {
    this->id = id;
    this->prevHop = prevHop;
    this->msgType = msgType;
    this->dataType = dataType;
    this->time = time;
}

bool operator==(const DataEntry& a, const DataEntry& b) {
    return a.getId() == b.getId();
}

long int DataEntry::getId() const {
    return id;
}

int DataEntry::getPrevHop() const {
    return prevHop;
}

int DataEntry::getMsgType() const {
    return msgType;
}

const string& DataEntry::getDataType() const {
    return dataType;
}

int DataEntry::getTime() const {
    return time;
}
