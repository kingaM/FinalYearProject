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

class Signal {
    public:
        Signal();
        Signal(double concentration);
        virtual ~Signal();
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
