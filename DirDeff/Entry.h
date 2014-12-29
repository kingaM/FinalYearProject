/*
 * Entry.h
 *
 *  Created on: Nov 23, 2014
 *      Author: kinga
 */
#include <string>
#include <set>
#include "Gradient.h"
#include <vector>
#include <string>

using namespace std;

#ifndef ENTRY_H_
#define ENTRY_H_

class Entry {
    public:
        Entry(string type, long timestamp, int dataRate, long duration,
                int neighbour);
        virtual ~Entry();
        const set<Gradient>& getGradients() const;
        void setGradients(const set<Gradient>& gradients);
        long getTimestamp() const;
        void setTimestamp(long timestamp);
        const string& getType() const;
        void setType(const string& type);
        void addGradient(int dataRate, long duration, int neighbour);
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
