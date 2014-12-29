/*
 * SignalMatrix.cpp
 *
 *  Created on: Dec 29, 2014
 *      Author: kinga
 */

#include <AIS/SignalMatrix.h>

SignalMatrix::SignalMatrix() {
    // TODO Auto-generated constructor stub

}

SignalMatrix::~SignalMatrix() {
    // TODO Auto-generated destructor stub
}

void SignalMatrix::addEntry(string type) {
    matrix[type] = MatrixEntry();
}

MatrixEntry SignalMatrix::getEntry(string type) {
    return matrix.at(type);
}
