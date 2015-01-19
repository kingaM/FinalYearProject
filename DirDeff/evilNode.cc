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

#define INTEREST 1
#define DATA 2
#define SENSOR 0
#define INTERVAL 3
#define DATA_RETRY 4
#define TIC 5

using namespace std;

typedef vector<char> char_array;

class EvilNode : public cSimpleModule {
    private:
        simsignal_t arrivalSignal;
        void sendBogusInterests();
        void forwardInterestPacket(Packet* ttmsg);
        string getRandomString(const int len);

        simtime_t lastSent;
        RandomNumberGenerator generator;

    protected:
        virtual Packet *generateMessage(simtime_t expiresAt, int interval,
                int type, simtime_t timestamp, string dataType, double psConc);
        virtual Packet *generateMessage(int type, string dataType);
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);

    public:
        EvilNode() {};
};

Define_Module(EvilNode);

void EvilNode::initialize() {
    arrivalSignal = registerSignal("arrival");
    lastSent = simTime();
    generator = RandomNumberGenerator("seeds.csv", 0);
//    scheduleAt(simTime() + 1, generateMessage(TIC, "sensor"));
}

void EvilNode::forwardInterestPacket(Packet* ttmsg) {
    // We need to forward the message.
    EV << "FORWARDING INTEREST PACKET with interval " << ttmsg->getInterval()
            << endl;
    sendBogusInterests();
}

void EvilNode::handleMessage(cMessage *msg) {
    Packet *ttmsg = check_and_cast<Packet *>(msg);
    int prevHop = -1;
    if (ttmsg->getArrivalGate()) {
        prevHop = ttmsg->getArrivalGate()->getIndex();
    }
    EV << "MSG type " << ttmsg->getType() << " prevHop " << prevHop << " id "
            << ttmsg->getMsgId() << endl;
    if (ttmsg->getType() == INTEREST) {
        forwardInterestPacket(ttmsg);
    }
    if (ttmsg->getType() == TIC) {
        scheduleAt(simTime() + 1, generateMessage(TIC, "sensor"));
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
    for (int i = 0; i < Cache::SIZE + 10; i++) {
        string type = getRandomString(20);
        EV << "EVIL NODE TYPE: " << type;
        Packet* msg = generateMessage(simTime() + 1000, 20, INTEREST, simTime(),
                type, 0);
        int n = gateSize("gate");
        for (int i = 0; i < n; i++) {
            Packet *dup = msg->dup();
            send(dup, "gate$o", i);
            EV << "Forwarding message " << msg << " on gate[" << i << "]\n";
        }
    }
}

string EvilNode::getRandomString(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    string s = "";
    for (int i = 0; i < len; ++i) {
        s += alphanum[generator.getNumber(0,(sizeof(alphanum) - 1))];
    }
    return s;
}
