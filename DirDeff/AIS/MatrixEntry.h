/*
 * MatrixEntry.h
 *
 *  Created on: Dec 29, 2014
 *      Author: kinga
 */

#ifndef MATRIXENTRY_H_
#define MATRIXENTRY_H_

#include "Signal.h"

/**
 * An entry in the signal matrix. It holds all signals that happened recently
 * in the node.
 * @see Node
 */
class MatrixEntry {
    public:
        MatrixEntry();
        const Signal& getDs1() const;
        const Signal& getDs2() const;
        void setDs2(long timestamp, long expiry, long currTime);
        const Signal& getPs() const;
        void setPs(int nrcvd, int nexp);
        void setPs(double conc);
        const Signal& getSs1() const;
        void setSs1();
        const Signal& getSs2() const;
        void setSs2();
        const Signal& getSs3() const;
        void setSs3Ds1(double numOfUpdates);

    private:
        /**
         * PAMP signal generated from data delivery failures
         */
        Signal ps;
        /**
         * Safe signal generated from data packet arrival
         */
        Signal ss1;
        /**
         * Safe signal generated from cache entry expiration
         */
        Signal ss2;
        /**
         * Safe signal generated from cache update rate
         */
        Signal ss3;
        /**
         * Danger signal generated from cache update rate
         */
        Signal ds1;
        /**
         * Danger signal generated from cache entry overwriting
         */
        Signal ds2;
};

#endif /* MATRIXENTRY_H_ */
