/*
 * PacketFilter.cc
 *
 *  Created on: Jan 18, 2015
 *      Author: kinga
 */

#include "PacketFilter.h"
#include <boost/circular_buffer.hpp>
#include "ContentClassifier.h"
#include "RandomNumberGenerator.h"
#include "debug.h"

PacketFilter::PacketFilter(int row, int column) {
    generator = RandomNumberGenerator("seeds.csv", row, column);
    benign.set_capacity(10);
    malicious.set_capacity(10);
}

void PacketFilter::addPacket(PacketInfo p) {
    if (p.classification == Class::BENIGN) {
        benign.push_back(p);
    } else {
        malicious.push_back(p);
    }
}

bool PacketFilter::filterPacket(PacketInfo p) {
    double prob = 0;
    if (p.classification == Class::BENIGN) {
        prob = getDropRate(benign);
        DEBUG_MSG("Benign drop " << prob);
    } else {
        prob = getDropRate(malicious);
        DEBUG_MSG("Malicious drop " << prob);
    }
    return generator.boolWithProbability(prob);
}

double PacketFilter::getDropRate(boost::circular_buffer<PacketInfo> list) {
    int count = 0;
    for (auto it = list.begin(); it != list.end(); it++) {
        if (it->decision == Maturity::MAT) {
            count++;
        }
    }
    return (double) count / (double) 10;
}
