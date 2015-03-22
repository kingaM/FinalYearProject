#include <string.h>
#include <omnetpp.h>
#include <vector>
#include <set>
#include <map>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/rolling_sum.hpp>
#include "packet_m.h"
#include "Cache.h"
#include "DataCache.h"
#include "RandomNumberGenerator.h"
#include "MultiLevelFeedbackQueue.h"
#include "AIS/DendricCells.h"
#include "AIS/ContentClassifier.h"
#include "AIS/PacketFilter.h"
#include "AIS/SignalMatrix.h"
#include "AIS/InterestCacheFilter.h"
#include "Node.h"

using namespace std;
using namespace boost::accumulators;

typedef accumulator_set<int, stats<tag::rolling_sum>> SumAcc;

Define_Module(Node);

Node::Node() :
        acc(SumAcc(tag::rolling_window::window_size = 10)),
        // Cannot be initialized yet, as the simulation is not up. Will be
        // overwritten in the "initialize" method.
        classifier(0, 0) {
}

Node::~Node() {

}

void Node::generateSensor() {
    EV << "Generated data message " << endl;
    emit(generatedDataSignal, 1);
    Packet* msg = generateMessage(SENSOR, "sensor");
    int r = generator.getNumber(2, 5);
    scheduleAt(simTime() + r, msg);
}

void Node::initialize() {
    setParameters();
    bool icfEnabled = par("interestCacheFilter");
    dataFilteringEnabled = par("dataFilter");
    generator = RandomNumberGenerator("seeds.csv", run, id);
    lastSent = simTime() - 20;
    acc = SumAcc(tag::rolling_window::window_size = 10);
    scheduleAt(simTime() + 1, generateMessage(TIC, "sensor"));
    matrix = new SignalMatrix();
    filter = new PacketFilter(run, numOfNodes + id);
    icf = new InterestCacheFilter();
    dcs = new DendricCells(matrix, filter, icf, this);
    cache.setDcs(dcs);
    if (icfEnabled) {
        cache.setFilter(icf);
    }
    classifier = ContentClassifier(run, numOfNodes * 2 + id);
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
    Gradient* prev = cache.addEntry(string(ttmsg->getDataType()),
            simTime().raw(), ttmsg->getSource(), ttmsg->getInterval(),
            ttmsg->getExpiresAt(), prevHop);
    if (prev != NULL) {
        matrix->getEntry(ttmsg->getDataType()).setSs2();
        numOfUpdates++;
        matrix->getEntry(ttmsg->getDataType()).setDs2(prev->getTimestamp(),
                prev->getExpiry(), simTime().raw());
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
    dcs->addCell(
            PacketInfo(ttmsg->getDataType(), classification, ttmsg->getSource(),
                    ttmsg->getMalicious()));
    forwardMessage(ttmsg);
    addToCache(ttmsg);
    saveToDataCache(ttmsg);
}

void Node::saveToBuffer(Packet* ttmsg) {
    if (buffer.count(ttmsg->getDataType()) == 0) {
        buffer.insert(
                make_pair(ttmsg->getDataType(),
                        MultiLevelFeedbackQueue(run, numOfNodes * 3 + id)));
        if (dataFilteringEnabled) {
            buffer[ttmsg->getDataType()].setInterestCacheFilter(icf);
        }
    }
    if (ttmsg->isSelfMessage()) {
        buffer.at(ttmsg->getDataType()).insert(ttmsg->dup(), Priority::HIGH);
    } else {
        buffer.at(ttmsg->getDataType()).insert(ttmsg->dup(), Priority::LOW);
    }
}

void Node::forwardDataPacket(Packet* ttmsg) {
    EV << "Forward data packet interest cache: " << cache.toString() << endl;
    if (lastSent + cache.getMinInterval(ttmsg->getDataType()) < simTime()) {
        vector<int> paths = cache.getPaths(ttmsg->getDataType(),
                simTime().raw());
        if (paths.empty()) {
            return;
        }
        emit(packetsSentSignal, 1);
        lastSent = simTime();
        EV << paths.size() << endl;
        for (auto it = paths.begin(); it != paths.end(); ++it) {
            if (*it == -1) {
                if (!ttmsg->getMalicious()) {
                    emit(receievedPacketsSignal, 1);
                }
                EV << "at the sink... YAAY!!" << endl;
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
    }
}

void Node::generateNewInterval(string dataType, int interval) {
    Packet* msg = NULL;
    if (dataCache.findBestNeighbour(dataType).size()
            > 0 && interval != EXPLORATORY_INT) {
        double psConc = 0;
        if (numExp.count(dataType)) {
            matrix->getEntry(dataType).setPs(numRcvd[dataType],
                    numExp[dataType]);
            psConc = matrix->getEntry(dataType).getPs().getConcentration();
        }
        msg = generateMessage(INT, INTEREST, dataType, psConc);
        forwardInterestPacket(msg, classifier.classify(msg));
        numExp[dataType] = 10 / INT;
        numRcvd[dataType] = 0;
        scheduleAt(simTime() + 10, generateMessage(INTERVAL, dataType));
    } else {
        EV << "Sending exploratory packet" << endl;
        // TODO: PS conc?
        msg = generateMessage(EXPLORATORY_INT, INTEREST, dataType, 0);
        forwardInterestPacket(msg, classifier.classify(msg));
        scheduleAt(simTime() + 20, generateMessage(EXP_INT, dataType));
    }
    delete msg;
}

void Node::deleteDataCacheEntries() {
    set<pair<string, int>> inactive = dataCache.getInactive(simTime().raw());
    EV << "Deleting " << inactive.size() << endl;
    cache.setInactive(inactive, simTime().raw());
    for (pair<string, int> p : inactive) {
        Packet *msg = generateMessage(EXPLORATORY_INT, INTEREST, p.first, -1);
        saveToDataCache(msg);
        send(msg, "gate$o", p.second);
    }
}

void Node::handleInterestPacket(Packet *ttmsg) {
    if (!ttmsg->getMalicious()) {
        emit(totalBenSignal, 1);
    } else {
        emit(totalMalSignal, 1);
    }
    Class classification = classifier.classify(ttmsg);

    if (filter->filterPacket(
            PacketInfo(ttmsg->getDataType(), classification, ttmsg->getSource(),
                    ttmsg->getMalicious()))) {
        // AIS deemed this packet unsafe, so drop
        EV << "Packet " << ttmsg->getDataType() << " dropped by AIS" << endl;
        if (!ttmsg->getMalicious()) {
            emit(fpfSignal, 1);
        } else if (ttmsg->getMalicious()) {
            emit(tpfSignal, 1);
        }
        saveToDataCache(ttmsg);
    } else {
        if (ttmsg->getMalicious()) {
            emit(fnfSignal, 1);
        } else if (!ttmsg->getMalicious()) {
            emit(tnfSignal, 1);
        }
        forwardInterestPacket(ttmsg, classification);
        if (ttmsg->getPsConc() > 0) {
            matrix->getEntry(ttmsg->getDataType()).setPs(ttmsg->getPsConc());
        }
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
    if (dataCache.isInCache(ttmsg->getMsgId())) {
        delete ttmsg;
        return;
    }
    switch (ttmsg->getType()) {
        case SENSOR:
            generateSensor();
            scheduleAt(simTime(), generateMessage(20, DATA, "sensor", 0));
            break;
        case INTEREST:
            handleInterestPacket(ttmsg);
            break;
        case DATA:
            EV << "Sending data MAL " << ttmsg->getMalicious() << endl;
            deleteDataCacheEntries();
            saveToDataCache(ttmsg);
            numRcvd[ttmsg->getDataType()]++;
            saveToBuffer(ttmsg);
            break;
        case INTERVAL:
            generateNewInterval(ttmsg->getDataType(), INT);
            break;
        case EXP_INT:
            generateNewInterval(ttmsg->getDataType(), EXPLORATORY_INT);
            break;
        case TIC:
            acc(numOfUpdates);
            numOfUpdates = 0;
            matrix->addGlobalSs3Ds1(rolling_sum(acc));
            dcs->cycle();
            if (buffer.count(ttmsg->getDataType()) > 0
                    && !buffer.at(ttmsg->getDataType()).empty()
                    && lastSent + cache.getMinInterval(ttmsg->getDataType())
                            < simTime()) {
                Packet *dataMsg = buffer.at(ttmsg->getDataType()).get();
                forwardDataPacket(dataMsg);
                delete dataMsg;
            }
            scheduleAt(simTime() + 1, generateMessage(TIC, "sensor"));
            break;
        default:
            cRuntimeError("Invalid message type");
            break;
    }
    delete msg;
}

void Node::forwardMessage(Packet *msg) {
    // If the message is not exploratory and there is data in cache that can be
    // used to determine where to send the packet
    if (msg->getInterval() == INT
            && cache.getPaths(msg->getDataType(), simTime().raw()).size() > 0
            && dataCache.findBestNeighbour(msg->getDataType()).size() > 0) {
        set<int> neighbours = dataCache.findBestNeighbour(msg->getDataType());
        for (auto it = neighbours.begin(); it != neighbours.end(); ++it) {
            int i = *it;
            EV << "Forwarding message to best " << msg << " on gate[" << i
                    << "]\n";
            if (i == -1) {
                continue;
            }
            Packet *dup = msg->dup();
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
