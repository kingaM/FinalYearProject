/*
 * Gradient.h
 *
 *  Created on: Nov 23, 2014
 *      Author: kinga
 */
#include <string>

using namespace std;

#ifndef GRADIENT_H_
#define GRADIENT_H_

class Gradient {
    public:
        Gradient(int dataRate, long timestamp, int neighbour, long currTime);
        virtual ~Gradient();
        int getDataRate() const;
        void setDataRate(int dataRate);
        int getNeighbour() const;
        void setNeighbour(int neighbour);
        long getTimestamp() const;
        void setTimestamp(long timestamp);
        string toString() const;
        friend bool operator==(const Gradient& a, const Gradient& b);
        friend bool operator<(const Gradient& a, const Gradient& b);
        long getExpiry() const;
        void setExpiry(long expiry);

    private:
        int dataRate;
        long expiry;
        int neighbour;
        long timestamp;
        int cmp(const Gradient& a, const Gradient& b);
};

#endif /* GRADIENT_H_ */
