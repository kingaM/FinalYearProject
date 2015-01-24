/*
 * PacketInfo.h
 *
 *  Created on: Jan 18, 2015
 *      Author: kinga
 */

#ifndef PACKETINFO_H_
#define PACKETINFO_H_

#include <AIS/ContentClassifier.h>
#include <AIS/DendricCell.h>

class PacketInfo {
    public:
        PacketInfo();
        PacketInfo(string type, Class classifiction, Maturity decision);
        PacketInfo(string type, Class classifiction, bool malicious);
        PacketInfo(string type);
        virtual ~PacketInfo();
        string type;
        Class classification;
        Maturity decision;
        bool malicious;
        friend bool operator==(const PacketInfo& a, const PacketInfo& b);
        friend bool operator<(const PacketInfo& a, const PacketInfo& b);
};

#endif /* PACKETINFO_H_ */
