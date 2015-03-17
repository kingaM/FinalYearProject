/*
 * InterestCacheFilter.h
 *
 *  Created on: Feb 28, 2015
 *      Author: kinga
 */

#ifndef INTERESTCACHEFILTER_H_
#define INTERESTCACHEFILTER_H_

#include <boost/circular_buffer.hpp>
#include <map>
#include "PacketInfo.h"
#include "RandomNumberGenerator.h"

/**
 * Class that is used to filter packets based on the information provided by the
 * content classifier and dendric cells
 * @see ContentClassifier
 * @see DendricCells
 */
class InterestCacheFilter {
    public:
        void addPacket(PacketInfo p);
        /**
         * Filters packet based on how many packets of this class were
         * classified as dangerous.
         * @return A value between 0 and 1 indicating how trustworthy the packet
         *  is. The higher the number the more trustworthy it is.
         */
        double getTrustworthiness(int source);
    private:
        /**
         * Key = source, Value = Last 10 packets from that source
         */
        std::map<int, boost::circular_buffer<PacketInfo>> classification;
        double getDropRate(boost::circular_buffer<PacketInfo> list);
};

#endif /* INTERESTCACHEFILTER_H_ */
