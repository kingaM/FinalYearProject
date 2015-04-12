/*
 * DendriticCells.h
 *
 *  Created on: Jan 3, 2015
 *      Author: kinga
 */

#ifndef DENDRITICCELLS_H_
#define DENDRITICCELLS_H_

#include "AIS/DendriticCell.h"
#include "AIS/SignalMatrix.h"
#include "PacketInfo.h"
#include "AIS/PacketFilter.h"
#include "AIS/InterestCacheFilter.h"
#include <omnetpp.h>
#include <map>

/**
 * A class that holds all cells that are currently maturing. The cells are
 * created per packet type.
 * @see DendriticCell
 */
class DendriticCells {
    public:
        DendriticCells();
        DendriticCells(SignalMatrix* matrix, PacketFilter* filter,
                InterestCacheFilter* icf, cSimpleModule* node);
        Maturity mature(std::string type);
        void cycle();
        void addCell(PacketInfo type);

    private:
        std::map<std::string, DendriticCell> table;
        std::map<std::string, PacketInfo> info;
        SignalMatrix* matrix = NULL;
        PacketFilter* filter = NULL;
        InterestCacheFilter* icf = NULL;
        cSimpleModule* node = NULL;
        simsignal_t fpSignal;
        simsignal_t fnSignal;
        simsignal_t tpSignal;
        simsignal_t tnSignal;
};

#endif /* DENDRICCELLS_H_ */
