/*
 * InterestCacheFilter.cc
 *
 *  Created on: Feb 28, 2015
 *      Author: kinga
 */

#include "InterestCacheFilter.h"
#include <boost/circular_buffer.hpp>
#include "ContentClassifier.h"
#include "RandomNumberGenerator.h"

void InterestCacheFilter::addPacket(PacketInfo p) {
    if (classification.count(p.source) == 0) {
        classification[p.source] = boost::circular_buffer<PacketInfo>(10);
    }
    classification.at(p.source).push_back(PacketInfo(p));
}

double InterestCacheFilter::getTrustworthiness(int source) {
    if (classification.count(source) == 0) {
        return 1;
    }
    return 1 - getDropRate(classification.at(source));
}

double InterestCacheFilter::getDropRate(
        boost::circular_buffer<PacketInfo> list) {
    int count = 0;
    for (auto it = list.begin(); it != list.end(); it++) {
        if (it->decision == Maturity::SEMI) {
            count++;
        }
    }
    return (double) count / (double) 10;
}
