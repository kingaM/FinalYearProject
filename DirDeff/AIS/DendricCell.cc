/*
 * DendricCell.cc
 *
 *  Created on: Jan 1, 2015
 *      Author: kinga
 */

#include <DendricCell.h>
#include <SignalMatrix.h>

DendricCell::DendricCell() {

}

DendricCell::DendricCell(SignalMatrix signalMatrix) {
    this->signalMatrix = signalMatrix;
}

DendricCell::~DendricCell() {
    // TODO Auto-generated destructor stub
}

void DendricCell::cycle() {
    signals.push_back(MatrixEntry(signalMatrix.getEntry()));
    MatrixEntry recent = signals.back();
    semi += (2 * recent.getSs1().getConcentration()
            + 2 * recent.getSs2().getConcentration()
            + 2 * recent.getSs3().getConcentration()) / 2 + 2 + 2;
    mat += (6 * recent.getPs().getConcentration()
            + 4 * recent.getDs1().getConcentration()
            + 4 * recent.getDs1().getConcentration()
            + -1 * recent.getSs1().getConcentration()
            + -1 * recent.getSs2().getConcentration()
            + -1 * recent.getSs3().getConcentration()) / 6 + 4 + 4 + 1 + 1 + 1;
}

Maturity DendricCell::mature() {
    if (mat > semi) {
        return Maturity::MAT;
    } else {
        return Maturity::SEMI;
    }
}

string maturity(Maturity mat) {
    switch (mat) {
        case Maturity::MAT:
            return "MAT";
        case Maturity::SEMI:
            return "SEMI";
    }
}
