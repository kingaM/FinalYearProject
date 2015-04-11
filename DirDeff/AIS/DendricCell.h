/*
 * DendricCell.h
 *
 *  Created on: Jan 1, 2015
 *      Author: kinga
 */

#ifndef DENDRICCELL_H_
#define DENDRICCELL_H_

#include "SignalMatrix.h"
#include <string>
#include <vector>

/**
 * Defines the possible stages of the cell.
 */
enum class Maturity {
    MAT, /*!< A mature cell is considered dangerous */
    SEMI /*!< A semi-mature cell is considered benign */
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
        static std::string maturity(Maturity mat);
        DendricCell();
        DendricCell(SignalMatrix* signalMatrix, std::string type);
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
        std::string type;
        std::vector<MatrixEntry> signals;
        /**
         * Concentration for semi-mature cytokines
         */
        double semi = 0;
        /**
         * Concentration for mature cytokines
         */
        double mat = 0;
};

#endif /* DENDRICCELL_H_ */
