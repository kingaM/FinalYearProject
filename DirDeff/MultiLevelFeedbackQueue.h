/*
 * MultiLevelFeedbackQueue.h
 *
 *  Created on: Nov 30, 2014
 *      Author: kinga
 */

#ifndef MULTILEVELFEEDBACKQUEUE_H_
#define MULTILEVELFEEDBACKQUEUE_H_

#include "packet_m.h"
#include "AIS/InterestCacheFilter.h"
#include "RandomNumberGenerator.h"
#include <boost/circular_buffer.hpp>

/**
 * Enum representing the priority in the queue.
 */
enum class Priority {
    HIGH, LOW
};

/**
 * A class representing a multilevel feedback queue. This class is used to
 * prioritise sending packets that originate from the current node, over
 * packets from other nodes.
 * @see Packet
 */
class MultiLevelFeedbackQueue {
    public:
        MultiLevelFeedbackQueue();
        MultiLevelFeedbackQueue(int row, int column);
        void insert(Packet *packet, Priority priority);
        void setInterestCacheFilter(InterestCacheFilter* icf);
        Packet* get();
        bool empty();
    private:
        int lastSwitch = 0;
        boost::circular_buffer<Packet*> high;
        boost::circular_buffer<Packet*> low;
        InterestCacheFilter *icf = NULL;
        int const switchLength = 10;
        RandomNumberGenerator generator;

        void addEntry(Packet* packet, boost::circular_buffer<Packet *>& buffer);
        Packet* getEntry(boost::circular_buffer<Packet *>& buffer);
};

#endif /* MULTILEVELFEEDBACKQUEUE_H_ */
