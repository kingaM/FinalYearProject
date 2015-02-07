/*
 * PacketFilter.h
 *
 *  Created on: Jan 18, 2015
 *      Author: kinga
 */

#ifndef PACKETFILTER_H_
#define PACKETFILTER_H_

#include <boost/circular_buffer.hpp>
#include "PacketInfo.h"
#include "RandomNumberGenerator.h"

class PacketFilter {
    public:
        PacketFilter();
        void addPacket(PacketInfo p);
        bool filterPacket(PacketInfo p);
    private:
        boost::circular_buffer<PacketInfo> benign;
        boost::circular_buffer<PacketInfo> malicious;
        RandomNumberGenerator generator;
        double getDropRate(boost::circular_buffer<PacketInfo> list);
};

#endif /* PACKETFILTER_H_ */
