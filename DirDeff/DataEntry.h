/*
 * Entry.h
 *
 *  Created on: Nov 23, 2014
 *      Author: kinga
 */

#ifndef DATAENTRY_H_
#define DATAENTRY_H_

#include <string>
/**
 * Class representing an entry in data cache.
 * @see DataCache
 */
class DataEntry {
    public:
        DataEntry(long int id, int prevHop, int msgType, std::string dataType,
                int time);
        friend bool operator==(const DataEntry& a, const DataEntry& b);
        long int getId() const;
        int getPrevHop() const;
        int getMsgType() const;
        const std::string& getDataType() const;
        int getTime() const;

    private:
        long int id;
        int prevHop;
        int msgType;
        std::string dataType;
        int time;
};

#endif /* DATAENTRY_H_ */
