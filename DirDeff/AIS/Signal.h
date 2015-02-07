/*
 * Signal.h
 *
 *  Created on: Dec 29, 2014
 *      Author: kinga
 */

#ifndef SIGNAL_H_
#define SIGNAL_H_

#include <string>

using namespace std;

/**
 * Represents a signal in the matrix entry.
 * @see MatrixEntry
 */
class Signal {
    public:
        Signal();
        Signal(double concentration);
        double getConcentration() const;
        void setConcentration(double concentration);
        bool isPresent() const;
        void setPresent(bool present);
        string toString() const;
    private:
        bool present;
        double concentration;
};

#endif /* SIGNAL_H_ */
