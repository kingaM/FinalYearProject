/*
 * PacketInfo.cc
 *
 *  Created on: Jan 18, 2015
 *      Author: kinga
 */

#include <PacketInfo.h>
#include <AIS/ContentClassifier.h>
#include <AIS/DendricCell.h>

PacketInfo::PacketInfo() {
    // TODO Auto-generated constructor stub
}

PacketInfo::PacketInfo(string type, Class classifiction, Maturity decision) {
    this->type = type;
    this->classification = classifiction;
    this->decision = decision;
}

PacketInfo::PacketInfo(string type, Class classifiction) {
    this->type = type;
    this->classification = classifiction;
}

PacketInfo::PacketInfo(string type) {
    this->type = type;
}
PacketInfo::~PacketInfo() {
    // TODO Auto-generated destructor stub
}

bool operator==(const PacketInfo& a, const PacketInfo& b) {
    return a.type == b.type;
}

bool operator<(const PacketInfo& a, const PacketInfo& b) {
    if (a.type.compare(b.type) < 0) {
        return true;
    } else {
        return false;
    }
}

