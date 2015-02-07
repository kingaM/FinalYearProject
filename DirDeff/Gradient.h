/*
 * Gradient.h
 *
 *  Created on: Nov 23, 2014
 *      Author: kinga
 */

#ifndef GRADIENT_H_
#define GRADIENT_H_

#include <string>

using namespace std;

class Gradient {
    public:
        Gradient(int dataRate, long timestamp, int neighbour, long currTime);
        virtual ~Gradient();
        int getDataRate() const;
        int getNeighbour() const;
        long getTimestamp() const;
        string toString() const;
        friend bool operator==(const Gradient& a, const Gradient& b);
        friend bool operator<(const Gradient& a, const Gradient& b);
        long getExpiry() const;

    private:
        int dataRate;
        long expiry;
        int neighbour;
        long timestamp;
        int cmp(const Gradient& a, const Gradient& b);
};

#endif /* GRADIENT_H_ */
