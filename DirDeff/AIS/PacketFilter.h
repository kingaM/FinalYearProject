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

/**
 * Class that is used to filter packets based on the information provided by the
 * content classifier and dendric cells
 * @see ContentClassifier
 * @see DendricCells
 */
class PacketFilter {
    public:
        /**
         * @param row The row to read the seed from
         * @param column The column to read the seed from
         */
        PacketFilter(int row, int column);
        void addPacket(PacketInfo p);
        /**
         * Filters packet based on how many packets of this class were
         * classified as dangerous.
         * @return True if the packet should be dropped, false otherwise
         */
        bool filterPacket(PacketInfo p);
    private:
        boost::circular_buffer<PacketInfo> benign;
        boost::circular_buffer<PacketInfo> malicious;
        RandomNumberGenerator generator;
        double getDropRate(boost::circular_buffer<PacketInfo> list);
};

#endif /* PACKETFILTER_H_ */
