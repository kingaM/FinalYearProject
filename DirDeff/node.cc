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
#include <SignalMatrix.h>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/rolling_sum.hpp>
#include <DendricCells.h>
#include <AIS/ContentClassifier.h>
#include <AIS/PacketFilter.h>
#include "node.h"

#define EXPLORATORY_INT 10
#define INT 2

using namespace std;
using namespace boost::accumulators;

typedef accumulator_set<int, stats<tag::rolling_sum>> SumAcc;


Define_Module(Node);

void Node::generateSensor() {
    emit(generatedDataSignal, 1);
    EV << "GENERATE DATA" << endl;
    Packet* msg = generateMessage(SENSOR, "sensor");
    int r = generator.getNumber(2, 5);
    scheduleAt(simTime() + r, msg);
}

void Node::initialize() {
    lastSent = simTime() - 20;
    generator = RandomNumberGenerator("seeds.csv", 0);
    acc = SumAcc(tag::rolling_window::window_size = 10);
    scheduleAt(simTime() + 1, generateMessage(TIC, "sensor"));
    matrix = new SignalMatrix();
    dcs = new DendricCells(matrix, this);
    cache.setDcs(dcs);
    classifier = ContentClassifier();
    filter = new PacketFilter();
    dcs->setFilter(filter);
    packetsSentSignal = registerSignal("pktSent");
    generatedDataSignal = registerSignal("genData");
    receievedPacketsSignal = registerSignal("rcvdPkt");
    fnfSignal = registerSignal("fnf");
    fpfSignal = registerSignal("fpf");
    tpfSignal = registerSignal("tpf");
    tnfSignal = registerSignal("tnf");
    totalBenSignal = registerSignal("totalb");
    totalMalSignal = registerSignal("totalm");
}

void Node::addToCache(Packet* ttmsg) {
    int prevHop = -1;
    if (ttmsg->getArrivalGate()) {
        prevHop = ttmsg->getArrivalGate()->getIndex();
    }
    EV << "CACHE type: " << string(ttmsg->getDataType()) << endl;
    Gradient* prev = cache.addEntry(string(ttmsg->getDataType()), simTime().raw(),
            ttmsg->getInterval(), ttmsg->getExpiresAt(), prevHop);
    EV << cache.toString() << endl;
    if (prev != NULL) {
        matrix->getEntry(ttmsg->getDataType()).setSs2();
        numOfUpdates++;
        matrix->getEntry(ttmsg->getDataType()).setDs2(prev->getTimestamp(), prev->getExpiry(), simTime().raw());
    }
    delete prev;
}

void Node::saveToDataCache(Packet* ttmsg) {
    int prevHop = -1;
    if (ttmsg->getArrivalGate()) {
        prevHop = ttmsg->getArrivalGate()->getIndex();
    }
    dataCache.add(ttmsg->getMsgId(), prevHop, ttmsg->getType(),
            ttmsg->getDataType(), simTime().raw());
    // TODO: reinforced path?
    matrix->getEntry(ttmsg->getDataType()).setSs1();
}

void Node::forwardInterestPacket(Packet* ttmsg, Class classification) {
    // We need to forward the message.
    EV << "FORWARDING INTEREST PACKET with interval " << ttmsg->getInterval()
            << endl;
    dcs->addCell(PacketInfo(ttmsg->getDataType(), classification, ttmsg->getMalicious()));
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
        vector<int> paths = cache.getPaths(ttmsg->getDataType(),
                simTime().raw());
        if (paths.empty()) {
            return;
        }
        emit(packetsSentSignal, 1);
        lastSent = simTime();
        EV << paths.size() << endl;
        for (vector<int>::iterator it = paths.begin(); it != paths.end();
                ++it) {
            if (*it == -1) {
                emit(receievedPacketsSignal, 1);
                EV << "at the sink... YAAY!!";
                // TODO: find a better way to do this, if not, add types
                if (first) {
                    scheduleAt(simTime() + 1,
                            generateMessage(INTERVAL, ttmsg->getDataType()));
                    first = false;
                }
            } else {
                Packet *dup = ttmsg->dup();
                send(dup, "gate$o", *it);
                EV << "Forwarding message " << ttmsg << " on gate[" << *it
                        << "]\n";
            }
        }
    } else {
        EV << "SHOULD NOT HAPPEN" << endl;
    }
}

void Node::generateNewInterval(string dataType, int interval) {
    if (dataCache.findBestNeighbour(dataType).size() > 0 && interval != EXPLORATORY_INT) {
        double psConc = 0;
        if (numExp.count(dataType)) {
            matrix->getEntry(dataType).setPs(numRcvd[dataType], numExp[dataType]);
            psConc = matrix->getEntry(dataType).getPs().getConcentration();
        }
        Packet* msg = generateMessage(simTime() + 40, INT, INTEREST, simTime(),
                dataType, psConc);
        forwardInterestPacket(msg, classifier.classify(msg));
        numExp[dataType] = 10 / INT;
        numRcvd[dataType] = 0;
        scheduleAt(simTime() + 10, generateMessage(INTERVAL, dataType));
    } else {
        EV << "Sending exploratory packet" << endl;
        // TODO: PS conc?
        Packet* msg = generateMessage(simTime() + 40, EXPLORATORY_INT, INTEREST,
                simTime(), dataType, 0);
        forwardInterestPacket(msg, classifier.classify(msg));
        scheduleAt(simTime() + 20, generateMessage(EXP_INT, dataType));
    }
}

void Node::deleteDataCacheEntries() {
    set<pair<string, int>> inactive = dataCache.getInactive(simTime().raw());
    EV << "deleting inactive " << inactive.size() << endl;
    cache.setInactive(inactive, simTime().raw());
    for (pair<string, int> p : inactive) {
        Packet *msg = generateMessage(simTime() + 1000, 20, INTEREST, simTime(),
                p.first, -1);
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
        generateSensor();
        Packet *msg = generateMessage(simTime() + 1000, 20, DATA, simTime(), "sensor", -1);
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
            EV << "buffer empty" << endl;
        }
    }
    if (dataCache.isInCache(ttmsg->getMsgId())) {
        EV << "Drop packet, already handled " << ttmsg->getType() << endl;
        addToCache(ttmsg);
        delete ttmsg;
        return;
    }
    if (ttmsg->getType() == INTEREST) {
        if (!ttmsg->getMalicious()) {
            emit(totalBenSignal, 1);
        } else {
            emit(totalMalSignal, 1);
        }
        Class classification = classifier.classify(ttmsg);

        if (filter->filterPacket(
                PacketInfo(ttmsg->getDataType(), classification,
                        ttmsg->getMalicious()))) {
            //AIS deemed this packet unsafe, so drop
            EV << "packet " << ttmsg->getDataType() << " dropped" << endl;
            if (!ttmsg->getMalicious()) {
                emit(fpfSignal, 1);

            } else if (ttmsg->getMalicious()) {
                emit(tpfSignal, 1);
            }
            saveToDataCache(ttmsg);
            delete msg;
            return;
        } else {
            if (ttmsg->getMalicious()) {
                emit(fnfSignal, 1);
            } else if (!ttmsg->getMalicious()) {
                emit(tnfSignal, 1);
            }
        }
        forwardInterestPacket(ttmsg, classification);
        if (ttmsg->getPsConc() > 0) {
            matrix->getEntry(ttmsg->getDataType()).setPs(ttmsg->getPsConc());
        }
    }
    if (ttmsg->getType() == DATA) {
        EV << "Sending data..." << endl;
        deleteDataCacheEntries();
        saveToDataCache(ttmsg);
        numRcvd[ttmsg->getDataType()]++;
        if (buffer.count(ttmsg->getDataType()) == 0) {
            Packet* msg = generateMessage(DATA_RETRY, ttmsg->getDataType());
            simtime_t scheduleTime = max(
                    lastSent + cache.getMinInterval(msg->getDataType()) + 1,
                    simTime());
            scheduleAt(scheduleTime, msg);
        }
        saveToBuffer(ttmsg);
        return;
    }
    if (ttmsg->getType() == INTERVAL) {
        generateNewInterval(ttmsg->getDataType(), INT);
    }
    if (ttmsg->getType() == EXP_INT) {
        generateNewInterval(ttmsg->getDataType(), EXPLORATORY_INT);
    }
    if (ttmsg->getType() == TIC) {
        acc(numOfUpdates);
        numOfUpdates = 0;
        EV << "SUM OF EVENTS : " << rolling_sum(acc);
        matrix->addGlobalSs3Ds1(rolling_sum(acc));
        dcs->cycle();
        scheduleAt(simTime() + 1, generateMessage(TIC, "sensor"));
        if (buffer.count(ttmsg->getDataType()) > 0
                && !buffer.at(ttmsg->getDataType()).empty()) {
            Packet *dataMsg = buffer.at(ttmsg->getDataType()).get();
            forwardDataPacket(dataMsg);
            delete dataMsg;
        }
    }
    delete msg;
}

Packet *Node::generateMessage(simtime_t expiresAt, int interval, int type,
        simtime_t timestamp, string dataType, double psConc) {
    int src = getIndex();

    Packet *msg = new Packet();
    msg->setMsgId(msg->getId());
    msg->setSource(src);
    msg->setExpiresAt((simTime() + 1000).raw());
    msg->setInterval(interval);
    msg->setType(type);
    msg->setTimestamp(timestamp);
    msg->setDataType(dataType.c_str());
    msg->setPsConc(psConc);
    msg->setMalicious(false);
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
    // If the message is not exploratory and there is data in cache that can be used to determine
    // where to send the packet
    if (msg->getInterval() == INT
            && cache.getPaths(msg->getDataType(), simTime().raw()).size() > 0
            && dataCache.findBestNeighbour(msg->getDataType()).size() > 0) {
        EV << "finding best0" << endl;
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
    // Default to broadcast
    int n = gateSize("gate");
    for (int i = 0; i < n; i++) {
        Packet *dup = msg->dup();
        send(dup, "gate$o", i);
        EV << "Forwarding message " << msg << " on gate[" << i << "]\n";
    }

}
