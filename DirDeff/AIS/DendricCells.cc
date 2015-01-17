/*
 * DendricCells.cpp
 *
 *  Created on: Jan 3, 2015
 *      Author: kinga
 */

#include <DendricCells.h>

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
    Maturity mat = table[type].mature();
    table.erase(table[type]);
    return mat;
}

void DendricCells::cycle() {
    for (auto it = table.begin(); it != table.end(); it++) {
        it->second.cycle();
    }
}

void DendricCells::addCell(string type) {
    if (table.count(type) == 0) {
        table[type] = DendricCell(matrix);
    }
}
