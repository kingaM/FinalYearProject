/*
 * PacketFilter.cc
 *
 *  Created on: Jan 18, 2015
 *      Author: kinga
 */

#include <PacketFilter.h>

#include <boost/circular_buffer.hpp>
#include "ContentClassifier.h"
#include "RandomNumberGenerator.h"

PacketFilter::PacketFilter() {
    generator = RandomNumberGenerator("seeds.csv", 0);
    benign.set_capacity(10);
    malicious.set_capacity(10);
}

PacketFilter::~PacketFilter() {
    // TODO Auto-generated destructor stub
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
        getDropRate(benign);
    } else {
        getDropRate(malicious);
    }
    return generator.boolWithProbability(prob);
}

double PacketFilter::getDropRate(boost::circular_buffer<PacketInfo> list) {
    int count = 0;
    for (auto it = list.begin(); it != list.end(); it++) {
        if(it->decision == Maturity::SEMI) {
            count++;
        }
    }
    return (double) count / (double) 10;
}
