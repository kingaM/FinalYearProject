/*
 * SignalMatrix.cpp
 *
 *  Created on: Dec 29, 2014
 *      Author: kinga
 */

#include <AIS/SignalMatrix.h>
#include <MatrixEntry.h>

SignalMatrix::SignalMatrix() {
    smallMatrix = MatrixEntry();
}

void SignalMatrix::addEntry(string type) {
    largeMatrix[type] = MatrixEntry();
}

MatrixEntry SignalMatrix::getEntry(string type) {
    if (largeMatrix.count(type) == 0) {
        largeMatrix[type] = MatrixEntry();
    }
    return largeMatrix.at(type);
}

MatrixEntry SignalMatrix::getEntry() {
    return smallMatrix;
}

void SignalMatrix::addGlobalSs3Ds1(double numOfUpdates) {
    for (auto it = largeMatrix.begin(); it != largeMatrix.end(); it++) {
        it->second.setSs3Ds1(numOfUpdates);
    }
}
