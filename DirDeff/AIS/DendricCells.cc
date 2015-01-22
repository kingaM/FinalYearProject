/*
 * DendricCells.cpp
 *
 *  Created on: Jan 3, 2015
 *      Author: kinga
 */

#include <DendricCells.h>
#include "PacketInfo.h"
#include <stdio.h>

DendricCells::DendricCells() {
    // TODO Auto-generated constructor stub
}

DendricCells::DendricCells(SignalMatrix* matrix) {
    this->matrix = matrix;
}

DendricCells::~DendricCells() {
    // TODO Auto-generated destructor stub
}

Maturity DendricCells::mature(string type) {
    Maturity mat = table[type].mature();
    PacketInfo p = getKey(type);
    p.decision = mat;
    filter->addPacket(p);
    table.erase(type);
    info.erase(type);
    return mat;
}

void DendricCells::cycle() {
    for (auto it = table.begin(); it != table.end(); it++) {
        it->second.cycle();
    }
}

PacketInfo DendricCells::getKey(string type) {
    return info[type];
}

void DendricCells::addCell(PacketInfo p) {
    if (table.count(p.type) == 0) {
        table[p.type] = DendricCell(matrix);
        info[p.type] = PacketInfo(p);
    }
}

void DendricCells::setFilter(PacketFilter* filter) {
    this->filter = filter;
}
