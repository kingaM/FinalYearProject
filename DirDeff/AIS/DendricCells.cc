/*
 * DendricCells.cc
 *
 *  Created on: Jan 3, 2015
 *      Author: kinga
 */

#include "DendricCells.h"
#include "PacketInfo.h"
#include <omnetpp.h>

using namespace std;

DendricCells::DendricCells() {
    // TODO Auto-generated constructor stub
}

DendricCells::DendricCells(SignalMatrix* matrix, PacketFilter* filter,
        cSimpleModule* node) {
    this->matrix = matrix;
    this->node = node;
    this->filter = filter;
    fpSignal = node->registerSignal("fp");
    fnSignal = node->registerSignal("fn");
    tpSignal = node->registerSignal("tp");
    tnSignal = node->registerSignal("tn");
}

Maturity DendricCells::mature(string type) {
    Maturity mat = table[type].mature();
    PacketInfo p = info[type];
    p.decision = mat;
    if (mat == Maturity::SEMI && !p.malicious) {
        node->emit(fpSignal, 1);
    } else if (mat == Maturity::MAT && p.malicious) {
        node->emit(fnSignal, 1);
    } else if (mat == Maturity::MAT && !p.malicious) {
        node->emit(tnSignal, 1);
    } else if (mat == Maturity::SEMI && p.malicious) {
        node->emit(tpSignal, 1);
    }
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

void DendricCells::addCell(PacketInfo p) {
    if (table.count(p.type) == 0) {
        table[p.type] = DendricCell(matrix, p.type);
        info[p.type] = PacketInfo(p.type, p.classification, p.malicious);
    }
}
