/*
 * MultiLevelFeedbackQueue.cc
 *
 *  Created on: Nov 30, 2014
 *      Author: kinga
 */

#include <MultiLevelFeedbackQueue.h>
#include "packet_m.h"
#include <queue>

using namespace std;

MultiLevelFeedbackQueue::MultiLevelFeedbackQueue() {
    // TODO Auto-generated constructor stub

}

MultiLevelFeedbackQueue::~MultiLevelFeedbackQueue() {
    // TODO Auto-generated destructor stub
}

void MultiLevelFeedbackQueue::insert(Packet* packet, Priority priority) {
    switch (priority) {
    case Priority::HIGH:
        high.push(packet);
        break;
    case Priority::LOW:
        low.push(packet);
        break;
    }
}

Packet* MultiLevelFeedbackQueue::get() {
    Packet *front = NULL;
    if (!high.empty() && (lastSwitch < switchLength || low.empty())) {
        front = high.front();
        high.pop();
        lastSwitch++;
    } else {
        front = low.front();
        low.pop();
        lastSwitch = 0;
    }
    return front;
}

bool MultiLevelFeedbackQueue::empty() {
    return high.empty() || low.empty();
}
