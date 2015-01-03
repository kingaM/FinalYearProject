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

SignalMatrix::~SignalMatrix() {
    // TODO Auto-generated destructor stub
}

void SignalMatrix::addEntry(string type) {
    largeMatrix[type] = MatrixEntry();
}

MatrixEntry SignalMatrix::getEntry(string type) {
    return largeMatrix.at(type);
}

MatrixEntry SignalMatrix::getEntry() {
    return smallMatrix;
}
