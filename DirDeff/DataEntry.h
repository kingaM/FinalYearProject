/*
 * Entry.h
 *
 *  Created on: Nov 23, 2014
 *      Author: kinga
 */

#include <vector>
#include <string>

using namespace std;

#ifndef DATAENTRY_H_
#define DATAENTRY_H_

class DataEntry {
public:
    DataEntry(long int id, int prevHop, int msgType);
    virtual ~DataEntry();
    friend bool operator==(const DataEntry& a, const DataEntry& b);
    long int getId() const;
    int getPrevHop() const;
    int getMsgType() const;

private:
    long int id;
    int prevHop;
    int msgType;
    int cmp(const DataEntry& a, const DataEntry& b);
};

#endif /* DATAENTRY_H_ */
