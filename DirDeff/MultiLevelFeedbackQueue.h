/*
 * MultiLevelFeedbackQueue.h
 *
 *  Created on: Nov 30, 2014
 *      Author: kinga
 */

#ifndef MULTILEVELFEEDBACKQUEUE_H_
#define MULTILEVELFEEDBACKQUEUE_H_

#include "packet_m.h"
#include <queue>

using namespace std;

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
        void insert(Packet *packet, Priority priority);
        Packet* get();
        bool empty();
    private:
        int lastSwitch = 0;
        queue<Packet*> high;
        queue<Packet*> low;
        int const switchLength = 10;

};

#endif /* MULTILEVELFEEDBACKQUEUE_H_ */
