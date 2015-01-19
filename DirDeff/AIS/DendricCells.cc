/*
 * DendricCells.cpp
 *
 *  Created on: Jan 3, 2015
 *      Author: kinga
 */

#include <DendricCells.h>
#include "PacketInfo.h"

DendricCells::DendricCells() {
    // TODO Auto-generated constructor stub
}

DendricCells::DendricCells(SignalMatrix matrix) {
    this->matrix = matrix;
}

DendricCells::~DendricCells() {
    // TODO Auto-generated destructor stub
}

Maturity DendricCells::mature(string type) {
    Maturity mat = table[PacketInfo(type)].mature();
    PacketInfo p = getKey(type);
    p.decision = mat;
    filter.addPacket(p);
    table.erase(PacketInfo(type));
    return mat;
}

void DendricCells::cycle() {
    for (auto it = table.begin(); it != table.end(); it++) {
        it->second.cycle();
    }
}

PacketInfo DendricCells::getKey(string type) {
    for (auto it = table.begin(); it != table.end(); it++) {
        if (it->first.type == type) {
            return it->first;
        }
    }
    return PacketInfo();
}

void DendricCells::addCell(PacketInfo p) {
    if (table.count(p) == 0) {
        table[p] = DendricCell(matrix);
    }
}

void DendricCells::setFilter(const PacketFilter& filter) {
    this->filter = filter;
}
