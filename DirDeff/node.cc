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
#include <map>
#include <utility>

#define INTEREST 1
#define DATA 2
#define SENSOR 0
#define INTERVAL 3
#define DATA_RETRY 4

using namespace std;

class Node : public cSimpleModule {
    private:
        simsignal_t arrivalSignal;
        void addToCache(Packet* ttmsg);
        void generateSensor();
        void saveToDataCache(Packet* ttmsg);
        void forwardInterestPacket(Packet* ttmsg);
        void forwardDataPacket(Packet* ttmsg);
        void generateNewInterval(string dataType);
        void deleteDataCacheEntries();
        void saveToBuffer(Packet* ttmsg);

        Cache cache;
        DataCache dataCache;
        map<string, MultiLevelFeedbackQueue> buffer;
        simtime_t lastSent;
        RandomNumberGenerator generator;

    protected:
        virtual Packet *generateMessage(simtime_t expiresAt, int interval,
                int type, simtime_t timestamp, string dataType);
        virtual Packet *generateMessage(int type, string dataType);
        virtual void forwardMessage(Packet *msg);
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};

Define_Module(Node);

void Node::generateSensor() {
    EV << "GENERATE DATA" << endl;
    Packet* msg = generateMessage(SENSOR, "sensor");
    int r = generator.getNumber(5, 25);
    scheduleAt(simTime() + r, msg);
}

void Node::initialize() {
    arrivalSignal = registerSignal("arrival");
    if (getIndex() == 0) {
        Packet *msg = generateMessage(INTERVAL, "sensor");
        scheduleAt(simTime() + 2, msg);
        msg = generateMessage(simTime() + 1000, 20, INTEREST, simTime(),
                msg->getDataType());
        forwardInterestPacket(msg);
    }
    if (getIndex() == 4 || getIndex() == 2) {
        generateSensor();
    }
    lastSent = simTime();
    generator = RandomNumberGenerator("seeds.csv", 0);
}

void Node::addToCache(Packet* ttmsg) {
    int prevHop = -1;
    if (ttmsg->getArrivalGate()) {
        prevHop = ttmsg->getArrivalGate()->getIndex();
    }
    cache.addEntry(string(ttmsg->getDataType()), simTime().raw(),
            ttmsg->getInterval(), ttmsg->getExpiresAt(), prevHop);
}

void Node::saveToDataCache(Packet* ttmsg) {
    int prevHop = -1;
    if (ttmsg->getArrivalGate()) {
        prevHop = ttmsg->getArrivalGate()->getIndex();
    }
    dataCache.add(ttmsg->getMsgId(), prevHop, ttmsg->getType(),
            ttmsg->getDataType(), simTime().raw());
}

void Node::forwardInterestPacket(Packet* ttmsg) {
    // We need to forward the message.
    EV << "FORWARDING INTEREST PACKET with interval " << ttmsg->getInterval()
            << endl;
    forwardMessage(ttmsg);
    addToCache(ttmsg);
    saveToDataCache(ttmsg);
}

void Node::saveToBuffer(Packet* ttmsg) {
    EV << "save to buffer" << endl;
    if (buffer.count(ttmsg->getDataType()) == 0) {
        buffer[ttmsg->getDataType()];
    }
    if (ttmsg->isSelfMessage()) {
        buffer.at(ttmsg->getDataType()).insert(ttmsg->dup(), Priority::HIGH);
    } else {
        buffer.at(ttmsg->getDataType()).insert(ttmsg->dup(), Priority::LOW);
    }
}

void Node::forwardDataPacket(Packet* ttmsg) {
    EV << "forward data packet" << endl;
    EV << cache.toString() << endl;
    if (lastSent + cache.getMinInterval(ttmsg->getDataType()) < simTime()) {
        EV << "data sent within interval id: " << ttmsg->getMsgId() << endl;
        lastSent = simTime();
        vector<int> paths = cache.getPaths(ttmsg->getDataType(),
                simTime().raw());
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
        Packet* msg = generateMessage(DATA_RETRY, ttmsg->getDataType());
        scheduleAt(lastSent + cache.getMinInterval(msg->getDataType()) + 1,
                msg);
    } else {
        EV << "SHOULD NOT HAPPEN" << endl;
    }
}

void Node::generateNewInterval(string dataType) {
    EV << "INTEREST PACKET RCVD" << endl;
    if (dataCache.findBestNeighbour(dataType).size() > 0) {
        EV << "generating a new interest with a lower rate" << endl;
        scheduleAt(simTime() + 20, generateMessage(INTERVAL, ""));
        forwardInterestPacket(
                generateMessage(simTime() + 40, 10, INTEREST, simTime(),
                        dataType));
    } else {
        EV << "no data yet... delaying" << simTime() << " " << simTime() + 2
                << "\n";
        scheduleAt(simTime() + 2, generateMessage(INTERVAL, dataType));
    }
}

void Node::deleteDataCacheEntries() {
    set<pair<string, int>> inactive = dataCache.getInactive(simTime().raw());
    EV << "deleting inactive " << inactive.size() << endl;
    if (inactive.size() == 0) {
        EV << "Nothing to delete" << endl;
        return;
    }
    cache.setInactive(inactive, simTime().raw());
    for (pair<string, int> p : inactive) {
        EV << "Inactive first" << p.first << " " << p.second << endl;
        Packet *msg = generateMessage(simTime() + 1000, 20, INTEREST, simTime(),
                p.first);
        send(msg, "gate$o", p.second);
    }
}

void Node::handleMessage(cMessage *msg) {
    Packet *ttmsg = check_and_cast<Packet *>(msg);
    int prevHop = -1;
    if (ttmsg->getArrivalGate()) {
        prevHop = ttmsg->getArrivalGate()->getIndex();
    }
    EV << "MSG type " << ttmsg->getType() << " prevHop " << prevHop << " id "
            << ttmsg->getMsgId() << endl;
    if (ttmsg->getType() == SENSOR) {
        EV << "recvd sensor data" << endl;
        generateSensor();
        Packet *msg = generateMessage(1000, 20, DATA, simTime(), "sensor");
        EV << "sending data from " << getIndex() << "id " << msg->getMsgId()
                << endl;
        scheduleAt(simTime(), msg);
    }
    if (ttmsg->getType() == DATA_RETRY) {
        EV << "forward data retry" << endl;
        if (buffer.count(ttmsg->getDataType()) > 0
                && !buffer.at(ttmsg->getDataType()).empty()) {
            Packet *dataMsg = buffer.at(ttmsg->getDataType()).get();
            forwardDataPacket(dataMsg);
            delete dataMsg;
        } else {
            EV << "buffer EMPTY!!" << endl;
        }
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
        EV << "Sending data..." << endl;
        deleteDataCacheEntries();
        saveToDataCache(ttmsg);
        if (buffer.count(ttmsg->getDataType()) == 0) {
            Packet* msg = generateMessage(DATA_RETRY, ttmsg->getDataType());
            simtime_t scheduleTime = max(
                    lastSent + cache.getMinInterval(msg->getDataType()) + 1,
                    simTime());
            scheduleAt(scheduleTime, msg);
        }
        saveToBuffer(ttmsg);
//        forwardDataPacket(ttmsg);
            return;
    }
    if (ttmsg->getType() == INTERVAL) {
        generateNewInterval(ttmsg->getDataType());
    }
    delete msg;
}

Packet *Node::generateMessage(simtime_t expiresAt, int interval, int type,
        simtime_t timestamp, string dataType) {
    int src = getIndex();

    Packet *msg = new Packet();
    msg->setMsgId(msg->getId());
    msg->setSource(src);
    msg->setExpiresAt(expiresAt.raw());
    msg->setInterval(interval);
    msg->setType(type);
    msg->setTimestamp(timestamp);
    msg->setDataType(dataType.c_str());
    return msg;
}

Packet *Node::generateMessage(int type, string dataType) {
    Packet *msg = new Packet();
    msg->setMsgId(msg->getId());
    msg->setType(type);
    msg->setDataType(dataType.c_str());
    return msg;
}

void Node::forwardMessage(Packet *msg) {
    EV << dataCache.toString() << endl;
    if (cache.getPaths(msg->getDataType(), simTime().raw()).size() > 0) {
        set<int> neighbours = dataCache.findBestNeighbour(msg->getDataType());
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
        Packet *dup = msg->dup();
        send(dup, "gate$o", i);
        EV << "Forwarding message " << msg << " on gate[" << i << "]\n";
    }

}
