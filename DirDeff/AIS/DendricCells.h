/*
 * DendricCells.h
 *
 *  Created on: Jan 3, 2015
 *      Author: kinga
 */

#ifndef DENDRICCELLS_H_
#define DENDRICCELLS_H_

#include "DendricCell.h"
#include "SignalMatrix.h"

class DendricCells {
    public:
        DendricCells();
        DendricCells(SignalMatrix matrix);
        virtual ~DendricCells();
        Maturity mature(string type);
        void cycle();
        void addCell(string type);
    private:
        map<string, DendricCell> table;
        SignalMatrix matrix;
};

#endif /* DENDRICCELLS_H_ */
