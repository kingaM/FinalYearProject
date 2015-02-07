/*
 * DendricCell.h
 *
 *  Created on: Jan 1, 2015
 *      Author: kinga
 */

#ifndef DENDRICCELL_H_
#define DENDRICCELL_H_

#include "SignalMatrix.h"
#include <string.h>
#include <vector>

using namespace std;

/**
 * Defines the possible stages of the cell.
 */
enum class Maturity {
    SEMI, /*!< A semi-mature cell is considered dangerous */
    MAT /*!< A mature cell is considered benign */
};

/**
 * A class representing a dendric cell used in the Artificial Immune System
 * algorithm.
 */
class DendricCell {
    public:
        /**
         * Converts the Maturity enum into a string
         * @see Maturity
         */
        static string maturity(Maturity mat);
        DendricCell();
        DendricCell(SignalMatrix* signalMatrix, string type);
        /**
         * Simulates one cycle of the cell. Retrieves the latest signals from
         * the signal matrix and updates the cytokines accordingly.
         * @see SignalMatrix
         */
        void cycle();
        /**
         * Matures the cell into either semi-mature (dangerous) or mature
         * (benign) cell.
         */
        Maturity mature();

    private:
        SignalMatrix* signalMatrix;
        string type;
        vector<MatrixEntry> signals;
        /**
         * Concentration for semi-mature cytokines
         */
        double semi;
        /**
         * Concentration for mature cytokines
         */
        double mat;
};

#endif /* DENDRICCELL_H_ */
