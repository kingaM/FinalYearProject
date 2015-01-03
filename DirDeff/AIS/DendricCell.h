/*
 * DendricCell.h
 *
 *  Created on: Jan 1, 2015
 *      Author: kinga
 */

#ifndef DENDRICCELL_H_
#define DENDRICCELL_H_

#include <SignalMatrix.h>
#include <string.h>
#include <vector>

using namespace std;

class DendricCell {
    public:
        DendricCell(SignalMatrix signalMatrix);
        virtual ~DendricCell();
        void cycle();
        void mature();

    private:
        SignalMatrix signalMatrix;
        string type;
        vector<MatrixEntry> signals;
        // concentration for semi-mature cytokines
        double semi;
        // concentration for mature cytokines
        double mat;
};

#endif /* DENDRICCELL_H_ */
