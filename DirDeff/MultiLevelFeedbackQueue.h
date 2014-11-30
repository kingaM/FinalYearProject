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

enum class Priority {HIGH, LOW};

class MultiLevelFeedbackQueue {
public:
    MultiLevelFeedbackQueue();
    virtual ~MultiLevelFeedbackQueue();

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
