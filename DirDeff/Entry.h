/*
 * Entry.h
 *
 *  Created on: Nov 23, 2014
 *      Author: kinga
 */

#ifndef ENTRY_H_
#define ENTRY_H_

#include <string>
#include <set>
#include "Gradient.h"
#include <vector>
#include <string>

using namespace std;
/**
 * Class representing an entry in the interest cache.
 * An entry is composed of a type and a list of gradients
 * @see Cache
 * @see Gradient
 */
class Entry {
    public:
        Entry(string type, long timestamp, int dataRate, long duration,
                int neighbour);
        long getTimestamp() const;
        const string getType() const;
        /**
         * Adds a gradient to an already existing entry.
         * @see Gradient
         * @return Previous gradient of the same specification (neighbour),
         *  NULL otherwise
         */
        Gradient* addGradient(int dataRate, long duration, int neighbour,
                long currTime);
        /**
         * Finds best paths for the current entry.
         * @return List of gates that the "next hop" neighbours lie on.
         */
        vector<int> getPaths(long currTime);
        string toString();
        friend bool operator==(const Entry& a, const Entry& b);
        int getMinInterval();
    private:
        string type;
        long timestamp;
        set<Gradient> gradients;
        int cmp(const Entry& a, const Entry& b);
};

#endif /* ENTRY_H_ */
