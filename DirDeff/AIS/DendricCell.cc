/*
 * DendricCell.cc
 *
 *  Created on: Jan 1, 2015
 *      Author: kinga
 */

#include <DendricCell.h>
#include <SignalMatrix.h>

#define ALPHA 0.7

DendricCell::DendricCell() {

}

DendricCell::DendricCell(SignalMatrix* signalMatrix, string type) {
    this->signalMatrix = signalMatrix;
    this->type = type;
}

void DendricCell::cycle() {
    signals.push_back(MatrixEntry(signalMatrix->getEntry(type)));
    MatrixEntry recent = signals.back();
    double semiTmp = (2 * recent.getSs1().getConcentration()
            + 2 * recent.getSs2().getConcentration()
            + 2 * recent.getSs3().getConcentration()) / 2 + 2 + 2;
    double matTmp = (6 * recent.getPs().getConcentration()
            + 4 * recent.getDs1().getConcentration()
            + 4 * recent.getDs1().getConcentration()
            + -1 * recent.getSs1().getConcentration()
            + -1 * recent.getSs2().getConcentration()
            + -1 * recent.getSs3().getConcentration()) / 6 + 4 + 4 + 1 + 1 + 1;
    semi = ALPHA * semiTmp + (1 - ALPHA) * semi;
    mat = ALPHA * matTmp + (1 - ALPHA) * mat;
}

Maturity DendricCell::mature() {
    if (mat > semi) {
        return Maturity::MAT;
    } else {
        return Maturity::SEMI;
    }
}

string DendricCell::maturity(Maturity mat) {
    switch (mat) {
        case Maturity::MAT:
            return "MAT";
        case Maturity::SEMI:
            return "SEMI";
    }
    // Will never happen
    return "NONE";
}
