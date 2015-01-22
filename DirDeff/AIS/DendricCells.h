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

class DendricCells {
    public:
        DendricCells();
        DendricCells(SignalMatrix* matrix);
        virtual ~DendricCells();
        Maturity mature(string type);
        void cycle();
        void addCell(PacketInfo type);
        void setFilter(PacketFilter* filter);

    private:
        PacketInfo getKey(string type);
        map<string, DendricCell> table;
        map<string, PacketInfo> info;
        SignalMatrix* matrix;
        PacketFilter* filter;
};

#endif /* DENDRICCELLS_H_ */
