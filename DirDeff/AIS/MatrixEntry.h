/*
 * MatrixEntry.h
 *
 *  Created on: Dec 29, 2014
 *      Author: kinga
 */

#ifndef MATRIXENTRY_H_
#define MATRIXENTRY_H_

#include "Signal.h"

class MatrixEntry {
    public:
        MatrixEntry();
        virtual ~MatrixEntry();
        const Signal& getDs1() const;
        const Signal& getDs2() const;
        void setDs2(double remainingTime, double expirationTime);
        const Signal& getPs() const;
        void setPs();
        const Signal& getSs1() const;
        void setSs1();
        const Signal& getSs2() const;
        void setSs2();
        const Signal& getSs3() const;
        void setSs3Ds1(double numOfUpdates);

    private:
        // Generated from data delivery failures
        Signal ps;
        // Generated from data packet arrival
        Signal ss1;
        // Generated from cache entry expiration
        Signal ss2;
        // Generated from cache update rate
        Signal ss3;
        // Generated from cache update rate
        Signal ds1;
        // Generated from cache entry overwriting
        Signal ds2;
};

#endif /* MATRIXENTRY_H_ */
