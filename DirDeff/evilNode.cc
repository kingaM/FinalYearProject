#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include "packet_m.h"
#include <set>
#include <map>
#include <utility>
#include "RandomNumberGenerator.h"
#include "Cache.h"
#include "DataCache.h"

#define INTEREST 1
#define DATA 2
#define SENSOR 0
#define INTERVAL 3
#define DATA_RETRY 4
#define TIC 5
#define BROADCAST 6

using namespace std;

typedef vector<char> char_array;

class EvilNode : public cSimpleModule {
    private:
        simsignal_t arrivalSignal;
        void sendBogusInterests();
        void forwardInterestPacket(Packet* ttmsg);
        string getRandomString(const int len);
        void addToDataCache(Packet* ttmsg);
        int broadcastInterest(int i);
        int wait(int i);

        simtime_t lastSent;
        RandomNumberGenerator generator;
        DataCache dataCache;

    protected:
        virtual Packet *generateMessage(simtime_t expiresAt, int interval,
                int type, simtime_t timestamp, string dataType, double psConc);
        virtual Packet *generateMessage(int type, string dataType);
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    public:
        EvilNode() {
        }
        ;
};

Define_Module(EvilNode);

void EvilNode::initialize() {
    arrivalSignal = registerSignal("arrival");
    lastSent = simTime();
    generator = RandomNumberGenerator("seeds.csv", 0);
    dataCache = DataCache();
}

void EvilNode::forwardInterestPacket(Packet* ttmsg) {
    broadcastInterest(
            wait(
                    wait(
                            wait(
                                    wait(
                                            broadcastInterest(
                                                    wait(
                                                            broadcastInterest(
                                                                    wait(
                                                                            broadcastInterest(
                                                                                    wait(
                                                                                            wait(
                                                                                                    broadcastInterest(
                                                                                                            wait(
                                                                                                                    wait(
                                                                                                                            wait(
                                                                                                                                    wait(
                                                                                                                                            broadcastInterest(
                                                                                                                                                    broadcastInterest(
                                                                                                                                                            broadcastInterest(
                                                                                                                                                                    wait(
                                                                                                                                                                            broadcastInterest(
                                                                                                                                                                                    broadcastInterest(
                                                                                                                                                                                            wait(
                                                                                                                                                                                                    broadcastInterest(
                                                                                                                                                                                                            0)))))))))))))))))))))))));
}

int EvilNode::wait(int i) {
    return ++i;
}

int EvilNode::broadcastInterest(int i) {
    scheduleAt(simTime() + i, generateMessage(BROADCAST, "sensor"));
    return i;
}

void EvilNode::handleMessage(cMessage *msg) {
    Packet *ttmsg = check_and_cast<Packet *>(msg);
    if (dataCache.isInCache(ttmsg->getMsgId())) {
        delete ttmsg;
        return;
    }
    if (ttmsg->getType() == INTEREST) {
        addToDataCache(ttmsg);
        forwardInterestPacket(ttmsg);
    }
    if (ttmsg->getType() == BROADCAST) {
        sendBogusInterests();
    }
    delete msg;
}

Packet *EvilNode::generateMessage(simtime_t expiresAt, int interval, int type,
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
    msg->setMalicious(true);
    return msg;
}

Packet *EvilNode::generateMessage(int type, string dataType) {
    Packet *msg = new Packet();
    msg->setMsgId(msg->getId());
    msg->setType(type);
    msg->setDataType(dataType.c_str());
    msg->setMalicious(true);
    return msg;
}

void EvilNode::sendBogusInterests() {
    string type = getRandomString(20);
    EV << "EVIL NODE TYPE: " << type;
    Packet* msg = generateMessage(simTime() + 1000, 20, INTEREST, simTime(),
            type, 0);
    int n = gateSize("gate");
    for (int i = 0; i < n; i++) {
        Packet *dup = msg->dup();
        send(dup, "gate$o", i);
        addToDataCache(dup);
        EV << "Forwarding message " << msg << " on gate[" << i << "]\n";
    }
}

string EvilNode::getRandomString(const int len) {
    static const char alphanum[] = "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
    string s = "";
    for (int i = 0; i < len; ++i) {
        s += alphanum[generator.getNumber(0, (sizeof(alphanum) - 1))];
    }
    return s;
}

void EvilNode::addToDataCache(Packet* ttmsg) {
    int prevHop = -1;
    if (ttmsg->getArrivalGate()) {
        prevHop = ttmsg->getArrivalGate()->getIndex();
    }
    dataCache.add(ttmsg->getMsgId(), prevHop, ttmsg->getType(),
            ttmsg->getDataType(), simTime().raw());
}
