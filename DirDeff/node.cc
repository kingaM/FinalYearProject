#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include "packet_m.h"
#include "Cache.h"
#include "DataCache.h"
#include "RandomNumberGenerator.h"
#include "MultiLevelFeedbackQueue.h"
#include <set>

#define INTEREST 1
#define DATA 2
#define SENSOR 0
#define INTERVAL 3
#define DATA_RETRY 4

using namespace std;

class Node: public cSimpleModule {
private:
    simsignal_t arrivalSignal;
    void addToCache(Packet* ttmsg);
    void generateSensor();
    void saveToDataCache(Packet* ttmsg);
    void forwardInterestPacket(Packet* ttmsg);
    void forwardDataPacket(Packet* ttmsg);
    void generateNewInterval();

    Cache cache;
    DataCache dataCache;
    MultiLevelFeedbackQueue buffer;
    simtime_t lastSent;
    int seed;
    RandomNumberGenerator generator;

protected:
    virtual Packet *generateMessage(simtime_t expiresAt, int interval, int type,
            simtime_t timestamp);
    virtual void forwardMessage(Packet *msg);
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(Node);

void Node::generateSensor() {
    EV << "GENERATE DATA" << endl;
    Packet* msg = generateMessage(0, 0, 0, simTime());
    int r = generator.getNumber(2, 10);
    scheduleAt(simTime() + r, msg);
}

void Node::initialize() {
    arrivalSignal = registerSignal("arrival");
    if (getIndex() == 0) {
        Packet *msg = generateMessage(simTime() + 40, 20, INTERVAL, simTime());
        scheduleAt(simTime() + 2, msg);
        msg = generateMessage(1000, 20, INTEREST, simTime());
        forwardInterestPacket(msg);
    }
    if (getIndex() == 4 || getIndex() == 2) {
        generateSensor();
    }
    lastSent = simTime();
    generator = RandomNumberGenerator("seeds.csv", 0);
}

void Node::addToCache(Packet* ttmsg) {
    int prEVHop = -1;
    if (ttmsg->getArrivalGate()) {
        prEVHop = ttmsg->getArrivalGate()->getIndex();
    }
    cache.addEntry((string) "sensor", simTime().raw(), ttmsg->getInterval(),
            ttmsg->getExpiresAt(), prEVHop);
}

void Node::saveToDataCache(Packet* ttmsg) {
    int prEVHop = -1;
    if (ttmsg->getArrivalGate()) {
        prEVHop = ttmsg->getArrivalGate()->getIndex();
    }
    dataCache.add(ttmsg->getMsgId(), prEVHop, ttmsg->getType());
}

void Node::forwardInterestPacket(Packet* ttmsg) {
    // We need to forward the message.
    EV << "FORWARDING INTEREST PACKET with interval " << ttmsg->getInterval()
              << endl;
    forwardMessage(ttmsg);
    addToCache(ttmsg);
    saveToDataCache(ttmsg);
}

void Node::forwardDataPacket(Packet* ttmsg) {
    if (lastSent + cache.getMinInterval("sensor") < simTime()) {
        EV << "data sent within interval id: " << ttmsg->getMsgId() << endl;
        lastSent = simTime();
        vector<int> paths = cache.getPaths("sensor", simTime().raw());
        for (vector<int>::iterator it = paths.begin(); it != paths.end();
                ++it) {
            if (*it == -1) {
                EV << "at the sink... YAAY!!";
            } else {
                Packet *dup = ttmsg->dup();
                send(dup, "gate$o", *it);
                EV << "Forwarding message " << ttmsg << " on gate[" << *it
                          << "]\n";
            }
        }
    } else {
        EV << "scheduling msg at "
                  << lastSent + cache.getMinInterval("sensor") + 1 << endl;
        if (ttmsg->isSelfMessage()) {
            buffer.insert(ttmsg, Priority::HIGH);
        } else {
            buffer.insert(ttmsg, Priority::LOW);
        }
        Packet* msg = generateMessage(0, 0, DATA_RETRY, simTime());
        scheduleAt(lastSent + cache.getMinInterval("sensor") + 1, msg);
    }
}

void Node::generateNewInterval() {
    EV << "INTEREST PACKET RCVD" << endl;
    if (dataCache.findBestNeighbour().size() > 0) {
        EV << "generating a new interest with a lower rate" << endl;
        scheduleAt(simTime() + 20,
                generateMessage(1000, 10, INTERVAL, simTime()));
        forwardInterestPacket(
                generateMessage(simTime() + 40, 10, INTEREST, simTime()));
    } else {
        EV << "no data yet... delaying" << simTime() << " " << simTime() + 2
                  << "\n";
        scheduleAt(simTime() + 2,
                generateMessage(1000, 10, INTERVAL, simTime()));
    }
}

void Node::handleMessage(cMessage *msg) {
    Packet *ttmsg = check_and_cast<Packet *>(msg);
    int prEVHop = -1;
    if (ttmsg->getArrivalGate()) {
        prEVHop = ttmsg->getArrivalGate()->getIndex();
    }
    EV << "MSG type " << ttmsg->getType() << " prEVHop " << prEVHop << " id "
              << ttmsg->getMsgId() << endl;
    if (ttmsg->getType() == SENSOR) {
        EV << "recvd sensor data" << endl;
        generateSensor();
        Packet *msg = generateMessage(1000, 20, DATA, simTime());
        EV << "sending data from " << getIndex() << "id " << msg->getMsgId()
                  << endl;
        scheduleAt(simTime(), msg);
        return;
    }
    if (ttmsg->getType() == DATA_RETRY) {
        forwardDataPacket(buffer.get());
    }
    if (dataCache.isInCache(ttmsg->getMsgId())) {
        EV << "Drop packet, already handled " << ttmsg->getType() << endl;
        addToCache(ttmsg);
        return;
    }
    if (ttmsg->getType() == INTEREST) {
        forwardInterestPacket(ttmsg);
    }
    if (ttmsg->getType() == DATA) {
        saveToDataCache(ttmsg);
        forwardDataPacket(ttmsg);
    }
    if (ttmsg->getType() == INTERVAL) {
        generateNewInterval();
    }
}

Packet *Node::generateMessage(simtime_t expiresAt, int interval, int type,
        simtime_t timestamp) {
    // Produce source and destination addresses.
    int src = getIndex();

    // Create message object and set source and destination field.
    Packet *msg = new Packet();
    msg->setMsgId(msg->getId());
    msg->setSource(src);
    msg->setExpiresAt(expiresAt.raw());
    msg->setInterval(interval);
    msg->setType(type);
    msg->setTimestamp(timestamp);
    return msg;
}

void Node::forwardMessage(Packet *msg) {
    EV << dataCache.toString() << endl;
    if (cache.getPaths("sensor", simTime().raw()).size() > 0) {
        set<int> neighbours = dataCache.findBestNeighbour();
        for (set<int>::iterator it = neighbours.begin(); it != neighbours.end();
                ++it) {
            int i = *it;
            EV << "Forwarding message to best " << msg << " on gate[" << i
                      << "]\n";
            Packet *dup = msg->dup();
            dup->setSource(getIndex());
            if (i == -1) {
                continue;
            }
            send(dup, "gate$o", i);
        }
        return;
    }
    int n = gateSize("gate");
    for (int i = 0; i < n; i++) {
//        if (msg->getArrivalGate() == NULL
//                || i != msg->getArrivalGate()->getIndex()) {
        Packet *dup = msg->dup();
        // dup->setMsgId(msg->getId());
        send(dup, "gate$o", i);
        EV << "Forwarding message " << msg << " on gate[" << i << "]\n";
//        }
    }

}
