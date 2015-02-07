/*
 * PacketInfo.h
 *
 *  Created on: Jan 18, 2015
 *      Author: kinga
 */

#ifndef PACKETINFO_H_
#define PACKETINFO_H_

#include "AIS/ContentClassifier.h"
#include "AIS/DendricCell.h"

/**
 * Class representing information about a packet needed for intrusion detection
 * and evaluation.
 * @see Packet
 */
class PacketInfo {
    public:
        PacketInfo();
        PacketInfo(string type, Class classifiction, Maturity decision);
        PacketInfo(string type, Class classifiction, bool malicious);
        PacketInfo(string type);
        /**
         * The type of the packet. This is based on data type in the packet.
         */
        string type;
        /**
         * The classification of the packet.
         * @see Class
         * @see ContentClassifier
         */
        Class classification;
        /**
         * The maturity of the packet.
         * @see Maturity
         * @see DendricCell
         */
        Maturity decision;
        /**
         * Flag indicating whether the packet is malicious or not. This is used
         * for evaluation only and is not used to classify the packet.
         */
        bool malicious;
        friend bool operator==(const PacketInfo& a, const PacketInfo& b);
        friend bool operator<(const PacketInfo& a, const PacketInfo& b);
};

#endif /* PACKETINFO_H_ */
