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

#define MAX_SIZE 10000

using namespace std;

class SignalMatrix {
    public:
        SignalMatrix();
        virtual ~SignalMatrix();
        void addEntry(string type);
        MatrixEntry getEntry(string type);
    private:
        // Maps each interest cache entry to an entry in a matrix
        map<string, MatrixEntry> matrix;
};

#endif /* SIGNALMATRIX_H_ */
