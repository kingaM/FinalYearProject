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

class SignalMatrix {
    public:
        SignalMatrix();
        void addEntry(string type);
        MatrixEntry getEntry(string type);
        MatrixEntry getEntry();
        void addGlobalSs3Ds1(double numOfUpdates);
    private:
        // Maps each interest cache entry to an entry in a matrix
        map<string, MatrixEntry> largeMatrix;
        MatrixEntry smallMatrix;
};

#endif /* SIGNALMATRIX_H_ */
