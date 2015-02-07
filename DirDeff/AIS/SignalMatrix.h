/*
 * SignalMatrix.h
 *
 *  Created on: Dec 29, 2014
 *      Author: kinga
 */

#ifndef SIGNALMATRIX_H_
#define SIGNALMATRIX_H_

#include <map>
#include "MatrixEntry.h"

using namespace std;

/**
 * Represents the signal matrix used for intrusion detection. Stores one
 * MatrixEntry per interest type (large) or just one MatrixEntry (small).
 * @see MatrixEntry
 */
class SignalMatrix {
    public:
        SignalMatrix();
        void addEntry(string type);
        MatrixEntry getEntry(string type);
        MatrixEntry getEntry();
        /**
         * Most signals are stored per MatrixEntry, however SS3 and DS1 signals
         * are global, thus need to be added to all entries in the matrix.
         * @see MatrixEntry::setSs3Ds1
         */
        void addGlobalSs3Ds1(double numOfUpdates);
    private:
        /**
         * Maps each interest cache entry to an entry in the matrix
         */
        map<string, MatrixEntry> largeMatrix;
        /**
         * Maps all interest cache entries to a single entry in the matrix
         */
        MatrixEntry smallMatrix;
};

#endif /* SIGNALMATRIX_H_ */
