/*
 * Entry.h
 *
 *  Created on: Nov 23, 2014
 *      Author: kinga
 */

using namespace std;

#ifndef DATAENTRY_H_
#define DATAENTRY_H_

#include <vector>
#include <string>

class DataEntry {
    public:
        DataEntry(long int id, int prevHop, int msgType, string dataType,
                int time);
        friend bool operator==(const DataEntry& a, const DataEntry& b);
        long int getId() const;
        int getPrevHop() const;
        int getMsgType() const;
        const string& getDataType() const;
        int getTime() const;

    private:
        long int id;
        int prevHop;
        int msgType;
        string dataType;
        int time;
};

#endif /* DATAENTRY_H_ */
