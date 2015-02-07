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

class Entry {
    public:
        Entry(string type, long timestamp, int dataRate, long duration,
                int neighbour);
        const set<Gradient>& getGradients() const;
        long getTimestamp() const;
        const string getType() const;
        Gradient* addGradient(int dataRate, long duration, int neighbour, long currTime);
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
