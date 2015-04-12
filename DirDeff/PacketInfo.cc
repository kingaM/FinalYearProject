/*
 * PacketInfo.cc
 *
 *  Created on: Jan 18, 2015
 *      Author: kinga
 */

#include "PacketInfo.h"
#include "AIS/ContentClassifier.h"
#include "AIS/DendriticCell.h"

using namespace std;

PacketInfo::PacketInfo() {
    // TODO Auto-generated constructor stub
}

PacketInfo::PacketInfo(string type, Class classifiction, int source,
        Maturity decision) {
    this->type = type;
    this->classification = classifiction;
    this->decision = decision;
    this->source = source;
}

PacketInfo::PacketInfo(string type, Class classifiction, int source,
        bool malicious) {
    this->type = type;
    this->classification = classifiction;
    this->malicious = malicious;
    this->source = source;
}

bool operator==(const PacketInfo& a, const PacketInfo& b) {
    return a.type.compare(b.type) == 0;
}

bool operator<(const PacketInfo& a, const PacketInfo& b) {
    if (a.type.compare(b.type) < 0) {
        return true;
    } else {
        return false;
    }
}

