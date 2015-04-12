/*
 * DendriticCells.cc
 *
 *  Created on: Jan 3, 2015
 *      Author: kinga
 */

#include "AIS/DendriticCells.h"
#include "PacketInfo.h"
#include <omnetpp.h>
#include "debug.h"
#include "AIS/InterestCacheFilter.h"

using namespace std;

DendriticCells::DendriticCells() {
    // TODO Auto-generated constructor stub
}

DendriticCells::DendriticCells(SignalMatrix* matrix, PacketFilter* filter,
        InterestCacheFilter* icf, cSimpleModule* node) {
    this->matrix = matrix;
    this->node = node;
    this->filter = filter;
    this->icf = icf;
    fpSignal = node->registerSignal("fp");
    fnSignal = node->registerSignal("fn");
    tpSignal = node->registerSignal("tp");
    tnSignal = node->registerSignal("tn");
}

Maturity DendriticCells::mature(string type) {
    Maturity mat = table[type].mature();
    PacketInfo p = info[type];
    p.decision = mat;
    DEBUG_MSG(
            "MALICIOUS " << p.malicious << "DECISION " << DendriticCell::maturity(mat));
    if (mat == Maturity::MAT && !p.malicious) {
        node->emit(fpSignal, 1);
    } else if (mat == Maturity::SEMI && p.malicious) {
        node->emit(fnSignal, 1);
    } else if (mat == Maturity::SEMI && !p.malicious) {
        node->emit(tnSignal, 1);
    } else if (mat == Maturity::MAT && p.malicious) {
        node->emit(tpSignal, 1);
    }
    filter->addPacket(p);
    icf->addPacket(p);
    table.erase(type);
    info.erase(type);
    return mat;
}

void DendriticCells::cycle() {
    for (auto it = table.begin(); it != table.end(); it++) {
        it->second.cycle();
    }
}

void DendriticCells::addCell(PacketInfo p) {
    if (table.count(p.type) == 0) {
        table[p.type] = DendriticCell(matrix, p.type);
        info[p.type] = PacketInfo(p.type, p.classification, p.source,
                p.malicious);
    }
}
