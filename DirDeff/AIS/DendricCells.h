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
#include "PacketInfo.h"
#include "PacketFilter.h"
#include <omnetpp.h>

/**
 * A class that holds all cells that are currently maturing. The cells are
 * created per packet type.
 * @see DendricCell
 */
class DendricCells {
    public:
        DendricCells();
        DendricCells(SignalMatrix* matrix, PacketFilter* filter,
                cSimpleModule* node);
        Maturity mature(std::string type);
        void cycle();
        void addCell(PacketInfo type);

    private:
        PacketInfo getKey(std::string type);
        std::map<std::string, DendricCell> table;
        std::map<std::string, PacketInfo> info;
        SignalMatrix* matrix;
        PacketFilter* filter;
        cSimpleModule* node;
        simsignal_t fpSignal;
        simsignal_t fnSignal;
        simsignal_t tpSignal;
        simsignal_t tnSignal;
};

#endif /* DENDRICCELLS_H_ */
