/*
 * MultiLevelFeedbackQueue.cc
 *
 *  Created on: Nov 30, 2014
 *      Author: kinga
 */

#include <boost/circular_buffer.hpp>
#include "MultiLevelFeedbackQueue.h"
#include "packet_m.h"
#include "debug.h"
#include "RandomNumberGenerator.h"

using namespace std;
using namespace boost;

MultiLevelFeedbackQueue::MultiLevelFeedbackQueue() {
    high.set_capacity(30);
    low.set_capacity(30);
    generator = RandomNumberGenerator("seeds.csv", 0, 0);
}

MultiLevelFeedbackQueue::MultiLevelFeedbackQueue(int row, int column) {
    high.set_capacity(30);
    low.set_capacity(30);
    generator = RandomNumberGenerator("seeds.csv", row, column);
}

void MultiLevelFeedbackQueue::setInterestCacheFilter(InterestCacheFilter* icf) {
    this->icf = icf;
}

void MultiLevelFeedbackQueue::addEntry(Packet* packet,
        circular_buffer<Packet *>& buffer) {
    if (buffer.full()) {
        delete buffer[0];
        buffer.pop_front();
    }
    buffer.push_back(packet);
}

void MultiLevelFeedbackQueue::insert(Packet* packet, Priority priority) {
    switch (priority) {
        case Priority::HIGH:
            addEntry(packet, high);
            break;
        case Priority::LOW:
            addEntry(packet, low);
            break;
    }
}

Packet* MultiLevelFeedbackQueue::get() {
    Packet *front = NULL;
    if (!high.empty() && (lastSwitch < switchLength || low.empty())) {
        front = getEntry(high);
        lastSwitch++;
    } else {
        front = getEntry(low);
        lastSwitch = 0;
    }
    return front;
}

bool MultiLevelFeedbackQueue::empty() {
    return high.empty() && low.empty();
}

Packet* MultiLevelFeedbackQueue::getEntry(
        boost::circular_buffer<Packet*>& buffer) {
    if (icf == NULL) {
        DEBUG_MSG("The InterestCacheFilter has not been set");
        Packet* entry = buffer[0];
        buffer.pop_front();
        return entry;
    }
    double sum = 0;
    boost::circular_buffer<Packet*>::iterator pos = buffer.begin();
    for (auto it = buffer.begin(); it != buffer.end(); it++) {
        sum += icf->getTrustworthiness((*it)->getSource());
    }
    int choice = generator.getDouble(0, sum);
    for (auto it = buffer.begin(); it != buffer.end(); it++) {
        if (choice < icf->getTrustworthiness((*it)->getSource())) {
            pos = it;
            break;
        }
        choice -= icf->getTrustworthiness((*it)->getSource());
    }
    Packet* entry = *pos;
    buffer.erase(pos);
    return entry;
}
